#pragma once

#include "pch.h"
#include "framework.h"

class CServerSocket
{
public:
	static CServerSocket* GetInstance();
	static void ReleaseInstance();

	// ���ÿ����͹��캯��
	CServerSocket(const CServerSocket&) = delete;
	CServerSocket(CServerSocket&&) = delete;
	CServerSocket& operator=(const CServerSocket&) = delete;
	CServerSocket& operator=(CServerSocket&&) = delete;

	BOOL InitSockEnv();
	BOOL InitSocket();
	BOOL AcceptClient();

	int DealCommand();
	BOOL SendData(const char* pData, int nSize);

private:
	CServerSocket();
	~CServerSocket();

private:
	class CHelper
	{
	public:
		CHelper();
		~CHelper();
	};

private:
	// ���������࣬��������������
	// ��̬�����Ĵ�����main����֮ǰ
	static CHelper m_helper;
	static CServerSocket* m_instance;


	SOCKET m_serverSocket;
	SOCKET m_clientSocket;
};

