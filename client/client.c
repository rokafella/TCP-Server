//client.c file for homework 1 - file transfer application using TCP sockets

//Client is sending a file by converting into a TCP stream to a server specified by user

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

main(int argc, char const *argv[])
{
	int sock;                                          //Initial socket descriptor
	struct sockaddr_in server_add;                     //Structure for server socket name setup
	char buff[1000];                                   //Buffer for sending the file
    int size;                                          //Storing the size of file

    //If there are more or less than 4 arguments show error
    //First argument: exec file         Second argument: server IP 
    //Third argument: local port number Fourth argument: file to send
    if (argc!=4){
        printf("Usage: %s <remote-IP> <remote-port> <local-file>\n",argv[0]);
        exit(1);
    }

    //Pointer to file being sent
    FILE* file = fopen(argv[3],"rb");

    if (file == NULL)
    {
        perror("Unable to open file");
        exit(1);
    }

    //Seek to end of the file to get the total size of the file
    fseek(file, 0L, SEEK_END);
    size = ftell(file);
    fseek(file, 0L, SEEK_SET);

    //Initialize socket for TCP in linux
    printf("Setting up socket...\n");
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
    	perror("Error openting datagram socket");
    	exit(1);
    }
    printf("Socket initialized \n");

    //Constructing socket name of the server to connect to
  	server_add.sin_family = AF_INET;
  	server_add.sin_port = htons(atoi(argv[2]));
  	server_add.sin_addr.s_addr = inet_addr(argv[1]);

    //Connecting to server using the socket
    printf("Connecting to server...\n");
  	if (connect(sock, (struct sockaddr *)&server_add, sizeof(struct sockaddr_in)) < 0)
  	{
  		close(sock);
  		perror("Error connecting to server");
  		exit(1);
  	}
    printf("Connected, preparing to write...\n");

    //Copy the size into buffer for sending
    memcpy(buff,&size,sizeof(size));

    //Send the size of the file in first 4 bytes
    if (write(sock, buff, sizeof(size)) < 0)
  	{
  		perror("Error writing on stream socket");
  		exit(1);
  	}
    printf("Sending file of size: %d\n", size);

    //Reinitialize the buffer to zero
    bzero(buff,sizeof(size));

    //Copy the name of file into buffer for sending
    memcpy(buff,argv[3],20);

    //Send the name of the file in the next 20 bytes
    if (write(sock, buff, 20) < 0)
    {
        perror("Error writing on stream socket");
        exit(1);
    }

    printf("Sending file: %s\n", buff);

    //Reinitialize buffer to zero
    bzero(buff,20);

    //Keeping track of bytes sent
    int sent = 0;

    //This variable tells about number of bytes to read from the file and send
    int sent_size = 1000;

    //If size of the file is less than buffer update sent_size
    if(size<1000){
        sent_size = size;
    }

    //Continue sending until all the bytes are sent to the server
    while(sent<size){

        //Read from the file and store in buffer
        fread(buff,1,sent_size,file);

        //Write buffer to socket for sending to the server
        int s = write(sock,buff,sent_size);

        if (s < 0)
        {
            perror("Error sending file");
            exit(1);
        }
        
        sent += s;

        //If remaining bytes are less than sent_size update sent_size
        if((size-sent)<sent_size){
            sent_size = size - sent;
        }
        printf("Sent.. %d%\n",(sent*100)/size);

        //Change buffer to zero
        bzero(buff,1000);
    }

    //Close the file pointer
    fclose(file);
    printf("File successfully sent.\n");

    //Close the socket connecting to the server
  	close(sock);
}