#ifndef __WEAPON_H_
#define __WEAPON_H_

#include "cocos2d.h"

/*
	weapon
	武器类 负责发射子弹 并 瞄准目标 此类的用法
	只能先瞄准(aimTo) 再开枪(shootHandle)，不然就会没有目标的乱射，
*/
class Weapon : public cocos2d::Node
{
public:
	enum BULLETKIND {			// 描述子弹的种类
		PLAYERBULLET = 1,
		ENEMYBULLET1 = 2,
		ENEMYBULLET2 = 3,
	};
	/* 武器的发射子弹的间隔时间 */
	Weapon( float pInterval, BULLETKIND bulletKind, float speed);

	/* 武器的析构函数 */
	~Weapon();

	/* 子弹位置 */
	void setPos(cocos2d::Vec2 pos);

	/* 瞄准的目标地址  */
	void aimTo(cocos2d::Vec2 target);

	/* 回收子弹  */
	void recycleOneBullet(cocos2d::Sprite * bullet);

	/* 获取子弹  */
	cocos2d::Sprite * getOneBullet();

	/* 开始向目标发射一颗子弹 PS： 由相应的层(Layer)调用 schedule 来定时射击 */
	void shootHandle(cocos2d::Layer * layer);
	/* 判断是否出界的扫描函数 PS： 由 相应的层(Layer)调用 schedule 来定时扫描使用 */
	void scanHandle(cocos2d::Layer * layer);

	/* 视图空间内是否仍然有子弹 */
	bool isExistBullet()
	{
		return m_aShootedBullet->count() ? true : false;
	}

	float m_BulletInterval;					//	子弹发射间隔时间
	cocos2d::CCArray * m_aShootedBullet;	//  子弹 缓冲池

private:

	BULLETKIND m_BulletKind;				//  子弹的种类
	float	m_speed;						//	子弹速度
	cocos2d::Vec2 m_Target;					//  子弹所飞向的目标
	cocos2d::Vec2 m_Position;				//	武器所有者的位置
	cocos2d::CCArray  * m_aBullet;			//  子弹 缓冲池
};

#endif