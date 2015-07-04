#ifndef _STATUSLAYER_H_
#define _STATUSLAYER_H_

#include "cocos2d.h"

/*
	游戏状态层
*/
class statusLayer : public cocos2d::Layer
{
public:
	CREATE_FUNC(statusLayer);
	virtual bool init() override;
	
	/* 显示分数 */
	void setScore(int score);

	void menuCallback(void * sender);
private:
	cocos2d::Sprite * m_gameOver;
	cocos2d::LabelTTF * m_label;
};

#endif