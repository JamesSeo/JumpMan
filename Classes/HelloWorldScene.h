#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#define TAG_RAFT 100
#define TAG_SHARK 2001
#define TAG_TRITION 3001

// 20% of window height 
#define SKY_HT 0.2

USING_NS_CC;

class HelloWorld : public Layer
{
    
    Size winSize;
    int m_gameScore;
    
    Vector<Sprite*> m_sharks;
    Vector<Sprite*> m_tritons;
    
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    virtual bool touchBegan(Touch *touch, Event *unused_event);
    void initRaft();
    void makeTriton(int direction, const Vec2 &pos);
    void makeShark(float delta);// share will be appear left or right side
    
    bool onContactBegin(PhysicsContact &contact);
    
    void update(float delta);
};

#endif // __HELLOWORLD_SCENE_H__
