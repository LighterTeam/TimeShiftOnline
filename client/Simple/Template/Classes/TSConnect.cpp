#include "TSConnect.h"
#include "TSEngine/TSTCP.h"
#include "TSEngine/TSEvent.h"
#include "TSEngine/TSSocket.h"
#include "TSEngine/TSScriptJS.h"
#include "json/json.h"

TSConnect::TSConnect() : m_iUUID(0) {

}

TSConnect::~TSConnect() {

}

// ´°¿Ú°ó¶¨
bool TSConnect::ReConnect()
{
    TSTCP::GetSingleTon()->CreateClient(m_sWG_IP, m_iWG_Port);
    int err = TSTCP::GetSingleTon()->GetConnectState();
    if (err >= 0)
    {
        Json::Value jValue;
        jValue["MM"] = "ReConnect";
        jValue["UUID"] = m_iUUID; 
        TSTCP::GetSingleTon()->SendMessageToServer(jValue.toStyledString());   
    }
    return true;
}

void TSConnect::initSocket()
{
    TSConnect* pC = TSConnect::GetSingleTon();
    TSScriptJS::GetSingleTon()->GetWebConfig(pC->m_Adapter_Ip, pC->m_Adapter_Port);
    if (TSTCP::GetSingleTon()->CreateClient(pC->m_Adapter_Ip, pC->m_Adapter_Port) != 0)
    {
        TSTCP::GetSingleTon()->SendMessageToServer("{\"MM\":\"ConnectGateWay\"}"); 
    } else {
        TSEvent::GetSingleTon()->SendMsg("Disconnect","Disconnect");
    }
}
