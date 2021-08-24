#include<stdio.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<stdlib.h>
#include<unistd.h>

int server (int client_socket) {
    while(1) {
        int length;
        char* text;

        if(read(client_socket, &length, sizeof(length)) == 0)
            return 0;
        text = (char *) malloc(length);

        read(client_socket, text, length);
        printf("%s\n", text);

        free(text);

        if(strcmp(text, "quit"))
            return 0;
    }
}

int main(int argc, char **argv) {
    const char *const socket_name = argv[1];
    int socket_fd;
    struct sockaddr_un name;
    
    socket_fd = socket(PF_LOCAL, SOCK_STREAM, 0);
    name.sun_family = AF_LOCAL;
    strcpy(name.sun_path, socket_name);
    bind(socket_fd, &name, SUN_LEN(&name));

    listen(socket_fd, 5);

    do {
        struct sockaddr_un client_name;
        socklen_t client_name_len;
        int client_socket_fd;

        client_socket_fd = accept(socket_fd, &client_name, &client_name_len);

        pid_t child_pid = fork();
        if(!child_pid) {
            server(client_socket_fd);
            return 0;
        }
    } while(1);
}