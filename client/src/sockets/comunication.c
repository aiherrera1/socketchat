#include "comunication.h"
#include <string.h>

#define ID_SEND_MESSAGE 3
#define ID_SEND_IMAGE 4
#define ID_SEND_AUDIO 5
#define ID_QUIT 6

int client_receive_id(int client_socket)
{
  // Se obtiene solamente el ID del mensaje
  int id = 0;
  recv(client_socket, &id, 1, 0);
  return id;
}

char *client_receive_payload(int client_socket)
{
  // Se obtiene el largo del payload
  int len = 0;
  recv(client_socket, &len, 1, 0);
  // Se obtiene el payload
  char *payload = malloc(len);
  int received = recv(client_socket, payload, len, 0);
  // Se retorna
  return payload;
}

void client_send_message(int client_socket, int pkg_id, char *message)
{
  int payloadSize = strlen(message) + 1; //+1 para considerar el caracter nulo.

  // Se arma el paquete
  char msg[1 + 1 + payloadSize];
  msg[0] = pkg_id;
  msg[1] = payloadSize;
  memcpy(&msg[2], message, payloadSize);
  // Se envía el paquete
  send(client_socket, msg, 2 + payloadSize, 0);
}

void client_send_chat(int client_socket, char *message)
{

  if (strlen(message) < 2)
  {
    perror("INVALID FORMAT");
    return;
  }
  char first_chars[2];
  first_chars[0] = message[0];
  first_chars[1] = message[1];

  char *real_message = (char *)malloc(sizeof(char) * (strlen(message) - 1));
  strcpy(real_message, message + 3);
  printf("message:%s.\n", real_message);

  if (strncmp(first_chars, "\\m", 2) == 0)
  {
    int payloadSize = strlen(real_message) + 1; //+1 para considerar el caracter nulo.
    printf("payloadSize %d\n", payloadSize);
    char msg[1 + 1 + payloadSize];
    msg[0] = ID_SEND_MESSAGE;
    msg[1] = payloadSize;
    memcpy(&msg[2], real_message, payloadSize);

    send(client_socket, msg, 2 + payloadSize, 0);
  }
  else if (strncmp(first_chars, "\\i", 2) == 0)
  {
  }
  else if (strncmp(first_chars, "\\a", 2) == 0)
  {
  }
  else if (strncmp(first_chars, "\\q", 2) == 0)
  {
    int payloadSize = 1; //+1 para considerar el caracter nulo.
    printf("payloadSize %d\n", payloadSize);
    char msg[1 + 1 + payloadSize];
    msg[0] = ID_QUIT;
    msg[1] = payloadSize;
    memcpy(&msg[2], "", payloadSize);

    send(client_socket, msg, 2 + payloadSize, 0);
  }
  else
  {
    perror("INVALID FORMAT");
  }
  free(real_message);
  return;
}