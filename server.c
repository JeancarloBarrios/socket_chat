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

#include <time.h>


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

#define TRUE    1
#define FALSE   0
#define PORT    8080
#define POLL_SIZE 32
#define LISTEN_QUEUE 5

void waitFor (unsigned int secs) {
    unsigned int retTime = time(0) + secs;   // Get finishing time.
    while (time(0) < retTime);               // Loop until it arrives.
}

struct user {
    int id;
    char* name;
    char* status;
    LIST_ENTRY(user) pointers;
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
    char id[20];
    sprintf(id, "%d", user.id);
    tmp = json_object_new_string(id);
    json_object_object_add(user_obj, "id", tmp);
    tmp = json_object_new_string(user.name);
    json_object_object_add(user_obj, "name", tmp);
    tmp = json_object_new_string(user.status);
    json_object_object_add(user_obj, "status", tmp);
    return user_obj;
}

struct json_object * change_status(struct user user){
    struct json_object *object, *tmp;
    object = json_object_new_object();
    tmp = json_object_new_string("CHANGED_STATUS");
    json_object_object_add(object, "action", tmp);
    json_object_object_add(object, "user", user_json_obj(user));
    return object;
}


struct json_object * list_user_base(struct json_object *users){
    struct json_object *tmp, *object;
    object = json_object_new_object();
    tmp = json_object_new_string("LIST_USER");
    json_object_object_add(object, "action", tmp);
    json_object_object_add(object, "users", users);
    return object;
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

struct json_object * status_ok(){
    struct json_object *tmp, *object;
    object = json_object_new_object();
    tmp = json_object_new_string("OK");
    json_object_object_add(object, "status", tmp);
    return object;
}

//struct user * reducer(struct user state[], json_object * obj){
//    struct json_object *tmp, *object;
//    if (json_object_object_get_ex(obj, "action", &tmp) == 1){
//
//    } else if (json_object_object_get_ex(obj, "user", &tmp) == 1 && json_object_object_get_ex(obj, "origin", &tmp) == 1 ){
//
//    }
//}



int main(int argc, char *argv[]) {
//    JSON TEST
    struct json_object *jobj, *tmp, *tmp2;
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
    if (json_object_object_get_ex(jobj, "test2", &test) != 0){

        printf("%s", json_object_to_json_string(test));
    }
    else {
        printf("Key Does not exisst\n");
    }

//    LIST TEST
//    LIST_HEAD(user_list, user) head;
//    struct listhead *headp;
//    LIST_INIT(&head);
//    struct user *n1, *n2, *np;
////    LIST_ENTRY(user) name;
//    n1 = malloc(sizeof(struct user));
//    n1->id = 10;
//    LIST_INSERT_HEAD(&head, n1, pointers);
//    n2 = malloc(sizeof(struct user));      /* Insert after. */
//    n2->id = 20;
//    LIST_INSERT_AFTER(n1, n2, pointers);
//    LIST_FOREACH(np, &head, pointers)
//        printf("ID is: %d \n", np -> id);
//
////    LIST_REMOVE(n1, pointers);
//    LIST_FOREACH(np, &head, pointers)
//        printf("ID is: %d \n", np -> id);
//
//    n1 = LIST_FIRST(&head);
//    do {
//        if (n1 ->id == 20){
//            break;
//        }
//        n2 = LIST_NEXT(n1, pointers);
//        n1 = n2;
//    } while (n1 != NULL);
//    printf("THE VALUE was found : %d \n", n1->id);



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
    char * action, response;
    char* ok = json_object_to_json_string(status_ok());

    //  List Init
    LIST_HEAD(user_list, user) head;
    struct listhead *headp;
    LIST_INIT(&head);
    struct user *node_1, *node_2, *node_tmp;

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
                    memset(&buffer, 0, sizeof(buffer));
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
                        node_1 = LIST_FIRST(&head);
                        char * response;
                        while (node_1 != NULL)
                        {
                            if (node_1->id == poll_set[i].fd)
                            {
                                response = json_object_to_json_string(new_user_action(*node_1, "USER_DISCONNECTED"));
                                break;
                            }
                            node_2 = LIST_NEXT(node_1, pointers);
                            if (node_2 == NULL){
                                break;
                            }
                            node_1 = node_2;
                        }
                        LIST_REMOVE(node_1, pointers);
                        free(node_1);
                        node_1 = LIST_FIRST(&head);
                        while (node_1 != NULL)
                        {
                            if (send(node_1->id, response, strlen(response), 0) < 0){
                                perror("  send() failed");
                                close_conn = TRUE;
                                break;
                            }
                            node_2 = LIST_NEXT(node_1, pointers);
                            if (node_2 == NULL){
                                break;
                            }
                            node_1 = node_2;
                        }
                        close(poll_set[i].fd);
                        break;
                    }
                    len = rx;
                    jobj = json_tokener_parse(buffer);
                    printf("  %d bytes received\n", len);
                    printf("JSON is:  %s \n", json_object_to_json_string(jobj));
                    //  Posible Actions to Take
                    if (strcmp (buffer, "bye") == 0){
                        if (send(poll_set[i].fd, buffer, rx, 0) < 0){
                            perror("  send() failed");
                            close_conn = TRUE;
                            break;
                        }
                        node_1 = LIST_FIRST(&head);
                        while (node_1 != NULL)
                        {
                            if (node_1->id == poll_set[i].fd)
                            {
                                LIST_REMOVE(node_1, pointers);
                                free(node_1);
                                break;
                            }
                            node_2 = LIST_NEXT(node_1, pointers);
                            if (node_2 == NULL){
                                break;
                            }
                            node_1 = node_2;
                        }
                        close(poll_set[i].fd);
                    }
                    if (json_object_object_get_ex(jobj, "action", &tmp) == 1){
                        action = json_object_get_string(tmp);
                        printf("ACTION: %s", action);
                        if (strcmp (action, "SEND_MESSAGE") == 0)
                        {
                            struct message *m;
                            m = malloc(sizeof(struct message));
                            m->action = "RECEIVE_MESSAGE";
                            json_object_object_get_ex(jobj, "from", &tmp);
                            m->from = json_object_get_string(tmp);
                            json_object_object_get_ex(jobj, "to", &tmp);
                            m->to = json_object_get_string(tmp);
                            json_object_object_get_ex(jobj, "message", &tmp);
                            m->message = json_object_get_string(tmp);
                            tmp = message_json(*m);
                            node_1 = LIST_FIRST(&head);
                            while (node_1 != NULL){
                                printf("Check node %d : %d \n", node_1->id, atoi(m->to));
                                if (node_1->id == atoi(m->to)){
                                    char * response;
                                    response = json_object_to_json_string(message_json(*m));
                                    if (send(node_1->id, response, strlen(response), 0) < 0){
                                        perror("  send() failed");
                                        close_conn = TRUE;
                                        break;
                                    }
                                    printf("RESPOND OK: %s \n", ok);
                                    if (send(poll_set[i].fd, ok, strlen(ok), 0) < 0){
                                        perror("  send() failed");
                                        close_conn = TRUE;
                                        break;
                                    }
                                    break;
                                }
                                node_2 = LIST_NEXT(node_1, pointers);
                                if (node_2 == NULL){
                                    break;
                                }
                                node_1 = node_2;
                            }
                        }
                        if (strcmp (action, "LIST_USER") == 0)
                        {

                            struct json_object *array;
                            array = json_object_new_array();
                            node_1 = LIST_FIRST(&head);
                            while (node_1 != NULL)
                            {
                                json_object_array_add(array, user_json_obj(*node_1));
                                node_2 = LIST_NEXT(node_1, pointers);
                                if (node_2 == NULL){
                                    break;
                                }
                                node_1 = node_2;
                            }
                            char * response = json_object_to_json_string(list_user_base(array));
                            printf("LIST_USER, %s", response);
                            if (send(poll_set[i].fd, response, strlen(response), 0) < 0){
                                perror("  send() failed");
                                close_conn = TRUE;
                                break;
                            }

                        }
                        if (strcmp (action, "CHANGE_STATUS") == 0)
                        {
                            json_object_object_get_ex(jobj, "user", &tmp);
                            int user_id = strtol(json_object_get_string(tmp), NULL, 10);
                            json_object_object_get_ex(jobj, "status", &tmp);
                            char * new_status = json_object_get_string(tmp);
                            node_1 = LIST_FIRST(&head);
                            while (node_1 != NULL)
                            {
                                if (node_1->id == user_id){
                                    node_1->status = new_status;
                                    break;
                                }

                                node_2 = LIST_NEXT(node_1, pointers);
                                if (node_2 == NULL){
                                    break;
                                }
                                node_1 = node_2;
                            }
                            char* change_status_response = json_object_to_json_string(change_status(*node_1));
                            if (send(poll_set[i].fd, ok, strlen(ok), 0) < 0){
                                perror("  send() failed");
                                close_conn = TRUE;
                                break;
                            }
                            node_1 = LIST_FIRST(&head);
                            while (node_1 != NULL) {
                                if (node_1->id == poll_set[i].fd){
                                    node_2 = LIST_NEXT(node_1, pointers);
                                    if (node_2 == NULL){
                                        break;
                                    }
                                    node_1 = node_2;
                                    continue;
                                }
                                printf("RESPONSE IS: %s \n", change_status_response);
                                if (send(node_1->id, change_status_response, strlen(change_status_response), 0) < 0){
                                    perror("  send() failed");
                                    close_conn = TRUE;
                                    break;
                                }
                                node_2 = LIST_NEXT(node_1, pointers);
                                if (node_2 == NULL){
                                    break;
                                }
                                node_1 = node_2;
                            }

                        }

                    } else if (json_object_object_get_ex(jobj, "origin", &tmp) == 1 && json_object_object_get_ex(jobj, "user", &tmp) == 1){
                        printf("ADD USER TO LIST \n");
                        node_tmp = malloc(sizeof(struct user));
                        node_tmp->id = poll_set[i].fd;
                        node_tmp->name = json_object_get_string(tmp);
                        node_tmp->status = "active";

                        if (LIST_EMPTY(&head) == 1) {
                            printf("FIRST USER ADDED \n");
                            LIST_INSERT_HEAD(&head, node_tmp, pointers);
                            printf("LIST INSERT \n");
                        }
                        else {
                            int exist = FALSE;
                            node_1 = LIST_FIRST(&head);
                            while (node_1 != NULL) {
                                if (node_1 ->id == node_tmp->id){
                                    exist = TRUE;
                                    break;
                                }
                                node_2 = LIST_NEXT(node_1, pointers);
                                if (node_2 == NULL){
                                    break;
                                }
                                node_1 = node_2;
                            }
                            if (exist == FALSE){
                                LIST_INSERT_BEFORE(node_1, node_tmp, pointers);
                            }
                        }
//                        response = malloc(sizeof(response));
                        char * response;
                        response = json_object_to_json_string(accept_user(*node_tmp));
                        printf("RESPONSE IS: %s \n", response);
                        if (send(poll_set[i].fd, response, strlen(response), 0) < 0){
                            perror("  send() failed");
                            close_conn = TRUE;
                            break;
                        }

                        node_1 = LIST_FIRST(&head);

                        response = json_object_to_json_string(new_user_action(*node_tmp, "USER_CONNECTED"));
                        while (node_1 != NULL) {
                            if (node_1->id == poll_set[i].fd){
                                node_2 = LIST_NEXT(node_1, pointers);
                                if (node_2 == NULL){
                                    break;
                                }
                                node_1 = node_2;
                                continue;
                            }
                            printf("RESPONSE IS: %s \n", response);
                            if (send(node_1->id, response, strlen(response), 0) < 0){
                                perror("  send() failed");
                                close_conn = TRUE;
                                break;
                            }
                            node_2 = LIST_NEXT(node_1, pointers);
                            if (node_2 == NULL){
                                break;
                            }
                            node_1 = node_2;
                        }


                    }
                    else
                    {
                        char * response;
                        struct response_error *error_struct;
                        error_struct = malloc(sizeof(struct message));
                        error_struct->status = "ERROR";
                        error_struct->message = "Invalid Action";
                        response = json_object_to_json_string(error_json(*error_struct));
                        printf("RESPONSE IS: %s \n", response);
                        if (send(poll_set[i].fd, response, strlen(response), 0) < 0){
                            perror("  send() failed");
                            close_conn = TRUE;
                            break;
                        }
                    }
                    LIST_FOREACH(node_tmp, &head, pointers)
                    printf("ID in Node is: %d \n", node_tmp -> id);
//                    tx = send(poll_set[i].fd, buffer, rx, 0);
//                    if (send(poll_set[i].fd, buffer, rx, 0) < 0){
//                        perror("  send() failed");
//                        close_conn = TRUE;
//                        break;
//                    }

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