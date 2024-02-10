#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

int main() {
    // Create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Define server address
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);  // Replace with your server's port
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Replace with your server's IP

    // Set socket options

    // 1. Set socket timeout (in seconds)
    struct timeval timeout;
    timeout.tv_sec = 5;  // 5 seconds
    timeout.tv_usec = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == -1) {
        perror("Set socket timeout failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // 2. Enable socket reuse (allows reusing the same address)
    int reuse = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) {
        perror("Set socket reuse failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // 3. Set receive buffer size
    int recv_buffer_size = 1024 * 1024;  // 1 MB
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &recv_buffer_size, sizeof(recv_buffer_size)) == -1) {
        perror("Set receive buffer size failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // 4. Set send buffer size
    int send_buffer_size = 1024 * 1024;  // 1 MB
    if (setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &send_buffer_size, sizeof(send_buffer_size)) == -1) {
        perror("Set send buffer size failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Socket options set successfully.\n");

    // Close the socket
    close(sockfd);

    return 0;
}
