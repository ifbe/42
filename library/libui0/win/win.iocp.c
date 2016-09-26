#include <iostream>
#include <string>
#include <windows.h>
#include <winsock2.h>
 
#define OP_READ   1
#define OP_WRITE 2
#define OP_ACCEPT 3
#define BUFFER_SIZE 1024
 
typedef struct _PER_HANDLE_DATA
{
	SOCKET	  s;	  // 对应的套接字句柄
	sockaddr_in addr;   // 对方的地址
 
}PER_HANDLE_DATA, *PPER_HANDLE_DATA;
 
typedef struct _PER_IO_DATA
{
	OVERLAPPED ol;				 // 重叠结构
	char		buf[BUFFER_SIZE];   // 数据缓冲区
	int		 nOperationType;	 // 操作类型
}PER_IO_DATA, *PPER_IO_DATA;
 
 


DWORD WINAPI ServerThread( LPVOID lpParam )
{
	HANDLE hIocp = ( HANDLE )lpParam;
	if( hIocp == NULL )
	{
		return -1;
	}
 
	DWORD dwTrans = 0;
	PPER_HANDLE_DATA pPerHandle;
	PPER_IO_DATA	 pPerIo;
	 
	while( TRUE )
	{
		// 在关联到此完成端口的所有套接字上等待I/O完成
		BOOL bRet = GetQueuedCompletionStatus( hIocp, &dwTrans, (LPDWORD)&pPerHandle, (LPOVERLAPPED*)&pPerIo, WSA_INFINITE );
		if( !bRet )	 // 发生错误
		{
			closesocket( pPerHandle->s );
			GlobalFree( pPerHandle );
			GlobalFree( pPerIo );
 
			cout << "error" << endl;
			continue;
		}
 
		// 套接字被对方关闭
		if( dwTrans == 0 && ( pPerIo->nOperationType == OP_READ || pPerIo->nOperationType&nb-sp;== OP_WRITE ) )
		{
			closesocket( pPerHandle->s );
			GlobalFree( pPerHandle );
			GlobalFree( pPerIo );
 
			cout << "client closed" << endl;
			continue;
		}
 
		switch ( pPerIo->nOperationType )
		{
			case OP_READ:	   // 完成一个接收请求
			{
				pPerIo->buf[dwTrans] = '\0';
				printf( "%s\n", pPerIo->buf );
 
				// 继续投递接受操作
				WSABUF buf;
				buf.buf = pPerIo->buf;
				buf.len = BUFFER_SIZE;
				pPerIo->nOperationType = OP_READ;
				 
				DWORD dwRecv = 0;
				DWORD dwFlags = 0;
				 
				WSARecv( pPerHandle->s, &buf, 1, &dwRecv, &dwFlags, &pPerIo->ol, NULL );
				break;
			}

			case OP_WRITE:
			case OP_ACCEPT:break;
		}
 
	}
 
	return 0;
}
void windowcreate()
{
	int ret;
	int nPort = 2222;
	WSADATA wsaData;

	ret = WSAStartup(MAKEWORD( 2, 2 ), &wsaData);
	if( ret != 0)
	{
		printf(
			"Using:%s (Status:%s)\n"
			"with API versions: %d.%d to %d.%d\n",
			wsaData.szDescription, wsaData.szSystemStatus,
			LOBYTE( wsaData.wVersion), HIBYTE( wsaData.wVersion ),
			LOBYTE( wsaData.wHighVersion), HIBYTE( wsaData.wHighVersion)
		);
 
		exit(-1);
	}
 
	// 创建完成端口对象
	// 创建工作线程处理完成端口对象的事件
	HANDLE hIocp = CreateIoCompletionPort( INVALID_HANDLE_VALUE, 0, 0, 0 );
	CreateThread( NULL, 0, ServerThread, (LPVOID)hIocp, 0, 0 );
 
	// 创建监听套接字，绑定本地端口，开始监听
	SOCKET sListen = socket( AF_INET,-SOCK_STREAM, 0 );
 
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons( nPort );
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	bind( sListen, (sockaddr *)&addr, sizeof( addr ) );
	listen( sListen, 5 );
 
	// 循环处理到来的请求
	printf( "forever!\n" );
	while ( TRUE )
	{
		// 等待接受未决的连接请求
		SOCKADDR_IN saRemote;
		int nRemoteLen = sizeof( saRemote );
		SOCKET sRemote = accept( sListen, (sockaddr *)&saRemote, &nRemoteLen );
 
		// 接受到新连接之后，为它创建一个per_handle数据，并将他们关联到完成端口对象
		PPER_HANDLE_DATA pPerHandle = ( PPER_HANDLE_DATA )GlobalAlloc( GPTR, sizeof( PPER_HANDLE_DATA ) );
		if( pPerHandle == NULL )
		{
			break;
		}
 
		pPerHandle->s = sRemote;
		memcpy( &pPerHandle->addr, &saRemote, nRemoteLen );
 
		CreateIoCompletionPort( ( HANDLE)pPerHandle->s, hIocp, (DWORD)pPerHandle, 0 );
 
		// 投递一个接受请求
		PPER_IO_DATA pIoData = ( PPER_IO_DATA )GlobalAlloc( GPTR, sizeof( PPER_IO_DATA ) );
		if( pIoData == NULL )
		{
			break;
		}
 
		pIoData->nOperationType = OP_READ;
		WSABUF buf;
		buf.buf = pIoData->buf;
		buf.len = BUFFER_SIZE;
		 
		DWORD dwRecv = 0;
		DWORD dwFlags = 0;
 
		WSARecv( pPerHandle->s, &buf, 1, &dwRecv, &dwFlags, &pIoData->ol, NULL );
	}
}
void windowdelete()
{
	WSACleanup();
}
