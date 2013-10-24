#ifndef  __TSHallNum_H__
#define  __TSHallNum_H__

#include "cocos2d.h"
#include <string>
using namespace std;
using namespace cocos2d;

struct CUserInfoStruct {
    CUserInfoStruct() : iICON(0), iFlowID(0){
    }
    std::string sName;
    std::string sVName;
    int iICON;
    int iFlowID;
};

class TSHallNum
{
public:
    TSHallNum();
    ~TSHallNum();

    static TSHallNum* getSingleTon(){
        static TSHallNum tsh;
        return &tsh;
    }

    bool ReConnect();
    void ReConnectSuccess_CallBack(string sBuffer);
    void OpenReConnectWnd();
    void OpenReConnectWnd_CallBack();
    void initSocket();

public:
    //房间号;
    std::vector<std::string> m_vRoomid;

    //桌子里面人物信息;
    std::vector<CUserInfoStruct> m_vDeskInfo; //先收场景包

    //重连需要的数据
    std::string m_sWG_IP;   //IP
    int m_iWG_Port;         //端口号

    //玩家基本信息
    std::string m_sName;    //登陆后的名字
    std::string m_sRMB;     //人民币
    std::string m_sVRMB;    //游戏币
    std::string m_sICON;    //头像ID
    std::string m_sUserID;  //DB分配ID.唯一标识GUID
    std::string m_sUUID;    //游戏全局唯一ID.关服重新分配
    std::string m_sVName;   //玩家昵称
	std::string m_Adapter_Ip;
	std::string m_Adapter_Port;

    //房间flowID
    int m_flowID;

    //单机模式
    int m_isRobot;
protected:

private:
};

#endif
