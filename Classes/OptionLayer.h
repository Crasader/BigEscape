#ifndef _OPTIONLAYER_H_
#define _OPTIONLAYER_H_

#include "cocos2d.h"

class optionLayer : public cocos2d::Layer
{
public:
	CREATE_FUNC(optionLayer);
	virtual bool init() override;

	void bgmCallback(void * sender);
	void effectCallback(void * sender);
	void resumeCallback(void * sender);
private:
	static bool m_bbgm;
	static bool m_beffect;

};

#endif