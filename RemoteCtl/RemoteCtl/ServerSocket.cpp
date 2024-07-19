#include "pch.h"
#include "ServerSocket.h"

#define RECV_BUFFER_SIZE 1024

CServerSocket::CHelper CServerSocket::m_helper;
CServerSocket *CServerSocket::m_instance = NULL;

CServerSocket* CServerSocket::GetInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new CServerSocket();
	}
	return m_instance;
}

void CServerSocket::ReleaseInstance()
{
	if (m_instance != NULL)
	{
		CServerSocket* temp = m_instance;
		m_instance = NULL;
		delete temp;
	}
}

CServerSocket::CServerSocket()
{
	if (!InitSockEnv())
	{
		MessageBox(NULL, _T("无法初始化套接字环境，请检查网络"), _T("初始化错误"), MB_OK | MB_ICONERROR);
		exit(0);
	}
	m_serverSocket = socket(PF_INET, SOCK_STREAM, 0);
	m_clientSocket = INVALID_SOCKET;
}

CServerSocket::~CServerSocket()
{
	closesocket(m_serverSocket);
	WSACleanup();
}

BOOL CServerSocket::InitSockEnv()
{
	WSADATA data;
	if (WSAStartup(MAKEWORD(1, 1), &data) != 0)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CServerSocket::InitSocket()
{
	if (m_serverSocket == -1)
	{
		return FALSE;
	}

	sockaddr_in servAddr;
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	servAddr.sin_port = htons(9100);
	if (bind(m_serverSocket, (sockaddr*)&servAddr, sizeof(servAddr)) == -1)
	{
		return FALSE;
	}

	if (listen(m_serverSocket, 1) == -1)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CServerSocket::AcceptClient()
{
	sockaddr_in clientAddr;
	int clientLen = sizeof(clientAddr);
	memset(&clientAddr, 0, sizeof(clientAddr));
	
	m_clientSocket = accept(m_serverSocket, (sockaddr*)&clientAddr, &clientLen);
	if (m_clientSocket == -1)
	{
		return FALSE;
	}

	return TRUE;
}

int CServerSocket::DealCommand()
{
	if (m_clientSocket == -1)
	{
		return -1;
	}

	size_t index = 0;
	char buffer[RECV_BUFFER_SIZE];
	while (TRUE)
	{
		int len = recv(m_clientSocket, buffer, RECV_BUFFER_SIZE, 0);
		if (len <= 0)
		{
			return -1;
		}

		index += len;
		len = index;
		m_packet = SPacket((BYTE*)buffer, len);
		if (len > 0)
		{
			memmove(buffer, buffer + len, RECV_BUFFER_SIZE - len);
			index -= len;
			return m_packet.m_sHead.m_sCmd;
		}
	}
}

BOOL CServerSocket::SendData(const char* pData, int nSize)
{
	return (send(m_clientSocket, pData, nSize, 0) > 0) ? TRUE : FALSE;
}

CServerSocket::CHelper::CHelper()
{
	CServerSocket::GetInstance();
}

CServerSocket::CHelper::~CHelper()
{
	CServerSocket::ReleaseInstance();
}
