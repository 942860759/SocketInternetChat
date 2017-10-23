// TCPServer.cpp�ļ�
/*
��ʹ��˵����
0.���г���ǰ��鿴�Ƿ�initsock.h
�ļ����뵽��Ŀ�С�
1.�����޸�����Է���IP��ַ
2.���������з���ˣ�TCPServer�����������пͻ��ˣ�TCPClient������
��������ȷ����˻��յ����������Ϣ��
3.���ӳɹ��󣬿����ڷ������˽���Ϳͻ��˽��滭ͼ
*/
#include "stdafx.h"
#pragma warning(disable:4996)
#include "InitSock.h"
#include <stdio.h>
#include <iostream>
#include <cstdio>
using namespace std;
CInitSock initSock;     // ��ʼ��Winsock��

SOCKET sockConn;//ȫ�ֱ��� 
SOCKET sockClient;//�ͻ���

				  //���ݷ����̵߳ĺ��� 
void ServerSend()
{

	char sendBuf[1024];
	memset(sendBuf, 0, 1024);
	//��ʼ�������� 
	//cin >> sendBuf;
	memcpy(sendBuf, &send_message, sizeof(send_message)); //�ṹ��ת�����ַ���

	::send(sockClient, sendBuf, sizeof(sendBuf), 0);  //�������� 

}

DWORD WINAPI ThreadRecvProc(LPVOID lpParam) {


	while (TRUE)
	{
		char buff[1024];
		memset(buff, 0, 1024);

		// �ӿͻ��˽�������
		int nRecv = ::recv(sockClient, buff, 1024, 0);
		int n = strlen(buff);
		if (n > 0)
		{
			memset(&recv_message, 0, sizeof(recv_message));//��սṹ��
			memcpy(&recv_message, buff, sizeof(recv_message));//�ѽ��յ�����Ϣת���ɽṹ��

			if (recv_message.category == 1) {
				m_ptOrigin.x = recv_message.m_ptOriginX;
				m_ptOrigin.y = recv_message.m_ptOriginY;
				m_bDraw = recv_message.m_bDraw;
			}
			else if (recv_message.category == 2) {
				m_bDraw = recv_message.m_bDraw;
			}
			else if (recv_message.category == 3) {
				HWND hWnd = FindWindowW(NULL, _T("SocketInternetChatServer"));
				HDC hdc = ::GetDC(hWnd);
				HPEN hNewPen, hOldPen;
				//�½�һ������  
				hNewPen = CreatePen(PS_SOLID, 1, 0x0000FF);
				//����ɵĻ���  
				hOldPen = (HPEN)SelectObject(hdc, hNewPen);
				if (m_bDraw == true) {
					MoveToEx(hdc, m_ptOrigin.x, m_ptOrigin.y, NULL);
					LineTo(hdc, recv_message.m_ptLaterX, recv_message.m_ptLaterY);
					m_ptOrigin.x = recv_message.m_ptLaterX;//ÿ�ζ��������
					m_ptOrigin.y = recv_message.m_ptLaterY;
				}

				::ReleaseDC(hWnd, hdc);//����ɶ�ʹ��
									   //������GetDC()������Ҫ��ReleaseDC()�����ɶ�ʹ��
			}
			else {

			}
		}
	}
}

int TCPServer()
{
	// �����׽���
	sockConn = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockConn == INVALID_SOCKET)
	{
		printf("Failed socket() \n");
		return 0;
	}

	// ���sockaddr_in�ṹ
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(4567);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;

	// ������׽��ֵ�һ�����ص�ַ
	if (::bind(sockConn, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("Failed bind() \n");
		return 0;
	}

	// �������ģʽ
	if (::listen(sockConn, 2) == SOCKET_ERROR)
	{
		printf("Failed listen() \n");
		return 0;
	}

	// ѭ�����ܿͻ�����������
	sockaddr_in remoteAddr;
	int nAddrLen = sizeof(remoteAddr);

	char szText[] = " TCP Server Demo! \r\n";

	// ����һ��������
	sockClient = ::accept(sockConn, (SOCKADDR*)&remoteAddr, &nAddrLen);
	if (sockClient == INVALID_SOCKET)
	{
		printf("Failed accept()");
	}

	printf("���ܵ�һ�����ӣ�%s \r\n", inet_ntoa(remoteAddr.sin_addr));

	//���ͼ��������̣߳� 
	//CreateThread(NULL, 0, ThreadSendProc, NULL, 0, NULL);
	CreateThread(NULL, 0, ThreadRecvProc, NULL, 0, NULL);

	// �ر�ͬ�ͻ��˵�����
	//::closesocket(sockClient);

	// �رռ����׽���
	//::closesocket(sockConn);
	return 0;
}
