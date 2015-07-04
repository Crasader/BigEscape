#include "cocos2d.h"
#include "Weapon.h"

USING_NS_CC;

#define BULLETSIZE 50	//�ӵ��Ļ���ظ���

/*
	�������� ɾ�����е�����
*/
Weapon::~Weapon()
{
	Ref * sprite_temp = nullptr;
	CCARRAY_FOREACH_REVERSE(m_aBullet, sprite_temp)
	{
		sprite_temp->release();								//	����ɾ��δʹ���ӵ�
	}
	CCARRAY_FOREACH_REVERSE(m_aShootedBullet, sprite_temp)
	{
		sprite_temp->release();								//  ����ɾ����ʹ���ӵ�
	}

	m_aBullet->release();									//  ɾ��δʹ���ӵ�����
	m_aShootedBullet->release();							//  ɾ����ʹ���ӵ�����
}

/*
	���캯�� ��ʼ���ӵ�����	
*/
Weapon::Weapon(float pInterval, BULLETKIND bulletKind, float speed) : m_BulletInterval(pInterval), m_BulletKind(bulletKind), m_speed(speed)
{
	Sprite * sprite;

	m_aBullet = CCArray::createWithCapacity(BULLETSIZE);
	m_aBullet->retain();													// ������������δʹ���ӵ� ����ӱ���

	m_aShootedBullet = CCArray::createWithCapacity(BULLETSIZE);				// ��������������ʹ���ӵ� ����ӱ���
	m_aShootedBullet->retain();

	auto cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("Plist/bullet/bullet.plist");
	
	for (int i = 0; i < BULLETSIZE; i++)									// �����������ӵ�
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
		sprite->retain();													// �����ӵ� , ������ڴ汣��

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

		sprite->setPhysicsBody(body);										// ������嵽�ӵ�������

		m_aBullet->addObject(sprite);
	}
}

/*���÷�������*/
void Weapon::aimTo(Vec2 target)
{
	m_Target = target;
}

/* �����ӵ��������  */
void Weapon::recycleOneBullet(cocos2d::Sprite * bullet)
{
	m_aBullet->addObject(bullet);
}

/* �ӻ���ػ�ȡ�ӵ�  */
cocos2d::Sprite * Weapon::getOneBullet()
{
	auto sprite = m_aBullet->getLastObject();
	m_aBullet->removeObject(sprite, false);
	return static_cast <Sprite *> (sprite);
}

/* �ӵ�����Դλ�� */
void Weapon::setPos(Vec2 pos)
{
	m_Position = pos;
}

/* ��ʼ��Ŀ�귢��Ŀ�� PS�� ����Ӧ�Ĳ�(Layer)���� schedule ����ʱ���*/
void Weapon::shootHandle(Layer * layer)
{
	if (0 == m_aBullet->count())	//	����û���ӵ����򲻻��ٷ�������
		return;

	auto visiable = Director::sharedDirector()->getVisibleSize();
	auto origin = Director::sharedDirector()->getVisibleOrigin();

	auto sprite = static_cast <Sprite *>(m_aBullet->getLastObject());
	m_aBullet->removeLastObject(false);
	m_aShootedBullet->addObject(sprite);		// ��ͬ����صĲ���
	sprite->cleanup();

	sprite->setRotation(0);
	sprite->setPosition(m_Position+Vec2(0,15));

	auto sec = 600 / m_speed;
	auto angle = (m_Target - m_Position).getAngle();

	auto action = MoveTo::create(sec, m_Position + Vec2(600 * cos(angle), 600 * sin(angle)));
	
	sprite->runAction(action);
	
	layer->addChild(sprite);
}

/* �ж��Ƿ�����ɨ�躯�� PS�� �� ��Ӧ�Ĳ�(Layer)���� schedule ����ʱɨ��ʹ�� */
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
			layer->removeChild(sprite, true);				//  �ӵ�ǰ�����
			m_aShootedBullet->removeObject(sprite, true);	//	��Խ����ӵ��ӳ���ɾ��
			m_aBullet->addObject(sprite);
		}
		else
			continue;
	}
}