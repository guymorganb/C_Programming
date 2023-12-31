#include <stdio.h>
#include <stdlib.h>

int main() {
    // Open the /proc/meminfo file for reading
    FILE *fp = fopen("/proc/meminfo", "r");
    // Check if the file was opened successfully
    if (fp == NULL) {
        // Print an error message if the file couldn't be opened
        perror("Error opening file");
        return -1; // Return with an error code
    }

    char buffer[256]; // Buffer to store lines read from the file
    // Read the file line by line
    while (fgets(buffer, sizeof(buffer), fp)) {
        printf("%s", buffer); // Print each line to the console
    }

    fclose(fp); // Close the file
    return 0; // Return success
}