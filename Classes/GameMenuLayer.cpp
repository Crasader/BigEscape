#include "cocos2d.h"
#include "GameMenuLayer.h"
#include "musicControl.h"
#include "GameScene.h"
#include "StartScene.h"

USING_NS_CC;

bool gameMenuLayer::init(void)
{
	if (Layer::init())
	{
		auto visableSize = Director::sharedDirector()->getVisibleSize();
		auto origin = Director::sharedDirector()->getVisibleOrigin();

		auto bgSprite = Sprite::create("Image/background.png");
		bgSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
		bgSprite->setPosition(visableSize.width / 2, visableSize.height / 2);
		addChild(bgSprite);

		auto optMainMenu = MenuItemLabel::create(LabelTTF::create("Main menu", "Felt.ttf", 48), CC_CALLBACK_1(gameMenuLayer::toStartCallback, this));
		auto optResum = MenuItemLabel::create(LabelTTF::create(" Resume ", "Felt.ttf", 48), CC_CALLBACK_1(gameMenuLayer::toResumeCallback, this));
		
		auto mu1 = Menu::create( optMainMenu, optResum, nullptr);
		mu1->alignItemsVerticallyWithPadding(40.0f);
		mu1->setPosition(origin.x + visableSize.width / 2, origin.y + visableSize.height / 2);
		
		addChild(mu1);
		
		return true;
	}
	else
		return false;
}

void gameMenuLayer::toResumeCallback(void * sender)
{
	static_cast<gameScene *>(this->getParent())->gameResume();
}

void gameMenuLayer::toStartCallback(void * sender)
{
	musicControl::getInstance()->stopPlay(musicControl::getInstance()->getgameBgmID());
	auto callback = [](Scene * scene){ CCTransitionFade::create(1, scene); };
	Director::getInstance()->popScene(callback);
}