//
// Created by ethan on 13/05/2022.
//
#include "namecom.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT "7027"

int namecom_get_address(char* name)
{
    char *message_fmt = "GET /Lookup/get/%s HTTP/1.1\r\nHost: %s:%s\r\n";

    // Inits
    int get_result, sock, connect_result;
    char message[1024];
    struct addrinfo *response, hints;
    memset(&hints,0,sizeof(hints));

    // Process of connecting to socket
    get_result = getaddrinfo(SERVER_IP,SERVER_PORT,&hints,&response);
    if(get_result == -1){return -1;}

    sock = socket(
            response->ai_family,
            response->ai_socktype,
            response->ai_protocol
            );
    if(sock == -1){return -1;}

    connect_result = connect(sock,response->ai_addr,response->ai_addrlen);
    if(connect_result == -1){return -1;}

    // Building the header + sending data
    sprintf(message,message_fmt,name,SERVER_IP,SERVER_PORT);
    send(sock,message,strlen(message),0);



}