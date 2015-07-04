#ifndef __STARTMENULAYER_H_
#define __STARTMENULAYER_H_

#include "cocos2d.h"

class startMenuLayer : public cocos2d::Layer
{
public :
	CREATE_FUNC(startMenuLayer);
	virtual bool init(void);

	void gameagain_callback(void *sender);
	void option_callback(void *sender);
	void about_callback(void *sender);
	void newgame_callback(void *sender);

private :

};

#endif