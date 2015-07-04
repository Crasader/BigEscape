#ifndef __STARTSCENE_H_
#define __STARTSCENE_H_

#include "cocos2d.h"
#include "StartBackgroundLayer.h"
#include "StartMenuLayer.h"

class startScene : public cocos2d::Scene
{
public:
	CREATE_FUNC(startScene);

	virtual bool init(void) override;

	virtual void onEnterTransitionDidFinish() override;
	virtual void onExitTransitionDidStart() override;

protected:
	startBackgroundLayer * m_backgroundLayer;
	startMenuLayer * m_menuLayer;
};

#endif