﻿#include "TSEngine/TSTCP.h"
#include "TSEngine/TSEvent.h"
#include "TSMainScene.h"
#include "TSConnect.h"

using namespace cocos2d;
using namespace std;

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

void TSMainLayout::draw()
{
    TSTCP::GetSingleTon()->ProcessMsg();
}

TSMainLayout::TSMainLayout()
{
    TSEvent* pE = TSEvent::GetSingleTon();
    pE->JSON_RegistEvent("ConnectGateWay", (void*)this, (TpInstEventJsonFun)&TSMainLayout::TSEventConnectGateWay);
    pE->RegistEvent("Disconnect", (void*)this, (TpInstEventFun)&TSMainLayout::TSEventDisconnect);
}

TSMainLayout::~TSMainLayout()
{
    TSEvent* pE = TSEvent::GetSingleTon();
    pE->JSON_UnRegistEvent("ConnectGateWay", (void*)this);
    pE->UnRegistEvent("Disconnect", (void*)this);
}

// 连接适配服分配的网关;
void TSMainLayout::TSEventConnectGateWay( Json::Value jValue)
{
    //{
    //    "IP" : "127.0.0.1",
    //    "MM" : "ConnectGateWay",
    //    "Port" : 30000
    //}

    TSTCP* pT = TSTCP::GetSingleTon();
    CCLog("TSEventConnectGateWay: %s", jValue.toStyledString().c_str());
    TSTCP::GetSingleTon()->CloseSocket(); // 断开网关
    
    // 连接网关服
    if (pT->CreateClient(jValue["IP"].asString(),jValue["Port"].asInt()) == 0) {
        CCLog("GateWayConnect Failed!");
    } else {
        CCLog("GateWayConnect Success!");
    }
}

void TSMainLayout::TSEventDisconnect( std::string sBuffer )
{
    CCLog("TSEventDisconnect %s", sBuffer.c_str());
    TSConnect::getSingleTon()->ReConnect();
}



