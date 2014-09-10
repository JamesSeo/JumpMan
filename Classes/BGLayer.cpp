//
//  BGLayer.cpp
//  JumpMan
//
//  Created by JunHo Seo on 2014. 7. 20..
//
//

#include "BGLayer.h"

bool BGLayer::init(){
    
    auto bg_sea = Sprite::create("bg_sea.jpg");
    Size bg_size = bg_sea->getContentSize();
    
    bg_sea->setAnchorPoint(Vec2::ZERO);
    
    Size win_sz = Director::getInstance()->getWinSize();
    bg_sea->setScale(win_sz.width/bg_size.width, win_sz.height/bg_size.height);
    this->addChild(bg_sea);
    
   
    this->schedule(schedule_selector(BGLayer::makeBubble),2);
    
    return true;
}

void BGLayer::makeBubble(float delta)
{
    auto bubble = Sprite::create("bubble.png");
    srand(time(NULL));
    int x = rand() % 500 + 100;
    
    bubble->setPosition(Vec2(x,0));
    bubble->setOpacity(128);
    Size win_sz = Director::getInstance()->getWinSize();

    auto seq_act = Sequence::create(
                                    MoveBy::create(2, Vec2(0, win_sz.height * 0.8)),
                                    RemoveSelf::create(),
                                    NULL);
    this->addChild(bubble);
    bubble->runAction(seq_act);
   
}