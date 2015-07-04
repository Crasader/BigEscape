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
	
	/* ������ʼ���� */
	virtual bool init(void) override;

	/* ������ʼ�� */
	void keyboardInit();

	/*  ��ͣ��Ϸ  */
	void gamePause();
	
	/*  �ָ���Ϸ  */
	void gameResume();

	/* ������Ϸ���� */
	void setScore(int score);

	/*  �ͷŻص�����  */
	void KeyreleaseCallback(cocos2d::EventKeyboard::KeyCode keyVal, cocos2d::Event* event);

private:
	bool m_isPause;								// �Ƿ��Ѿ���ͣ
	enemyLayer * m_enemyPlayer ;				// ���˲�
	playerLayer * m_playLayer ;					// ��Ҳ�
	statusLayer	* m_statusLayer;				// �����
	gameMenuLayer * m_menuLayer;				// �˵���
	gameBackgroundLayer * m_backgroundLayer;	// ������
};

#endif