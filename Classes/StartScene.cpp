/*
	描述 ： 开始场景的方法实现
*/
#include "cocos2d.h"
#include "startScene.h"
#include "musicControl.h"
#include "StartBackgroundLayer.h"
#include "StartMenuLayer.h"
USING_NS_CC;

// 初始化开始场景背景层
bool startScene::init(void)
{
	if (Scene::init())
	{
		/* 背景层 */
		m_backgroundLayer = startBackgroundLayer::create();
		addChild(m_backgroundLayer);

		/* 菜单层 */
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

