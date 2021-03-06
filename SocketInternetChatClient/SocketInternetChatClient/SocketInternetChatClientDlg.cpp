
// SocketInternetChatClientDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "SocketInternetChatClient.h"
#include "SocketInternetChatClientDlg.h"
#include "afxdialogex.h"
#include "initsock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPoint m_ptOrigin;
bool m_bDraw;
DrawMessage send_message;//发送信号
DrawMessage recv_message;//接收信号

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSocketInternetChatClientDlg 对话框



CSocketInternetChatClientDlg::CSocketInternetChatClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_SOCKETINTERNETCHATCLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSocketInternetChatClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSocketInternetChatClientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CSocketInternetChatClientDlg 消息处理程序

BOOL CSocketInternetChatClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	TCPClient();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSocketInternetChatClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSocketInternetChatClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSocketInternetChatClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSocketInternetChatClientDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//MessageBox(L"View Clicked!");
	m_ptOrigin = point;
	m_bDraw = true;

	send_message.category = 1;
	send_message.m_ptOriginX = point.x;
	send_message.m_ptOriginY = point.y;
	send_message.m_bDraw = true;

	ClientSend();

	CDialog::OnLButtonDown(nFlags, point);
}


void CSocketInternetChatClientDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bDraw = false;

	send_message.category = 2;
	send_message.m_bDraw = false;

	ClientSend();

	CDialog::OnLButtonUp(nFlags, point);
}


void CSocketInternetChatClientDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CClientDC dc(this);
	//创建一个画笔
	CPen pen(PS_SOLID, 1, RGB(255, 0, 0));
	//将创建的画笔选入设备描述表
	CPen *pOldPen = dc.SelectObject(&pen);
	if (m_bDraw == true)
	{
		dc.MoveTo(m_ptOrigin);
		dc.LineTo(point);
		m_ptOrigin = point;  //每次都更新起点

		send_message.category = 3;
		send_message.m_ptLaterX = point.x;
		send_message.m_ptLaterY = point.y;

		ClientSend();
	}

	

	CDialog::OnMouseMove(nFlags, point);
}


void CSocketInternetChatClientDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// 关闭套节字
	::closesocket(sockClient);

	CDialog::OnClose();
}
