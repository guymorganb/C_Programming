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


## TCP Server (Connection to Multiple Clients) API in C

### Header Files

- `stdio.h`, `stdlib.h`, `string.h`: Standard C libraries for input/output operations, memory allocation, and string manipulation.
- `unistd.h`: Provides access to the POSIX operating system API, like read and close.
- `sys/types.h`, `sys/socket.h`: Required for using sockets.
- `netinet/in.h`: Contains constants and structures needed for internet domain addresses.
- `pthread.h`: POSIX threading library.

### Error Handling Function

```c
void error(const char *msg) {
    perror(msg);
    exit(1);
}
```
`error`: A custom function to print error messages and exit the program. `perror` prints a descriptive error message based on the global `errno`.

### Client Handler Function

```c
void *client_handler(void *sockfd) {
    int newsockfd = *(int*)sockfd;
    char buffer[256];
    int n;

    bzero(buffer, 256);
    n = read(newsockfd, buffer, 255);
    if (n < 0) error("ERROR reading from socket");

    printf("Client: %s\n", buffer);

    close(newsockfd);
    return NULL;
}
```
`client_handler`: A function that handles communication with the client. It reads data from the client into `buffer` and prints the received message. It then closes the client's socket and exits the thread.

### Main Function Setup

- `int sockfd, newsockfd, portno;`: Declares variables for the socket file descriptors (`sockfd`, `newsockfd`) and the port number (`portno`).
- `struct sockaddr_in serv_addr, cli_addr;`: Declares structures to hold the internet address of the server (`serv_addr`) and the client (`cli_addr`).
- `pthread_t thread;`: Declares a POSIX thread for handling client connections.

### Creating a Socket

```c
sockfd = socket(AF_INET, SOCK_STREAM, 0);
if (sockfd < 0) 
    error("ERROR opening socket");
```
`socket()`: Creates a new socket. `AF_INET` specifies the IPv4 Internet protocols. `SOCK_STREAM` indicates a TCP socket. Returns a socket file descriptor.

### Setting up the Server Address

```c
bzero((char *)&serv_addr, sizeof(serv_addr));
portno = 12345;
serv_addr.sin_family = AF_INET;
serv_addr.sin_addr.s_addr = INADDR_ANY;
serv_addr.sin_port = htons(portno);
```
- `bzero`: Clears the `serv_addr` structure.
- `sin_family`: Sets the address family to IPv4.
- `sin_addr.s_addr`: Allows the server to accept connections from any IP address.
- `sin_port`: Sets the port number, converted to network byte order using `htons`.

### Binding the Socket

```c
if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    error("ERROR on binding");
```
`bind()`: Binds the socket to the specified address (`serv_addr`).

### Listening for Connections

```c
listen(sockfd, 5);
```
`listen()`: Marks the socket as a passive socket that will be used to accept incoming connection requests. `5` is the backlog, the maximum number of pending connections.

### Accepting Connections and Creating Threads

```c
while (1) {
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0) error("ERROR on accept");

    if (pthread_create(&thread, NULL, client_handler, (void *)&newsockfd) < 0)
        error("ERROR creating thread");

    pthread_detach(thread);
}
```
This is the main loop of the server. It accepts connections from clients and creates a new thread for each client using `pthread_create`. The `client_handler` function is used as the start routine for the new thread. The `newsockfd` is passed as an argument to the `client_handler` function. `pthread_detach` is called to automatically release resources when the thread finishes.

### Closing the Server Socket

```c
close(sockfd);
```
Closes the server socket. Note that this line may never be reached in this setup because the server runs in an infinite loop.

### Summary

This code creates a TCP server that listens on a specified port (`12345`), accepts multiple client connections, reads a message from each client, prints it, and then closes the connection. This server can handle multiple clients concurrently by creating a new thread for each client. The server continues to listen for new connections until it is manually stopped. If any error occurs during this process, the program will print an error message and exit with a status code of 1. The server can be stopped by pressing Ctrl+C in the terminal where it is running. The server does not send any response back to the client in this setup. This is a basic example and does not include any error handling for client disconnections or other potential issues that might arise in a real-world application. Please modify and use this code according to your needs.