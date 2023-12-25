#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;

     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");

     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = 12345; // Port number
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);

     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
              error("ERROR on binding");

     listen(sockfd,5);
     clilen = sizeof(cli_addr);

     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");

     bzero(buffer,256);
     n = read(newsockfd,buffer,255);
     if (n < 0) error("ERROR reading from socket");

     printf("Here is the message: %s\n",buffer);

     close(newsockfd);
     close(sockfd);
     return 0; 
}

/*
Basic Steps to Create a TCP Server in C
Initialize a Socket:

Use the socket() function to create a socket. This function returns a socket descriptor, which is used for further operations on the socket.
Example: int sockfd = socket(AF_INET, SOCK_STREAM, 0);
Define Server Address:

Define a sockaddr_in structure with details of the address and port on which the server will run.
Specify the IP address and port number. For a server, you often use INADDR_ANY to bind to all available interfaces.
Bind the Socket:

Use the bind() function to bind the socket to the server address and port you've specified.
Example: bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
Listen for Connections:

Use the listen() function to allow the socket to listen for incoming connections.
Example: listen(sockfd, 5); where 5 is the number of allowed pending connections.
Accept Connections:

Use the accept() function to accept incoming client connections. This function blocks the server until a client connects.
It returns a new socket descriptor for the accepted connection, which is used for sending and receiving data.
Receive and Send Data:

Use the recv() and send() functions on the connected socket to receive and send data, respectively.
Close the Socket:

After communication is complete, close the socket using the close() function.

When you create a TCP server in C, it can interact with any client that implements the TCP protocol, including an iPhone app.

The iPhone app would need to be written in a way that it can establish a TCP connection to your server's IP address and port.
Remember, for an iPhone to communicate with your server, they need to be on the same network or your server needs to be accessible over the internet.

Important Notes
Error Handling: Proper error handling is crucial in network programming. The example above uses a very basic error handling approach.
Concurrency: Real-world servers often handle multiple clients simultaneously, which requires concurrency (like multi-threading or select/poll mechanisms).
Security: When building networked applications, always consider security aspects like encrypting data and protecting against common vulnerabilities.
Creating a TCP server in C is a great way to learn about low-level network programming, and it gives you a deeper understanding of how networked applications work under the hood.
*/