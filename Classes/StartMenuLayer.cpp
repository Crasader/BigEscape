#include "cocos2d.h"

#include "StartMenuLayer.h"
#include "GameScene.h"
#include "OptionLayer.h"
#include "AboutLayer.h"
#include "musicControl.h"

USING_NS_CC;

bool startMenuLayer::init(void)
{
	if (Layer::init())
	{
		auto origin = Director::sharedDirector()->getVisibleOrigin();
		auto visibleSize = Director::sharedDirector()->getVisibleSize();
		auto menu_cache = SpriteFrameCache::getInstance();
		menu_cache->addSpriteFramesWithFile("Plist/menu/menu.plist");

		auto sprite_newgame_o = Sprite::createWithSpriteFrameName("newgame_o.png");
		auto sprite_newgame_r = Sprite::createWithSpriteFrameName("newgame_r.png");
		auto item_newgame = MenuItemSprite::create(sprite_newgame_r, sprite_newgame_o, CC_CALLBACK_1(startMenuLayer::newgame_callback, this));

		auto sprite_about_o = Sprite::createWithSpriteFrameName("about_o.png");
		auto sprite_about_r = Sprite::createWithSpriteFrameName("about_r.png");
		auto item_about = MenuItemSprite::create(sprite_about_r, sprite_about_o, CC_CALLBACK_1(startMenuLayer::about_callback, this));

		auto sprite_option_o = Sprite::createWithSpriteFrameName("option_o.png");
		auto sprite_option_r = Sprite::createWithSpriteFrameName("option_r.png");
		auto item_option = MenuItemSprite::create(sprite_option_r, sprite_option_o, CC_CALLBACK_1(startMenuLayer::option_callback, this));

		auto mu = Menu::create(item_newgame, item_option, item_about, nullptr);
		mu->alignItemsVerticallyWithPadding(30.0);
		mu->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 50));

		addChild(mu);

		return true;
	}
	else
		return false;
}

void startMenuLayer::option_callback(void *sender)
{
	auto optlayer = optionLayer::create();
	getParent()->pauseSchedulerAndActions();
	getParent()->addChild(optlayer);
}
void startMenuLayer::about_callback(void *sender)
{
	auto aboutlayer = aboutLayer::create();
	getParent()->pauseSchedulerAndActions();
	getParent()->addChild(aboutlayer);
}
void startMenuLayer::newgame_callback(void *sender)
{
	musicControl::getInstance()->playEffect("Music/buttonEffet.mp3");
	auto scene = gameScene::create();
	Director::sharedDirector()->pushScene(CCTransitionFade::create(1, scene));
}