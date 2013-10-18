//
//  GameOverLayer.cpp
//  飞镖忍者
//
//  Created by 杜 甲 on 13-8-17.
//
//

#include "GameOverLayer.h"
#include"HelloWorldScene.h"
using namespace cocos2d;

GameOverLayer::GameOverLayer(void)
{
}

GameOverLayer::~GameOverLayer(void)
{
}

GameOverLayer* GameOverLayer::createWithWon(bool won)
{
    GameOverLayer *pRet = new GameOverLayer();
    if (pRet && pRet->initWithWon(won))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}

bool GameOverLayer::initWithWon(bool won)
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(! CCLayerColor::initWithColor(ccc4(255,255,255, 255)));
        
        char *message;
        //根据传过来的条件  won值判断
        if (won)
        {
            message = "You Won!";
        }
        else
        {
            message = "You Lose :[";
        }
        
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        CCLabelTTF *label = CCLabelTTF::create(message, "Arial", 32);
        label->setColor(ccc3(0, 0, 0));
        label->setPosition(ccp(winSize.width / 2, winSize.height /2));
        this->addChild(label);
        //停留3秒钟，执行gameOverDone函数
        this->runAction(CCSequence::create(CCDelayTime::create(3),
                                           CCCallFunc::create(this, callfunc_selector(GameOverLayer::gameOverDone)),
                                           NULL));
        
        bRet = true;
    } while (0);
    
    return bRet;
}

cocos2d::CCScene* GameOverLayer::sceneWithWon(bool won)
{
    CCScene * scene = NULL;
    do
    {
        scene = CCScene::create();
        CC_BREAK_IF(! scene);
        
        GameOverLayer *layer = GameOverLayer::createWithWon(won);
        CC_BREAK_IF(! layer);
        
        scene->addChild(layer);
    } while (0);
    
    return scene;
}

void GameOverLayer::gameOverDone()
{
    //重新加载场景
    CCDirector::sharedDirector()->replaceScene(HelloWorld::scene());
}