// RemoteCtl.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

// 设置子系统和入口函数
//#pragma comment(linker,"subsystem:windows /entry:WinMainCRTStartup")
//#pragma comment(linker,"subsystem:windows /entry:mainCRTStartup")
//#pragma comment(linker,"subsystem:console /entry:mainCRTStartup")
//#pragma comment(linker,"subsystem:console /entry:WinMainCRTStartup")


#include "pch.h"
#include "framework.h"
#include "RemoteCtl.h"
#include "ServerSocket.h"
#include "Packet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一的应用程序对象

CWinApp theApp;

using namespace std;

void Dump(BYTE* pData, size_t nSize)
{
    std::string strOut;
    for (size_t i = 0; i < nSize; i++)
    {
        char buf[8] = "";
        if (i % 16 == 0)
        {
            strOut += "\n";
        }

        snprintf(buf, sizeof(buf), "%02X", pData[i] & 0xFF);
        strOut += buf;
    }

    strOut += "\n";
    OutputDebugStringA(strOut.c_str());
}

std::string MakeDirveInfo()
{
    std::string ret;
    DWORD drives = GetLogicalDrives();
    if (drives == 0)
    {
        TRACE("Cannot get logical drives");
        return ret;
    }
    else
    {
        for (int i = 0; i < 26; i++)
        {
            if (drives & (1 << i))
            {
                ret += ('A' + i);
            }
        }
    }

    SPacket packet = SPacket(1, (BYTE *)ret.c_str(), ret.size());
    Dump((BYTE*)&packet, packet.m_sHead.m_nLength);

    return ret;
}

int main()
{
    int nRetCode = 0;

    HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        // 初始化 MFC 并在失败时显示错误
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: 在此处为应用程序的行为编写代码。
            wprintf(L"错误: MFC 初始化失败\n");
            nRetCode = 1;
        }
        else
        {
            // TODO: 在此处为应用程序的行为编写代码。
            CServerSocket* pserver = CServerSocket::GetInstance();
            int cnt = 0;

            if (pserver->InitSocket() == FALSE)
            {
                MessageBox(NULL, _T("网络初始化异常"), _T("网络初始化失败"), MB_OK | MB_ICONERROR);
                exit(0);
            }

            while (CServerSocket::GetInstance() != NULL)
            {
                
                if (pserver->AcceptClient() == FALSE)
                {
                    if (cnt >= 3)
                    {
                        MessageBox(NULL, _T("用户多次连接失败，程序自动结束"), _T("连接失败"), MB_OK | MB_ICONERROR);
                        exit(0);
                    }

                    cnt++;
                    MessageBox(NULL, _T("用户连接失败"), _T("连接失败"), MB_OK | MB_ICONERROR);
                }
                
                int ret = pserver->DealCommand();
            }
        }
    }
    else
    {
        // TODO: 更改错误代码以符合需要
        wprintf(L"错误: GetModuleHandle 失败\n");
        nRetCode = 1;
    }

    return nRetCode;
}
