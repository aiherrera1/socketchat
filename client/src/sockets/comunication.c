#include "comunication.h"
#include <string.h>

#define ID_SEND_MESSAGE 3
#define ID_SEND_IMAGE 4
#define ID_SEND_AUDIO 5
#define ID_QUIT 6

int client_receive_id(int client_socket) {
  // Se obtiene solamente el ID del mensaje
  int id = 0;
  recv(client_socket, &id, 1, 0);
  return id;
}

char *client_receive_payload(int client_socket) {
  // Se obtiene el largo del payload
  int len = 0;
  recv(client_socket, &len, 1, 0);
  // Se obtiene el payload
  char *payload = malloc(len);
  int received = recv(client_socket, payload, len, 0);
  // Se retorna
  return payload;
}
char *client_receive_file_payload(int client_socket) {
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

void client_send_message(int client_socket, int pkg_id, char *message) {
  int payloadSize = strlen(message) + 1; //+1 para considerar el caracter nulo.

  // Se arma el paquete
  char msg[1 + 1 + payloadSize];
  msg[0] = pkg_id;
  msg[1] = payloadSize;
  memcpy(&msg[2], message, payloadSize);
  // Se envía el paquete
  send(client_socket, msg, 2 + payloadSize, 0);
}

void client_send_chat(int client_socket, char *message) {

  if (strlen(message) < 2) {
    perror("INVALID FORMAT");
    return;
  }
  char first_chars[2];
  first_chars[0] = message[0];
  first_chars[1] = message[1];

  char *real_message = (char *)malloc(sizeof(char) * (strlen(message) - 1));
  strcpy(real_message, message + 3);
  printf("message:%s.\n", real_message);

  if (strncmp(first_chars, "\\m", 2) == 0) {
    int payloadSize =
        strlen(real_message) + 1; //+1 para considerar el caracter nulo.
    printf("payloadSize %d\n", payloadSize);
    char msg[1 + 1 + payloadSize];
    msg[0] = ID_SEND_MESSAGE;
    msg[1] = payloadSize;
    memcpy(&msg[2], real_message, payloadSize);

    send(client_socket, msg, 2 + payloadSize, 0);
  } else if (strncmp(first_chars, "\\i", 2) == 0) {
    // Open the image file
    FILE *image_file = fopen(real_message, "rb");
    if (image_file == NULL) {
      printf("Image File Not Found: %s\n", real_message);
      return;
    }

    // Get the file size
    fseek(image_file, 0, SEEK_END);
    long file_size = ftell(image_file);
    fseek(image_file, 0, SEEK_SET);

    printf("%ld", file_size);
    // Calculate the number of chunks needed
    int numChunks = (file_size + 255 - 1) / 255;
    printf("numChunks: %d\n", numChunks);

    char header[2 + numChunks * 256];
    header[0] = ID_SEND_IMAGE;
    header[1] = numChunks;

    int i;
    for (i = 0; i < numChunks; i++) {
      // char chunk[256];
      int chunkSize = fread(&header[3 + i * 256], 1, 255,
                            image_file); // Read the chunk content

      header[2 + i * 256] =
          (unsigned char)chunkSize; // Set the number of bytes used in the chunk
      printf("chunksize: %d\n", chunkSize);
      printf("stored chunksize: %02X \n", (unsigned char)header[2 + i * 256]);

      // Fill the remaining unused bytes with 0
      int unusedBytes = 255 - chunkSize;
      if (unusedBytes > 0) {
        memset(&header[3 + i * 256 + chunkSize], 0, unusedBytes);
      }
    }

    send(client_socket, header, 2 + numChunks * 256, 0);
    printf("-------------------------------\n");
    int x;
    for (x = 0; x < 2 + numChunks * 256; x++) {
      printf("%02X ",
             (unsigned char)header[x]); // Print each byte in hexadecimal format
      if ((x + 1) % 16 == 0) {
        printf("\n"); // Print a new line after every 16 bytes
      }
    }
    // Close the image file
    fclose(image_file);
  } else if (strncmp(first_chars, "\\a", 2) == 0) {
  } else if (strncmp(first_chars, "\\q", 2) == 0) {
    int payloadSize = 1; //+1 para considerar el caracter nulo.
    printf("payloadSize %d\n", payloadSize);
    char msg[1 + 1 + payloadSize];
    msg[0] = ID_QUIT;
    msg[1] = payloadSize;
    memcpy(&msg[2], "", payloadSize);

    send(client_socket, msg, 2 + payloadSize, 0);
  } else {
    perror("INVALID FORMAT");
  }
  free(real_message);
  return;
}
