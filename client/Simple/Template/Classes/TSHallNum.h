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
    //�����;
    std::vector<std::string> m_vRoomid;

    //��������������Ϣ;
    std::vector<CUserInfoStruct> m_vDeskInfo; //���ճ�����

    //������Ҫ������
    std::string m_sWG_IP;   //IP
    int m_iWG_Port;         //�˿ں�

    //��һ�����Ϣ
    std::string m_sName;    //��½�������
    std::string m_sRMB;     //�����
    std::string m_sVRMB;    //��Ϸ��
    std::string m_sICON;    //ͷ��ID
    std::string m_sUserID;  //DB����ID.Ψһ��ʶGUID
    std::string m_sUUID;    //��Ϸȫ��ΨһID.�ط����·���
    std::string m_sVName;   //����ǳ�
	std::string m_Adapter_Ip;
	std::string m_Adapter_Port;

    //����flowID
    int m_flowID;

    //����ģʽ
    int m_isRobot;
protected:

private:
};

#endif
