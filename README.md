# SocketInternetChat
Socket，mfc，Draw
# 一、	开发环境和配置说明（列出开发工具、开发包的名称、版本、特殊配置）
本程序在vs2017下编译运行，对开发工具没有特殊要求，vs系列以及能编译C++MFC的开发工具都可运行，运行程序前请查看是否将initsock.h文件引入到项目中。
# 二、	程序的网络结构、流程和原理
参考网址：http://blog.csdn.net/doudouxuexi/article/details/48544959
以及课件《网络与socket编程》

程序完成分三个阶段。
一：创建客户端和服务端并能够相互间通信。
二：创建MFC鼠标画图程序。
三：将MFC鼠标响应事件转换为字符串参数并通过socket在客户端和服务端传递。
原理主要是Socket编程，MFC编程。要注意的是，实时通信要使用进程来完成，鼠标响应事件对应发送消息（即事件响应），而接收消息是一直在运行（即一直接收消息，但消息满足条件才执行）。
# 三、	程序的输入和输出
注意：必须先运行SocketInternetChatServer程序，然后运行SocketInternetChatClient程序。因为server程序先运行等待client程序连接它，否则不能正常运行（这个bug可以添加连接按钮响应事件完成）
用鼠标在窗口绘制图形，则在另一个窗口实时绘制相同的图形。


