#ifndef __TSAnimate_H__
#define __TSAnimate_H__

#include "cocos2d.h"

using namespace cocos2d;

class TSAnimate : public CCActionInterval
{
public:
    TSAnimate();
    ~TSAnimate();

    /** initializes the action with an Animation and will restore the original frame when the animation is over */
    bool initWithAnimation(CCAnimation *pAnimation);

    // frame limit
    bool SetFrameLimit(int beginFrame, int endFrame);
    bool ClearFrameLimit();

    virtual CCObject* copyWithZone(CCZone* pZone);
    virtual void startWithTarget(CCNode *pTarget);
    virtual void stop(void);
    virtual void update(float t);
    virtual CCActionInterval* reverse(void);

public:
    /** creates the action with an Animation and will restore the original frame when the animation is over */
    static TSAnimate* create(CCAnimation *pAnimation);
    static TSAnimate* create(CCAnimation *pAnimation, int beginFrame, int endFrame);
    CC_SYNTHESIZE_RETAIN(CCAnimation*, m_pAnimation, Animation)
protected:
    std::vector<float>* m_pSplitTimes;
    int                m_nNextFrame;
    CCSpriteFrame*  m_pOrigFrame;
    unsigned int    m_uExecutedLoops;
    int             m_LimitBeginFrame;
    int             m_LimitEndFrame;
};

#endif
