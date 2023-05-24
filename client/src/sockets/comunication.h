#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

void client_send_message(int client_socket, int pkg_id, char *message);
int client_receive_id(int client_socket);
char *client_receive_payload(int client_socket);
char *client_receive_file_payload(int client_socket);
void client_send_chat(int client_socket, char *message);
