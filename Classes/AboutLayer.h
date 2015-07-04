#ifndef _ABOUTLAYER_H_
#define _ABOUTLAYER_H_

#include "cocos2d.h"

class aboutLayer : public cocos2d::Layer
{
public:
	CREATE_FUNC(aboutLayer);
	virtual bool init() override;

	void resumeCallback(void * sender);
};

#endif