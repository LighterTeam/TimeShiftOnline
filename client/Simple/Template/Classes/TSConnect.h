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

    static TSConnect* getSingleTon(){
        static TSConnect tsh;
        return &tsh;
    }

    bool ReConnect();
    void initSocket();

public:
    //�������IP�Ͷ˿ں�
    std::string m_Adapter_Ip;
    int m_Adapter_Port;

    //������Ҫ������
    std::string m_sWG_IP;   //IP
    int m_iWG_Port;         //�˿ں�

    //UUID
    std::string m_sUUID;    //���ط�����ͻ��˵�ΨһID
protected:

private:
};

#endif
