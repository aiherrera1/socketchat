#include "../sockets/comunication.h"
#include "../sockets/conection.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

char *get_input() {
  char *response = malloc(20);
  int pos = 0;
  while (1) {
    char c = getchar();
    if (c == '\n')
      break;
    response[pos] = c;
    pos++;
  }
  response[pos] = '\0';
  return response;
}

int main(int argc, char *argv[]) {
  // Se obtiene la ip y el puerto donde está escuchando el servidor (la ip y
  // puerto de este cliente da igual)
  char *IP = "0.0.0.0";
  int PORT = 8040;

  // Se prepara el socket
  printf("Conectando con el servidor...\n");
  int server_socket = prepare_socket(IP, PORT);
  printf("Conexión establecida\n");

  // Se inicializa un loop para recibir todo tipo de paquetes y tomar una acción
  // al respecto
  while (1) {
    int msg_code = client_receive_id(server_socket);

    if (msg_code == 0) { // Servidor envía mensaje de conexión inicial
      char *message = client_receive_payload(server_socket);
      printf("El servidor dice: %s\n", message);
      free(message);
    }

    else if (msg_code == 1) { // Recibimos un mensaje del servidor
      char *message = client_receive_payload(server_socket);
      printf("El servidor dice: %s\n", message);
      free(message);

      printf("¿Qué desea hacer?\n   1)Enviar mensaje al servidor\n   2)Enviar "
             "mensaje al otro cliente\n");
      int option = getchar() - '0';
      getchar(); // Para capturar el "enter" que queda en el buffer de entrada
                 // stdin

      printf("Ingrese su mensaje: ");
      char *response = get_input();

      client_send_chat(server_socket, response);
    }

    if (msg_code == 2) { // Recibimos un mensaje que proviene del otro cliente
      char *message = client_receive_payload(server_socket);
      printf("El otro cliente dice: %s\n", message);
      free(message);

      printf("¿Qué desea hacer?\n   1)Enviar mensaje al servidor\n   2)Enviar "
             "mensaje al otro cliente\n");
      int option = getchar() - '0';
      getchar(); // Para capturar el "enter" que queda en el buffer de entrada
                 // stdin

      printf("Ingrese su mensaje: ");
      char *response = get_input();

      client_send_chat(server_socket, response);
    }
    if (msg_code == 4) {
      printf("HIIIII\n");
      char *message = client_receive_file_payload(server_socket);

      FILE *output_file = fopen("reconstructed_image.png", "wb");
      if (output_file == NULL) {
        printf("Failed to create the output file.\n");
      } else {
        printf("Output file created successfully.\n");
      }

      printf("chunk size: %02X\n", (unsigned char)message[1]);
      int x;
      for (x = 0; x < 2 + 1 * 256; x++) {
        printf(
            "%02X ",
            (unsigned char)message[x]); // Print each byte in hexadecimal format
        if ((x + 1) % 16 == 0) {
          printf("\n"); // Print a new line after every 16 bytes
        }
      }

      printf("--------------------------\n");
      for (int i = 0; i < 10; i++) {
        int chunkSize = (unsigned char)message[2 + i * 256];
        printf("chunkSize: %02X\n", (unsigned char)chunkSize);
        printf("chunkSize: %d\n", chunkSize);
        fwrite(&message[3 + i * 256], 1, chunkSize, output_file);

        // Print bytes written to the file
        for (int j = 0; j < chunkSize; j++) {
          printf("%02X ", (unsigned char)message[3 + i * 256 + j]);
        }
        printf("\n");
      }

      fclose(output_file);
      printf("¿Qué desea hacer?\n   1)Enviar mensaje al servidor\n   2)Enviar "
             "mensaje al otro cliente\n");
      int option = getchar() - '0';
      getchar(); // Para capturar el "enter" que queda en el buffer de entrada
                 // stdin

      printf("Ingrese su mensaje: ");
      char *response = get_input();

      client_send_chat(server_socket, response);
    }
    printf("------------------\n");
  }

  // Se cierra el socket
  close(server_socket);
  free(IP);

  return 0;
}
