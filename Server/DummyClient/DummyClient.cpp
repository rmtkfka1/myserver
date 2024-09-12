#include "pch.h"
#include <iostream>

//클라

// 1) 소켓 생성
// 2) 서버에 연결요쳥
// 3) 통신
int main()
{


		WSADATA wsadata;

		WSAStartup(MAKEWORD(2, 2), &wsadata);

		SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (clientSocket == INVALID_SOCKET) err_quit("클라오류");

		sockaddr_in serveraddr;
		memset(&serveraddr, 0, sizeof(serveraddr));

		serveraddr.sin_family = AF_INET;

		//접속할 상대방의 주소
		::inet_pton(AF_INET, "127.0.0.1", &serveraddr.sin_addr);
		serveraddr.sin_port = htons(9000); // 포트번호 => 응용프로그램 식별


		if (::connect(clientSocket, (sockaddr*)&serveraddr, sizeof(serveraddr)) == SOCKET_ERROR)
		{
			err_quit("클라 접속실패");
		}

		cout << "[클라이언트] " << endl;

		//서버와의 데이터통신
		while (1)
		{

			char sendBuffer[100] = "HELLO IM CLIENT";
			int32 resultCode = send(clientSocket, sendBuffer, sizeof(sendBuffer), 0);
			if (resultCode == SOCKET_ERROR)
			{
				err_quit("FAIL");
			}

			char recvBuffer[100];
			int32 recvLen = recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
			if (recvLen > 0)
			{
				cout << recvBuffer << endl;
			}
			this_thread::sleep_for(1s);
		}


		closesocket(clientSocket);
		WSACleanup();
	
}