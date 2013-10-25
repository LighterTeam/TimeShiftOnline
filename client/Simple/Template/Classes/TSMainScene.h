#ifndef __TSMainLayout_H__
#define __TSMainLayout_H__

#include "cocos2d.h"
#include "TSEngine/TSObject.h"
#include "json/json.h"

using namespace cocos2d;
using namespace std;

class TSMainLayout : public TSObject, public cocos2d::CCLayer
{
public:
    TSMainLayout();
    ~TSMainLayout();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    virtual void onEnter();
    virtual void onExit();

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    void draw();

    // implement the "static node()" method manually
    CREATE_FUNC(TSMainLayout);

    // Message
    void TSEventConnectGateWay(Json::Value jValue);
    void TSEventDisconnect(std::string sBuffer);
public:
    CCSprite* m_rb;
};

#endif  // __HELLOWORLD_SCENE_H__