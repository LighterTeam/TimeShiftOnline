#include "TSHallNum.h"
#include "TSEngine/TSTCP.h"
#include "TSEngine/TSEvent.h"
#include "TSEngine/TSSocket.h"
#include "TSEngine/TSScriptJS.h"

TSHallNum::TSHallNum() : m_flowID(-1), m_isRobot(0) {
    TSEvent::GetSingleTon()->RegistEvent("", (void*)this, (TpInstEventFun)&TSHallNum::ReConnectSuccess_CallBack);
}

TSHallNum::~TSHallNum() {

}

// ���ڰ�.;
bool TSHallNum::ReConnect()
{
    TSTCP::GetSingleTon()->CreateClient(m_sWG_IP, m_iWG_Port);
    int err = TSSocket::getSingleTon()->getConnectState();
    if (err >= 0)
    {
        TSTCP::GetSingleTon()->SendMessageToServer(std::string("ReConnect,") + m_sUUID);   
    }
    return true;
}

void TSHallNum::ReConnectSuccess_CallBack(string sBuffer)
{
    //�رմ����������;
}

void TSHallNum::OpenReConnectWnd()
{
    //OpenReConnectWnd_CallBack ��ťע�� //���������ť֮��.����. ReConnect + UUID ��������.֮��...
}

void TSHallNum::OpenReConnectWnd_CallBack()
{
    ReConnect();
}

void TSHallNum::initSocket()
{
    TSScriptJS::GetSingleTon()->GetWebConfig();
    char* adapter_ip=(char*)TSHallNum::getSingleTon()->m_Adapter_Ip.c_str();
    int  adapter_port=atoi(TSHallNum::getSingleTon()->m_Adapter_Port.c_str());
    TSTCP::GetSingleTon()->CreateClient(adapter_ip, adapter_port);
    int m_iConnectState = TSSocket::getSingleTon()->getConnectState();
    if (m_iConnectState >= 0)
    {
        TSTCP::GetSingleTon()->SendMessageToServer("ConnectGateWay"); 
    }
}
