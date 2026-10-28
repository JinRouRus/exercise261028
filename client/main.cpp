#include <iostream>
#include <netinet/in.h>
#include <signal.h>
#include <sys/un.h>
#include <unistd.h>

#include "../packet_info.h"

int main() {
    struct sockaddr_in socket_info;
    memset(&socket_info, 0, sizeof(socket_info));
    socket_info.sin_family = AF_INET;
    socket_info.sin_port = htons(SOCKET_PORT);
    socket_info.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // 127.0.0.1

    char buffer[] = "UDP\0"; //4 bytes 

    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

    sendto(socket_fd, buffer, 4, 0, (struct sockaddr *)&socket_info, sizeof(socket_info));

    close(socket_fd);

    return 0;
}
