#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "GameOverLayer.h"
using namespace cocos2d;
using namespace CocosDenshion;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
        bool bRet = false;
        do
        {
            //将背景设置为白色
            CC_BREAK_IF(! CCLayerColor::initWithColor(ccc4(255, 255, 255, 255)));
            //获取屏幕的大小
            CCSize winSize = CCDirector::sharedDirector()->getWinSize();
            //初始化一个精灵作为主人公
            _player = CCSprite::create("player2.png");
            //设置精灵的位置
            _player->setPosition(ccp(_player->getContentSize().width / 2, winSize.height / 2));
            //将精灵添加到当前层上
            this->addChild(_player);
            
            //添加定时器每秒执行一次
            this->schedule(schedule_selector(HelloWorld::gameLogic), 1.0);
            
            //开启多点触摸事件
            this->setTouchEnabled(true);
            
            //初始化存放敌人的数组
            this->_monsters = CCArray::create();
            //因为要再其他函数中使用，因此要retain一下
            this->_monsters->retain();
            //初始化存放子弹的数组
            this->_projectiles = CCArray::create();
            //因为要再其他函数中使用，因此要retain一下
            this->_projectiles->retain();
            //开启每帧更新的方法
            this->scheduleUpdate();
            
            bRet = true;
        } while (0);
        return bRet;
}


void HelloWorld::addMonster()
{
    //初始化敌人
    _monster = CCSprite::create("monster.png");
    //设置敌人的tag值，tag就相当于一个记号
    _monster->setTag(1);
    //添加到数组中，以便以后碰撞检测时使用
    _monsters->addObject(_monster);

    
    
    //屏幕的尺寸
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    //敌人出现的Y坐标的最小值
    int minY = _monster->getContentSize().height / 2;
    //敌人出现的Y坐标的最大值  ，其中getContentSize()精灵图片的尺寸
    int maxY = winSize.height - _monster->getContentSize().height / 2;
    //敌人出现的Y坐标的范围
    int rangeY = maxY - minY;
    //计算随机值
    int actualY = (rand() % rangeY) + minY;
    //设置敌人的坐标
    _monster->setPosition(ccp(winSize.width + _monster->getContentSize().width / 2, actualY));
    //将敌人添加到游戏层上
    this->addChild(_monster);
    
    int minDuration = 2.0;
    int maxDuration = 4.0;
    int rangeDuration = maxDuration - minDuration;
    //计算一个随机数
    int actualDuration = (rand() % rangeDuration) + minDuration;
    //   动作  第一个参数用多长时间执行完这个动作，第二个参数到达的位置
    CCMoveTo *actionMove = CCMoveTo::create(actualDuration, ccp(-_monster->getContentSize().width / 2, actualY));
    //动作执行完的回调方法，  哪个对象调用就将谁作为参数传过去
    CCCallFuncN *actionMoveDone = CCCallFuncN::create(this, callfuncN_selector(HelloWorld::spriteMoveFinished));
    //执行动作
    _monster->runAction(CCSequence::create(actionMove, actionMoveDone, NULL));
}

void HelloWorld::spriteMoveFinished(CCNode *sender)
{
    //获取精灵
    CCSprite *sprite = (CCSprite*)sender;
    //将精灵移除
    this->removeChild(sprite, true);
    
    //判断
    if (sprite->getTag() == 1)
    {
        //从数组中移除
        _monsters->removeObject(sprite);
        //失败条件
   //     CCScene *gameOverScene = GameOverLayer::sceneWithWon(false);
        //切换场景
      //  CCDirector::sharedDirector()->replaceScene(gameOverScene);
    }
    else if (sprite->getTag() == 2)
    {
        //从数组中移除
        _projectiles->removeObject(sprite);
    }
}


void HelloWorld::gameLogic( float dt )
{
    //添加怪物
    this->addMonster();
}
void HelloWorld::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
    //获取触摸的点
    CCTouch *touch = (CCTouch*)pTouches->anyObject();
    
    //获取触摸点的坐标
    CCPoint location = this->convertTouchToNodeSpace(touch);
    //获取当前屏幕的大小
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    //初始化子弹精灵
    projectile = CCSprite::create("projectile2.png");
    projectile->retain();
    
    //设置tag值，就相当于一个记号
    projectile->setTag(2);
    
    
  
    
    
    
    //子弹发射时的位置设置位置
    projectile->setPosition(ccp(20, winSize.height / 2));
    
    //得到的值是上图小Y和小X的值
    CCPoint offset = ccpSub(location, projectile->getPosition());
    
    if (offset.x <= 0)
    {
        return;
    }
   
    //得到长的X的值
    int realX = winSize.width + projectile->getContentSize().width / 2;
    //计算出比率
    float ratio = (float)offset.y / (float)offset.x;
    //得到实际Y的值  后面加的是子弹Y轴的坐标  只有加上子弹Y轴的坐标才是实际的Y
    int realY = realX * ratio + projectile->getPosition().y;
    CCPoint realDest = ccp(realX, realY);
    
    //计算旋转角度的公式如下：
    // CCPoint normalized = ccpNormalize(ccp(目标点或触摸点的X轴坐标 - 需要转角的精灵的x轴坐标, 目标点或触摸点的Y轴坐标 - 需要转角的精灵的Y轴坐标);
    //目标点和当前人物的标准化坐标点
    CCPoint normalized = ccpNormalize(ccp(location.x - _player->getPosition().x, location.y - _player->getPosition().y));
    //计算弧度的方法
    float angleRadians = atan2(normalized.y, -normalized.x);
    //精灵转的角度  后面的加180 主要是根据初始图片的角度得到的，最简单的方法是试试如下4个数0、90、180、270，就能解决问题。
    float angleDegrees = CC_RADIANS_TO_DEGREES(angleRadians) + 180;
    CCLOG("%f",angleDegrees);
   
    
    //注掉下面这行代码
    //添加到子弹数组中，以便以后碰撞检测时使用
    // _projectiles->addObject(projectile);
    
    //  this->addChild(projectile);
    
    
     //_player->setRotation(angleDegrees);
    //旋转动作
    _player->runAction(CCSequence::create(CCRotateTo::create(0.07f, angleDegrees),CCCallFunc::create(this, callfunc_selector(HelloWorld::finishShoot)), NULL));
    
    
    
    projectile->runAction(CCSequence::create(CCMoveTo::create(1.5f, realDest),CCCallFuncN::create(this, callfuncN_selector(HelloWorld::spriteMoveFinished)), NULL));
   
}


void HelloWorld::finishShoot()
{
    //将子弹添加到场景中
    this->addChild(projectile);
    //添加到子弹数组中
    _projectiles->addObject(projectile);
     //释放
    projectile->release();
    //置空
    projectile = NULL;
}



HelloWorld::HelloWorld()
{
    //在构造函数中将指针置空，防止野指针，如果不置空调用，再没有初始化就调用就是野指针了
    _monsters = NULL;
    _projectiles = NULL;
    
    _monstersDestroyed = 0;
}



void HelloWorld::update(float delta)
{
    //创建一个要删除子弹的数组
    CCArray *projectilesToDelete = CCArray::create();
    //创建2个临时的CCObject对象，用来接收遍历出来的对象
    CCObject *pObject = NULL;
    CCObject *pObject2 = NULL;
    //遍历子弹数组
    CCARRAY_FOREACH(_projectiles, pObject)
    {
        //得到子弹对象
        CCSprite *projectile = (CCSprite*)pObject;
        //创建一个要删除敌人的数组
        CCArray *monstersToDelete = CCArray::create();
        //遍历敌人数组
        CCARRAY_FOREACH(_monsters, pObject2)
        {
            //得到敌人对象
            CCSprite *monster = (CCSprite*)pObject2;
            //碰撞检测，projectile->boundingBox()代表子弹碰撞盒子,intersectsRect判断是否相交的方法，monster->boundingBox()敌人的碰撞
            if (projectile->boundingBox().intersectsRect(monster->boundingBox()))
                
            {
                //如果上述判断为真,说明子弹和敌人发生了碰撞，将敌人加入要删除敌人的数组
                monstersToDelete->addObject(monster);
                //将子弹加入要删除敌人的数组
                projectilesToDelete->addObject(projectile);

            }
        }
        //遍历要删除敌人的数组
        CCARRAY_FOREACH(monstersToDelete, pObject2)
        {
            //得到敌人对象
            CCSprite *monster = (CCSprite*)pObject2;
            //从数组中移除
            _monsters->removeObject(monster);
            //从游戏层上移除
            this->removeChild(monster, true);
            //消灭一个敌人，让_monstersDestroyed加1；
            _monstersDestroyed++;
            //胜利条件
            if (_monstersDestroyed > 30)
            {
                //创建创建，并传递参数：即胜利条件true
                CCScene *gameOverScene = GameOverLayer::sceneWithWon(true);
                //场景切换
                CCDirector::sharedDirector()->replaceScene(gameOverScene);
            }
        }
        
        //将要删除敌人的数组释放掉
        monstersToDelete->release();
    }
    //遍历要删除子弹的数组
    CCARRAY_FOREACH(projectilesToDelete, pObject)
    {
         //得到子弹对象
        CCSprite *projectile = (CCSprite*)pObject;
        //从数组中移除
        _projectiles->removeObject(projectile);
        //从游戏层上移除
        this->removeChild(projectile, true);
    }
    //将要删除子弹的数组释放掉
    projectilesToDelete->release();
}
