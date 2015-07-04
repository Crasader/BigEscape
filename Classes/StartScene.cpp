/*
	���� �� ��ʼ�����ķ���ʵ��
*/
#include "cocos2d.h"
#include "startScene.h"
#include "musicControl.h"
#include "StartBackgroundLayer.h"
#include "StartMenuLayer.h"
USING_NS_CC;

// ��ʼ����ʼ����������
bool startScene::init(void)
{
	if (Scene::init())
	{
		/* ������ */
		m_backgroundLayer = startBackgroundLayer::create();
		addChild(m_backgroundLayer);

		/* �˵��� */
		m_menuLayer = startMenuLayer::create();
		addChild(m_menuLayer);

		return true;
	}
	else
		return false;
}

void startScene::onEnterTransitionDidFinish()
{
	resumeSchedulerAndActions();
	musicControl::getInstance()->resumePlay(musicControl::getInstance()->getstartBgmID());
}
void startScene::onExitTransitionDidStart()
{
	pauseSchedulerAndActions();
	musicControl::getInstance()->pausePlay(musicControl::getInstance()->getstartBgmID());
}

