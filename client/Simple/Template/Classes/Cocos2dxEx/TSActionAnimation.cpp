
#include "TSActionAnimation.h"
//
// Animate
//
TSAnimate* TSAnimate::create(CCAnimation *pAnimation)
{
    TSAnimate *pAnimate = new TSAnimate();
    pAnimate->initWithAnimation(pAnimation);
    pAnimate->autorelease();

    return pAnimate;
}

TSAnimate* TSAnimate::create(CCAnimation *pAnimation, int beginFrame, int endFrame)
{
    TSAnimate *pAnimate = new TSAnimate();
    pAnimate->autorelease();
    pAnimate->setAnimation(pAnimation);
    pAnimate->SetFrameLimit(beginFrame, endFrame);
    return pAnimate;
}

bool TSAnimate::initWithAnimation(CCAnimation *pAnimation)
{
    CCAssert( pAnimation!=NULL, "Animate: argument Animation must be non-NULL");

    float singleDuration = pAnimation->getDuration();

    if ( CCActionInterval::initWithDuration(singleDuration * pAnimation->getLoops() ) ) 
    {
        m_nNextFrame = 0;
        setAnimation(pAnimation);
        m_pOrigFrame = NULL;
        m_uExecutedLoops = 0;

        m_pSplitTimes->clear();

        float accumUnitsOfTime = 0;
        float newUnitOfTimeValue = singleDuration / pAnimation->getTotalDelayUnits();

        CCArray* pFrames = pAnimation->getFrames();
        CCARRAY_VERIFY_TYPE(pFrames, CCAnimationFrame*);

        CCObject* pObj = NULL;
        CCARRAY_FOREACH(pFrames, pObj)
        {
            CCAnimationFrame* frame = (CCAnimationFrame*)pObj;
            float value = (accumUnitsOfTime * newUnitOfTimeValue) / singleDuration;
            accumUnitsOfTime += frame->getDelayUnits();
            m_pSplitTimes->push_back(value);
        }    
        return true;
    }
    return false;
}

CCObject* TSAnimate::copyWithZone(CCZone *pZone)
{
    CCZone* pNewZone = NULL;
    TSAnimate* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (TSAnimate*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new TSAnimate();
        pZone = pNewZone = new CCZone(pCopy);
    }

    CCActionInterval::copyWithZone(pZone);

    pCopy->initWithAnimation((CCAnimation*)m_pAnimation->copy()->autorelease());

    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

TSAnimate::TSAnimate()
    : m_pAnimation(NULL)
    , m_pSplitTimes(new std::vector<float>)
    , m_nNextFrame(0)
    , m_pOrigFrame(NULL)
    , m_uExecutedLoops(0)
    , m_LimitBeginFrame(-1)
    , m_LimitEndFrame(-1)
{

}

TSAnimate::~TSAnimate()
{
    CC_SAFE_RELEASE(m_pAnimation);
    CC_SAFE_RELEASE(m_pOrigFrame);
    CC_SAFE_DELETE(m_pSplitTimes);
}

void TSAnimate::startWithTarget(CCNode *pTarget)
{
    CCActionInterval::startWithTarget(pTarget);
    CCSprite *pSprite = (CCSprite*)(pTarget);

    CC_SAFE_RELEASE(m_pOrigFrame);

    if (m_pAnimation->getRestoreOriginalFrame())
    {
        m_pOrigFrame = pSprite->displayFrame();
        m_pOrigFrame->retain();
    }
    m_nNextFrame = 0;
    if (m_LimitBeginFrame >= 0)
    {
        m_nNextFrame = m_LimitBeginFrame;
    }
    
    m_uExecutedLoops = 0;
}

void TSAnimate::stop(void)
{
    if (m_pAnimation->getRestoreOriginalFrame() && m_pTarget)
    {
        ((CCSprite*)(m_pTarget))->setDisplayFrame(m_pOrigFrame);
    }

    CCActionInterval::stop();
}

void TSAnimate::update(float t)
{
    // if t==1, ignore. Animation should finish with t==1
    if( t < 1.0f ) {
        t *= m_pAnimation->getLoops();

        // new loop?  If so, reset frame counter
        unsigned int loopNumber = (unsigned int)t;
        if( loopNumber > m_uExecutedLoops ) {
            m_nNextFrame = 0;
            m_uExecutedLoops++;
        }

        // new t for animations
        t = fmodf(t, 1.0f);
    }

    CCArray* frames = m_pAnimation->getFrames();
    unsigned int numberOfFrames = frames->count();
    CCSpriteFrame *frameToDisplay = NULL;

    if (m_LimitEndFrame >= 0)
    {
        numberOfFrames = m_LimitEndFrame;
    }

    for( unsigned int i=m_nNextFrame; i < numberOfFrames; i++ ) {
        float splitTime = 0;
        if (m_LimitBeginFrame >= 0)
        {
            splitTime = m_pSplitTimes->at(i-m_LimitBeginFrame);
        }
        else 
        {
            splitTime = m_pSplitTimes->at(i);
        }
        

        if( splitTime <= t ) {
            CCAnimationFrame* frame = (CCAnimationFrame*)frames->objectAtIndex(i);
            frameToDisplay = frame->getSpriteFrame();
            ((CCSprite*)m_pTarget)->setDisplayFrame(frameToDisplay);

            CCDictionary* dict = frame->getUserInfo();
            if( dict )
            {
                //TODO: [[NSNotificationCenter defaultCenter] postNotificationName:CCAnimationFrameDisplayedNotification object:target_ userInfo:dict];
            }
            m_nNextFrame = i+1;
        }
        // Issue 1438. Could be more than one frame per tick, due to low frame rate or frame delta < 1/FPS
        else {
            break;
        }
    }
}

CCActionInterval* TSAnimate::reverse(void)
{
    CCArray* pOldArray = m_pAnimation->getFrames();
    CCArray* pNewArray = CCArray::createWithCapacity(pOldArray->count());

    CCARRAY_VERIFY_TYPE(pOldArray, CCAnimationFrame*);

    if (pOldArray->count() > 0)
    {
        CCObject* pObj = NULL;
        CCARRAY_FOREACH_REVERSE(pOldArray, pObj)
        {
            CCAnimationFrame* pElement = (CCAnimationFrame*)pObj;
            if (! pElement)
            {
                break;
            }

            pNewArray->addObject((CCAnimationFrame*)(pElement->copy()->autorelease()));
        }
    }

    CCAnimation *newAnim = CCAnimation::create(pNewArray, m_pAnimation->getDelayPerUnit(), m_pAnimation->getLoops());
    newAnim->setRestoreOriginalFrame(m_pAnimation->getRestoreOriginalFrame());
    return create(newAnim);
}

bool TSAnimate::SetFrameLimit( int beginFrame, int endFrame )
{
    CCArray* frames = m_pAnimation->getFrames();
    unsigned int numberOfFrames = frames->count();
    if (beginFrame > endFrame)
    {
        return false;
    }

    if (beginFrame > numberOfFrames)
    {
        return false;
    }

    if (endFrame > numberOfFrames)
    {
        return false;
    }
    
    m_LimitBeginFrame = beginFrame;
    m_LimitEndFrame = endFrame;
    auto limit = m_LimitEndFrame - m_LimitBeginFrame;

    CCAssert( m_pAnimation!=NULL, "Animate: argument Animation must be non-NULL");

    float singleDuration = limit * m_pAnimation->getDelayPerUnit();
    //float singleDuration = m_pAnimation->getDuration();
    float loops = m_pAnimation->getLoops();
    if ( CCActionInterval::initWithDuration(singleDuration * loops ) ) 
    {
        m_nNextFrame = 0;
        setAnimation(m_pAnimation);
        m_pOrigFrame = NULL;
        m_uExecutedLoops = 0;

        m_pSplitTimes->clear();

        float accumUnitsOfTime = 0;
        float newUnitOfTimeValue = singleDuration / limit;

        CCArray* pFrames = m_pAnimation->getFrames();
        CCARRAY_VERIFY_TYPE(pFrames, CCAnimationFrame*);

        CCObject* pObj = NULL;
        for (auto i = m_LimitBeginFrame; i < m_LimitEndFrame; i++)
        {
            CCAnimationFrame* frame = (CCAnimationFrame*)pFrames->data->arr[i];
            float value = (accumUnitsOfTime * newUnitOfTimeValue) / singleDuration;
            accumUnitsOfTime += frame->getDelayUnits();
            m_pSplitTimes->push_back(value);
        }    
    }
    return true;
}

bool TSAnimate::ClearFrameLimit()
{
    m_LimitBeginFrame = -1;
    m_LimitEndFrame = -1;
    initWithAnimation(m_pAnimation);
    return true;
}
