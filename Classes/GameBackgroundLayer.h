#ifndef __GAMEBACKGROUNDLAYER_H_
#define __GAMEBACKGROUNDLAYER_H_

#include "cocos2d.h"

#include "GameMenuLayer.h"

class gameBackgroundLayer : public cocos2d::Layer
{
public:

	CREATE_FUNC(gameBackgroundLayer);
	
	virtual bool init(void) override;
};

#endif
