// TCPServer.cpp文件
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

SOCKET sockConn;//全局变量 
SOCKET sockClient;//客户端

				  //数据发送线程的函数 
void ServerSend()
{

	char sendBuf[1024];
	memset(sendBuf, 0, 1024);
	//初始化缓冲区 
	//cin >> sendBuf;
	memcpy(sendBuf, &send_message, sizeof(send_message)); //结构体转换成字符串

	::send(sockClient, sendBuf, sizeof(sendBuf), 0);  //发送数据 

}

DWORD WINAPI ThreadRecvProc(LPVOID lpParam) {


	while (TRUE)
	{
		char buff[1024];
		memset(buff, 0, 1024);

		// 从客户端接收数据
		int nRecv = ::recv(sockClient, buff, 1024, 0);
		int n = strlen(buff);
		if (n > 0)
		{
			memset(&recv_message, 0, sizeof(recv_message));//清空结构体
			memcpy(&recv_message, buff, sizeof(recv_message));//把接收到的信息转换成结构体

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

int TCPServer()
{
	// 创建套节字
	sockConn = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockConn == INVALID_SOCKET)
	{
		printf("Failed socket() \n");
		return 0;
	}

	// 填充sockaddr_in结构
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(4567);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;

	// 绑定这个套节字到一个本地地址
	if (::bind(sockConn, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("Failed bind() \n");
		return 0;
	}

	// 进入监听模式
	if (::listen(sockConn, 2) == SOCKET_ERROR)
	{
		printf("Failed listen() \n");
		return 0;
	}

	// 循环接受客户的连接请求
	sockaddr_in remoteAddr;
	int nAddrLen = sizeof(remoteAddr);

	char szText[] = " TCP Server Demo! \r\n";

	// 接受一个新连接
	sockClient = ::accept(sockConn, (SOCKADDR*)&remoteAddr, &nAddrLen);
	if (sockClient == INVALID_SOCKET)
	{
		printf("Failed accept()");
	}

	printf("接受到一个连接：%s \r\n", inet_ntoa(remoteAddr.sin_addr));

	//发送键盘数据线程： 
	//CreateThread(NULL, 0, ThreadSendProc, NULL, 0, NULL);
	CreateThread(NULL, 0, ThreadRecvProc, NULL, 0, NULL);

	// 关闭同客户端的连接
	//::closesocket(sockClient);

	// 关闭监听套节字
	//::closesocket(sockConn);
	return 0;
}
