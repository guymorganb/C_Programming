## TCP2 (basic server) API in C

Explaination of each line of the code:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h> // header, which is typically used in Unix/Linux for definitions and functions related to internet operations. It's common in network programming to deal with IP addresses and ports.
```
These are the header files included in the program. They contain functions and macros that the program uses.

```c
#define MAX_BUFFER 1024
#define PORT 8080
```
These lines define constants that will be used in the program. `MAX_BUFFER` is the maximum size of the buffer that will store data received from the client, and `PORT` is the port number on which the server will listen for connections.

```c
int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[MAX_BUFFER] = {0};
    char *message = "Hello from server!\n";
```
This is the start of the `main` function. It declares some variables that will be used later. `server_fd` is the file descriptor of the listening server socket, `new_socket` is the file descriptor of a socket for a new client connection, `address` is a structure that will hold the server's IP address and port, `addrlen` is the size of `address`, `buffer` is an array that will store data received from a client, and `message` is a string that will be sent to a client when it connects.

```c
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
```
This block creates a new socket and assigns its file descriptor to `server_fd`. If the socket creation fails, it prints an error message and exits the program.

```c
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
```
This block sets the fields of the `address` structure. `sin_family` is set to `AF_INET` to indicate that the server will use IPv4. `sin_addr.s_addr` is set to `INADDR_ANY` to indicate that the server can accept connections sent to any of the host's IP addresses. `sin_port` is set to the port number defined earlier, converted to network byte order with the `htons` function.

```c
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
```
This block binds the server socket to the specified IP address and port. If the binding fails, it prints an error message and exits the program.

```c
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
```
This block makes the server socket listen for incoming client connections. If the listening fails, it prints an error message and exits the program.

```c
    printf("Listening on port %d \n", PORT);
```
This line prints a message to the console indicating that the server is listening for connections.

```c
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }
```
This block makes the server accept a new client connection. If the acceptance fails, it prints an error message and exits the program.

```c
    send(new_socket, message, strlen(message), 0);
    printf("Welcome message sent to client \n");
```
This block sends the welcome message to the client and prints a message to the console indicating that the message has been sent.

```c
    return 0;
}
```
This is the end of the `main` function. It returns 0 to indicate that the program has executed successfully.