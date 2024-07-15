# Simple Chat Application using Winsock

This repository contains a simple chat application developed in C++ using Winsock library. The application consists of a server and multiple clients that can communicate with each other over a network.

## Features
- **Server-Client Architecture**: The project is structured with a server that listens for incoming connections and multiple clients that can connect to the server to send and receive messages.
- **Multithreading**: Each client interaction is handled in a separate thread, allowing the server to handle multiple clients simultaneously.
- **Basic Chat Functionality**: Clients can send messages to the server, and the server broadcasts these messages to all connected clients.

## How to Use
1. Clone the repository to your local machine.
2. Open the server project and build it.
3. Open multiple instances of the client project, specifying the server IP and port.
4. Connect the clients to the server and start chatting!

## Prerequisites
- Visual Studio (or any C++ compiler that supports Winsock)
- Windows Operating System

## Usage
1. Build and run the server project (`Server.cpp`).
2. Build and run multiple instances of the client project (`Client.cpp`).
3. Enter your name when prompted and start sending messages.

## Image
![ChatApp](https://github.com/user-attachments/assets/15b198b8-42ec-4686-b757-bd4b21130fc6)




