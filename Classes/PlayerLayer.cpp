#include "cocos2d.h"

#include "PlayerLayer.h"
#include "musicControl.h"
#include "StatusLayer.h"
#include "GameScene.h"
#include "Weapon.h"

USING_NS_CC;

/*  析构玩家处理  */
playerLayer::~playerLayer()
{
	m_playerWeapon->autorelease();
}

/*  玩家层的初始化  */
bool playerLayer::init()
{
	if (Layer::init())
	{
		auto visable = Director::sharedDirector()->getVisibleSize();

		auto cache = SpriteFrameCache::getInstance();
		cache->addSpriteFramesWithFile("Plist/ship/ship.plist");				//建立内存缓冲列表

		m_ship = Sprite::createWithSpriteFrameName("ship03.png");

		m_ship->setPosition(visable.width / 2, visable.height / 5);
		m_ship->setAnchorPoint(Vec2(0.5f, 0.5f));

		auto body = PhysicsBody::createBox(m_ship->getContentSize());			// 玩家外形
		body->setCategoryBitmask(0x00000001);
		body->setCollisionBitmask(0x00000000);
		body->setContactTestBitmask(0x0000000C);
		m_ship->setPhysicsBody(body);

		m_playerWeapon = new Weapon(0.20f, Weapon::BULLETKIND::PLAYERBULLET, 200);
		m_playerWeapon->retain();											
		m_ship->addChild(m_playerWeapon);										// 飞船获取武器 并 添加内存管理

		addChild(m_ship);
		
		m_label = LabelTTF::create("10", "Felt.ttf", 18);			// 生命条
		m_label->setAnchorPoint(Vec2(1, 1));
		m_label->setPosition(visable.width, visable.height);
		addChild(m_label);

		initTouched();															// 触摸初始化

		pauseShoot();															// 停止射击
	
		schedule(schedule_selector(playerLayer::weaponHadle), 0.15f);			// 开启定时器

		initContact();															// 初始化碰撞

		m_blood = 10;
		m_Score = 0;		
		return true;
	}
	else
		return false;
}

/*	玩家层触摸更新  */
void playerLayer::initTouched()
{
	listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(false);

	/*  触摸开始 匿名函数  */
	listener->onTouchBegan = [=](Touch * touch, Event * event)->bool{ 
		this->resumeShoot();
		return true; 
	};

	/*  触摸移动 匿名函数  */
	listener->onTouchMoved = [](Touch * touch, Event * event)
	{
		auto target = static_cast<Sprite *> (event->getCurrentTarget());
		
		Vec2 pos = target->getPosition() + touch->getDelta();			// 限定活动范围
		auto x = pos.x <= 0 ? 0 :
				 pos.x >= 320  ? 320 : pos.x;
		auto y = pos.y <= target->getContentSize().height / 2 ? target->getContentSize().height / 2 :
			pos.y >= 480 - target->getContentSize().height / 2 ? 480 - target->getContentSize().height / 2 : pos.y;
		
		target->setPosition(x, y);
	};

	/*  触摸结束 匿名函数  */
	listener->onTouchEnded = [=](Touch * touch, Event * event){
		this->pauseShoot();
	};

	/*  触摸取消 匿名函数  */
	listener->onTouchCancelled = [](Touch * touch, Event * event){ };

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, m_ship);
}

/*  玩家处理武器回调函数  */
void playerLayer::weaponHadle(float fDelta)
{
	if (true == m_shoot)
	{
		auto cache = SpriteFrameCache::getInstance();
		cache->addSpriteFramesWithFile("Plist/ship/ship.plist");				//建立内存缓冲列表
		cocos2d::Vector <cocos2d::SpriteFrame *> aFrame;
		
		aFrame.pushBack(cache->getSpriteFrameByName("ship02.png"));
		aFrame.pushBack(cache->getSpriteFrameByName("ship01.png"));
		aFrame.pushBack(cache->getSpriteFrameByName("ship03.png"));

		musicControl::getInstance()->playEffect("Music/fireEffect.mp3");

		m_ship->runAction(Animate::create(Animation::createWithSpriteFrames(aFrame, 0.05f)));
		m_playerWeapon->setPos(m_ship->getPosition());
		m_playerWeapon->aimTo(Vec2(m_ship->getPosition().x, 520));
		m_playerWeapon->shootHandle(this);

	}
	m_playerWeapon->scanHandle(this);
}

/*	获取玩家层中玩家精灵 */
Sprite * playerLayer::getPlayerShip()
{
	return m_ship;
}

/* 玩家停止射击 */
void playerLayer::pauseShoot()
{
	m_shoot = false;
}

/* 玩家恢复射击 */
void playerLayer::resumeShoot()
{
	m_shoot = true;
}

/* 玩家毁灭时执行 */
void playerLayer::playerDestory()
{

	if (0 == --m_blood)							// 血量处理
	{
		/*  建立爆炸效果 */
		Vector<SpriteFrame *> frame;
		SpriteFrameCache *cache = SpriteFrameCache::getInstance();
		cache->addSpriteFramesWithFile("Plist/explosion/explosion.plist");

		auto sprite1 = Sprite::createWithSpriteFrameName("explosion_01.png");

		const int numFrames = 35;
		for (int i = 0; i < numFrames; i++)
		{
			char filename[40];
			sprintf(filename, "explosion_%02d.png", i + 1);
			frame.pushBack(cache->getSpriteFrameByName(filename));
		}

		auto animation = Animate::create(Animation::createWithSpriteFrames(frame, 0.03f));

		sprite1->setPosition(Vec2::ZERO);
		sprite1->setScale(2);
		sprite1->runAction(animation);

		m_ship->addChild(sprite1, 1);
		musicControl::getInstance()->playEffect("Music/shipDestroyEffect.mp3");
		
		_eventDispatcher->removeEventListener(listener);
		pauseShoot();

		auto layer = statusLayer::create();								// 游戏结果层
		layer->setScore(m_Score);
		getParent()->addChild(layer);
	}
	
	m_blood = m_blood <= 0 ? 0 : (musicControl::getInstance()->playEffect("Music/explodeEffect.mp3"),m_blood);
	char buff[10];
	sprintf(buff, "%d", m_blood);
	m_label->setString(buff);
}

/*  碰撞初始化  */
void playerLayer::initContact()
{
	auto listener = EventListenerPhysicsContact::create();

	listener->onContactBegin = [=](PhysicsContact& contact){
		auto crash1 = static_cast<Sprite *>(contact.getShapeA()->getBody()->getNode());
		auto crash2 = static_cast<Sprite *>(contact.getShapeB()->getBody()->getNode());
		if (crash1 == this->getPlayerShip() || crash2 == this->getPlayerShip())
		{
			this->playerDestory();
		}
			return true;
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

/* 设置杀敌数 */
void playerLayer::setScore(int x)
{
	m_Score = x;
}