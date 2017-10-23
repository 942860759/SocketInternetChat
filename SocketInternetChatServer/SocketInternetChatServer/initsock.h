#pragma once
// initsock.h�ļ�
#include <winsock2.h>
#include <stdlib.h> 
#include <conio.h> 
#include <stdio.h> 
#pragma comment(lib, "WS2_32")  // ���ӵ�WS2_32.lib
class CInitSock
{
public:
	CInitSock(BYTE minorVer = 2, BYTE majorVer = 2)
	{
		// ��ʼ��WS2_32.dll
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
	int category;//1��ʾOnLButtonDown��2��ʾOnLButtonUp��3��ʾOnMouseMove
	int m_ptOriginX;//��ʼ��x
	int m_ptOriginY;//��ʼ��y
	int m_ptLaterX;//����ƶ���ĵ�x
	int m_ptLaterY;//����ƶ���ĵ�y
	bool m_bDraw;//�жϱ�־
};
extern DrawMessage send_message;//�����ź�
extern DrawMessage recv_message;//�����ź�
extern CPoint m_ptOrigin;
extern bool m_bDraw;

extern SOCKET sockConn;//ȫ�ֱ��� 
extern SOCKET sockClient;//�ͻ���