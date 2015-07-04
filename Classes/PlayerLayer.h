#ifndef _PLAYERLAYER_H_
#define _PLAYERLAYER_H_

#include "cocos2d.h"
#include "Weapon.h"

class playerLayer : public cocos2d::Layer
{
public:
	~playerLayer();

	CREATE_FUNC(playerLayer);

	/*  初始化处理  */
	virtual bool init() override;

	/*	获取玩家飞船 */
	cocos2d::Sprite * getPlayerShip();

	/*	暂停射击	 */
	void pauseShoot(void);

	/*  恢复射击  */
	void resumeShoot(void);

	/*  玩家毁灭 */
	void playerDestory(void);

	/* 设置杀敌数 */
	void setScore(int x);
private:
	/*  触摸处理  */
	void initTouched();

	/*  碰撞初始化  */
	void initContact();

	/*  武器处理  */
	void weaponHadle(float fDelta);
	
	cocos2d::LabelTTF * m_label;
	int m_blood;
	int m_Score;
	bool m_shoot;				// 是否射击
	cocos2d::EventListenerTouchOneByOne * listener;
	cocos2d::Sprite * m_ship;	// 玩家精灵
	Weapon * m_playerWeapon;	// 所拥有的武器
};

#endif