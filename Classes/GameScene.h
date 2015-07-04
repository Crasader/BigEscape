#ifndef __GAMESCENE_H_
#define __GAMESCENE_H_

#include "cocos2d.h"

#include "EnemyLayer.h"
#include "PlayerLayer.h"
#include "GameMenuLayer.h"
#include "GameBackgroundLayer.h"
#include "StatusLayer.h"

class gameScene : public cocos2d::Scene
{
public:
	~gameScene();

	CREATE_FUNC(gameScene);
	
	/* 场景初始化层 */
	virtual bool init(void) override;

	/* 按键初始化 */
	void keyboardInit();

	/*  暂停游戏  */
	void gamePause();
	
	/*  恢复游戏  */
	void gameResume();

	/* 设置游戏分数 */
	void setScore(int score);

	/*  释放回调函数  */
	void KeyreleaseCallback(cocos2d::EventKeyboard::KeyCode keyVal, cocos2d::Event* event);

private:
	bool m_isPause;								// 是否已经暂停
	enemyLayer * m_enemyPlayer ;				// 敌人层
	playerLayer * m_playLayer ;					// 玩家层
	statusLayer	* m_statusLayer;				// 结果层
	gameMenuLayer * m_menuLayer;				// 菜单层
	gameBackgroundLayer * m_backgroundLayer;	// 背景层
};

#endif