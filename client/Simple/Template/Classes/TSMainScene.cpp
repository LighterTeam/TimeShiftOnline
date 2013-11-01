#include "TSEngine/TSTCP.h"
#include "TSEngine/TSEvent.h"
#include "TSEngine/TSLog.h"
#include "Cocos2dxEx/TSActionAnimation.h"

#include "TSMainScene.h"
#include "TSConnect.h"

using namespace cocos2d;
using namespace std;

CCScene* TSMainScene::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        TSMainScene *layer = TSMainScene::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

bool TSMainScene::init()
{
    bool bRet = false;
    do 
    {
        CC_BREAK_IF(! CCLayer::init());
        CCSize size = CCDirector::sharedDirector()->getWinSize();

        CCMenuItemImage * pCloseItem = CCMenuItemImage::create(
            "CloseNormal.png", 
            "CloseSelected.png", 
            this, 
            menu_selector(TSMainScene::menuCloseCallback));
        CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
        pMenu->setPosition(size.width-20,20);
        this->addChild(pMenu, 1);

        m_cn = CCNode::create();
        m_cn->setPosition(size.width/2, size.height/2);

        m_pSprite = CCSprite::create("HelloWorld.png");
        CC_BREAK_IF(! m_pSprite);
        m_pSprite->setAnchorPoint(ccp(0.5f,0.0f));
        m_cn->addChild(m_pSprite, 0);
        
        {
            // 精灵帧缓存
            CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
            // 添加 plist文件到缓存
            cache->addSpriteFramesWithFile("explosion.plist");
            // 创建数组用来存放帧序列
            CCArray *animFrames = CCArray::create();

            char str[64] = {0};
            for (int i = 1; i <= 35; ++i) {
                sprintf(str, "explosion_%02d.png", i);
                CCSpriteFrame *frame = cache->spriteFrameByName(str);
                CCLOG(str);
                // 添加帧到数组
                animFrames->addObject(frame);
            }

            // 用帧序列生成帧动画信息，设定帧间隔时间
            CCAnimation *animation = CCAnimation::createWithSpriteFrames(animFrames, 0.05f);
            animation->setRestoreOriginalFrame(true);

            // 动画缓存，取名字
            CCAnimationCache::sharedAnimationCache()->addAnimation(animation, "Explosion");
            // 获取第一帧
            CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("explosion_01.png");

            // 用名字在缓存取帧动画信息
            CCAnimation *anim = CCAnimationCache::sharedAnimationCache()->animationByName("Explosion");
            // 有动画信息生成动画
            TSAnimate *animate = TSAnimate::create(anim, 30, 35);

            // 用第一帧作静态图
            CCSprite *first = CCSprite::createWithSpriteFrame(frame);
            first->setPosition(ccp(100, 100));

            // 执行动画
            first->runAction(CCRepeatForever::create(animate));
            m_cn->addChild(first);
        }

        CCSprite* pSprite = CCSprite::create("CloseNormal.png");
        CC_BREAK_IF(! pSprite);
        pSprite->setPosition(ccp(0, -320));
        m_cn->addChild(pSprite, 2);

        CCAction* ac = CCMoveTo::create(5.0f, ccp(0,0));
        pSprite->runAction(ac);

        this->addChild(m_cn);

        //CCTextFieldTTF* text = CCTextFieldTTF::textFieldWithPlaceHolder(  
        //"Input Your Name...", "Arial", 20);
        //text->setPosition(ccp(size.width / 2, size.height / 2 + 40));  
        //this->addChild(text);
        //text->attachWithIME(); 

        setTouchEnabled(true);

        TSConnect::GetSingleTon();
        bRet = true;
    } while (0);

    return bRet;
}

void TSMainScene::menuCloseCallback(CCObject*) {
    exit(0);
}

void TSMainScene::onEnter(){
    TSConnect::GetSingleTon()->initSocket(); // 初始化服务器连接
    CCLayer::onEnter();
}

void TSMainScene::onExit(){
    CCLayer::onExit();
}

void TSMainScene::draw()
{
    TSTCP::GetSingleTon()->ProcessMsg();

    // Game [10/29/2013 Administrator]
    float cp = m_cn->getRotation();
    if(cp > 360.0f) {
        cp = 0.0f;
    }
    m_cn->setRotation(cp + 1.0f);

    m_pSprite->setRotation(cp + 1.0f);
}

TSMainScene::TSMainScene()
{
    TSEvent* pE = TSEvent::GetSingleTon();
    pE->JSON_RegistEvent("ConnectGateWay", (void*)this, (TpInstEventJsonFun)&TSMainScene::TSEventConnectGateWay);
    pE->JSON_RegistEvent("RegistUUID", (void*)this, (TpInstEventJsonFun)&TSMainScene::TSEventRegistUUID);
    pE->RegistEvent("Disconnect", (void*)this, (TpInstEventFun)&TSMainScene::TSEventDisconnect);
}

TSMainScene::~TSMainScene()
{
    TSEvent* pE = TSEvent::GetSingleTon();
    pE->JSON_UnRegistEvent("ConnectGateWay");
    pE->JSON_UnRegistEvent("RegistUUID");
    pE->UnRegistEvent("Disconnect");
}

// 连接适配服分配的网关;
void TSMainScene::TSEventConnectGateWay( Json::Value jValue)
{
    //{
    //    "IP" : "127.0.0.1",
    //    "MM" : "ConnectGateWay",
    //    "Port" : 30000
    //}
    TSEvent* pE = TSEvent::GetSingleTon();
    TSTCP* pT = TSTCP::GetSingleTon();
    TSConnect* pC = TSConnect::GetSingleTon();
    TSLog("TSEventConnectGateWay: %s", jValue.toStyledString().c_str());
    TSTCP::GetSingleTon()->CloseSocket(); // 断开网关
    
    std::string& sIP = jValue["IP"].asString();
    int iPort = jValue["Port"].asInt();

    // 连接网关服
    if (pT->CreateClient(sIP, iPort) == 0) {
        TSLog("GateWayConnect Failed!");
    } else {
        TSLog("GateWayConnect Success!");
        TSConnect::GetSingleTon()->m_sWG_IP = sIP;
        TSConnect::GetSingleTon()->m_iWG_Port = iPort;
    }
}

void TSMainScene::TSEventDisconnect( std::string sBuffer )
{
    TSConnect* pC = TSConnect::GetSingleTon();
    TSLog("TSEventDisconnect %s", sBuffer.c_str());
    if (pC->m_iUUID)
    {
        pC->ReConnect();
    }
}

void TSMainScene::TSEventRegistUUID( Json::Value jValue )
{
    TSConnect* pC = TSConnect::GetSingleTon();
    pC->m_iUUID = jValue["UUID"].asInt();
}




