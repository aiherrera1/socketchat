#include "comunication.h"
#define ID_SEND_MESSAGE 3
#define ID_SEND_IMAGE 4
#define ID_SEND_AUDIO 5
#define ID_QUIT 6

// LINKS REFERENCIAS
// https://pubs.opengroup.org/onlinepubs/009695399/functions/recv.html
// https://pubs.opengroup.org/onlinepubs/009695399/functions/send.html

int server_receive_id(int client_socket) {
  // Se obtiene solamente el ID del mensaje
  int id = 0;
  recv(client_socket, &id, 1, 0);
  return id;
}

char *server_receive_payload(int client_socket) {
  // Se obtiene el largo del payload
  int len = 0;
  recv(client_socket, &len, 1, 0);
  printf("len: %d\n", len);
  // Se obtiene el payload
  char *payload = malloc(len);
  int received = recv(client_socket, payload, len, 0);
  // Se retorna
  return payload;
}

char *server_receive_file_payload(int client_socket) {
  // Obtain the length of the payload
  int len = 0;
  recv(client_socket, &len, 1, 0);

  printf("len: %d\n", len);
  // Obtain the payload
  char *payload = malloc((len * 256) + 2);
  int received = recv(client_socket, payload + 2, len * 256, 0);

  payload[0] = ID_SEND_IMAGE;
  payload[1] = len;
  // Store the ID and numChunks values in the payload
  // payload[1] = len;

  // Return the payload
  return payload;
}

char *server_receive_audio_payload(int client_socket) {
  // Obtain the length of the payload
  int len = 0;
  recv(client_socket, &len, 1, 0);

  printf("len: %d\n", len);
  // Obtain the payload
  char *payload = malloc((len * 256) + 2);
  int received = recv(client_socket, payload + 2, len * 256, 0);

  payload[0] = ID_SEND_AUDIO;
  payload[1] = len;
  // Store the ID and numChunks values in the payload
  // payload[1] = len;

  // Return the payload
  return payload;
}
void server_send_message(int client_socket, int pkg_id, char *message) {
  int payloadSize = strlen(message) + 1;
  // printf("payload size: %d\n", payloadSize);
  //  Se arma el paquete
  char msg[1 + 1 + payloadSize];
  msg[0] = pkg_id;
  msg[1] = payloadSize;
  memcpy(&msg[2], message, payloadSize);
  // Se envía el paquete
  send(client_socket, msg, 2 + payloadSize, 0);
}

void server_send_file(int client_socket, int pkg_id, char *message) {
  printf("size: %d\n", (unsigned char)message[1]);

  int numChunks = message[1];
  int x;
  for (x = 0; x < 2 + 1 * 256; x++) {
    printf("%02X ",
           (unsigned char)message[x]); // Print each byte in hexadecimal format
    if ((x + 1) % 16 == 0) {
      printf("\n"); // Print a new line after every 16 bytes
    }
  }
  // printf("payload size: %d\n", payloadSize);
  //  Se arma el paquete
  // char msg[payloadSize];
  // // msg[0] = pkg_id;
  // // msg[1] = payloadSize;
  // memcpy(msg, message, payloadSize);
  // // Se envía el paquete
  send(client_socket, message, numChunks * 256 + 2, 0);
}
