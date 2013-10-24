#include "TSSocket.h"
#include "..\CCCommon.h"

#ifdef WIN32

#else

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include "netdb.h"
#include "netinet/in.h"
#include <arpa/inet.h>
  
#endif
using namespace std;

SOCKET TSSocket::CreateClient(string sIp, unsigned short usPort) {
    #ifdef WIN32
    WORD wVersionRequested;
    WSAData wasdata;
    int err;
    wVersionRequested = MAKEWORD(1,1);

    err = WSAStartup( wVersionRequested, &wasdata );
    if(err!=0){
        return NULL;
    }

    if ( LOBYTE( wasdata.wVersion ) != 1 ||
        HIBYTE( wasdata.wVersion ) != 1 ) {
            WSACleanup( );
            return NULL; 
    }

    //�����׽���
    m_hSocket = socket(AF_INET,SOCK_STREAM,0);
    SOCKADDR_IN addrSrv;

    //�����׽��ֵ���ز���;
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_port = htons(usPort);
    addrSrv.sin_addr.S_un.S_addr=inet_addr(sIp.c_str());

    //����TCP
    m_ConnectState = connect(m_hSocket,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
	#else
	 //���������ַ�ṹ��
     struct sockaddr_in addrSrv;
     //���ݳ�ʼ��--����
     memset(&addrSrv, 0, sizeof(addrSrv));
     //����socket��ز���
     addrSrv.sin_family = AF_INET;
     addrSrv.sin_port = htons(usPort);
     addrSrv.sin_addr.s_addr = inet_addr(sIp.c_str());
     //����һ��socket
     m_hSocket = socket(AF_INET, SOCK_STREAM, 0);
    //socket����ʧ��
    if (m_hSocket < 0)
    {
      //CCLog("%s", "socket failed!!!");
    }
    else
    {
      //CCLog("%s", "socket success!!!");
    }
    //����һ��socket����
    m_ConnectState = connect(m_hSocket, (struct sockaddr *) &addrSrv, sizeof(addrSrv));
	#endif
    if (m_ConnectState < 0)
    {   
        #ifdef WIN32
		closesocket(m_hSocket);
		#else
		close(m_hSocket);
		#endif
    }

    return m_hSocket;
}

void TSSocket::CloseSocket()
{   
    #ifdef WIN32
    ::closesocket(m_hSocket);
	#else
	::close(m_hSocket);
	#endif
    m_hSocket = 0;
}







