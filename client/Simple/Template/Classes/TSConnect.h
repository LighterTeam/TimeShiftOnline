#ifndef  __TSConnect_H__
#define  __TSConnect_H__

#include "cocos2d.h"
#include <string>
using namespace std;
using namespace cocos2d;

class TSConnect
{
public:
    TSConnect();
    ~TSConnect();

    static TSConnect* GetSingleTon(){
        static TSConnect tsh;
        return &tsh;
    }

    bool ReConnect();
    void initSocket();

public:
    //适配服的IP和端口号
    std::string m_Adapter_Ip;
    int m_Adapter_Port;

    //重连需要的数据
    std::string m_sWG_IP;   //IP
    int m_iWG_Port;         //端口号

    //UUID
    int m_iUUID;    //网关分配给客户端的唯一ID
protected:

private:
};

#endif
