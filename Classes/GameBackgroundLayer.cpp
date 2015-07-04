#include "cocos2d.h"

#include "gameBackgroundLayer.h"
#include "musicControl.h"
#include "GameMenuLayer.h"

USING_NS_CC;

bool gameBackgroundLayer::init(void)
{
	if (Layer::init())
	{
		auto visableSize = Director::sharedDirector()->getVisibleSize();
		auto origin = Director::sharedDirector()->getVisibleOrigin();

		auto sprite1 = Sprite::create("Image/background2.png");
		sprite1->setPosition(visableSize.width, 0);
		sprite1->setAnchorPoint(Vec2(1, 0));

		auto sprite2 = Sprite::create("Image/background1.png");
		sprite2->setPosition(visableSize.width, sprite1->getContentSize().height);
		sprite2->setAnchorPoint(Vec2(1, 0));

		auto action1 = MoveBy::create(20, Vec2(0, 0-sprite1->getContentSize().height));
		auto action2 = MoveTo::create(0, Vec2(sprite1->getContentSize().width, sprite1->getContentSize().height));

		sprite1->runAction(RepeatForever::create(Sequence::create(action1, action2, action1->clone(), nullptr)));
		sprite2->runAction(RepeatForever::create(Sequence::create(action1->clone(), action1->clone(), action2->clone(), nullptr)));

		addChild(sprite1);
		addChild(sprite2);
		
		musicControl::getInstance()->playGameBackground("Music/bgMusic.mp3");

		return true;
	}
	else
		return false;
}