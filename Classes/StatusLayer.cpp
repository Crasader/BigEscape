#include "cocos2d.h"
#include "StatusLayer.h"
#include "musicControl.h"

USING_NS_CC;

bool statusLayer::init()
{
	if (Layer::init())
	{
		auto visibale = Director::sharedDirector()->getVisibleSize();

		auto bgSprite = Sprite::create("Image/background.png");
		bgSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
		bgSprite->setPosition(visibale.width / 2, visibale.height / 2);
		addChild(bgSprite);

		auto m_gameOver = Sprite::create("Image/gameOver.png");
		m_gameOver->setAnchorPoint(Vec2(0.5f, 0.5f));
		m_gameOver->setPosition(visibale.width / 2, visibale.height * 4 / 5);

		auto action1 = RotateBy::create(0.2f, 15);
		auto action2 = RotateBy::create(0.2f, -15);
		auto seqact = Sequence::create(action1, action1->reverse(), action2, action2->reverse(), nullptr);
		auto jump = JumpBy::create(2, Vec2::ZERO, 30, 5);
		auto spwact = Spawn::createWithTwoActions(jump, seqact);
		m_gameOver->runAction(RepeatForever::create(spwact));

		addChild(m_gameOver);									// 振动的gameover

		auto score = LabelTTF::create("score", "Felt.ttf", 60);
		score->setColor(Color3B(200, 38, 12));
		score->setAnchorPoint(Vec2(0.5f, 0.5f));
		score->setPosition(visibale.width / 2, visibale.height * 4 / 7);
		addChild(score);										// 分数

		m_label = LabelTTF::create("", "Felt.ttf", 60);
		m_label->setColor(Color3B(200, 38, 12));
		m_label->setAnchorPoint(Vec2(0.5f, 0.5f));
		m_label->setPosition(visibale.width / 2, visibale.height * 3 / 7);
		addChild(m_label);										// 分数

		auto optMainMenu = MenuItemLabel::create(LabelTTF::create("Main menu", "Felt.ttf", 48), CC_CALLBACK_1(statusLayer::menuCallback, this));
		optMainMenu->setColor(Color3B(200, 38, 12));
		auto mu1 = Menu::create(optMainMenu, nullptr);
		mu1->alignItemsVerticallyWithPadding(40.0f);
		mu1->setPosition(visibale.width / 2, visibale.height * 1 / 5);

		addChild(mu1);

		return true;
	}
	return false;
}

void statusLayer::setScore(int score)
{
	char buff[10];
	sprintf(buff, "%d", score);
	m_label->setString(buff);
}

void statusLayer::menuCallback(void * sender)
{
	musicControl::getInstance()->stopPlay(musicControl::getInstance()->getgameBgmID());
	auto callback = [](Scene * scene){ CCTransitionFade::create(1, scene); };
	Director::getInstance()->popScene(callback);
}