//////////////////////////////////////////////////////////
// TCPClient.cpp文件
/*
《使用说明》
0.运行程序前请查看是否将initsock.h
文件引入到项目中。
1.首先修改聊天对方的IP地址
2.请首先运行服务端（TCPServer）程序，再运行客户端（TCPClient）程序：
如配置正确服务端会收到相关连接信息。
3.连接成功后，可以在服务器端界面和客户端界面画图
*/
#include "stdafx.h"
#pragma warning(disable:4996)
#include "InitSock.h"
#include <stdio.h>
#include <iostream>
#include <cstdio>
using namespace std;
CInitSock initSock;     // 初始化Winsock库
						//全局变量定义，多线程共享的变量 
SOCKET sockClient;

//数据发送线程 
void ClientSend()
{
	
		char sendBuf[1024];
		memset(sendBuf, 0, 1024);    //初始化缓冲区 
									 //扫描键盘 
		//cin >> sendBuf;
		memcpy(sendBuf, &send_message, sizeof(send_message)); //结构体转换成字符串

		::send(sockClient, sendBuf, sizeof(sendBuf), 0);  //发送数据 
	
}

DWORD WINAPI ThreadRecvProc(LPVOID lpParam) {

	while (TRUE)
	{
		char recvBuf[1024];
		memset(recvBuf, 0, 1024);    //初始化缓冲区 
		//从服务器端接收数据
		int nRecv = ::recv(sockClient, recvBuf, 1024, 0);
		int n = strlen(recvBuf);
		if (n > 0)
		{
			memset(&recv_message, 0, sizeof(recv_message));//清空结构体
			memcpy(&recv_message, recvBuf, sizeof(recv_message));//把接收到的信息转换成结构体
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
				//新建一个画笔  
				hNewPen = CreatePen(PS_SOLID, 1, 0x0000FF);
				//保存旧的画笔  
				hOldPen = (HPEN)SelectObject(hdc, hNewPen);
				if (m_bDraw == true) {
					MoveToEx(hdc, m_ptOrigin.x, m_ptOrigin.y, NULL);
					LineTo(hdc, recv_message.m_ptLaterX, recv_message.m_ptLaterY);
					m_ptOrigin.x = recv_message.m_ptLaterX;//每次都更新起点
					m_ptOrigin.y = recv_message.m_ptLaterY;
				}

				::ReleaseDC(hWnd, hdc);//必须成对使用
									   //这里获得GetDC()函数需要跟ReleaseDC()函数成对使用
			}
			else {

			}
		}
	}
}

int TCPClient()
{
	// 创建套节字
	sockClient = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockClient == INVALID_SOCKET)
	{
		printf(" Failed socket() \n");
		return 0;
	}

	// 也可以在这里调用bind函数绑定一个本地地址
	// 否则系统将会自动安排

	// 填写远程地址信息
	sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(4567);
	// 注意，这里要填写服务器程序（TCPServer程序）所在机器的IP地址
	// 如果你的计算机没有联网，直接使用127.0.0.1即可
	servAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	if (::connect(sockClient, (sockaddr*)&servAddr, sizeof(servAddr)) == -1)
	{
		printf(" Failed connect() \n");
		return 0;
	}

	

	//创建数据接收线程，入口函数是ThreadRecvProc(   ) 
	CreateThread(NULL, 0, ThreadRecvProc, NULL, 0, NULL);

	// 关闭套节字
	//::closesocket(sockClient);
	return 0;
}
