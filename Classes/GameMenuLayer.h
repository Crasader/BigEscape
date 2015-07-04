#ifndef __GAMEMENULAYER_H_
#define __GAMEMENULAYER_H_

#include "cocos2d.h"

class gameMenuLayer : public cocos2d::Layer
{
public:
	CREATE_FUNC(gameMenuLayer);

	virtual bool init(void) override;

	/* 恢复游戏回调函数 */
	void toResumeCallback(void * sender);

	/* 回到主菜单回调函数 */
	void toStartCallback(void * sender);

};

#endif