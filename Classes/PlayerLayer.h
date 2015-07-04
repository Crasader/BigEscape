#ifndef _PLAYERLAYER_H_
#define _PLAYERLAYER_H_

#include "cocos2d.h"
#include "Weapon.h"

class playerLayer : public cocos2d::Layer
{
public:
	~playerLayer();

	CREATE_FUNC(playerLayer);

	/*  ��ʼ������  */
	virtual bool init() override;

	/*	��ȡ��ҷɴ� */
	cocos2d::Sprite * getPlayerShip();

	/*	��ͣ���	 */
	void pauseShoot(void);

	/*  �ָ����  */
	void resumeShoot(void);

	/*  ��һ��� */
	void playerDestory(void);

	/* ����ɱ���� */
	void setScore(int x);
private:
	/*  ��������  */
	void initTouched();

	/*  ��ײ��ʼ��  */
	void initContact();

	/*  ��������  */
	void weaponHadle(float fDelta);
	
	cocos2d::LabelTTF * m_label;
	int m_blood;
	int m_Score;
	bool m_shoot;				// �Ƿ����
	cocos2d::EventListenerTouchOneByOne * listener;
	cocos2d::Sprite * m_ship;	// ��Ҿ���
	Weapon * m_playerWeapon;	// ��ӵ�е�����
};

#endif