////
//// Created by ebon1 on 10/12/18.
////
//
//#include <stdio.h>
//#include <errno.h>
//#include <stdlib.h>
//#include <unistd.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <netdb.h>
//#include <poll.h>
//
//#define TRUE    1
//#define FALSE   0
//#define PORT    8080
//
//int main(int argc, char *argv[]) {
//    int master_socket, max_clients;
//    int opt = TRUE;
//    char buffer[1025];  //data buffer of 1K
//    char server_message[256] = "Welcome to Machetes Server!";
//    if (1) {
//        max_clients = 10;
//    } else {
//        max_clients = 30;
//    }
//    uint16_t port;
//    if (1) {
//        port = PORT;
//    } else {
//        port = 8000;
//    }
//    int client_socket[max_clients];
//
//    for (int i = 0; i < max_clients; i++){
//        client_socket[i] = 0;
//    }
//
//    if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0){
//        perror("SOCKET FAILED");
//        exit(EXIT_FAILURE);
//    }
//
//    if ( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 ) {
//        perror("setsockopt failed");
//        exit(EXIT_FAILURE);
//    }
//
//    struct sockaddr_in socket_address;
//    socket_address.sin_family = AF_INET;
//    socket_address.sin_port = htons(port);
//    socket_address.sin_addr.s_addr = INADDR_ANY;
//
//    if ( bind(master_socket, (struct sockaddr *)&socket_address, sizeof(socket_address)) < 0) {
//        perror("Failed on Bind");
//        exit(EXIT_FAILURE);
//    }
//
//    if (listen(master_socket, 3) < 0)
//    {
//        perror("listen");
//        exit(EXIT_FAILURE);
//    }
//    printf("Server on Port %d \n", port);
//    int address_length = sizeof(socket_address);
//    puts("Machete Server: Waiting For Connections ...");
//
//    fd_set set_fd;
//    int max_sd, sd, socket_activity, new_socket;
//    while (1) {
//        FD_ZERO(&set_fd);
//        FD_SET(master_socket, &set_fd);
//        max_sd = master_socket;
//        for (int i = 0; i < max_clients; i++){
//            sd = client_socket[i];
//
//            if (sd >0) {
//                FD_SET(sd, &set_fd);
//            }
//            if (sd > max_sd){
//                max_sd = sd;
//            }
//        }
//        socket_activity = select(max_sd + 1, &set_fd, NULL, NULL, NULL);
////        socket_activity = poll();
//        if ((socket_activity < 0) && (errno!=EINTR))
//        {
//            printf("Error on Select");
//        }
//
//        if (FD_ISSET(master_socket, &set_fd)) {
//            if (( new_socket = accept(master_socket, (struct sockaddr *)&socket_address, (socklen_t *)&address_length)) <0 ){
//                printf("Something wrong with accept");
//                perror("Accept new Connection");
//                exit(EXIT_FAILURE);
//            }
//            if( send(new_socket, server_message, sizeof(server_message), 0) != sizeof(server_message) )
//            {
//                perror("send");
//            }
//            for (int i = 0; i < max_clients; i++)
//            {
//                //if position is empty
//                if( client_socket[i] == 0 )
//                {
//                    client_socket[i] = new_socket;
//                    printf("Adding to list of sockets as %d\n" , i);
//
//                    break;
//                }
//            }
//
//        }
//        int read_value;
//        for (int i = 0; i < max_clients; i++){
//            sd = client_socket[i];
//            if (FD_ISSET(sd, &set_fd)) {
//                if ((read_value = read(sd, buffer, 1024)) ==0) {
//                    getpeername(sd, (struct sockaddr*)&socket_address, (socklen_t*)&socket_address);
//
//                    close(sd);
//                    client_socket[i] = 0;
//                }
//                else {
//                    buffer[read_value] = '\0';
//                    send(sd, buffer, sizeof(buffer), 0);
//                }
//            }
//        }
//
//
//    }
//
//    return 0;
//}