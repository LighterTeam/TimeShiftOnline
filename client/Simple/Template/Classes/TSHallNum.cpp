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

// 窗口绑定.;
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
    //关闭窗口完成重连;
}

void TSHallNum::OpenReConnectWnd()
{
    //OpenReConnectWnd_CallBack 按钮注册 //点击重连按钮之后.发送. ReConnect + UUID 到服务器.之后...
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
