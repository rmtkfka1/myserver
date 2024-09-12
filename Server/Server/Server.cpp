#include "pch.h"

//서버
// 1) 새로운 소켓 생성
// 2) 소켓에 주소/ 포트 번호설정(bind)
// 3) 소켓 일시키기 ( listen )
// 4) 손님 접속(accept)
// 5) 클라와 통신

int main()
{

	//윈도우 소켓 초기화 , 생성은 아님.//
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
		return -1;

	//소켓을 생성함
	// AF_INET => IPv4  [ Internetwork: UDP, TCP, etc ]
	// AF_INET6 => IPv6 
	// SOCK_STREAM -> TCP / SOCK_DGRAM -> UDP
	SOCKET listenSocket =  socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == INVALID_SOCKET) err_quit("소켓 생성 실패");

	// 2) 주소/포트 번호설정 (bind)단계
	sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); //IP주소 다받아줌
	serveraddr.sin_port = htons(9000); // 포트번호 => 응용프로그램 식별

	if (bind(listenSocket, (sockaddr*)&serveraddr, sizeof(serveraddr)) == SOCKET_ERROR)
	{
		err_quit("bind 실패");
	};


	//3) 업무개시( LISTEN ) 

	if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		err_quit("LISTEN 실패");
	}

	while (1)
	{
		sockaddr_in clientAddr;
		memset(&clientAddr, 0, sizeof(clientAddr));
		int32 addrLen = sizeof(clientAddr);
		
		SOCKET clientSocket = accept(listenSocket, (sockaddr*)&clientAddr, &addrLen);
		
		if (clientSocket == SOCKET_ERROR)
		{
			err_display("accept 실패");
			break;
		}

		char ip[16];

		inet_ntop(AF_INET, &clientAddr.sin_addr, ip, sizeof(ip));
		cout << "[서버] " << endl;
		cout << "Client Conneted IP: " << ip << endl;


		// 클라이언트와 데이터 통신
		while(1)
		{

			char recvBuffer[100];
			int32 recvLen = recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
			if (recvLen <= 0)
			{
				err_display("메세지 없음");
			}
			cout << "[메세지]:" << recvBuffer <<endl;

			char sendBuffer[100] = "하이용";
			int result = send(clientSocket, sendBuffer, sizeof(sendBuffer), 0);
			if (result == SOCKET_ERROR)
			{
				err_display("클라에게 메세지 전송 실패");
				break;
			}

		}

		closesocket(clientSocket);
	}



	closesocket(listenSocket);
	WSACleanup();

}