#include <stdlib.h>
#include <string.h>
#include "csapp.h"

#define MAX_LINE 1024

void fatal(const char *msg) {
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

// Function to reverse a string
void reverse_string(char s[]) {
    size_t len = strlen(s);
    for (size_t i = 0; i < len / 2; i++) {
        char tmp = s[i];
        s[i] = s[len - i - 1];
        s[len - i - 1] = tmp;
    }
}

// Trim any trailing newlines or carriage returns
void trim_line_terminator(char s[]) {
    char *p = strchr(s, '\r');
    if (p != NULL) {
        *p = '\0';
    } else {
        p = strchr(s, '\n');
        if (p != NULL) {
            *p = '\0';
        }
    }
}

// Main function to handle communication with a client
void chat_with_client(int client_fd) {
    rio_t rio;
    char buf[MAX_LINE];
    ssize_t n;

    // Initialize robust I/O
    Rio_readinitb(&rio, client_fd);

    while (1) {
        // Read a line of text from the client
        n = Rio_readlineb(&rio, buf, MAX_LINE);
        if (n <= 0) {
            break;  // EOF or error
        }

        // Trim line terminator
        trim_line_terminator(buf);

        // Check if client wants to quit
        if (strcmp(buf, "quit") == 0) {
            printf("Server: Received 'quit'. Closing connection.\n");
            break;
        }

        // Reverse the string
        reverse_string(buf);

        // Append a newline safely
        snprintf(buf + strlen(buf), MAX_LINE - strlen(buf), "\n");

        // Send reversed string back to client
        Rio_writen(client_fd, buf, strlen(buf));

        printf("Server: Sent reversed message: %s", buf);
    }

    // Close the connection
    close(client_fd);
}
