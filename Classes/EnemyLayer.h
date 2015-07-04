#ifndef __ENEMYLAYER_H_
#define __ENEMYLAYER_H_

#include "cocos2d.h"
#include "Weapon.h"

/* �������ĵл��� */
class EnemyWithWeapon : public cocos2d::Node
{
public:
	Weapon * weapon;
	cocos2d::Sprite * enemyShip;
};

/* ���˲� */
class enemyLayer : public cocos2d::Layer
{
	enum {
		ENEMYSHIPBOOLSIZE = 20,				// �л��ɴ��ش�С
	};
public:
	~enemyLayer();

	CREATE_FUNC(enemyLayer);
	virtual bool init() override;

	/*  ������ײ��ʼ��  */
	void initContact(void);

	/*  ������ҵ�ǰλ��  */
	void setPlayer(cocos2d::Sprite * plasyer);

	/*  ���л��Ƿ�ɳ��˴��ڽ���  */
	void checkOutOfWindow(void);

	/*  �����������ĺ��� */
	EnemyWithWeapon * createEnemy();

	/*  ɨ��ص�����  */
	void scanSchedule(float Delta);

	/*  ��ʱ�����л��Ļص����� */
	void genertorSchedule(float Delta);

	/*  �л�����  */
	void enemyDestory(cocos2d::Sprite * enemyship);

	cocos2d::Sprite * m_player;			// �۲���ģʽ
	cocos2d::CCArray * m_UsedShips;		// ����Ļ�л�ĵл���
private:
	int m_score;
	cocos2d::Vec2 m_PlayerPos;			// ���λ��
	cocos2d::CCArray * m_EnemyShips;	// �л������
};

#endif