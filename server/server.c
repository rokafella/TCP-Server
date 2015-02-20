//server.c file for homework 1 - file transfer application using TCP sockets

//Server is accepting a TCP stream on port specified by the user

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
	int sock;										//Initial socket descriptor
	struct sockaddr_in my_addr;						//Structure for socket name setup
	int newsock;									//Accepted socket descriptor
	char buff[1000];								//Buffer for holding data

	//If there are more or less than 2 arguments show error
	//First argument: exec file Second argument: local port number
	if (argc!=2){
        printf("Usage: %s <local-port>\n",argv[0]);
        exit(1);
    }

	//Initialize socket for TCP in linux
	printf("Setting up socket...\n");
	if ((sock = socket(AF_INET,SOCK_STREAM,0)) < 0)
	{
		perror("Error opeining datagram socket!");
		exit(1);
	}
	printf("Socket initialized\n");

	//Constructing socket name for receiving
	my_addr.sin_family = AF_INET;
	my_addr.sin_addr.s_addr = INADDR_ANY;			//Listen to any IP address
	my_addr.sin_port = htons(atoi(argv[1]));

	//Binding socket name to socket
	printf("Binding socket to socket name...\n");
	if (bind(sock, (struct sockaddr *)&my_addr, sizeof(struct sockaddr_in)) < 0)
	{
		perror("Error binding stream socket");
		exit(1);
	}
	printf("Socket stream binded\n");

	//listen to the defined socket for upto 1 connections
	listen(sock, 1);

	printf("Accepting new connection...\n");

	//Accepting a new connection and creating a new socket for that connection
	newsock = accept(sock, (struct sockaddr *)NULL, (int *) NULL);
	if(newsock<0)
	{
		perror("Error connecting socket stream");
		exit(1);
	}
	
	close(sock);									//Initial socket descriptor not required now
	printf("New connection accepted\n");

	//Initializing buffer to zero
	bzero(buff,1000);

	//size will hold the size of the file we are sending								
	int size;

	//Reading first 4 bytes from stream to get the file size
	if (read(newsock, buff, 4) < 0)
	{
		perror("Error reading socket stream");
		exit(1);
	}

	//Copying the 4 bytes into size variable
	memcpy(&size,buff,sizeof(size));
	printf("Receiving file of size: %d\n", size);
	
	//Setting the bytes used in buffer back to zero
	bzero(buff,sizeof(size));

	//name will hold the name of the file
	char name[20];

	//Reading next 20 bytes to get the name of the file
	if (read(newsock, buff, 20) < 0)
	{
		perror("Error reading socket stream");
		exit(1);
	}

	//Copying 20 bytes into name variable
	memcpy(name,buff,20);

	printf("Receiving file: %s\n", name);

	//Setting the buffer to zero again
	bzero(buff,20);

	//This variable will keep track of the bytes received
	int received = 0;

	//Pointer to file that is being written
	FILE *file = fopen(name,"wb");

	//This variable tells about number of bytes to receive and write to file
	int rec_size = 1000;

	//Continue reading from the socket until the recieved bytes are equal to size of the file
	while(received<size){

		int rec = read(newsock, buff, rec_size);

		if(rec<0){
			perror("Error receiving file");
			exit(1);
		}

		received += rec;
		printf("Received.. %d%\n",(received*100)/size);

		//Write the received content to the file
		fwrite(buff,1,rec,file);

		//If remaining bytes are less than rec_size update rec_size
		if((size-received)<rec_size){
			rec_size = size-received;
		}

		//Change buffer to zero
		bzero(buff,1000);
	}

	//Close the file being written
	fclose(file);
	printf("File successfully received.\n");

	//Close the socket which is receiving file
	close(newsock);
}