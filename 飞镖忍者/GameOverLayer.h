//
//  GameOverLayer.h
//  飞镖忍者
//
//  Created by 杜 甲 on 13-8-17.
//
//

#ifndef ________GameOverLayer__
#define ________GameOverLayer__

#include"cocos2d.h"

class GameOverLayer :
public cocos2d::CCLayerColor
{
public:
    //构造函数
    GameOverLayer(void);
    ~GameOverLayer(void);
    //初始化游戏层
    bool initWithWon(bool won);
    //创建场景
    static cocos2d::CCScene* sceneWithWon(bool won);
    //创建游戏层
    static GameOverLayer* createWithWon(bool won);
    //判断游戏结束的方法
    void gameOverDone();
};

#endif /* defined(________GameOverLayer__) */
