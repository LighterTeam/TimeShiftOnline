#include "TSConnect.h"
#include "TSEngine/TSTCP.h"
#include "TSEngine/TSEvent.h"
#include "TSEngine/TSSocket.h"
#include "TSEngine/TSScriptJS.h"

TSConnect::TSConnect() {

}

TSConnect::~TSConnect() {

}

// ���ڰ�
bool TSConnect::ReConnect()
{
    TSTCP::GetSingleTon()->CreateClient(m_sWG_IP, m_iWG_Port);
    int err = TSTCP::GetSingleTon()->GetConnectState();
    if (err >= 0)
    {
        TSTCP::GetSingleTon()->SendMessageToServer(std::string("ReConnect,") + m_sUUID);   
    }
    return true;
}

void TSConnect::ReConnectSuccess_CallBack(string sBuffer)
{
    //�رմ����������;
}

void TSConnect::OpenReConnectWnd()
{
    //OpenReConnectWnd_CallBack ��ťע�� //���������ť֮��.����. ReConnect + UUID ��������.֮��...
}

void TSConnect::OpenReConnectWnd_CallBack()
{
    ReConnect();
}

void TSConnect::initSocket()
{
    TSScriptJS::GetSingleTon()->GetWebConfig();
    std::string& adapter_ip = TSConnect::getSingleTon()->m_Adapter_Ip;
    int  adapter_port = TSConnect::getSingleTon()->m_Adapter_Port;
    TSTCP::GetSingleTon()->CreateClient(adapter_ip, adapter_port);
    int m_iConnectState = TSTCP::GetSingleTon()->GetConnectState();
    if (m_iConnectState >= 0)
    {
        TSTCP::GetSingleTon()->SendMessageToServer("{\"MM\":\"ConnectGateWay\"}"); 
    }
}
