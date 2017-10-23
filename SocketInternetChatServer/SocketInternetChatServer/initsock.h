#pragma once
// initsock.h文件
#include <winsock2.h>
#include <stdlib.h> 
#include <conio.h> 
#include <stdio.h> 
#pragma comment(lib, "WS2_32")  // 链接到WS2_32.lib
class CInitSock
{
public:
	CInitSock(BYTE minorVer = 2, BYTE majorVer = 2)
	{
		// 初始化WS2_32.dll
		WSADATA wsaData;
		WORD sockVersion = MAKEWORD(minorVer, majorVer);
		if (::WSAStartup(sockVersion, &wsaData) != 0)
		{
			exit(0);
		}
	}
	~CInitSock()
	{
		::WSACleanup();
	}
};

extern void ServerSend();
extern DWORD WINAPI ThreadRecvProc(LPVOID lpParam);
int TCPServer();

struct DrawMessage {
	int category;//1表示OnLButtonDown，2表示OnLButtonUp，3表示OnMouseMove
	int m_ptOriginX;//初始点x
	int m_ptOriginY;//初始点y
	int m_ptLaterX;//鼠标移动后的点x
	int m_ptLaterY;//鼠标移动后的点y
	bool m_bDraw;//判断标志
};
extern DrawMessage send_message;//发送信号
extern DrawMessage recv_message;//接收信号
extern CPoint m_ptOrigin;
extern bool m_bDraw;

extern SOCKET sockConn;//全局变量 
extern SOCKET sockClient;//客户端