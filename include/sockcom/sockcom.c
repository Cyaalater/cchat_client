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
#include <assert.h>
#define MAX_LINE 50

// Function to create an already built json struct
/*
 * {
 *      action: String,
 *      info: [
 *          ["text":"my man"],
 *          ["name":"menashe"]
 *      ]
 * }
 */
cJSON* JsonBuild(char* action, cJSON* info)
{
    // Check if params are okay
//    assert(cJSON_IsArray(info) == cJSON_True);

    // Inits
    cJSON *action_json = NULL;
    cJSON *main_object = cJSON_CreateObject();

    // Modifying the object
    action_json = cJSON_CreateString(action);

    cJSON_AddItemToObject(main_object,"action",action_json);
    cJSON_AddItemToObject(main_object,"info",info);

    return main_object;
}

char*
sockcom_recv_socket(int sock)
{
    fprintf(stdout,"Working\n");
    char* res = malloc(sizeof(char)* 40);
    recv(sock,res,40,0);
//    fprintf(stdout,"%s",res);
    return res;
}


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
    cJSON *info = cJSON_CreateArray();
    cJSON *name = cJSON_CreateArray();
    cJSON_AddItemToArray(name, cJSON_CreateString("name"));
    cJSON_AddItemToArray(name, cJSON_CreateString("menashe"));
    cJSON_AddItemToArray(
            info,
            name
            );
    cJSON *js_object = JsonBuild("welcome",info);

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