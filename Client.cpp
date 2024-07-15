#include<iostream>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<thread>
#include<string>
using namespace std;

#pragma comment(lib, "ws2_32.lib");
/*
	initialize winsock

	create socket
	
	connect to server
	
	send/receive
	
	close the socket

*/

bool Initialize()
{
	WSADATA data; //to initialise windows socket library
	return WSAStartup(MAKEWORD(2, 2), &data) == 0;
}

void SendMsg(SOCKET s)
{
	cout << "Enter your Chat Name: " << endl;
	string name;
	getline(cin, name);
	string message;

	while (1)
	{
		getline(cin, message);
		string msg = name + " : " + message;
		int bytesent = send(s, msg.c_str(), msg.length(), 0);
		if (bytesent == SOCKET_ERROR)
		{
			cout << "Error Sending Message " << endl;
			break;
		}
		if (message == "Quit")
		{
			cout << "Stopping the Application " << endl;
			break;
		}
	}
	closesocket(s);
	WSACleanup(); 
}

void ReceiveMsg(SOCKET s)
{
	char buffer[4096];
	int recvlength;
	string msg = "";
	while (1)
	{
		recvlength = recv(s, buffer, sizeof(buffer), 0);
		if (recvlength <= 0)
		{
			cout << "Disconnected from the Server " << endl;
			break;
		}
		else
		{
			msg = string(buffer, recvlength);
			cout << msg << endl;
		}
	}
	closesocket(s);
	WSACleanup();
}

int main()
{

	if (!Initialize())
	{
		cout << "Winsock Initialization Failed " << endl;
		return 1;
	}

	cout << "Client Program Started" << endl;

	SOCKET s;
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == INVALID_SOCKET)
	{
		cout << "Invalid Socket created" << endl;
		return 1;
	}


	//we need to get the ip addr and port where server is listening
	int port = 12345;
	string serveraddress = "127.0.0.1";
	sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);
	inet_pton(AF_INET, serveraddress.c_str(), &(serveraddr.sin_addr));


	if (connect(s, reinterpret_cast<sockaddr*>(&serveraddr), sizeof(serveraddr)) == SOCKET_ERROR)
	{
		cout << "Not able to connect to server" << endl;
		closesocket(s);
		WSACleanup();
		return 1;
	}

	cout << "Successfully Connected to Server" << endl;

	//For broadcast messaging, the client should have two properties, it should be alble to send the message to server so that it could
	//get broadcasted to other clients and also, should be able to receive the broadcasted messages from other clients via server

	thread senderthread(SendMsg, s);
	thread receiverthread(ReceiveMsg, s);

	senderthread.join();
	receiverthread.join();

	


	return 0;
}