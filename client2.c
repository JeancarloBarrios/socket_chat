//
// Created by ebon1 on 10/18/18.
//

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>

#include <json.h>
#include <sys/queue.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define username "user 1"

#define PORT 8080

void prompt(){
    printf("Welcome TO Machetes client\n");
    printf("Imput 1 if you want to send a message\n");
    printf("Input 2 if you want to change status \n");
    printf("Input 3 if you want to exit \n");
}

void statuses(){
    printf("Imput 1 if active\n");
    printf("Input 2 if busy \n");
    printf("Input 3 if inactive \n");
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

struct json_object * message_json(struct message message){
    struct json_object *tmp, *obj;
    obj = json_object_new_object();
    tmp = json_object_new_string(message.action);
    json_object_object_add(obj, "action", tmp);
    tmp = json_object_new_string(message.to);
    json_object_object_add(obj, "to", tmp);
    tmp = json_object_new_string(message.from);
    json_object_object_add(obj, "from", tmp);
    tmp = json_object_new_string(message.message);
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

struct json_object * hand_shake()
{
    struct json_object *tmp, *object;
    object = json_object_new_object();
    tmp = json_object_new_string("192.168.1.2");
    json_object_object_add(object, "host", tmp);
    tmp = json_object_new_string("192.168.1.3");
    json_object_object_add(object, "origin", tmp);
    tmp = json_object_new_string(username);
    json_object_object_add(object, "user", tmp);
    return object;

}

struct json_object * change_status(char* id, char* status)
{
    struct json_object *tmp, *object;
    object = json_object_new_object();
    tmp = json_object_new_string("CHANGE_STATUS");
    json_object_object_add(object, "action", tmp);
    tmp = json_object_new_string(id);
    json_object_object_add(object, "user", tmp);
    tmp = json_object_new_string(status);
    json_object_object_add(object, "status", tmp);
    return object;

}

struct json_object * list_users_request()
{
    struct json_object *tmp, *object;
    object = json_object_new_object();
    tmp = json_object_new_string("LIST_USER");
    json_object_object_add(object, "action", tmp);
    return object;

}


int main(int argc, char *argv[]) {
    int sockfd, portno, n;
    portno = PORT;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[1025];
    char buffer2[1025];

//    if (argc < 3) {
//        fprintf(stderr,"usage %s hostname port\n", argv[0]);
//        exit(0);
//    }

//    portno = atoi(argv[2]);
    portno = PORT;
    /* Create a socket point */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

//    server = gethostbyname(argv[1]);
    server = gethostbyname("0.0.0.0");
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(PORT);

    /* Now connect to the server */
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR connecting");
        exit(1);
    }

    /* Now ask for a message from the user, this message
       * will be read by server
    */
    struct json_object *jobj, *tmp, *tmp2, *tmp3, *list_users_response;
//    LIST
    LIST_HEAD(user_list, user) head;
    struct listhead *headp;
    LIST_INIT(&head);
    struct user *node_1, *node_2, *node_tmp, *my_user;
    char * action;
//    HANDSHAKE
    char* handshake;
    handshake = json_object_to_json_string(hand_shake());
    if (write(sockfd, handshake, strlen(handshake)) < 0) {
        perror("ERROR writing to socket");
        exit(1);
    }
    bzero(buffer, 1025);
    n = read(sockfd, buffer, 1025);
    printf("response: %s \n", buffer);
    jobj = json_tokener_parse(buffer);

    char *user_id;
    printf("SERVER RESPONSE %s\n", json_object_to_json_string(jobj));
    if (json_object_object_get_ex(jobj, "status", &tmp) != 0 && strcmp(json_object_get_string(tmp), "OK") == 0){
        my_user = malloc(sizeof(struct user));
        json_object_object_get_ex(jobj, "user", &tmp2);
        json_object_object_get_ex(tmp2, "id", &tmp);
        my_user->id = atoi(json_object_get_string(tmp));
        user_id = (json_object_get_string(tmp));
        my_user->name = username;
        json_object_object_get_ex(tmp2, "status", &tmp);
        my_user->status = json_object_get_string(tmp);
    }
    else {
        printf("ERROR on handshake \n");
        exit(1);
    }
    // LIST USER
    char* action_list_user;
    action_list_user = json_object_to_json_string(list_users_request());
    if (write(sockfd, action_list_user, strlen(action_list_user)) < 0) {
        perror("ERROR writing to socket");
        exit(1);
    }
    bzero(buffer, 1025);
    n = read(sockfd, buffer, 1025);
    printf("response: %s \n", buffer);
    list_users_response = json_tokener_parse(buffer);
    if (json_object_object_get_ex(list_users_response, "action", &tmp) != 0 && strcmp(json_object_get_string(tmp), "LIST_USER") == 0){
        json_object_object_get_ex(list_users_response, "users", &tmp);
//        tmp2 = json_object_get_array(tmp);
        for (int i = 0; i < json_object_array_length(tmp); i++) {
            // Set in tmp the json_object of the secu_code array at index i
            tmp2 = json_object_array_get_idx(tmp, i);
            printf("user[%d] = %s\n", i, json_object_to_json_string(tmp2));

            node_tmp = malloc(sizeof(struct user));
            json_object_object_get_ex(tmp2, "id", &tmp3);
            node_tmp->id = atoi(json_object_get_string(tmp3));
            json_object_object_get_ex(tmp2, "name", &tmp3);
            node_tmp->name = json_object_get_string(tmp3);
            json_object_object_get_ex(tmp2, "status", &tmp3);
            node_tmp->status = json_object_get_string(tmp3);

            if (LIST_EMPTY(&head) == 1) {
                printf("FIRST USER ADDED \n");
                LIST_INSERT_HEAD(&head, node_tmp, pointers);
                printf("LIST INSERT \n");
            }
            else{
                LIST_INSERT_AFTER(node_2, node_tmp, pointers);
            }
            node_2 = node_tmp;
        }
    }
    else {
        printf("ERROR on handshake \n");
        exit(1);
    }
    LIST_FOREACH(node_tmp, &head, pointers)
        printf("ID in Node is: %d \n", node_tmp -> id);
    while (1) {
        prompt();
        bzero(buffer, 1025);
        fgets(buffer, 1025, stdin);
        char *buffer_ptr = &buffer[0];
        if (strcmp (buffer_ptr, "1\n") == 0){
            struct message *m;
            m = malloc(sizeof(struct message));
            printf("Type id of user you want to message \n");
            bzero(buffer, 1025);
            fgets(buffer, 1025, stdin);
            m->to = buffer;
            printf("Type message \n");
            bzero(buffer2, 1025);
            fgets(buffer2, 1025, stdin);
            m->from = user_id;
            m->message = buffer2;
            m->action = "SEND_MESSAGE";
            char * message_packet = json_object_to_json_string(message_json(*m));
            n = write(sockfd, message_packet, strlen(message_packet));
            if (n < 0) {
                perror("ERROR writing to socket");
                exit(1);
            }

        }else if (strcmp (buffer_ptr, "2\n") == 0){
            statuses();
            bzero(buffer, 1025);
            fgets(buffer, 1025, stdin);
            char* status;
            if (strcmp (buffer_ptr, "1\n") == 0){
                status = "active";
            } else if (strcmp (buffer_ptr, "2\n") == 0){
                status = "busy";
            } else if (strcmp (buffer_ptr, "3\n") == 0){
                status = "inactive";
            }
            char * request = json_object_to_json_string(change_status(user_id, status));
            n = write(sockfd, request, strlen(request));
            if (n < 0) {
                perror("ERROR writing to socket");
                exit(1);
            }

        }
        else if (strcmp (buffer_ptr, "3\n") == 0){
            printf("GOOD LUCK in Life");
            break;

        }

        bzero(buffer, 1025);
        n = read(sockfd, buffer, 1025);
        jobj = json_tokener_parse(buffer);
        printf("Receive data: %s", json_object_to_json_string(jobj));
        if (json_object_object_get_ex(jobj, "action", &tmp) == 1){
            action = json_object_get_string(tmp);
            if (strcmp (action, "RECEIVE_MESSAGE") == 0)
            {
                char* from, message;
                json_object_object_get_ex(jobj, "from", &tmp);
                from = json_object_get_string(tmp);
                json_object_object_get_ex(jobj, "message", &tmp);
                message =  json_object_get_string(tmp);
                printf("USER[%s] : %s ", from, message);
            }
        }
        if (n < 0) {
            perror("ERROR reading from socket");
            exit(1);
        }

        printf("%s\n",buffer);
        int i = strncmp("bye", buffer, 3);
        if(i==0)
            break;

    }
    close(sockfd);
    return 0;
}