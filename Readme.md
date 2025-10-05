#### Setup

- gcc -o server server.c
- ./server

#### Client-Server interaction

<pre>
MacBook-Air:rce-server anish$ nc 127.0.0.1 8888
command> ls -al
total 104
drwxr-xr-x   8 anish  staff    256 Feb 26 16:16 .
drwxr-xr-x  20 anish  staff    640 Feb 26 16:16 ..
drwxr-xr-x  12 anish  staff    384 Feb 26 16:16 .git
-rw-r--r--   1 anish  staff      7 Feb 26 16:16 .gitignore
-rw-r--r--   1 anish  staff   1358 Feb 26 16:16 Readme.md
-rwxr-xr-x   1 anish  staff  34563 Feb 26 16:16 server
-rw-r--r--   1 anish  staff   3360 Feb 26 16:16 server.c
-rwxr-xr-x   1 anish  staff    115 Feb 26 16:16 test
command> pwd    
/Users/anish/anpa6841/rce-server
command> uname -a
Darwin MacBook-Air.local 22.1.0 Darwin Kernel Version 22.1.0: Sun Oct  9 20:14:30 PDT 2022; root:xnu-8792.41.9~2/RELEASE_ARM64_T8103 arm64
command> ./test
0
1
2
3
4
5
6
7
8
9
command> quit
MacBook-Air:rce-server anish$ 
</pre>

#### Server Logs

<pre>
[Client connected] IP address: [127.0.0.1] Port: [49381]

Received command or script: ls -al
Received command or script: pwd
Received command or script: uname -a
Received command or script: ./test
Received command or script: quit

Client Disconnected
</pre>
