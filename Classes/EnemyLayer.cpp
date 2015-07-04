#include "cocos2d.h"
#include "EnemyLayer.h"
#include "GameScene.h"
#include "musicControl.h"
#include "Weapon.h"

USING_NS_CC;

/* �������� �ͷų��е��������� */
enemyLayer::~enemyLayer()
{
	Ref * enemy;
	CCARRAY_FOREACH_REVERSE(m_EnemyShips, enemy)
	{
		enemy->release();
	}
	CCARRAY_FOREACH_REVERSE(m_UsedShips, enemy)
	{
		enemy->release();
	}
	m_EnemyShips->release();
	m_UsedShips->release();
}

/* ��ʼ�����˲������ */
bool enemyLayer::init()
{
	if (Layer::init())
	{
		auto visiableSize = Director::sharedDirector()->getVisibleSize();
		auto origin = Director::sharedDirector()->getVisibleOrigin();

		m_EnemyShips = CCArray::createWithCapacity(ENEMYSHIPBOOLSIZE);
		m_UsedShips = CCArray::createWithCapacity(ENEMYSHIPBOOLSIZE);
		m_EnemyShips->retain();
		m_UsedShips->retain();

		auto cache = SpriteFrameCache::getInstance();				// �����������б�
		cache->addSpriteFramesWithFile("Plist/enemy/Enemy.plist");

		EnemyWithWeapon * enemy;
		for (int i = 0; i < enemyLayer::ENEMYSHIPBOOLSIZE; i++)
		{
			enemy = new EnemyWithWeapon;
			enemy->retain();

			char buff[10];
			sprintf(buff, "E%d.png", ((int)rand_0_1() * 10) % 6);
		
			/* �����л� */
			enemy->enemyShip = Sprite::createWithSpriteFrameName(buff);
			enemy->enemyShip->retain();								// �л�����ڴ汣��

			auto body = PhysicsBody::createBox(enemy->enemyShip->getContentSize());
			body->setCategoryBitmask(0x00000004);
			body->setCollisionBitmask(0x00000000);
			body->setContactTestBitmask(0x00000003);
			
			enemy->enemyShip->setPhysicsBody(body);

			/* �������� */
			srand(time(nullptr));
			if (0 == ((int)(rand_0_1() * 10)) % 3)
				enemy->weapon = new Weapon(1.5f, Weapon::BULLETKIND::ENEMYBULLET2, rand_minus1_1()*rand_0_1()*50+150);	// С��
			else
				enemy->weapon = new Weapon(1.0f, Weapon::BULLETKIND::ENEMYBULLET1, rand_minus1_1()*rand_0_1() * 20+70);	// ��
			enemy->weapon->retain();								// ��������ڴ汣��
			
			m_EnemyShips->addObject(enemy);							// ���δʹ�õл�����
		}

		initContact();

		m_score = 0;												// ��������

		schedule(schedule_selector(enemyLayer::genertorSchedule), 1.0f);	// ��ʱ����	�����л�
		schedule(schedule_selector(enemyLayer::scanSchedule), 0.5f);		// ��ʱ����	�л�����

		return true;
	}
	else
		return false;
}

/*  ������ҵ�ǰλ�� : �۲���ģʽ */
void enemyLayer::setPlayer(Sprite * player)
{
	m_player = player;
}

/*  ���л��Ƿ�ɳ��˴��ڽ���  */
void enemyLayer::checkOutOfWindow()
{
	EnemyWithWeapon * enemy;
	Ref * sprite = NULL;

	auto visiable = Director::sharedDirector()->getVisibleSize();
	auto origin = Director::sharedDirector()->getVisibleOrigin();

	CCARRAY_FOREACH_REVERSE(m_UsedShips, sprite)
	{
		enemy = static_cast <EnemyWithWeapon *> (sprite);
		auto pos = enemy->enemyShip->getPosition();
		if ((pos.x < 0|| pos.x >(visiable.width + origin.x )) || (pos.y <= 0))
		{
			this->removeChild(enemy->enemyShip, true);		// �� ��ǰ�����Ƴ�,��ɾ�����ж���
			m_UsedShips->removeObject(enemy, false);		// �� ��ʹ�û���� �Ƴ�, ���������ͷ�
			m_EnemyShips->addObject(enemy);					// �� �л� ��ӵ� δʹ�û����
		}
		else
			continue;
	}
}

/*	���������������ĵ���  */
EnemyWithWeapon * enemyLayer::createEnemy()
{
	if (0 == m_EnemyShips->count())					// δʹ�ó���û��ʱ�򲻲���
		return nullptr;
	
	Size visiable = Director::sharedDirector()->getVisibleSize();

	EnemyWithWeapon * sprite = static_cast <EnemyWithWeapon *> (m_EnemyShips->getRandomObject());	// �� δʹ�û���� ȡ��Ԫ��
	m_EnemyShips->removeObject(sprite,false);														// �� δʹ�û������ ɾ�� ȡ��Ԫ��
	m_UsedShips->addObject(sprite);																	// �� ȡ��Ԫ�� ���뵽 ��ʹ�û����

	srand(time(nullptr));
	sprite->enemyShip->setPosition(rand_minus1_1()*rand_0_1()*160 + visiable.width/2, rand_0_1() * 5  + visiable.height);
	sprite->enemyShip->setRotation(180);

	ccBezierConfig bzConfig;
	float xEnd = rand_minus1_1()*rand_0_1() * 40;
	xEnd = xEnd <= 0 ? xEnd : xEnd + visiable.width;
	float yEnd = 0 - rand_0_1() * 40;
	bzConfig.endPosition = Vec2(xEnd, yEnd);		// ���������� �յ�
	
	float xMid = xEnd <= 0 ? (sprite->enemyShip->getPosition() - bzConfig.endPosition).x * 3 / 4 + bzConfig.endPosition.x +150:
							 (sprite->enemyShip->getPosition() - bzConfig.endPosition).x  / 4 + sprite->enemyShip->getPosition().x - 50;
	float yMid = (sprite->enemyShip->getPosition() - bzConfig.endPosition).y * 3 / 4;
	bzConfig.controlPoint_1 = Vec2(xMid, yMid);		// ���������߿��Ƶ� 1 �ŵ�

	xEnd <= 0 ? (sprite->enemyShip->getPosition() - bzConfig.endPosition).x / 4 + bzConfig.endPosition.x :
		(sprite->enemyShip->getPosition() - bzConfig.endPosition).x * 3 / 4 + sprite->enemyShip->getPosition().x;
	yMid = (sprite->enemyShip->getPosition() - bzConfig.endPosition).y / 4 + 50;
	
	bzConfig.controlPoint_2 = Vec2(xMid, yMid);		// ���������߿��Ƶ� 2 �ŵ�

	auto action = BezierTo::create(rand_0_1() * rand_minus1_1() * 5 + 10.0f, bzConfig);// ���������� ����

	sprite->enemyShip->runAction(action);
	
	sprite->weapon->setPos(sprite->enemyShip->getPosition() - Vec2(0.0f,40.0f));

	return sprite;
}

/*  ɨ��ص�����  */
void enemyLayer::scanSchedule(float Delta)
{
	Ref * sprite_temp = NULL;

	auto visiable = Director::sharedDirector()->getVisibleSize();

	CCARRAY_FOREACH_REVERSE(m_UsedShips, sprite_temp)
	{
		EnemyWithWeapon * sprite = static_cast <EnemyWithWeapon *>(sprite_temp);
		
		/*  ����ͼ���ڲ��ܹ��л����ܷ����ӵ�  */
		auto pos = sprite->enemyShip->getPosition();
		if ((pos.x >=20 && pos.x<=visiable.width-20) &&
			(pos.y >=20 && pos.y<=visiable.height+20))										
		{ 
			sprite->weapon->setPos(sprite->enemyShip->getPosition() - Vec2(0, 20));
			sprite->weapon->aimTo(m_player->getPosition());								//	�۲���ģʽ
			sprite->weapon->shootHandle(this);
		}

		sprite->weapon->scanHandle(this);
	}
}

/*  ��ʱ�����л��Ļص����� */
void enemyLayer::genertorSchedule(float Delta)
{
	EnemyWithWeapon * sprite = createEnemy();
	if (nullptr != sprite)
		addChild(sprite->enemyShip);				// ���л����뵽 ��ǰ��

	checkOutOfWindow();								// ִ�� ���պ���
}

/*  �������ٶ���  */
void enemyLayer::enemyDestory(Sprite * enemyShip)
{
	removeChild(enemyShip, false);
	Ref * ship = nullptr;
	CCARRAY_FOREACH_REVERSE(m_UsedShips, ship)
	{
		if (enemyShip == static_cast<EnemyWithWeapon *> (ship)->enemyShip)		// ���Ѿ�ʹ�õ��ӵ������������ٵľ���
		{
			this->removeChild(enemyShip, true);									// �� ��ǰ�����Ƴ�,��ɾ�����ж���
			m_UsedShips->removeObject(ship, false);								// �� ��ʹ�û���� �Ƴ�, ���������ͷ�
			m_EnemyShips->addObject(ship);										// �� �л� ��ӵ� δʹ�û����
			static_cast<gameScene *>(getParent())->setScore(++m_score);
		}
	}
	musicControl::getInstance()->playEffect("Music/explodeEffect.mp3");
}

void enemyLayer::initContact(void)
{
	auto listener = EventListenerPhysicsContact::create();

	listener->onContactBegin = [=](PhysicsContact& contact){
		auto crash1 = static_cast<Sprite *>(contact.getShapeA()->getBody()->getNode());
		auto crash2 = static_cast<Sprite *>(contact.getShapeB()->getBody()->getNode());

		Ref * enemy_tmp = nullptr;
		CCARRAY_FOREACH_REVERSE(m_UsedShips, enemy_tmp)
		{
			auto enemy = static_cast<EnemyWithWeapon *> (enemy_tmp);
			if (crash1 == enemy->enemyShip)
			{
				this->enemyDestory(crash1);
				if (crash2 != this->m_player)
					crash2->setPosition(380, 520);
			}
			else if(crash2 == enemy->enemyShip)
			{
				this->enemyDestory(crash2);
				if (crash1 != this->m_player)
					crash1->setPosition(380, 520);
			}
		}
		return true;
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}