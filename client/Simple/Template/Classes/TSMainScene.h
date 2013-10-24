#ifndef __TSMainLayout_H__
#define __TSMainLayout_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "TSEngine/TSObject.h"

using namespace cocos2d;
using namespace std;
USING_NS_CC_EXT;

class TSMainLayout : public TSObject, public cocos2d::CCLayer, public cocos2d::extension::CCEditBoxDelegate
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
    
    // a selector callback
    void menuCallback(CCObject* pSender);

    // 场景;
	void initView(CCSprite* sp);

	// 添加对话框;
	void add_diaog(CCSprite* sp);

    void draw();

	virtual void editBoxEditingDidBegin(cocos2d::extension::CCEditBox* editBox);
	virtual void editBoxEditingDidEnd(cocos2d::extension::CCEditBox* editBox);
	virtual void editBoxTextChanged(cocos2d::extension::CCEditBox* editBox, const std::string& text);
	virtual void editBoxReturn(cocos2d::extension::CCEditBox* editBox);
	//解析字符串;
	int strJqH(string str,int len);

    // implement the "static node()" method manually
    CREATE_FUNC(TSMainLayout);

    void TSEventLogin(std::string sBuffer);
    void TSEventVerify(std::string sBuffer);
    void TsEventRegisterUser(std::string sBuffer);
    void TsEventGuestLogin(std::string sBuffer);
    void TseventRoomOpen(std::string sBuffer);
    void TSEventConnectGateWay(std::string sBuffer);

    void TSEventMusic(char* cBuffer, int iLen);
    void FillUserInfo(std::vector<std::string>& oPacket);


    virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
public:
    CCSprite* m_rb;
};

#endif  // __HELLOWORLD_SCENE_H__