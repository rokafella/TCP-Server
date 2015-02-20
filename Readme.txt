CSE 5462: Homework 1 - Write a file transfer application using TCP sockets.

Submitted By: Rohit Kapoor - kapoor.83@osu.edu

How to transfer a file:

1) The whole project is divided into 2 parts: client and server. 
2) Client part is in the directory Client and Server part is in the directory Server. 
3) There is a Makefile in the default directory which compiles both client and server at once.
4) To compile both files go to directory "Homework 1" and type "make".
4) To start the server go to the server directory and type "ftps <local-port>". This will start the server at the specified port number.
5) Next, start a new connection or go to the Client directory and start the client and provide server IP, port number of server, and path of file you are going to send by typing "ftpc <remote-IP> <remote-port> <local-file-to-transfer>".
6) Server will then accept the connection and receive the file and then exit after saving it in the Server directory.
7) You can confirm the file is transferred successfully by checking the "diff" of local file and file received inside the server directory.