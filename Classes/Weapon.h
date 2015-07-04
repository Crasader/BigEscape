#ifndef __WEAPON_H_
#define __WEAPON_H_

#include "cocos2d.h"

/*
	weapon
	������ �������ӵ� �� ��׼Ŀ�� ������÷�
	ֻ������׼(aimTo) �ٿ�ǹ(shootHandle)����Ȼ�ͻ�û��Ŀ������䣬
*/
class Weapon : public cocos2d::Node
{
public:
	enum BULLETKIND {			// �����ӵ�������
		PLAYERBULLET = 1,
		ENEMYBULLET1 = 2,
		ENEMYBULLET2 = 3,
	};
	/* �����ķ����ӵ��ļ��ʱ�� */
	Weapon( float pInterval, BULLETKIND bulletKind, float speed);

	/* �������������� */
	~Weapon();

	/* �ӵ�λ�� */
	void setPos(cocos2d::Vec2 pos);

	/* ��׼��Ŀ���ַ  */
	void aimTo(cocos2d::Vec2 target);

	/* �����ӵ�  */
	void recycleOneBullet(cocos2d::Sprite * bullet);

	/* ��ȡ�ӵ�  */
	cocos2d::Sprite * getOneBullet();

	/* ��ʼ��Ŀ�귢��һ���ӵ� PS�� ����Ӧ�Ĳ�(Layer)���� schedule ����ʱ��� */
	void shootHandle(cocos2d::Layer * layer);
	/* �ж��Ƿ�����ɨ�躯�� PS�� �� ��Ӧ�Ĳ�(Layer)���� schedule ����ʱɨ��ʹ�� */
	void scanHandle(cocos2d::Layer * layer);

	/* ��ͼ�ռ����Ƿ���Ȼ���ӵ� */
	bool isExistBullet()
	{
		return m_aShootedBullet->count() ? true : false;
	}

	float m_BulletInterval;					//	�ӵ�������ʱ��
	cocos2d::CCArray * m_aShootedBullet;	//  �ӵ� �����

private:

	BULLETKIND m_BulletKind;				//  �ӵ�������
	float	m_speed;						//	�ӵ��ٶ�
	cocos2d::Vec2 m_Target;					//  �ӵ��������Ŀ��
	cocos2d::Vec2 m_Position;				//	���������ߵ�λ��
	cocos2d::CCArray  * m_aBullet;			//  �ӵ� �����
};

#endif