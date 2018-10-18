//
// Created by ebon1 on 10/12/18.
//

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <poll.h>

#include <json.h>
#include <sys/queue.h>
#include <string.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

#define TRUE    1
#define FALSE   0
#define PORT    8080
#define POLL_SIZE 32
#define LISTEN_QUEUE 5

struct user {
    int id;
    char* name;
    char* status;
};

struct message {
    char* action;
    char* from;
    char* to;
    char* message;
};

struct response_error {
    char* status;
    char* message;
};

struct json_object * message_json(struct message message){
    struct json_object *tmp, *obj;
    obj = json_object_new_object();
    tmp = json_object_new_string(message.action);
    json_object_object_add(obj, "action", tmp);
    tmp = json_object_new_string(message.from);
    json_object_object_add(obj, "from", tmp);
    tmp = json_object_new_string(message.to);
    json_object_object_add(obj, "to", tmp);
    tmp = json_object_new_string(message.message);
    json_object_object_add(obj, "message", tmp);
    return obj;
}

struct json_object * error_json(struct response_error error){
    struct json_object *tmp, *obj;
    obj = json_object_new_object();
    tmp = json_object_new_string(error.status);
    json_object_object_add(obj, "status", tmp);
    tmp = json_object_new_string(error.message);
    json_object_object_add(obj, "message", tmp);
    return obj;
}

struct json_object * user_json_obj(struct user user){
    struct json_object *tmp, *user_obj;
    user_obj = json_object_new_object();
    tmp = json_object_new_int(user.id);
    json_object_object_add(user_obj, "id", tmp);
    tmp = json_object_new_string(user.name);
    json_object_object_add(user_obj, "name", tmp);
    tmp = json_object_new_string(user.status);
    json_object_object_add(user_obj, "status", tmp);
    return user_obj;
}

struct json_object * new_user_action(struct user user, char *action){
    struct json_object *tmp, *object;
    object = json_object_new_object();
    tmp = json_object_new_string(action);
    json_object_object_add(object, "action", tmp);
    json_object_object_add(object, "user", user_json_obj(user));
    return object;
}

struct json_object * accept_user(struct user user){
    struct json_object *tmp, *object;
    object = json_object_new_object();
    tmp = json_object_new_string("OK");
    json_object_object_add(object, "status", tmp);
    json_object_object_add(object, "user", user_json_obj(user));
    return object;
}

struct user * reducer(struct user state[], json_object * obj){
    struct json_object *tmp, *object;
    if (json_object_object_get_ex(obj, "action", &tmp) == 1){

    } else if (json_object_object_get_ex(obj, "user", &tmp) == 1 && json_object_object_get_ex(obj, "origin", &tmp) == 1 ){

    }
}

int main(int argc, char *argv[]) {
//    JSON TEST
    struct json_object *jobj;
//    char *str = "{\"test\": \"test\"}";
    struct user user;
    user.status = "active";
    user.id = 1;
    user.name = "test";
    const char *str = json_object_to_json_string(new_user_action(user, "THE_ACTION"));
    printf("str:\n---\n%s\n---\n\n", str);

    jobj = json_tokener_parse(str);
    struct json_object* test;
//    json_object_object_get_ex(jobj, "test", &test);
    if (json_object_object_get_ex(jobj, "test2", &test) != NULL){

        printf("%s", json_object_to_json_string(test));
    }
    else {
        printf("Key Does not exisst");
    }

//    LIST TEST



    // SERVER CODE

    int master_socket, max_clients;
    int opt = TRUE;
    char buffer[1025];
    char server_welcome_message[256] = "Welcome to Machetes Server!";
    if (1) {
        max_clients = 10;
    } else {
        max_clients = 30;
    }
    uint16_t port;
    if (1) {
        port = PORT;
    } else {
        port = 8000;
    }


    if ((master_socket = socket(AF_INET, SOCK_STREAM|SOCK_NONBLOCK, 0)) == 0){
        perror("SOCKET FAILED");
        exit(EXIT_FAILURE);
    }

    if ( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 ) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in socket_address;
    memset(&socket_address, 0, sizeof(socket_address));
    socket_address.sin_family = AF_INET;
    socket_address.sin_port = htons(port);
    socket_address.sin_addr.s_addr = INADDR_ANY;

    if ( bind(master_socket, (struct sockaddr *)&socket_address, sizeof(socket_address)) < 0) {
        perror("Failed on Bind");
        close(master_socket);
        exit(EXIT_FAILURE);
    }


    if (listen(master_socket, 10) < 0){
        perror("Failed on Listen");
        close(master_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server on Port %d \n", port);
    int address_length = sizeof(socket_address);
    puts("Machete Server: Waiting For Connections ...");
    int new_sd = -1;
    struct pollfd poll_set[max_clients];
    int num_fds = 1;
    memset(poll_set, 0, sizeof(poll_set));
    poll_set[0].fd = master_socket;
    poll_set[0].events = POLLIN;
    int timeout = (1000 * 60 * 10);
    int compress_array = FALSE;
    int poll_flag, current_size, close_conn, rx, tx, len;
    int end_server = FALSE;

    while(end_server == FALSE) {
        printf("Starting POLL...\n");
        poll_flag = poll(poll_set, num_fds, timeout);
        if (poll_flag < 0){
            perror("  poll() failed");
            break;
        }
        if (poll_flag == 0){
            perror("Poll Timeout");
            break;
        }
        current_size = num_fds;
        for (int i =0; i < current_size; i++){
            printf("ITS in main for FD = %d\n", poll_set[i].fd);
            if(poll_set[i].revents == 0){
                continue;
            }
            if(poll_set[i].revents != POLLIN){
                printf("  This was not an entry = %d\n", poll_set[i].revents);
                end_server = TRUE;
                break;
            }
            if (poll_set[i].fd == master_socket){
                printf(" Listening socket is readable\n");

                do {
                    new_sd = accept(master_socket, (struct sockaddr *)&socket_address, (socklen_t *)&address_length);
                    if (new_sd < 0){
                        if (errno != EWOULDBLOCK){
                            perror("  accept() failed");
                            end_server = TRUE;
                        }
                        break;
                    }
                    printf("  New incoming socket - %d\n", new_sd);
                    poll_set[num_fds].fd = new_sd;
                    poll_set[num_fds].events = POLLIN;
                    num_fds++;
                } while (new_sd != -1);
            }
            else {
                printf("  Descriptor %d is readable\n", poll_set[i].fd);
                close_conn = FALSE;

                do {
                    rx = recv(poll_set[i].fd, buffer, sizeof(buffer), 0);
                    printf("  Receive data %d for fd \n", poll_set[i].fd);
                    if (rx < 0) {
                        if (errno != EWOULDBLOCK){
                            perror("  recv() failed");
                            close_conn = TRUE;
                        }
                        break;
                    }
                    if (rx == 0)
                    {
                        printf("  Connection closed\n");
                        close_conn = TRUE;
                        break;
                    }
                    len = rx;
                    jobj = json_tokener_parse(buffer);
                    printf("  %d bytes received\n", len);
                    printf("JSON is:  %s \n", json_object_to_json_string(jobj));
                    if (json_object_object_get_ex(jobj, "user", &test) != NULL){

                        printf("Key exists congrats %s\n", json_object_to_json_string(test));
                    }
                    else {
                        printf("Key Does not exisst");
                    }
//                    tx = send(poll_set[i].fd, buffer, rx, 0);
                    if (send(poll_set[i].fd, buffer, rx, 0) < 0){
                        perror("  send() failed");
                        close_conn = TRUE;
                        break;
                    }

                    if (TRUE){
                        break;
                    }
                } while (TRUE);

                if (close_conn)
                {
                    close(poll_set[i].fd);
                    poll_set[i].fd = -1;
                    compress_array = TRUE;
                }
            }
        }
        if (compress_array){
            compress_array = FALSE;
            for (int i =0; i < num_fds; i++){
                if (poll_set[i].fd == -1)
                {
                    for(int j = i; j < num_fds; j++)
                    {
                        poll_set[j].fd = poll_set[j+1].fd;
                    }
                    i--;
                    num_fds--;
                }
            }
        }
    }
    for (int i = 0; i < num_fds; i++)
    {
        if(poll_set[i].fd >= 0)
            close(poll_set[i].fd);
    }



    return 0;
}
#pragma clang diagnostic pop