////
//// Created by ebon1 on 10/12/18.
////
//
//#include "utils.h"
//
//
//int make_socket (uint16_t port)
//{
//    int sock;
//    struct sockaddr_in name;
//
//
//    /* Create the socket. */
//    sock = socket (PF_INET, SOCK_STREAM, 0);
//    if (sock < 0)
//    {
//        perror ("socket");
//        exit (EXIT_FAILURE);
//    }
//
//    /* Give the socket a name. */
//    name.sin_family = AF_INET;
//    name.sin_port = htons (port);
//    name.sin_addr.s_addr = htonl (INADDR_ANY);
//    if (bind (sock, (struct sockaddr *) &name, sizeof (name)) < 0)
//    {
//        perror ("bind");
//        exit (EXIT_FAILURE);
//    }
//
//    return sock;
//}
//
