#include "cocos2d.h"
#include "Weapon.h"

USING_NS_CC;

#define BULLETSIZE 50	//子弹的缓冲池个数

/*
	析构函数 删除其中的数据
*/
Weapon::~Weapon()
{
	Ref * sprite_temp = nullptr;
	CCARRAY_FOREACH_REVERSE(m_aBullet, sprite_temp)
	{
		sprite_temp->release();								//	遍历删除未使用子弹
	}
	CCARRAY_FOREACH_REVERSE(m_aShootedBullet, sprite_temp)
	{
		sprite_temp->release();								//  遍历删除已使用子弹
	}

	m_aBullet->release();									//  删除未使用子弹容器
	m_aShootedBullet->release();							//  删除已使用子弹容器
}

/*
	构造函数 初始化子弹容器	
*/
Weapon::Weapon(float pInterval, BULLETKIND bulletKind, float speed) : m_BulletInterval(pInterval), m_BulletKind(bulletKind), m_speed(speed)
{
	Sprite * sprite;

	m_aBullet = CCArray::createWithCapacity(BULLETSIZE);
	m_aBullet->retain();													// 建立容器保存未使用子弹 并添加保存

	m_aShootedBullet = CCArray::createWithCapacity(BULLETSIZE);				// 建立容器保存已使用子弹 并添加保存
	m_aShootedBullet->retain();

	auto cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("Plist/bullet/bullet.plist");
	
	for (int i = 0; i < BULLETSIZE; i++)									// 产生带纹理子弹
	{
		switch (m_BulletKind)
		{
			case PLAYERBULLET:
				sprite = Sprite::createWithSpriteFrameName("bullet03.png");
				break;
			case ENEMYBULLET1:
				sprite = Sprite::createWithSpriteFrameName("bullet02.png");
				break;
			case ENEMYBULLET2:
				sprite = Sprite::createWithSpriteFrameName("bullet01.png");
				break;
		}
		sprite->retain();													// 生成子弹 , 并添加内存保存

		auto body = PhysicsBody::createBox(sprite->getContentSize());
		body->setCollisionBitmask(0x00000000);

		if (BULLETKIND::PLAYERBULLET == bulletKind)
		{
			body->setCategoryBitmask(0x00000002);
			body->setContactTestBitmask(0x00000004);
		}
		else
		{
			body->setCategoryBitmask(0x00000008);
			body->setContactTestBitmask(0x00000001);
		}

		sprite->setPhysicsBody(body);										// 添加物体到子弹精灵上

		m_aBullet->addObject(sprite);
	}
}

/*设置发送属性*/
void Weapon::aimTo(Vec2 target)
{
	m_Target = target;
}

/* 回收子弹到缓冲池  */
void Weapon::recycleOneBullet(cocos2d::Sprite * bullet)
{
	m_aBullet->addObject(bullet);
}

/* 从缓冲池获取子弹  */
cocos2d::Sprite * Weapon::getOneBullet()
{
	auto sprite = m_aBullet->getLastObject();
	m_aBullet->removeObject(sprite, false);
	return static_cast <Sprite *> (sprite);
}

/* 子弹发射源位置 */
void Weapon::setPos(Vec2 pos)
{
	m_Position = pos;
}

/* 开始向目标发送目标 PS： 由相应的层(Layer)调用 schedule 来定时射击*/
void Weapon::shootHandle(Layer * layer)
{
	if (0 == m_aBullet->count())	//	池中没有子弹，则不会再发生发射
		return;

	auto visiable = Director::sharedDirector()->getVisibleSize();
	auto origin = Director::sharedDirector()->getVisibleOrigin();

	auto sprite = static_cast <Sprite *>(m_aBullet->getLastObject());
	m_aBullet->removeLastObject(false);
	m_aShootedBullet->addObject(sprite);		// 不同对象池的操作
	sprite->cleanup();

	sprite->setRotation(0);
	sprite->setPosition(m_Position+Vec2(0,15));

	auto sec = 600 / m_speed;
	auto angle = (m_Target - m_Position).getAngle();

	auto action = MoveTo::create(sec, m_Position + Vec2(600 * cos(angle), 600 * sin(angle)));
	
	sprite->runAction(action);
	
	layer->addChild(sprite);
}

/* 判断是否出界的扫描函数 PS： 由 相应的层(Layer)调用 schedule 来定时扫描使用 */
void Weapon::scanHandle(Layer * layer)
{
	auto visiable = Director::sharedDirector()->getVisibleSize();
	auto origin = Director::sharedDirector()->getVisibleOrigin();

	Ref * sprite_temp = NULL;
	CCARRAY_FOREACH_REVERSE(m_aShootedBullet, sprite_temp)
	{
		Sprite * sprite = static_cast <Sprite *> (sprite_temp);
		auto pos = sprite->getPosition();
		if ((pos.x <= -20 || pos.x >= (visiable.width + origin.x + 20)) ||
			(pos.y <= -20 || pos.y >= (visiable.height + origin.y + 20)))
		{ 
			layer->removeChild(sprite, true);				//  从当前层回收
			m_aShootedBullet->removeObject(sprite, true);	//	将越界的子弹从池中删除
			m_aBullet->addObject(sprite);
		}
		else
			continue;
	}
}