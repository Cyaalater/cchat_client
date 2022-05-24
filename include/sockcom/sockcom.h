//
// Created by ethan on 09/05/2022.
//

#ifndef CCHAT_SOCKCOM_H
#define CCHAT_SOCKCOM_H


int sockcom_send_welcome(char* IP, char* PORT);

char*
sockcom_recv_socket(int sock);

int sockcom_send_data(int sock, char* text);
//
//int sockcom_send_data_recv();

#endif //CCHAT_SOCKCOM_H
