//
//  BGLayer.h
//  JumpMan
//
//  Created by JunHo Seo on 2014. 7. 20..
//
//

#ifndef __JumpMan__BGLayer__
#define __JumpMan__BGLayer__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

class BGLayer : public Layer {
    
    
    
public:
    
    void makeBubble(float delta);
    virtual bool init();
    // implement the "static create()" method manually
    CREATE_FUNC(BGLayer);
};

#endif /* defined(__JumpMan__BGLayer__) */
