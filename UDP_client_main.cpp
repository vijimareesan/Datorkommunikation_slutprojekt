#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h>

#pragma comment(lib, "Ws2_32.lib")

#pragma warning(disable:4996) 

using namespace std;

int main() 
{
	//Init
	WSADATA data;
	WSAStartup(MAKEWORD(2, 2), &data);

	SOCKET s;
	struct sockaddr_in server;

	s = socket(AF_INET, SOCK_DGRAM, 0);

	if (s == INVALID_SOCKET) 
	{
		printf("Failed to create socket\n");

		WSACleanup();
		return 1;
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(5000);
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	//server.sin_addr.s_addr = InetPton(AF_INET, _T("127.0.0.1"), &server.sin_addr.s_addr); // this failed it

	while (true) 
	{
		const char* message = "TEST";
		sendto(s, message, strlen(message), 0, (struct sockaddr*)&server, sizeof(server));
		cout << "Sending data..." << endl;
		Sleep(1000);
	}

	closesocket(s);
	WSACleanup();
	return 0;
}