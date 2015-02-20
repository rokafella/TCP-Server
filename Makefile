# Makefile for client and server

CC = gcc
OBJCLI = client/client.c 
OBJSRV = server/server.c
CFLAGS = 
# setup for system
LIBS =

all: ftpc ftps 

ftpc:	$(OBJCLI)
	$(CC) $(CFLAGS) -o client/$@ $(OBJCLI) $(LIBS)

ftps:	$(OBJSRV)
	$(CC) $(CFLAGS) -o server/$@ $(OBJSRV) $(LIBS)

clean:
	rm ftps ftpc