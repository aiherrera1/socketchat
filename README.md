# SocketChat

SocketChat is a client application designed for real-time communication between multiple clients through a server using TCP sockets. The program allows for direct messaging, server messaging, and file transfers, including images and audio files. Built with C, SocketChat is efficient and optimized for lightweight communication over a network.

## Features

- **Direct Messaging**: Send and receive messages from the server and other connected clients.
- **File Transfer**: Send and receive image and audio files, with support for chunked data transfer.
- **Command-based Interaction**: Simple commands for sending messages, images, and audio files, as well as quitting the chat.
- **Customizable**: Easily adaptable for different IPs and port configurations.

## Commands

- `\m <message>`: Send a text message to the server.
- `\i <image_path>`: Send an image file to the server.
- `\a <audio_path>`: Send an audio file to the server.
- `\q`: Quit the chat session.

## Installation

1. **Clone the repository**:
   ```bash
   git clone https://github.com/aiherrera1/socketchat.git
   cd socketchat
   ```

2. **Compile the code**:
   Ensure you have GCC installed and then compile the project:
   ```bash
   gcc -o socketchat main.c comunication.c conection.c -lpthread
   ```

3. **Run the client**:
   ```bash
   ./socketchat
   ```

## Usage

1. On startup, the client connects to the server specified by the IP and PORT in the code.
2. Once connected, you will receive a message from the server if available.
3. Use the command prompts to send messages or files as described above.

## Code Structure

- `main.c`: Main client logic, handles the connection loop and response actions.
- `comunication.c`: Handles sending and receiving messages and files between clients.
- `conection.c`: Manages the initial socket setup and server connection.
- `comunication.h`, `conection.h`: Header files for function declarations.

## Dependencies

- **POSIX Sockets**: Ensure your system supports POSIX socket programming (Linux, macOS).
- **Standard C Libraries**: Uses standard libraries like `stdio.h`, `stdlib.h`, `string.h`, and `unistd.h`.
