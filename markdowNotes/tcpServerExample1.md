## TCP (Server connection to 1 client) API in C
## Header Files

- `stdio.h`, `stdlib.h`, `string.h`: Standard C libraries for input/output operations, memory allocation, and string manipulation.
- `unistd.h`: Provides access to POSIX operating system API, like read and close.
- `sys/types.h`, `sys/socket.h`: Required for using sockets.
- `netinet/in.h`: Contains constants and structures needed for internet domain addresses.

## Error Handling Function

```c
void error(const char *msg) {
    perror(msg);
    exit(1);
}
```
`error`: A custom function to print error messages and exit the program. `perror` prints a descriptive error message based on the global `errno`.

## Main Function Setup

- `int sockfd, newsockfd, portno;`: Declares variables for the socket file descriptors (`sockfd`, `newsockfd`) and the port number (`portno`).
- `struct sockaddr_in serv_addr, cli_addr;`: Declares structures to hold the internet address of the server (`serv_addr`) and the client (`cli_addr`).
- `char buffer[256];`: A buffer to store data received from the client.

## Creating a Socket

```c
sockfd = socket(AF_INET, SOCK_STREAM, 0);
if (sockfd < 0) 
    error("ERROR opening socket");
```
`socket()`: Creates a new socket. `AF_INET` specifies the IPv4 Internet protocols. `SOCK_STREAM` indicates a TCP socket. Returns a socket file descriptor.

## Setting up the Server Address

```c
bzero((char *) &serv_addr, sizeof(serv_addr));
portno = 12345;
serv_addr.sin_family = AF_INET;
serv_addr.sin_addr.s_addr = INADDR_ANY;
serv_addr.sin_port = htons(portno);
```
- `bzero`: Clears the `serv_addr` structure.
- `sin_family`: Sets the address family to IPv4.
- `sin_addr.s_addr`: Allows the server to accept connections from any IP address.
- `sin_port`: Sets the port number, converted to network byte order using `htons`.

## Binding the Socket

```c
if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    error("ERROR on binding");
```
`bind()`: Binds the socket to the specified address (`serv_addr`).

## Listening for Connections

```c
listen(sockfd, 5);
clilen = sizeof(cli_addr);
```
`listen()`: Marks the socket as a passive socket that will be used to accept incoming connection requests. `5` is the backlog, the maximum number of pending connections.

## Accepting Connections

```c
newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
if (newsockfd < 0)
    error("ERROR on accept");
```
`accept()`: Blocks the process until a client connects to the server. It returns a new file descriptor for the client connection.

## Reading Data from the Client

```c
bzero(buffer, 256);
n = read(newsockfd, buffer, 255);
if (n < 0) error("ERROR reading from socket");
printf("Here is the message: %s\n", buffer);
```
Clears the buffer and then reads data from the client into `buffer`. Prints the received message.

## Closing the Socket

```c
close(newsockfd);
close(sockfd);
```
Closes the client and server sockets.

## Summary

This code creates a TCP server that listens on a specified port (`12345`), accepts one client connection, reads a message from the client, prints it, and then closes the connection. This server can handle one client at a time and will terminate after processing the first client's message. For a real-world application, you would typically add a loop to handle multiple clients and possibly implement multithreading or multiplexing for handling concurrent connections.
```