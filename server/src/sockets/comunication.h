#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

int server_receive_id(int client_socket);
char *server_receive_payload(int client_socket);
char *server_receive_file_payload(int client_socket);
char *server_receive_audio_payload(int client_socket);
void server_send_message(int client_socket, int pkg_id, char *message);
void server_send_file(int client_socket, int pkg_id, char *message);
