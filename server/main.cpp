#include <iostream>
#include <netinet/in.h>
#include <signal.h>
#include <sys/un.h>
#include <unistd.h>

#include "../packet_info.h"

int stop_flag;

void signal_handler(int signum) {
    std::cout << "Shutting down..." << std::endl;
    stop_flag = 1;
}

int main() {

    stop_flag = 0;
    struct sigaction action;
    action.sa_handler = signal_handler;
    sigaction(SIGINT, &action, NULL);

    struct sockaddr_in socket_info;
    memset(&socket_info, 0, sizeof(socket_info));
    socket_info.sin_family = AF_INET;
    socket_info.sin_addr.s_addr = htonl(INADDR_ANY);
    socket_info.sin_port = htons(SOCKET_PORT);

    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(socket_fd == -1) return -1;
    
    if(bind(socket_fd, (struct sockaddr *) &socket_info, sizeof(socket_info)) == -1)  return -2;

    char buffer[BUFFER_SIZE];
    unsigned long int counter = 0;
    while(stop_flag == 0){
        struct sockaddr_in client_info;
        socklen_t client_len = sizeof(client_info);

        ssize_t received = recvfrom(socket_fd, buffer, BUFFER_SIZE - 1, 0, (struct sockaddr *)&client_info, &client_len);
        if(received > 0) {
            std::cout << "UDP recieved. Counter = " << ++counter << std::endl;
        }
        if(received < 0) break;
    }

    std::cout << "Server loop stop." << std::endl;

    close(socket_fd);

    return 0;
}
