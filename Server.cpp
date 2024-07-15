#include<iostream>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<tchar.h>
#include<thread>
#include<vector>


using namespace std;
/*
	//initialise winsock library
	//create the socket
	//get the ip and port where server will be running
	//bind the ip/port with the socket
	//listen on the socket
	//accept the connection from client
	//receive and send
	//close the connection
	//cleanup the WinSock library




*/
#pragma comment(lib,"ws2_32.lib") //to automatically link the Winsock 2.0 library to the code, which is necessary for socket programming on Windows

bool Initialize()
{
	WSADATA data; //to initialise windows socket library
	return WSAStartup(MAKEWORD(2,2), &data) == 0;
}


void InteractWithClient(SOCKET clientSocket, vector<SOCKET>& clients)
{
	//now send/recv will be donw here so that the main thread gets freed to accept other connection requests
	cout << "Client Connected" << endl;
	char buffer[4096]; //data received from client stored inside buffer

	while (1) 
	{
		
		int bytesrecvd = recv(clientSocket, buffer, sizeof(buffer), 0);

		if (bytesrecvd <= 0)
		{
			cout << "Client Disconnected" << endl;
			break;
		}
		string message(buffer, bytesrecvd);
		cout << "Message from Client: " << message << endl;

		for (auto client : clients)
		{
			if(client != clientSocket) //we need to send the message recieved from this client to all other clients except this one who sent it to server
				send(client, message.c_str(), message.length(), 0);

		}
		
	}
	auto it = find(clients.begin(), clients.end(), clientSocket);
	if (it != clients.end())
	{
		clients.erase(it);
	}

	closesocket(clientSocket);

}



int main()
{
	if (!Initialize())
	{
		cout << "Winsock Initialization Failed " << endl;
		return 1;
	}

	cout << "Server Program" << endl;

	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, 0); //creating the socket, AF_INET=> IPv4 family, SOCK_STREAM=> for TCP

	if(listenSocket == INVALID_SOCKET)
	{
		cout << "Socket Creation Failed" << endl;
		return 1;
	}

	//create address structure
	int port = 12345;
	sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);   //api called host_to_network(htons)

	//convert the ipaddress (0.0.0.0)  put it inside the sin_family in binary format
	if (InetPton(AF_INET, _T("0.0.0.0"), &serveraddr.sin_addr) != 1)
	{
		cout << "Setting Address Structure Failed" << endl;
		closesocket(listenSocket);
		WSACleanup();
		return 1;

	}

	//bind the ip and port to socket
	if (bind(listenSocket, reinterpret_cast<sockaddr*>(&serveraddr), sizeof(serveraddr)) == SOCKET_ERROR)
	{
		cout << "Bind Failed" << endl;
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	//listen on the socket
	if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		cout << "Listen Failed" << endl;
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	cout << "Server has started listening on Port: " << port << endl;
	vector<SOCKET> clients;
	//listenSocket is used to used to only listen so that other clients can connect with the server,
	// but after listening and accepting when client is connected, listenSocket will start listening other requests and now clientSocket handles this request
	//Accept the connection from client

	while (1)   //so that server keeps on accepting requests and sending it to particular different threads to handle them
	{
		SOCKET clientSocket = accept(listenSocket, nullptr, nullptr);

		//now the code returns a new entity of socket type from accept which is called clientSocket
		//now all the communication with that particular client will only be done on clientSocket, listenSocket will again be used to listen other clients

		if (clientSocket == INVALID_SOCKET)
		{
			cout << "Invalid Client Socket" << endl;
		}

		//once the client is connected, it could be sent to the other thread, so that server can keep on accepting other requests as well
		clients.push_back(clientSocket);  //saving all the clients in a vector
		thread t1(InteractWithClient, clientSocket, std::ref(clients));
		t1.detach();
	}

	


	
	closesocket(listenSocket);


	WSACleanup(); //cleanup the winsock library

	return 0;
}