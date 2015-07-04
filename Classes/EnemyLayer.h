#ifndef __ENEMYLAYER_H_
#define __ENEMYLAYER_H_

#include "cocos2d.h"
#include "Weapon.h"

/* 带武器的敌机类 */
class EnemyWithWeapon : public cocos2d::Node
{
public:
	Weapon * weapon;
	cocos2d::Sprite * enemyShip;
};

/* 敌人层 */
class enemyLayer : public cocos2d::Layer
{
	enum {
		ENEMYSHIPBOOLSIZE = 20,				// 敌机飞船池大小
	};
public:
	~enemyLayer();

	CREATE_FUNC(enemyLayer);
	virtual bool init() override;

	/*  敌人碰撞初始化  */
	void initContact(void);

	/*  设置玩家当前位置  */
	void setPlayer(cocos2d::Sprite * plasyer);

	/*  检查敌机是否飞出了窗口界限  */
	void checkOutOfWindow(void);

	/*  创建带武器的函数 */
	EnemyWithWeapon * createEnemy();

	/*  扫描回调函数  */
	void scanSchedule(float Delta);

	/*  定时产生敌机的回调函数 */
	void genertorSchedule(float Delta);

	/*  敌机毁灭  */
	void enemyDestory(cocos2d::Sprite * enemyship);

	cocos2d::Sprite * m_player;			// 观察者模式
	cocos2d::CCArray * m_UsedShips;		// 在屏幕中活动的敌机池
private:
	int m_score;
	cocos2d::Vec2 m_PlayerPos;			// 玩家位置
	cocos2d::CCArray * m_EnemyShips;	// 敌机缓冲池
};

#endif