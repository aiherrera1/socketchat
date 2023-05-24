#include "../sockets/comunication.h"
#include "../sockets/conection.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ID_SEND_MESSAGE 3
#define ID_SEND_IMAGE 4
#define ID_SEND_AUDIO 5
#define ID_QUIT 6

char *revert(char *message) {
  // Se invierte el mensaje

  int len = strlen(message) + 1;
  char *response = malloc(len);

  for (int i = 0; i < len - 1; i++) {
    response[i] = message[len - 2 - i];
  }
  response[len - 1] = '\0';
  return response;
}

int main(int argc, char *argv[]) {
  // Se define una IP y un puerto
  char *IP = "0.0.0.0";
  int PORT = 8040;

  // Se crea el servidor y se obtienen los sockets de ambos clientes.
  printf("Creando servidor...\n");
  PlayersInfo *players_info = prepare_sockets_and_get_clients(IP, PORT);

  // // Le enviamos al primer cliente un mensaje
  char *first_message = "Hola cliente 1!";
  server_send_message(players_info->socket_c1, 1, first_message);

  // Guardaremos los sockets en un arreglo e iremos alternando a quién escuchar.
  int sockets_array[2] = {players_info->socket_c1, players_info->socket_c2};
  int my_attention = 0;
  while (1) {
    // Se obtiene el paquete del cliente 1
    int msg_code = server_receive_id(sockets_array[my_attention]);
    printf("msg_code %d\n", msg_code);

    if (msg_code == 1) // El cliente me envió un mensaje a mi (servidor)
    {
      char *client_message =
          server_receive_payload(sockets_array[my_attention]);
      printf("El cliente %d dice: %s\n", my_attention + 1, client_message);

      // Le enviaremos el mismo mensaje invertido jeje
      char *response = revert(client_message);

      // Le enviamos la respuesta
      server_send_message(sockets_array[my_attention], 1, response);
    } else if (msg_code == ID_SEND_MESSAGE) { // El cliente le envía un mensaje
                                              // al otro cliente
      char *client_message =
          server_receive_payload(sockets_array[my_attention]);
      printf("Servidor traspasando desde %d a %d el mensaje: %s\n",
             my_attention + 1, ((my_attention + 1) % 2) + 1, client_message);

      // Mi atención cambia al otro socket
      my_attention = (my_attention + 1) % 2;

      server_send_message(sockets_array[my_attention], 2, client_message);
    } else if (msg_code ==
               ID_SEND_IMAGE) // El cliente le envía una imagen al otro cliente
    {

      char *client_message =
          server_receive_file_payload(sockets_array[my_attention]);

      // int x;
      // for (x = 0; x < 2 + 10 * 256; x++) {
      //   printf("%02X ",
      //          client_message[x]); // Print each byte in hexadecimal format
      //   if ((x + 1) % 16 == 0) {
      //     printf("\n"); // Print a new line after every 16 bytes
      //   }
      // }
      // FILE *output_file = fopen("reconstructed_image.png", "wb");
      // if (output_file == NULL) {
      //   printf("Failed to create the output file.\n");
      // }
      printf("chuck size: %02X\n", client_message[1]);

      // for (int i = 0; i < 10; i++) {
      //   int chunkSize = client_message[2 + i * 256];
      //   printf("chunkSize: %02X\n", chunkSize);
      //   fwrite(&client_message[3 + i * 255], 1, chunkSize, output_file);
      // }
      //
      // fclose(output_file);

      // printf("El cliente %d dice: %s\n", my_attention + 1, client_message);
      //
      my_attention = (my_attention + 1) % 2;
      //
      server_send_file(sockets_array[my_attention], 3, client_message);
    } else if (msg_code ==
               ID_SEND_AUDIO) // El cliente le envía un audio al otro cliente
    {
    } else if (msg_code == ID_QUIT) // El cliente quiere salir de la sala
    {
      printf("El cliente %d quiere salir de la sala\n", my_attention + 1);
      char *client_message =
          server_receive_payload(sockets_array[my_attention]);
      printf("No payload: %s.\n", client_message);
      // Sacar de la sala y enviar opciones de Lobby
      // server_send_message(sockets_array[my_attention], 1, message);
      server_send_message(sockets_array[my_attention], 1, "Saliendo\n");
    }
    printf("------------------\n");
  }

  return 0;
}
