//
// Created by ethan on 16/05/2022.
//

#include "client.h"
#include <gtk/gtk.h>
#include <pthread.h>
#include <unistd.h>
#include <netdb.h>
#include <stdlib.h>

#include <cjson/cJSON.h>

void
client_chat_insert(char* text, GtkTextBuffer *buffer)
{
    GtkTextIter iter;
    gtk_text_buffer_get_iter_at_offset(buffer,&iter, gtk_text_buffer_get_char_count(buffer));
    gtk_text_buffer_insert(buffer,&iter,text,-1);
}

void
client_chat_handler(char* buffer, GtkTextBuffer *textBuffer)
{
//    int endIndex;
//    while(buffer[endIndex] != '\0')
//        endIndex++;
//
//    buffer = realloc(buffer,endIndex);
    cJSON *responseObject = cJSON_Parse(buffer);
    g_print("Working object? : %i\n", cJSON_IsObject(responseObject));

    // For later use
    cJSON* responseAction = cJSON_GetObjectItem(responseObject,"action");

    cJSON* info = cJSON_GetObjectItem(responseObject,"info");
    char* name = cJSON_GetStringValue(
            cJSON_GetArrayItem(cJSON_GetArrayItem(info,0),1)
            );
    char* message = cJSON_GetStringValue(
            cJSON_GetArrayItem(cJSON_GetArrayItem(info,1),1)
    );


    if (cJSON_IsString(responseAction) && strcmp(cJSON_GetStringValue(responseAction),"chat") == 0)
    {
        char* formatted_buffer = malloc(sizeof(char) * 48);
        sprintf(formatted_buffer, "%s -> %s\n",name,message);
        client_chat_insert(formatted_buffer,textBuffer);
    }

}



struct thread_data{
    void (*callback)(GtkTextBuffer *textBuffer, char* text);
    GtkTextBuffer *buffer;
    int socket;
};

_Noreturn static void
*thread_listen(void *ptr_data)
{
    struct thread_data *data = ptr_data;
    char *text = malloc(sizeof(char) * 30);
    text = "1337";
    while(1)
    {
        fprintf(stdout,"working\n");
//        text = sockcom_recv_socket(data->socket);
        if (recv(data->socket,text,sizeof(text),0) == -1)
        {
            fprintf(stdout,"error: %s\n",strerror(errno));
        }else{
            fprintf(stdout,"working recv");
        }

        // Here we do data processing

        // Transfer to the text view buffer
//        data->callback(data->buffer,text);
//        text = NULL;
        sleep(3);
    }
//    if (recv(data->socket,text,sizeof(text)+1,0) == -1)
//        {
//            fprintf(stdout,"error: %s\n",strerror(errno));
//        }else{
//            fprintf(stdout,"working recv");
//        }
}
/*
 * Create a thread that will wait for each message and then add it to the buffer
 */
void
client_connect_socket(int sock, GtkTextBuffer *buffer, void (*fptr)(GtkTextBuffer *textBuffer, char* text))
{
//    struct thread_data *data = g_new0(struct thread_data,0);
    struct thread_data data;
    data.buffer = buffer;
    data.socket = sock;
    data.callback = fptr;
    pthread_t thread_id;
    pthread_create(&thread_id,NULL,thread_listen,&data);

}