/*
	描述 ： 开始场景的方法实现
*/
#include "cocos2d.h"

#include "startScene.h"
#include "StartMenuLayer.h"
#include "musicControl.h"

USING_NS_CC;

// 初始化开始场景背景层
bool startBackgroundLayer::init(void)
{
	if (Layer::init())
	{
		/*	获取整体界面大小及原点位置  */
		auto visablesize = Director::sharedDirector()->getVisibleSize();
		auto origin = Director::sharedDirector()->getVisibleOrigin();

		/*	背景图片	 */
		auto bgsprite = Sprite::create("Image/loading.png");
		bgsprite->setPosition(Vec2(visablesize.width / 2, visablesize.height / 2));
		addChild(bgsprite);

		/*	闪烁的星星	*/
		createStar(Vec2(150, 380), 0.3f, 4.0f, 2.0f,"Image/6.png");
		createStar(Vec2(0, 240), 0.1f, 2.0f, 2.0f, "Image/13.png");
		createStar(Vec2(200, 150), 0.1f, 1.0f, 0.8f, "Image/17.png");
		createStar(Vec2(310, 10), 0.2f, 1.5f, 1.0f, "Image/12.png");
		createStar(Vec2(100, 100), 0.1f, 0.5f, 0.5f, "Image/3.png");

		/*	开始界面上的LOGO	*/
		auto logosprite = Sprite::create("Image/logo.png");
		logosprite->setPosition(Vec2(origin.x+visablesize.width/2, origin.y+visablesize.height/2+100));
		logosprite->setAnchorPoint(Vec2(0.5, 0.5));
		logosprite->setScale(0.8f);
		addChild(logosprite);
		auto logoaction = ScaleBy::create(3.0f, 0.9f);
		
		logosprite->runAction(RepeatForever::create(Sequence::createWithTwoActions(logoaction, logoaction->reverse())));


		/*	背景中飞过的飞船 */
		SpriteFrameCache * cache = SpriteFrameCache::getInstance();
		cache->addSpriteFramesWithFile("Plist/ship/ship.plist");
		auto shipSprite = Sprite::createWithSpriteFrameName("ship03.png");
		shipSprite->setPosition(Vec2(origin.x - 5, 0));
		shipSprite->setRotation(30.0f);
		auto actionmt = MoveTo::create(20, Vec2(270.0f, 500.0f));
		auto action = MoveTo::create(0, Vec2(origin.x - 40.0f, 0.0f));
		shipSprite->runAction(RepeatForever::create(Sequence::create(actionmt, action, nullptr)));
		addChild(shipSprite);
		
		/*	播放背景音乐  */
		musicControl::getInstance()->playStartBackground("Music/mainMainMusic.mp3");

		return true;
	}
	else
		return false;
}

void startBackgroundLayer::createStar(Vec2 point, float scale, float timebf, float timeaf, const std::string name)
{
	//some decorator on background layer
	auto star1 = Sprite::create(name);
	star1->setScale(scale);
	star1->setPosition(point);
	star1->setAnchorPoint(Vec2(0.5, 0.5));
	auto actionIn = FadeOut::create(timeaf);
	auto actionOut = FadeIn::create(timebf);
	srand(time(nullptr));
	auto delay1 = DelayTime::create(CCRANDOM_0_1()*3);
	auto delay2 = DelayTime::create(CCRANDOM_0_1() * 2);
	auto star1Action = Sequence::create(delay1,actionIn, actionOut,delay2, nullptr);

	star1->runAction(RepeatForever::create(star1Action));
	addChild(star1);
}
