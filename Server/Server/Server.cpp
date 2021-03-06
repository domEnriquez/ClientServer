// Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <iostream>
#include <string>

SOCKET Connections[100];
int ConnectionCounter = 0;

void ClientHandlerThread(int index) 
{
	int bufferLength;
	while (true) 
	{
		recv(Connections[index], (char*)&bufferLength, sizeof(int), NULL);
		char * buffer = new char[bufferLength];
		recv(Connections[index], buffer, bufferLength, NULL);

		for (int i = 0; i < ConnectionCounter; i++) 
		{
			if (i == index)
				continue;

			send(Connections[i], (char*)&bufferLength, sizeof(int), NULL);
			send(Connections[i], buffer, bufferLength, NULL);
		}

		delete[] buffer;
	}
}

int main()
{
	WSADATA wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0) {
		MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	SOCKADDR_IN addr;
	int addrLen = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR *)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);

	SOCKET newConnection;
	for (int i = 0; i < 100; i++) 
	{
		newConnection = accept(sListen, (SOCKADDR *)&addr, &addrLen);

		if (newConnection == 0)
			std::cout << "Failed to accept the client's connection." << std::endl;
		else
		{
			std::cout << "Client connected!" << std::endl;
			std::string MOTD = "Welcome! This is the message of the day.";
			int MOTDLength = MOTD.size();
			send(newConnection, (char*)&MOTDLength, sizeof(int), NULL);
			send(newConnection, MOTD.c_str(), MOTDLength, NULL);
			Connections[i] = newConnection;
			ConnectionCounter++;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandlerThread, (LPVOID)(i), NULL, NULL);
		}
	}

	system("pause");
    return 0;
}

