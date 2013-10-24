#include "main.h"
#include "AppDelegate.h"
#include "CCEGLView.h"
#include "TSEngine\TSTCP.h"
#include "TSEngine\TSScriptJS.h"
#include "TSHallNum.h"
#include "TSEngine\TSSocket.h"
#include <time.h>
#include "TSEngine\TSEngine.h"
#include "TSEngine\TSEvent.h"
#include "ScriptingCore.h"
#include "generated/jsb_cocos2dx_auto.hpp"
#include "generated/jsb_cocos2dx_extension_auto.hpp"
#include "jsb_cocos2dx_extension_manual.h"
#include "cocos2d_specifics.hpp"
#include "js_bindings_chipmunk_registration.h"
#include "js_bindings_system_registration.h"

USING_NS_CC;

// uncomment below line, open debug console
// #define USE_WIN32_CONSOLE

//int APIENTRY _tWinMain(HINSTANCE hInstance,
//                       HINSTANCE hPrevInstance,
//                       LPTSTR    lpCmdLine,
//                       int       nCmdShow)
//{
//    UNREFERENCED_PARAMETER(hPrevInstance);
//    UNREFERENCED_PARAMETER(lpCmdLine);
//
//#ifdef USE_WIN32_CONSOLE
//    AllocConsole();
//    freopen("CONIN$", "r", stdin);
//    freopen("CONOUT$", "w", stdout);
//    freopen("CONOUT$", "w", stderr);
//#endif
//
//    // create the application instance
//    AppDelegate app;
//    CCEGLView* eglView = CCEGLView::sharedOpenGLView();
//
//    eglView->setFrameSize(1136, 640);
//
//    int ret = CCApplication::sharedApplication()->run();
//
//#ifdef USE_WIN32_CONSOLE
//    FreeConsole();
//#endif
//
//    return ret;
//}
using namespace std;

char buf[32] = {0};

class Msg : public TSObject
{
public:
    void TSEventLogin(std::string sBuffer)
    {
         CCLog("TSEventLogin===%s",sBuffer.c_str());
         //´óÌü;
         std::string sPacketss=string("")+"JoinRoom,"+"R1,638";
         TSTCP::GetSingleTon()->SendMessageToServer(sPacketss);
    }
        
    void TSEventVerify(std::string sBuffer){
        CCLog("TSEventVerify===%s",sBuffer.c_str());
     
        //µÇÂ¼;
        std::vector<std::string> oPacket;
        TSEngine::TST_StringFilt(sBuffer, ',', oPacket);
        if(oPacket[1]=="NO"){
            std::string sPacket=std::string("")+"Regist,"+buf+","+buf;
            TSTCP::GetSingleTon()->SendMessageToServer(sPacket);
         
        }
      
    }
    void TsEventRegisterUser(std::string sBuffer){
        CCLog("TsEventRegisterUser===%s",sBuffer.c_str());
        std::vector<std::string> oPacket;
        TSEngine::TST_StringFilt(sBuffer,',',oPacket);
     
        std::string sPackets = std::string("") + "Login," +buf + "," + buf;
        TSTCP::GetSingleTon()->SendMessageToServer(sPackets);
        
    }
    void TsEventGuestLogin(std::string sBuffer){
         CCLog("TsEventGuestLogin===%s",sBuffer.c_str());
    }
    void TseventRoomOpen(std::string sBuffer){
         CCLog("TseventRoomOpen===%s",sBuffer.c_str());
    }
    void TSEventConnectGateWay(std::string sBuffer){
        CCLog("TSEventConnectGateWay===%s",sBuffer.c_str());
        std::vector<std::string> vBuffer;
        TSEngine::GetSingleTon()->TST_StringFilt(sBuffer, ',', vBuffer);

        if (vBuffer[0] == "ConnectGateWay")
        {
            string sIP = vBuffer[1];
            int iPort = atoi(vBuffer[2].c_str());

            TSHallNum::getSingleTon()->m_sWG_IP = sIP;
            TSHallNum::getSingleTon()->m_iWG_Port = iPort;

            TSTCP::GetSingleTon()->CreateClient(sIP, iPort);

            TSTCP::GetSingleTon()->SendMessageToServer("Connect");
        }

         //ÑéÖ¤;
        if(TSSocket::getSingleTon()->getConnectState()==0){

            std::string sPacket = std::string("Verify,") + buf;
            TSTCP::GetSingleTon()->SendMessageToServer(sPacket);

        }
    }
    void TSEventMusic(char* cBuffer, int iLen){
         CCLog("TSEventMusic");
    }
    
    void TSEventJoinRoom(std::string sBuffer){
        CCLog("TSEventJoinRoom===%s",sBuffer.c_str());
    }
protected:

private:

};




void main(){
    const time_t t = time(NULL);
    struct tm* current_time = localtime(&t);
    sprintf(buf,"%d%d%d",(int)current_time->tm_hour,(int)current_time->tm_min,(int)current_time->tm_sec);

    Msg msg;
    TSEvent::GetSingleTon()->RegistEvent("Login", (void*)&msg, (TpInstEventFun)&Msg::TSEventLogin);
    TSEvent::GetSingleTon()->RegistEvent("Verify", (void*)&msg, (TpInstEventFun)&Msg::TSEventVerify);
    TSEvent::GetSingleTon()->RegistEvent("Regist",(void*)&msg,(TpInstEventFun)&Msg::TsEventRegisterUser);
    TSEvent::GetSingleTon()->RegistEvent("GuestLogin",(void*)&msg,(TpInstEventFun)&Msg::TsEventGuestLogin);
    TSEvent::GetSingleTon()->RegistEvent("RoomOpen",(void*)&msg,(TpInstEventFun)&Msg::TseventRoomOpen);
    TSEvent::GetSingleTon()->RegistEventRoot( (void*)&msg, (TpInstEventFunRoot)&Msg::TSEventMusic);
    TSEvent::GetSingleTon()->RegistEvent("ConnectGateWay", (void*)&msg,(TpInstEventFun)&Msg::TSEventConnectGateWay);
    TSEvent::GetSingleTon()->RegistEvent("JoinRoom",(void*)&msg,(TpInstEventFun)&Msg::TSEventJoinRoom);
    ScriptingCore* sc = ScriptingCore::getInstance();
    sc->addRegisterCallback(register_all_cocos2dx);
    sc->addRegisterCallback(register_cocos2dx_js_extensions);
    sc->start();

    TSScriptJS::GetSingleTon()->Init();

    CCScriptEngineProtocol *pEngine = ScriptingCore::getInstance();
    CCScriptEngineManager::sharedManager()->setScriptEngine(pEngine);
    sc->runScript("CMAI/Config.js");
    TSScriptJS::GetSingleTon()->GetWebConfig();
    char* adapter_ip=(char*)TSHallNum::getSingleTon()->m_Adapter_Ip.c_str();
    int  adapter_port=atoi(TSHallNum::getSingleTon()->m_Adapter_Port.c_str());
    TSTCP::GetSingleTon()->CreateClient(adapter_ip, adapter_port);
    int m_iConnectState = TSSocket::getSingleTon()->getConnectState();
    if (m_iConnectState >= 0)
    {
        TSTCP::GetSingleTon()->SendMessageToServer("ConnectGateWay"); 
    }

    while(true){

        TSTCP::GetSingleTon()->ProcessMsg();
    }
}