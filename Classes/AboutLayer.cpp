#include "cocos2d.h"
#include "AboutLayer.h"

USING_NS_CC;

bool aboutLayer::init()
{
	if (Layer::init())
	{
		auto visiableSize = Director::sharedDirector()->getVisibleSize();

		auto bgSprite = Sprite::create("Image/background.png");
		bgSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
		bgSprite->setPosition(visiableSize.width / 2, visiableSize.height / 2);
		addChild(bgSprite);

		auto header = Sprite::create("Image/header.jpg");
		header->setScale(0.5f);
		header->setAnchorPoint(Vec2(0.5f, 0.5f));
		header->setPosition(visiableSize.width / 2, visiableSize.height * 4 / 5);
		addChild(header);

		auto author = LabelTTF::create("Author: Castro Liu","Felt.ttf", 20);
		author->setPosition(visiableSize.width / 2, visiableSize.height * 2 / 5+30);
		author->setColor(Color3B(255, 0, 0));
		addChild(author);
		auto email = LabelTTF::create("E-mail: liuhu0821@gmail.com", "Felt.ttf", 20);
		email->setPosition(visiableSize.width / 2, visiableSize.height * 1 / 5+65);
		email->setColor(Color3B(255, 0, 0));
		addChild(email);

		auto optResum = MenuItemLabel::create(LabelTTF::create(" Resume ", "Felt.ttf", 40), CC_CALLBACK_1(aboutLayer::resumeCallback, this));
		optResum->setColor(Color3B(200, 38, 12));
		auto menu = Menu::create(optResum, nullptr);
		menu->setPosition(visiableSize.width/2, visiableSize.height/5);
		addChild(menu);

		return true;
	}
	else
		return false;
}

void aboutLayer::resumeCallback(void *sender)
{
	getParent()->resumeSchedulerAndActions();
	getParent()->removeChild(this);
}