#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::CCLayerColor
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();

    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    //主人公
    cocos2d::CCSprite * _player;
    //敌人
    cocos2d::CCSprite * _monster;
    //子弹精灵
    cocos2d::CCSprite *projectile;
    
    //添加敌人的方法
    void addMonster();
    //移除精灵的函数
    void spriteMoveFinished(CCNode *sender);
    //调用添加敌人的函数
    void gameLogic( float dt );
    

    
    
    void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    //构造函数
    HelloWorld();
    //存放敌人的数组
    cocos2d::CCArray *_monsters;
    //存放子弹的数组
    cocos2d::CCArray *_projectiles;
    //每帧刷新的方法
     void update(float delta);
    
    void finishShoot();
    
    
    //记录玩家消灭怪物的数量
    int _monstersDestroyed;
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
