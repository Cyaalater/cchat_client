#include <stdio.h>
#include <gtk-3.0/gtk/gtk.h>
#include "include/sockcom/sockcom.h"

struct applyData{
    GtkWidget * typeSwitch;
    GtkWidget * userNameEntry;
    GtkWidget * serverNameEntry;
};


static void
on_click(GtkButton *button, gpointer user_data)
{
    sockcom_send_welcome("127.0.0.1","3000");
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

}


static void
on_activate(GtkApplication *app, gpointer user_data)
{
    // C is ruthless
    struct applyData *d = g_new0(struct applyData, 1);


    // Getting glade file
    GtkBuilder *builder = gtk_builder_new_from_file("main.glade");
    GtkWidget *win = GTK_WIDGET(gtk_builder_get_object(builder,"win"));

    // Widget getters
    GtkWidget *button = GTK_WIDGET(gtk_builder_get_object(builder,"accept"));
    GtkWidget *typeSwitch = GTK_WIDGET(gtk_builder_get_object(builder,"user-type"));
    GtkWidget *userNameEntry = GTK_WIDGET(gtk_builder_get_object(builder,"user-name"));
    GtkWidget *serverNameEntry = GTK_WIDGET(gtk_builder_get_object(builder,"server-name"));

    // Widget data
//    const gchar* username = gtk_entry_get_text(userNameEntry);
//    const gchar* servername = gtk_entry_get_text(serverNameEntry);
//    const int type = gtk_switch_get_state(typeSwitch);

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
