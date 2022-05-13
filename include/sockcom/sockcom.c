//
// Created by ethan on 09/05/2022.
//

#include "sockcom.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cjson/cJSON.h>

#define MAX_LINE 30

int
sockcom_send_welcome(char* IP, char* PORT)
{
    // GET ADDRINFO
    struct addrinfo hints;
    struct addrinfo *response_info;
    memset(&hints, 0, sizeof hints);

    if(getaddrinfo(IP,PORT,&hints,&response_info) == -1)
    {
        return -1;
    }

    // CREATE SOCKET
    int sock = socket(
            response_info->ai_family,
            response_info->ai_socktype,
            response_info->ai_protocol
            );

    // CONNECT
    if(connect(sock,response_info->ai_addr,response_info->ai_addrlen) == -1)
    {
        return -1;
    }

    // DATA SEND
    cJSON *js_object = cJSON_CreateObject();
    cJSON *js_text =  NULL;

    js_text = cJSON_CreateString("Hello world");

    cJSON_AddItemToObject(
            js_object,
            "text",
            js_text
            );

    char text[MAX_LINE] = "hey";
    send(sock,cJSON_PrintUnformatted(js_object),strlen(cJSON_PrintUnformatted(js_object)),0);


    // WAIT AND PROCESS DATA
    char* welcome_response = malloc(sizeof(char) * 12);
    recv(sock,welcome_response,12,0);
    for(int i=0; welcome_response[i] != '\0'; i++)
    {
        fprintf(stdout,"%c",welcome_response[i]);
    }
    return sock;
}