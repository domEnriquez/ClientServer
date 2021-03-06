// Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <string>
#include <WinSock2.h>
#include <iostream>

SOCKET Connection;

void ClientThread() 
{
	int bufferLength;

	while (true) 
	{
		recv(Connection, (char*)&bufferLength, sizeof(int), NULL);
		char * buffer = new char[bufferLength+1];
		buffer[bufferLength] = '\0';
		recv(Connection, buffer, bufferLength, NULL);
		std::cout << buffer << std::endl;
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

	Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR *)&addr, sizeof(addr)) != 0) 
	{
		MessageBoxA(NULL, "Failed to connect", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	std::cout << "Connected!" << std::endl;
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientThread, NULL, NULL, NULL);

	std::string buffer;
	while (true) 
	{
		std::getline(std::cin, buffer);
		int bufferLength = buffer.size();
		send(Connection, (char*)&bufferLength, sizeof(int), NULL);
		send(Connection, buffer.c_str(), bufferLength, NULL);
		Sleep(10);
	}

	system("pause");
    return 0;
}

