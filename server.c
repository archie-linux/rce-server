#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8888


void *handle_client(void *socket_fd) {
    int client_socket = *(int *)socket_fd;
    char buffer[1024] = {0};
    char response[1024] = {0};

    char *prompt = "command> ";
    send(client_socket, prompt, strlen(prompt), 0);

    while (1) {
        // Receive command or script name from client
        memset(buffer, 0, sizeof(buffer));

        if (recv(client_socket, buffer, sizeof(buffer), 0) <= 0) {
            printf("Connection closed by client or error.\n");
            break;
        }
        printf("Received command or script: %s", buffer);

        // Check if the client want's to close the connection
        if (strcmp(buffer, "quit\n") == 0) {
            printf("\nClient Disconnected\n\n");
            break;
        }

        // Execute command using popen() to capture output
        FILE *fp = popen(buffer, "r");
        if (fp == NULL) {
            perror("open");
            break;
        }

        // Read command output and send it back to client
        while (fgets(response, sizeof(response), fp) != NULL) {
            send(client_socket, response, strlen(response), 0);
        }

        // Close the pipe
        pclose(fp);

        // Send prompt to client again
        send(client_socket, prompt, strlen(prompt), 0);
    }

    // Close client socket
    close(client_socket);


    // Free socket_fd memory
    free(socket_fd);

    return NULL;
}


int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Bind socket
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n\n", PORT);

    // Accept incoming connections and handle them in separate threads
    while (1) {
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        printf("[Client connected] IP address: [%s] Port: [%d]\n\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

        // Allocate memory for socket_fd to pass to thread
        int *socket_fd = malloc(sizeof(int));
        *socket_fd = client_socket;

        // Create thread to handle client
        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handle_client, (void *)socket_fd) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }

        // Detach thread to avoid memory leak
        pthread_detach(thread_id);
    }


    return 0;
}