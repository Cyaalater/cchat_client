//
// Created by ethan on 16/05/2022.
//

#ifndef CCHAT_CLIENT_H
#define CCHAT_CLIENT_H

#include <gtk/gtk.h>
#include "sockcom/sockcom.h"

void
client_connect_socket(int sock, GtkTextBuffer *buffer, void (*fptr)(GtkTextBuffer *textBuffer, char* text));

#endif //BUILDDIR_CLIENT_H