#include "cocos2d.h"
#include "EnemyLayer.h"
#include "GameScene.h"
#include "musicControl.h"
#include "Weapon.h"

USING_NS_CC;

/* 析构任务 释放池中的所有内容 */
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

/* 初始化敌人层的内容 */
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

		auto cache = SpriteFrameCache::getInstance();				// 加载纹理缓冲列表
		cache->addSpriteFramesWithFile("Plist/enemy/Enemy.plist");

		EnemyWithWeapon * enemy;
		for (int i = 0; i < enemyLayer::ENEMYSHIPBOOLSIZE; i++)
		{
			enemy = new EnemyWithWeapon;
			enemy->retain();

			char buff[10];
			sprintf(buff, "E%d.png", ((int)rand_0_1() * 10) % 6);
		
			/* 创建敌机 */
			enemy->enemyShip = Sprite::createWithSpriteFrameName(buff);
			enemy->enemyShip->retain();								// 敌机添加内存保留

			auto body = PhysicsBody::createBox(enemy->enemyShip->getContentSize());
			body->setCategoryBitmask(0x00000004);
			body->setCollisionBitmask(0x00000000);
			body->setContactTestBitmask(0x00000003);
			
			enemy->enemyShip->setPhysicsBody(body);

			/* 创建武器 */
			srand(time(nullptr));
			if (0 == ((int)(rand_0_1() * 10)) % 3)
				enemy->weapon = new Weapon(1.5f, Weapon::BULLETKIND::ENEMYBULLET2, rand_minus1_1()*rand_0_1()*50+150);	// 小弹
			else
				enemy->weapon = new Weapon(1.0f, Weapon::BULLETKIND::ENEMYBULLET1, rand_minus1_1()*rand_0_1() * 20+70);	// 大弹
			enemy->weapon->retain();								// 武器添加内存保留
			
			m_EnemyShips->addObject(enemy);							// 添加未使用敌机容器
		}

		initContact();

		m_score = 0;												// 死亡个数

		schedule(schedule_selector(enemyLayer::genertorSchedule), 1.0f);	// 定时任务	产生敌机
		schedule(schedule_selector(enemyLayer::scanSchedule), 0.5f);		// 定时任务	敌机武器

		return true;
	}
	else
		return false;
}

/*  设置玩家当前位置 : 观察者模式 */
void enemyLayer::setPlayer(Sprite * player)
{
	m_player = player;
}

/*  检查敌机是否飞出了窗口界限  */
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
			this->removeChild(enemy->enemyShip, true);		// 从 当前层中移出,并删除所有动作
			m_UsedShips->removeObject(enemy, false);		// 从 已使用缓冲池 移出, 不能立刻释放
			m_EnemyShips->addObject(enemy);					// 将 敌机 添加到 未使用缓冲池
		}
		else
			continue;
	}
}

/*	创建单个带武器的敌人  */
EnemyWithWeapon * enemyLayer::createEnemy()
{
	if (0 == m_EnemyShips->count())					// 未使用池中没有时则不产生
		return nullptr;
	
	Size visiable = Director::sharedDirector()->getVisibleSize();

	EnemyWithWeapon * sprite = static_cast <EnemyWithWeapon *> (m_EnemyShips->getRandomObject());	// 从 未使用缓冲池 取出元素
	m_EnemyShips->removeObject(sprite,false);														// 从 未使用缓冲池中 删除 取出元素
	m_UsedShips->addObject(sprite);																	// 将 取出元素 加入到 已使用缓冲池

	srand(time(nullptr));
	sprite->enemyShip->setPosition(rand_minus1_1()*rand_0_1()*160 + visiable.width/2, rand_0_1() * 5  + visiable.height);
	sprite->enemyShip->setRotation(180);

	ccBezierConfig bzConfig;
	float xEnd = rand_minus1_1()*rand_0_1() * 40;
	xEnd = xEnd <= 0 ? xEnd : xEnd + visiable.width;
	float yEnd = 0 - rand_0_1() * 40;
	bzConfig.endPosition = Vec2(xEnd, yEnd);		// 贝赛尔曲线 终点
	
	float xMid = xEnd <= 0 ? (sprite->enemyShip->getPosition() - bzConfig.endPosition).x * 3 / 4 + bzConfig.endPosition.x +150:
							 (sprite->enemyShip->getPosition() - bzConfig.endPosition).x  / 4 + sprite->enemyShip->getPosition().x - 50;
	float yMid = (sprite->enemyShip->getPosition() - bzConfig.endPosition).y * 3 / 4;
	bzConfig.controlPoint_1 = Vec2(xMid, yMid);		// 贝赛尔曲线控制点 1 号点

	xEnd <= 0 ? (sprite->enemyShip->getPosition() - bzConfig.endPosition).x / 4 + bzConfig.endPosition.x :
		(sprite->enemyShip->getPosition() - bzConfig.endPosition).x * 3 / 4 + sprite->enemyShip->getPosition().x;
	yMid = (sprite->enemyShip->getPosition() - bzConfig.endPosition).y / 4 + 50;
	
	bzConfig.controlPoint_2 = Vec2(xMid, yMid);		// 贝塞尔曲线控制点 2 号点

	auto action = BezierTo::create(rand_0_1() * rand_minus1_1() * 5 + 10.0f, bzConfig);// 贝赛尔曲线 动作

	sprite->enemyShip->runAction(action);
	
	sprite->weapon->setPos(sprite->enemyShip->getPosition() - Vec2(0.0f,40.0f));

	return sprite;
}

/*  扫描回调函数  */
void enemyLayer::scanSchedule(float Delta)
{
	Ref * sprite_temp = NULL;

	auto visiable = Director::sharedDirector()->getVisibleSize();

	CCARRAY_FOREACH_REVERSE(m_UsedShips, sprite_temp)
	{
		EnemyWithWeapon * sprite = static_cast <EnemyWithWeapon *>(sprite_temp);
		
		/*  在视图框内才能够敌机才能发射子弹  */
		auto pos = sprite->enemyShip->getPosition();
		if ((pos.x >=20 && pos.x<=visiable.width-20) &&
			(pos.y >=20 && pos.y<=visiable.height+20))										
		{ 
			sprite->weapon->setPos(sprite->enemyShip->getPosition() - Vec2(0, 20));
			sprite->weapon->aimTo(m_player->getPosition());								//	观察者模式
			sprite->weapon->shootHandle(this);
		}

		sprite->weapon->scanHandle(this);
	}
}

/*  定时产生敌机的回调函数 */
void enemyLayer::genertorSchedule(float Delta)
{
	EnemyWithWeapon * sprite = createEnemy();
	if (nullptr != sprite)
		addChild(sprite->enemyShip);				// 将敌机加入到 当前层

	checkOutOfWindow();								// 执行 回收函数
}

/*  敌人销毁动作  */
void enemyLayer::enemyDestory(Sprite * enemyShip)
{
	removeChild(enemyShip, false);
	Ref * ship = nullptr;
	CCARRAY_FOREACH_REVERSE(m_UsedShips, ship)
	{
		if (enemyShip == static_cast<EnemyWithWeapon *> (ship)->enemyShip)		// 在已经使用的子弹中搜索被销毁的精灵
		{
			this->removeChild(enemyShip, true);									// 从 当前层中移出,并删除所有动作
			m_UsedShips->removeObject(ship, false);								// 从 已使用缓冲池 移出, 不能立刻释放
			m_EnemyShips->addObject(ship);										// 将 敌机 添加到 未使用缓冲池
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