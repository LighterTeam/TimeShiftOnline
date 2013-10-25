#include "TSTCP.h"
//#include "TSCommon.h"
#include "TSSocket.h"
#include "TSEvent.h"
#include "exbuffer.h"
#include <vector>
#include "..\CCCommon.h"
#include "TSEngine.h"
#include "TSConnect.h"
#include "json/json.h"

#ifdef WIN32
#include "pthread\pthread.h"
#else
#include <sys/socket.h>
#include <pthread.h>
#endif

char LB_BUFFER[1024*30] = {0};

TSTCP* TSTCP::g_pTCPPtr = NULL;

TSTCP::TSTCP()
    : m_hSocket(0)
{

}

char buffer[1024*2] = {0};
static void recvHandle(unsigned char *rbuf, size_t len)
{
    TSTCP::GetSingleTon()->Lock();
    {
        memcpy(buffer, (char*)(rbuf), len);    
        std::string& serverRecv = std::string("ServerRecv: ") + buffer;
        cocos2d::CCLog(serverRecv.c_str());
        Json::Reader reader;
        Json::Value root;
        if (reader.parse(buffer, root))  
        {
            std::string sHeader = root["MM"].asString();
            TSEvent::GetSingleTon()->PushMessge(sHeader, buffer);
        }
        else
        {
            cocos2d::CCLog("Message Error!");
        }
    }
    TSTCP::GetSingleTon()->UnLock();
}

static void* GF_thread_function(void *arg) 
{
    SOCKET tcpsocket = ((TSTCP*)arg)->m_hSocket;
    char cBuffer[1024] = {0};

    exbuffer_t* exB;
    exB = exbuffer_new();
    exB->recvHandle = recvHandle;

    for (;;) {
        memset(cBuffer, 0, sizeof(cBuffer));
        int bufLen = recv(tcpsocket, cBuffer, 1024, 0);
        if (bufLen == -1)
        {
            ((TSTCP*)arg)->m_hSocket = 0;
            cocos2d::CCLog("Disconnect TCP Break Thread!");
            break;
        }
        
        if (bufLen > 0) {
            exbuffer_put(exB,(unsigned char*)cBuffer,0,bufLen);
        }
    }
    cocos2d::CCLog("Close TCP Thread!");
    exbuffer_free(&exB);
    return NULL;
}

SOCKET TSTCP::CreateClient( std::string sIP, int iPort)
{
    cocos2d::CCLog("Connect Server IP:%s Port:%d", sIP.c_str(), iPort);

    TSSocket* tsS = TSSocket::GetSingleTon();
    m_hSocket = tsS->CreateClient(sIP, iPort);

    pthread_attr_t tAttr;
    int errCode = pthread_attr_init(&tAttr);
    errCode = pthread_attr_setdetachstate(&tAttr, PTHREAD_CREATE_DETACHED);
    if (errCode != 0) {
        pthread_attr_destroy(&tAttr);
    }
    pthread_mutex_init(&m_mMutex,NULL);
    pthread_create(&m_idThread, &tAttr, GF_thread_function, this);
    return m_hSocket;
}

void TSTCP::threadFunction( std::string& sBuffer )
{

}

void TSTCP::Lock()
{
    if (!m_hSocket)
    {
        return;
    }
    pthread_mutex_lock(&m_mMutex);
}

void TSTCP::UnLock()
{
    if (!m_hSocket)
    {
        return;
    }
    pthread_mutex_unlock(&m_mMutex);
}

void TSTCP::ProcessMsg()
{
    if (!m_hSocket)
    {
        return;
    }

    Lock();
    TSEvent* pE = TSEvent::GetSingleTon();
    if (pE->m_MsgList.size() > 0)
    {
        pE->SendMsg(pE->m_MsgList.begin()->first, pE->m_MsgList.begin()->second);
        pE->m_MsgList.pop_front();
    }

    if (pE->m_MsgListRoot.size() > 0)
    {
		CCLOG("SendMM");
        char* pSrc = pE->m_MsgListRoot.begin()->first;
        pE->SendRoot(pSrc, pE->m_MsgListRoot.begin()->second);
        delete pSrc;
        pE->m_MsgListRoot.pop_front();
    }
    UnLock();
}

int TSTCP::SendMessageToServer( char* cBuffer, int iLen ) 
{
    cocos2d::CCLog("SendBuffer: BUF:%s Len:%d", cBuffer, iLen);
    char* sendBuf = new char[iLen + 4];
    unsigned short* BufLen = (unsigned short*)sendBuf;
    *BufLen = _ntohs(iLen + 2, EXBUFFER_BIG_ENDIAN);
    BufLen += 1;
    *BufLen = _ntohs(iLen, EXBUFFER_BIG_ENDIAN);
    memcpy(sendBuf + 4, cBuffer, iLen);
    int err = send(m_hSocket, sendBuf, iLen + 4, 0);

    delete [] sendBuf;
    return err;
}

int TSTCP::SendMessageToServer( std::string sBuffer )
{
    cocos2d::CCLog("SendBuffer: BUF:%s", sBuffer.c_str());
    int len = sBuffer.length();
    char* sendBuf = new char[len + 4];
    unsigned short* BufLen = (unsigned short*)sendBuf;
    *BufLen = _ntohs(len+2, EXBUFFER_BIG_ENDIAN);
    BufLen += 1;
    *BufLen = _ntohs(len, EXBUFFER_BIG_ENDIAN);
    memcpy(sendBuf+4, sBuffer.c_str(), len);
    int err = send(m_hSocket, sendBuf, len+4, 0);

    if (err < 0)
    {
        //µ¯³ö´°¿Ú.
        TSConnect::getSingleTon()->OpenReConnectWnd();
    }

    delete [] sendBuf;
    return err;
}

