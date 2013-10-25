#include "TSMainScene.h"
#include "TSConnect.h"
#include "cocos-ext.h"
//#include "Register.h"

#ifdef WIN32
#include <WinSock.h>
#pragma comment(lib, "WS2_32.lib")
#else
#include "JNICalls/InterfaceJNI.h"
#endif

#include "TSEngine/TSCommon.h"
#include "TSEngine/TSTCP.h"

#include <stdio.h>
#include <stdlib.h>
#include "../CCFileUtils.h"
#include "ScriptingCore.h"
#include "jsapi.h"
#include "TSEngine/TSScriptJS.h"

using namespace cocos2d;
using namespace std;
USING_NS_CC_EXT;

CCScene* TSMainLayout::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        TSMainLayout *layer = TSMainLayout::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

bool TSMainLayout::init()
{
    bool bRet = false;
    do 
    {
        CC_BREAK_IF(! CCLayer::init());
        CCSize size = CCDirector::sharedDirector()->getWinSize();

        CCSprite* pSprite = CCSprite::create("HelloWorld.png");
        CC_BREAK_IF(! pSprite);
        pSprite->setPosition(ccp(size.width/2, size.height/2));
        this->addChild(pSprite, 0);

        setTouchEnabled(true);

        TSConnect::getSingleTon();
        bRet = true;
    } while (0);

    return bRet;
}

void TSMainLayout::onEnter(){
    TSConnect::getSingleTon()->initSocket(); // 初始化服务器连接

    CCLayer::onEnter();
}

void TSMainLayout::onExit(){
    CCLayer::onExit();
}

void TSMainLayout::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent) {
    CCSetIterator iter = pTouches->begin();
    for (; iter != pTouches->end(); iter++)
    {
        //这里你要手动处理每一个点！ 
        CCTouch* pTouch = (CCTouch*)(*iter);
        CCPoint location = pTouch->getLocation();
        pTouch = NULL;
    }
}

void TSMainLayout::initView(CCSprite* sp) 
{   

}

void TSMainLayout::menuCallback(CCObject* pSender)
{

}

void TSMainLayout::add_diaog ( CCSprite* sp ) 
{

}

void TSMainLayout::editBoxEditingDidBegin(cocos2d::extension::CCEditBox* editBox)
{

}

void TSMainLayout::editBoxEditingDidEnd(cocos2d::extension::CCEditBox* editBox)
{

}

void TSMainLayout::editBoxReturn(cocos2d::extension::CCEditBox* editBox)
{

}

void TSMainLayout::editBoxTextChanged(cocos2d::extension::CCEditBox* editBox, const std::string& text)
{
  
}

int TSMainLayout::strJqH(string str,int len)
{

    return 0;
}

void TSMainLayout::draw()
{
    TSTCP::GetSingleTon()->ProcessMsg();
}

void TSMainLayout::FillUserInfo(std::vector<std::string>& oPacket)
{

}

void TSMainLayout::TSEventLogin( std::string sBuffer )
{   
}

TSMainLayout::TSMainLayout()
{
    TSEvent::GetSingleTon()->RegistEvent("Login", (void*)this, (TpInstEventFun)&TSMainLayout::TSEventLogin);
    TSEvent::GetSingleTon()->RegistEvent("Verify", (void*)this, (TpInstEventFun)&TSMainLayout::TSEventVerify);
    TSEvent::GetSingleTon()->RegistEvent("Regist",(void*)this,(TpInstEventFun)&TSMainLayout::TsEventRegisterUser);
    TSEvent::GetSingleTon()->RegistEvent("GuestLogin",(void*)this,(TpInstEventFun)&TSMainLayout::TsEventGuestLogin);
    TSEvent::GetSingleTon()->RegistEvent("RoomOpen",(void*)this,(TpInstEventFun)&TSMainLayout::TseventRoomOpen);

    TSEvent::GetSingleTon()->RegistEventRoot( (void*)this, (TpInstEventFunRoot)&TSMainLayout::TSEventMusic);
    TSEvent::GetSingleTon()->RegistEvent("ConnectGateWay", (void*)this,(TpInstEventFun)&TSMainLayout::TSEventConnectGateWay);
}

TSMainLayout::~TSMainLayout()
{
    TSEvent::GetSingleTon()->UnRegistEvent("Login", (void*)this);
    TSEvent::GetSingleTon()->UnRegistEvent("Verify", (void*)this);
    TSEvent::GetSingleTon()->UnRegistEvent("Regist",(void*)this);
    TSEvent::GetSingleTon()->UnRegistEvent("GuestLogin",(void*)this);
    TSEvent::GetSingleTon()->UnRegistEvent("RoomOpen",(void*)this);

    TSEvent::GetSingleTon()->UnRegistEventRoot();
    TSEvent::GetSingleTon()->UnRegistEvent("ConnectGateWay", (void*)this);
}

void TSMainLayout::TSEventVerify(std::string sBuffer) 
{
}

void TSMainLayout::TSEventMusic( char* cBuffer, int iLen )
{
}

void TSMainLayout::TsEventRegisterUser(std::string sBuffer)
{
}

void TSMainLayout::TsEventGuestLogin(std::string sBuffer)
{
}

// 连接适配服分配的网关;
void TSMainLayout::TSEventConnectGateWay( std::string sBuffer )
{
    CCLog(sBuffer.c_str());
    TSTCP::GetSingleTon()->CloseSocket();
}

void TSMainLayout::TseventRoomOpen(std::string sBuffer)
{

}
