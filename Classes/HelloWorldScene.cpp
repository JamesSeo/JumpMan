#include "HelloWorldScene.h"
#include "BGLayer.h"
USING_NS_CC;

Scene* HelloWorld::createScene()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    Vect gravity = Vect(0, -100.f);
    // 'scene' is an autorelease object
    //auto scene = Scene::create();
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(gravity);
    
    auto body = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT,3);
    auto edge_node = Node::create();
    edge_node->setPosition(visibleSize.width/2, visibleSize.height/2);
    edge_node->setPhysicsBody(body);
    scene->addChild(edge_node);
    
    
    
    
    auto bg_layer = BGLayer::create();
    scene->addChild(bg_layer);
    
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    
    
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    winSize = visibleSize;

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    
    auto listner = EventListenerTouchOneByOne::create();
    listner->onTouchBegan = CC_CALLBACK_2(HelloWorld::touchBegan, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listner, 1);
    
    
    
    auto contactListener = EventListenerPhysicsContact::create();

    contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
    
    
    
    this->scheduleUpdate();
    
    
    initRaft();
    
    this->schedule(schedule_selector(HelloWorld::makeShark), 5);
    
    
    return true;
}




void HelloWorld::initRaft(){
    auto raft = Sprite::create("raft.png");
    raft->setTag(TAG_RAFT);
    raft->setScale(0.8);
    raft->setPosition( winSize.width/2, winSize.height*(1.0-SKY_HT));
    this->addChild(raft);
    
    auto act = Sequence::create(
                                MoveBy::create(3, Vec2(400,0)),
                                MoveBy::create(3, Vec2(-400,0)),
                                MoveBy::create(3, Vec2(-400,0)),
                                MoveBy::create(3, Vec2(400,0)),
                                               NULL);
    auto rep_act = RepeatForever::create(act);
    raft->runAction(rep_act);
}

void HelloWorld::makeTriton(int direction, const Vec2 &pos){
    
    auto spr = Sprite::create("triton.png");
    auto scale = 0.2f;
    spr->setScale(scale);
    spr->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    auto material = PhysicsMaterial(0.01f,0.2f,1.0f);//density, elastic, friction
    spr->setPosition(pos);
    spr->setTag(TAG_TRITION);
    
    Size sz = spr->getContentSize();
    sz = sz * scale;
    auto body = PhysicsBody::createBox(sz, material);
    body->setCategoryBitmask(0x00000001);
    body->setCollisionBitmask(0x00000002);
    
    body->setContactTestBitmask(0x00000002);
    
    Vec2 pts[4]={{0,0}, {50,10}, {75,0}, {50,-10}};
    auto shape = PhysicsShapePolygon::create(pts, 4);
    body->addShape(shape);
    

    body->applyForce(Vec2(1000.f,0.0f), Vec2(-150.f,0));
    spr->setPhysicsBody(body);
    //spr->setRotation(20.f);
    this->addChild(spr);
    this->m_tritons.pushBack(spr);
}

void HelloWorld::makeShark(float delta){
   
    if (m_sharks.size()>5){
        return;
    }
    int isLeftSide = true;
    int startY = 0;
    
    srand(time(NULL));
    startY = rand() % 200 + 50;
    isLeftSide = ((startY %2 )==0);
    
    Vec2 startPos(100, startY);
    auto spr = Sprite::create();
    auto scale = 0.3f;
    auto material = PhysicsMaterial(0.0f,0.2f,1.0f);//density, elastic, friction
    Vec2 applyForce(50.f, 100.f);
    
    if (isLeftSide){
        spr = Sprite::create("shark_left.png");
    }
    else {
        startPos.x = winSize.width - 100;
        spr = Sprite::create("shark_right.png");
        applyForce.x = -50.f;
        
    }
    
    spr->setTag(TAG_SHARK);
    spr->setScale(scale);
    spr->setPosition(startPos);
    
    Size sz = spr->getContentSize();
    sz = sz * scale;
    
    auto body = PhysicsBody::createBox(sz, material);
    body->setCategoryBitmask(0x00000002);
    body->setCollisionBitmask(0x00000001);
    
    body->setContactTestBitmask(0x00000001);

    
    spr->setPhysicsBody(body);
    body->applyForce(applyForce, Vec2(0,0));
    
    this->addChild(spr);
    
    this->m_sharks.pushBack(spr);
    
   
}


void HelloWorld::update(float delta){

    return ;
    // This app use physics collision listner instead of for-each.
  /*
    auto target = Sprite::create();
    for(auto shark : m_sharks){
        auto rect = shark->getBoundingBox();
        
        if (rect.origin.x <10) {
            target = shark;
            CCLOG("shark:%f %f", rect.origin.x, rect.origin.y);
        }
    }
    
    if(m_sharks.contains(target)){
        this->removeChild(target);
        m_sharks.eraseObject(target);
    }
    
    // bottom check.
    auto tgt_trion = Sprite::create();
    for (auto triton : m_tritons){
        auto pos = triton->getPosition();
        if (pos.y<10) {
            tgt_trion = triton;
        }
    }
    
    if (m_tritons.contains(tgt_trion)){
        this->removeChild(tgt_trion);
        m_tritons.eraseObject(tgt_trion);
    }
    
    target = Sprite::create();
    tgt_trion = Sprite::create();
    
    // intersect check
    bool bFind = false;
     for(auto shark : m_sharks){
         for (auto triton : m_tritons){
             if (triton->getBoundingBox().intersectsRect(shark->getBoundingBox())){
                 target = shark;
                 tgt_trion = triton;
                 bFind = true;
                 break;
             }
         }
         if (bFind == true)
             break;
     }
    
    if(m_sharks.contains(target)){
        this->removeChild(target);
        m_sharks.eraseObject(target);
    }
    if (m_tritons.contains(tgt_trion)){
        this->removeChild(tgt_trion);
        m_tritons.eraseObject(tgt_trion);
    }
    */
    
}

bool HelloWorld::touchBegan(Touch *touch, Event *unused_event){
    
    //Vec2 pos = touch->getLocation();
    auto raft = this->getChildByTag(TAG_RAFT);
    Vec2 pos = raft->getPosition();
    this->makeTriton(1, pos);
    return true;
}


bool HelloWorld::onContactBegin(PhysicsContact &contacts){
    
    auto spA = (Sprite*)contacts.getShapeA()->getBody()->getNode();
    auto spB = (Sprite*)contacts.getShapeB()->getBody()->getNode();
    
    if (spA== NULL || spB==NULL)
        return true;
    
    int tagA = spA->getTag();
    int tagB = spB->getTag();
    CCLOG("Collision tag: A=>%d, B=>%d", tagA, tagB);
    
    if ((tagA==TAG_SHARK && tagB==TAG_TRITION)) {
        this->removeChild(spA);
        this->removeChild(spB);
        m_sharks.eraseObject(spA);
        m_tritons.eraseObject(spB);
    }
    else if ((tagB==TAG_SHARK && tagA==TAG_TRITION)){
        this->removeChild(spA);
        this->removeChild(spB);
        m_sharks.eraseObject(spB);
        m_tritons.eraseObject(spA);
    }
    
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
