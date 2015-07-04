#include "cocos2d.h"

#include "GameScene.h"
#include "musicControl.h"

USING_NS_CC;

/*  初始化游戏场景  */
bool gameScene::init()
{
	if (Scene::initWithPhysics())
	{
		auto visableSize = Director::sharedDirector()->getVisibleSize();

		/*  开启调试物理引擎  */
		//getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
		
		/*  设置重力因子 为无重力 */
		getPhysicsWorld()->setGravity(Vec2::ZERO);

		/*  加入背景层  */
		m_backgroundLayer = gameBackgroundLayer::create();
		addChild(m_backgroundLayer);

		/*  游戏者的加入  */
		m_playLayer = playerLayer::create();
		addChild(m_playLayer);

		/*  敌人的加入  */
		m_enemyPlayer = enemyLayer::create();
		addChild(m_enemyPlayer);
		m_enemyPlayer->setPlayer(m_playLayer->getPlayerShip());
		
		/* 菜单层的加入 */
		m_menuLayer = gameMenuLayer::create();
		CC_SAFE_RETAIN(m_menuLayer);
		
		/* 结果层的加入 */
		m_statusLayer = statusLayer::create();
		CC_SAFE_RETAIN(m_statusLayer);

		m_isPause = false;
		keyboardInit();

		return true;
	}
	else
		return false;
}

/* 析构函数:处理 子菜单层的删除 */
gameScene::~gameScene()
{
	CC_SAFE_RELEASE(m_menuLayer);
	CC_SAFE_RELEASE(m_statusLayer);
}

/*	游戏暂停 */
void gameScene::gamePause()
{
	m_backgroundLayer->pauseSchedulerAndActions();
	m_enemyPlayer->pauseSchedulerAndActions();
	m_playLayer->pauseSchedulerAndActions();
	m_playLayer->setTouchEnabled(false);
	addChild(m_menuLayer);
	m_isPause = true;
}

/*  游戏恢复 */
void gameScene::gameResume()
{
	m_backgroundLayer->resumeSchedulerAndActions();
	m_enemyPlayer->resumeSchedulerAndActions();
	m_playLayer->resumeSchedulerAndActions();
	m_playLayer->setTouchEnabled(true);
	removeChild(m_menuLayer, true);
	m_isPause = false;
}

/*  按键回调函数  */
void gameScene::KeyreleaseCallback(EventKeyboard::KeyCode keyVal, Event* event)
{
	if (EventKeyboard::KeyCode::KEY_SPACE == keyVal)
	{
		if (false == m_isPause)
		{
			gamePause();

		}
		else
		{
			gameResume();
		}
	}
}

/*  按键事件初始化*/
void gameScene::keyboardInit()
{
	auto listener = EventListenerKeyboard::create();

	listener->onKeyPressed = CC_CALLBACK_2(gameScene::KeyreleaseCallback,this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void gameScene::setScore(int score)
{
	m_playLayer->setScore(score);
}