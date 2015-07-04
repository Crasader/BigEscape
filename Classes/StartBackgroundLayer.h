#ifndef __STARTBACKGROUNDLAYER_H_
#define __STARTBACKGROUNDLAYER_H_

#include "cocos2d.h"

class startBackgroundLayer : public cocos2d::Layer
{
public:
	CREATE_FUNC(startBackgroundLayer);

	virtual bool init(void) override;

	/*   在登录背景创建闪烁的星光  */
	void createStar(cocos2d::Vec2 point, float scale, float timesbf,float bf,const std::string name);

};

#endif
