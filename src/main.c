#include <stdio.h>
#include <gtk-3.0/gtk/gtk.h>
#include <glib.h>
#include "include/sockcom/sockcom.h"
#include "include/namecom/namecom.h"
#include "include/client/client.h"
#include <pthread.h>

// TODO: remove later
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


//#include "include/client/client.h"

int sock;

struct applyData{
    GtkWidget * typeSwitch;
    GtkWidget * userNameEntry;
    GtkWidget * serverNameEntry;
};
struct sendData{
    int socket;
    GtkEntry* chat_input;
    GtkTextBuffer* chat_buffer;
    const gchar *user_name;
};

static void
insert_handler(GtkTextBuffer *buffer, char* text, char* name)
{
    if(text == NULL)
    {
        return;
    }
    sockcom_send_data(sock,text,name);
//    client_chat_insert(text,buffer);
}

static void
activate_send(GtkButton *button, gpointer user_data)
{
    struct sendData* data = user_data;
    const gchar* text = gtk_entry_get_text(GTK_ENTRY(data->chat_input));
    g_print("%s\n",text);
    insert_handler(data->chat_buffer, text,data->user_name);
}

//TODO: Change it to a file later
void *chat_thread(void *arg)
{
    GtkTextBuffer *textBuffer = arg;
    while(1)
    {
        char* text = malloc(sizeof(char) * 128);
        if(recv(sock,text,128,0) < 0 )
        {
            g_print("failed\n");
        }
        g_print("Data recieved: \n");
        for(int i=0; text[i] != '\0'; i++)
        {
            g_print("%c",text[i]);
        }
        g_print("\n");
        client_chat_handler(text,arg);
        sleep(2);
    }

}



static void
activate_chat(int sock,const gchar *userP)
{
    GtkBuilder *chat = gtk_builder_new_from_file("resources/chat.glade");
    GtkWidget *chatObject = GTK_WIDGET(gtk_builder_get_object(chat,"main"));

    // Get objects from builder
    GtkEntry *chat_input = GTK_ENTRY(gtk_builder_get_object(chat,"chat_input"));
    GtkTextBuffer *chat_view_text_buffer = GTK_TEXT_BUFFER(gtk_builder_get_object(chat, "chat_view_text_buffer"));
    GtkWidget *chat_send = GTK_WIDGET(gtk_builder_get_object(chat,"chat_send"));

    struct sendData *data = g_new0(struct sendData,1);
    data->socket = 0;
    data->chat_buffer = chat_view_text_buffer;
    data->chat_input = chat_input;
    data->user_name = userP;

    g_signal_connect(chat_send,"clicked",G_CALLBACK(activate_send),data);

    pthread_t thread_id;
    pthread_create(&thread_id,NULL,chat_thread,data->chat_buffer);

    gtk_widget_show_all(chatObject);

}



static void
on_click(GtkButton *button, gpointer user_data)
{
    struct applyData *data = user_data;
    GtkSwitch* switchP = GTK_SWITCH(data->typeSwitch);
    GtkEntry* userP = GTK_ENTRY(data->userNameEntry);
    GtkEntry* serverP = GTK_ENTRY(data->serverNameEntry);
    g_print(
            "State: %d\n",
            gtk_switch_get_state(switchP));

    const gchar* text = gtk_entry_get_text(userP);
    for(int i=0; text[i]; i++)
        g_print("%c",text[i]);

    sock = sockcom_send_welcome(gtk_entry_get_text(serverP),"3000");
    // Kind of assertion
//    if (sock == -1){return;}

    /*
     * Upon having a working socket:
     * - Open up a thread to check for each data received
     * - Open chat to show the new data and so allow the user to send data
     */
    // Open up the chat
    activate_chat(sock,gtk_entry_get_text(userP));

}


static void
on_activate(GtkApplication *app, gpointer user_data)
{
    // C is ruthless
    struct applyData *d = g_new0(struct applyData, 1);


    // Getting glade file
    GtkBuilder *builder = gtk_builder_new_from_file("resources/main.glade");
    GtkWidget *win = GTK_WIDGET(gtk_builder_get_object(builder,"win"));

    // Widget getters
    GtkWidget *button = GTK_WIDGET(gtk_builder_get_object(builder,"accept"));
    GtkWidget *typeSwitch = GTK_WIDGET(gtk_builder_get_object(builder,"user-type"));
    GtkWidget *userNameEntry = GTK_WIDGET(gtk_builder_get_object(builder,"user-name"));
    GtkWidget *serverNameEntry = GTK_WIDGET(gtk_builder_get_object(builder,"server-name"));


    d->typeSwitch = typeSwitch;
    d->serverNameEntry = serverNameEntry;
    d->userNameEntry = userNameEntry;

    // Signals
    g_signal_connect(button,"pressed",G_CALLBACK(on_click),d);



    // Window properties
    gtk_window_set_application(GTK_WINDOW(win),app);
    gtk_window_set_default_size(GTK_WINDOW(win), 300,400);
    gtk_widget_show_all(win);

}

int
main(int argc, char* argv[]) {

    //Initialization
    GtkApplication *app;
    app = gtk_application_new("org.gtk.cool", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app,"activate",G_CALLBACK(on_activate), NULL);
    g_application_run(G_APPLICATION(app),argc,argv);
    g_object_unref(app);

    return 0;
}
