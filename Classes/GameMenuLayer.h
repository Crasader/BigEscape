#ifndef __GAMEMENULAYER_H_
#define __GAMEMENULAYER_H_

#include "cocos2d.h"

class gameMenuLayer : public cocos2d::Layer
{
public:
	CREATE_FUNC(gameMenuLayer);

	virtual bool init(void) override;

	/* �ָ���Ϸ�ص����� */
	void toResumeCallback(void * sender);

	/* �ص����˵��ص����� */
	void toStartCallback(void * sender);

};

#endif