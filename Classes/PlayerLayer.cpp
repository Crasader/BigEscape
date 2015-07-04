#include "cocos2d.h"

#include "PlayerLayer.h"
#include "musicControl.h"
#include "StatusLayer.h"
#include "GameScene.h"
#include "Weapon.h"

USING_NS_CC;

/*  ������Ҵ���  */
playerLayer::~playerLayer()
{
	m_playerWeapon->autorelease();
}

/*  ��Ҳ�ĳ�ʼ��  */
bool playerLayer::init()
{
	if (Layer::init())
	{
		auto visable = Director::sharedDirector()->getVisibleSize();

		auto cache = SpriteFrameCache::getInstance();
		cache->addSpriteFramesWithFile("Plist/ship/ship.plist");				//�����ڴ滺���б�

		m_ship = Sprite::createWithSpriteFrameName("ship03.png");

		m_ship->setPosition(visable.width / 2, visable.height / 5);
		m_ship->setAnchorPoint(Vec2(0.5f, 0.5f));

		auto body = PhysicsBody::createBox(m_ship->getContentSize());			// �������
		body->setCategoryBitmask(0x00000001);
		body->setCollisionBitmask(0x00000000);
		body->setContactTestBitmask(0x0000000C);
		m_ship->setPhysicsBody(body);

		m_playerWeapon = new Weapon(0.20f, Weapon::BULLETKIND::PLAYERBULLET, 200);
		m_playerWeapon->retain();											
		m_ship->addChild(m_playerWeapon);										// �ɴ���ȡ���� �� ����ڴ����

		addChild(m_ship);
		
		m_label = LabelTTF::create("10", "Felt.ttf", 18);			// ������
		m_label->setAnchorPoint(Vec2(1, 1));
		m_label->setPosition(visable.width, visable.height);
		addChild(m_label);

		initTouched();															// ������ʼ��

		pauseShoot();															// ֹͣ���
	
		schedule(schedule_selector(playerLayer::weaponHadle), 0.15f);			// ������ʱ��

		initContact();															// ��ʼ����ײ

		m_blood = 10;
		m_Score = 0;		
		return true;
	}
	else
		return false;
}

/*	��Ҳ㴥������  */
void playerLayer::initTouched()
{
	listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(false);

	/*  ������ʼ ��������  */
	listener->onTouchBegan = [=](Touch * touch, Event * event)->bool{ 
		this->resumeShoot();
		return true; 
	};

	/*  �����ƶ� ��������  */
	listener->onTouchMoved = [](Touch * touch, Event * event)
	{
		auto target = static_cast<Sprite *> (event->getCurrentTarget());
		
		Vec2 pos = target->getPosition() + touch->getDelta();			// �޶����Χ
		auto x = pos.x <= 0 ? 0 :
				 pos.x >= 320  ? 320 : pos.x;
		auto y = pos.y <= target->getContentSize().height / 2 ? target->getContentSize().height / 2 :
			pos.y >= 480 - target->getContentSize().height / 2 ? 480 - target->getContentSize().height / 2 : pos.y;
		
		target->setPosition(x, y);
	};

	/*  �������� ��������  */
	listener->onTouchEnded = [=](Touch * touch, Event * event){
		this->pauseShoot();
	};

	/*  ����ȡ�� ��������  */
	listener->onTouchCancelled = [](Touch * touch, Event * event){ };

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, m_ship);
}

/*  ��Ҵ��������ص�����  */
void playerLayer::weaponHadle(float fDelta)
{
	if (true == m_shoot)
	{
		auto cache = SpriteFrameCache::getInstance();
		cache->addSpriteFramesWithFile("Plist/ship/ship.plist");				//�����ڴ滺���б�
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

/*	��ȡ��Ҳ�����Ҿ��� */
Sprite * playerLayer::getPlayerShip()
{
	return m_ship;
}

/* ���ֹͣ��� */
void playerLayer::pauseShoot()
{
	m_shoot = false;
}

/* ��һָ���� */
void playerLayer::resumeShoot()
{
	m_shoot = true;
}

/* ��һ���ʱִ�� */
void playerLayer::playerDestory()
{

	if (0 == --m_blood)							// Ѫ������
	{
		/*  ������ըЧ�� */
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

		auto layer = statusLayer::create();								// ��Ϸ�����
		layer->setScore(m_Score);
		getParent()->addChild(layer);
	}
	
	m_blood = m_blood <= 0 ? 0 : (musicControl::getInstance()->playEffect("Music/explodeEffect.mp3"),m_blood);
	char buff[10];
	sprintf(buff, "%d", m_blood);
	m_label->setString(buff);
}

/*  ��ײ��ʼ��  */
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

/* ����ɱ���� */
void playerLayer::setScore(int x)
{
	m_Score = x;
}