#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h>

#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

//#pragma warning(disable:4996) 

using namespace std;

int main()
{
	//Init-------

	WSADATA data;
	WSAStartup(MAKEWORD(2, 2), &data);

	SOCKET s;
	struct sockaddr_in server;
	struct sockaddr_in client;
	int clientAddrSize = sizeof(client);
	char buffer[1024];

	s = socket(AF_INET, SOCK_DGRAM, 0);

	if (s == INVALID_SOCKET) 
	{
		cout << "Socket creation failed" << endl;
		WSACleanup();
		return 1;
	}

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(5000);
	//InetPton(AF_INET, _T("127.0.0.1"), &ServerAddr.sin_addr.s_addr);

	if (bind(s, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) 
	{
		printf("Bind failed\n");
		closesocket(s);
		WSACleanup();
		return 1;
	}

	while (true)
	{
		memset(buffer, 0, sizeof(buffer));
		int n = recvfrom(s, buffer, sizeof(buffer), 0, (struct sockaddr*)&client, &clientAddrSize);
		
		if(n > 0) 
		{
			char clientIP[INET_ADDRSTRLEN];
			if (inet_ntop(AF_INET, &client.sin_addr, clientIP, INET_ADDRSTRLEN)) 
			{
				cout << "Received from " << clientIP << ": " << buffer << endl;

			}
			else 
			{
				cout << "Error in ntop\n" << endl;
			}
		}

		Sleep(1000);
	}

	closesocket(s);
	WSACleanup();
	return 0;
}