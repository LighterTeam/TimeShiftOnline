#ifndef  __TSHallNum_H__
#define  __TSHallNum_H__

#include "cocos2d.h"
#include <string>
using namespace std;
using namespace cocos2d;

class TSConnect
{
public:
    TSConnect();
    ~TSConnect();

    static TSConnect* getSingleTon(){
        static TSConnect tsh;
        return &tsh;
    }

    bool ReConnect();
    void ReConnectSuccess_CallBack(string sBuffer);
    void OpenReConnectWnd();
    void OpenReConnectWnd_CallBack();
    void initSocket();

public:
    //适配服的IP和端口号
    std::string m_Adapter_Ip;
    int m_Adapter_Port;

    //重连需要的数据
    std::string m_sWG_IP;   //IP
    int m_iWG_Port;         //端口号

    //UUID
    std::string m_sUUID;    //网关分配给客户端的唯一ID
protected:

private:
};

#endif
