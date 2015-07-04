#include "cocos2d.h"

#include "GameScene.h"
#include "musicControl.h"

USING_NS_CC;

/*  ��ʼ����Ϸ����  */
bool gameScene::init()
{
	if (Scene::initWithPhysics())
	{
		auto visableSize = Director::sharedDirector()->getVisibleSize();

		/*  ����������������  */
		//getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
		
		/*  ������������ Ϊ������ */
		getPhysicsWorld()->setGravity(Vec2::ZERO);

		/*  ���뱳����  */
		m_backgroundLayer = gameBackgroundLayer::create();
		addChild(m_backgroundLayer);

		/*  ��Ϸ�ߵļ���  */
		m_playLayer = playerLayer::create();
		addChild(m_playLayer);

		/*  ���˵ļ���  */
		m_enemyPlayer = enemyLayer::create();
		addChild(m_enemyPlayer);
		m_enemyPlayer->setPlayer(m_playLayer->getPlayerShip());
		
		/* �˵���ļ��� */
		m_menuLayer = gameMenuLayer::create();
		CC_SAFE_RETAIN(m_menuLayer);
		
		/* �����ļ��� */
		m_statusLayer = statusLayer::create();
		CC_SAFE_RETAIN(m_statusLayer);

		m_isPause = false;
		keyboardInit();

		return true;
	}
	else
		return false;
}

/* ��������:���� �Ӳ˵����ɾ�� */
gameScene::~gameScene()
{
	CC_SAFE_RELEASE(m_menuLayer);
	CC_SAFE_RELEASE(m_statusLayer);
}

/*	��Ϸ��ͣ */
void gameScene::gamePause()
{
	m_backgroundLayer->pauseSchedulerAndActions();
	m_enemyPlayer->pauseSchedulerAndActions();
	m_playLayer->pauseSchedulerAndActions();
	m_playLayer->setTouchEnabled(false);
	addChild(m_menuLayer);
	m_isPause = true;
}

/*  ��Ϸ�ָ� */
void gameScene::gameResume()
{
	m_backgroundLayer->resumeSchedulerAndActions();
	m_enemyPlayer->resumeSchedulerAndActions();
	m_playLayer->resumeSchedulerAndActions();
	m_playLayer->setTouchEnabled(true);
	removeChild(m_menuLayer, true);
	m_isPause = false;
}

/*  �����ص�����  */
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

/*  �����¼���ʼ��*/
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