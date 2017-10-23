//////////////////////////////////////////////////////////
// TCPClient.cpp�ļ�
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
						//ȫ�ֱ������壬���̹߳���ı��� 
SOCKET sockClient;

//���ݷ����߳� 
void ClientSend()
{
	
		char sendBuf[1024];
		memset(sendBuf, 0, 1024);    //��ʼ�������� 
									 //ɨ����� 
		//cin >> sendBuf;
		memcpy(sendBuf, &send_message, sizeof(send_message)); //�ṹ��ת�����ַ���

		::send(sockClient, sendBuf, sizeof(sendBuf), 0);  //�������� 
	
}

DWORD WINAPI ThreadRecvProc(LPVOID lpParam) {

	while (TRUE)
	{
		char recvBuf[1024];
		memset(recvBuf, 0, 1024);    //��ʼ�������� 
		//�ӷ������˽�������
		int nRecv = ::recv(sockClient, recvBuf, 1024, 0);
		int n = strlen(recvBuf);
		if (n > 0)
		{
			memset(&recv_message, 0, sizeof(recv_message));//��սṹ��
			memcpy(&recv_message, recvBuf, sizeof(recv_message));//�ѽ��յ�����Ϣת���ɽṹ��
			if (recv_message.category == 1) {
				m_ptOrigin.x = recv_message.m_ptOriginX;
				m_ptOrigin.y = recv_message.m_ptOriginY;
				m_bDraw = recv_message.m_bDraw;
			}
			else if (recv_message.category == 2) {
				m_bDraw = recv_message.m_bDraw;
			}
			else if (recv_message.category == 3) {
				
				HWND hWnd = FindWindowW(NULL, _T("SocketInternetChatClient"));
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

int TCPClient()
{
	// �����׽���
	sockClient = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockClient == INVALID_SOCKET)
	{
		printf(" Failed socket() \n");
		return 0;
	}

	// Ҳ�������������bind������һ�����ص�ַ
	// ����ϵͳ�����Զ�����

	// ��дԶ�̵�ַ��Ϣ
	sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(4567);
	// ע�⣬����Ҫ��д����������TCPServer�������ڻ�����IP��ַ
	// �����ļ����û��������ֱ��ʹ��127.0.0.1����
	servAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	if (::connect(sockClient, (sockaddr*)&servAddr, sizeof(servAddr)) == -1)
	{
		printf(" Failed connect() \n");
		return 0;
	}

	

	//�������ݽ����̣߳���ں�����ThreadRecvProc(   ) 
	CreateThread(NULL, 0, ThreadRecvProc, NULL, 0, NULL);

	// �ر��׽���
	//::closesocket(sockClient);
	return 0;
}
