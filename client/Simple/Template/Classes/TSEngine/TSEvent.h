#ifndef __TSEVENT_H__
#define __TSEVENT_H__

#include <string>
#include <map>
#include <list>
#include "TSObject.h"

using namespace std;

// ȫ�ֻص����Ͷ���
typedef void (TSObject::*TpInstEventFun)(string sBuffer); 
typedef void (TSObject::*TpInstEventFunRoot)(char* cBuffer, int iLen); 

// TS�¼�ϵͳ
class TSEvent : public TSObject {
public:
    TSEvent () : m_RootEvent(0), m_pFunRootEvent(0){
    }

    static TSEvent* GetSingleTon() {
        static TSEvent tsE;
        return &tsE;
    };

    // ע�ắ��
    void RegistEvent(string sEventKey, void* pInst, TpInstEventFun pFun);
    void RegistEventRoot(void* pInst, TpInstEventFunRoot pFun);

    // ��ע��
    void UnRegistEvent(string sEventKey, void* pInst); 
    void UnRegistEvent(string sEventKey);
    void UnRegistEventRoot();

    // ������Ϣ
    void SendMsg(string sEventKey, string sBuffer );
    void SendRoot(char* pBuffer, int iLen);

    // Push�������Ϣ��
    void PushMessge(string sEventKey, string sBuffer);
    void PushRoot(char* pBuffer, int iLen);

public:
    map<string, map<void*, TpInstEventFun> > m_MapEvent;

    std::list<std::pair<string, string> > m_MsgList;
    std::list<std::pair<char*, int> > m_MsgListRoot;

    void* m_RootEvent;
    TpInstEventFunRoot m_pFunRootEvent;
};

#endif
