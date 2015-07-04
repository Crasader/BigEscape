#include "cocos2d.h"
#include "OptionLayer.h"
#include "musicControl.h"

USING_NS_CC;

bool optionLayer::m_bbgm = true;
bool optionLayer::m_beffect = true;

bool optionLayer::init()
{
	if (Layer::init())
	{
		auto visiable = Director::sharedDirector()->getVisibleSize();
		auto sprite = Sprite::create("Image/musicOpt.png");
		sprite->setPosition(visiable.width / 2, visiable.height / 2);
		sprite->setAnchorPoint(Vec2(0.5f, 0.5f));
		addChild(sprite);
		
		auto on = MenuItemFont::create("On");			// 这句话是多余的 ， 菜单选项的BUG, 第一个不能正常设置字体
		on->setFontName("Felt.ttf");
		on->setFontSize(40);

		auto on1 = MenuItemFont::create("On");
		on1->setFontName("Felt.ttf");
		on1->setColor(Color3B(200, 38, 12));
		on1->setFontSize(40);

		auto off1 = MenuItemFont::create("Off");
		off1->setFontName("Felt.ttf");
		off1->setColor(Color3B(200, 38, 12));
		off1->setFontSize(40);

		auto on2 = MenuItemFont::create("On");
		on2->setFontName("Felt.ttf");
		on2->setColor(Color3B(200, 38, 12));
		on2->setFontSize(40);

		auto off2 = MenuItemFont::create("Off");
		off2->setFontName("Felt.ttf");
		off2->setColor(Color3B(200, 38, 12));
		off2->setFontSize(40);

		auto bgmSw = MenuItemToggle::createWithCallback(CC_CALLBACK_1(optionLayer::bgmCallback, this),
														on1,
														off1,
														nullptr);
		bgmSw->setPosition(70, 30);
		auto effectSw = MenuItemToggle::createWithCallback(CC_CALLBACK_1(optionLayer::effectCallback, this),
														on2,
														off2,
														nullptr);
		effectSw->setPosition(70, -30);

		auto optResum = MenuItemLabel::create(LabelTTF::create(" Resume ", "Felt.ttf", 40), CC_CALLBACK_1(optionLayer::resumeCallback, this));
		optResum->setColor(Color3B(200, 38, 12));
		optResum->setPosition(0, -80);

		auto menu = Menu::create(bgmSw, effectSw, optResum,nullptr);
		
		addChild(menu);

		m_bbgm ? bgmSw->setSelectedIndex(0) : bgmSw->setSelectedIndex(1);
		m_beffect ? effectSw->setSelectedIndex(0) : effectSw->setSelectedIndex(1);

		return true;
	}
	return false;
}

void optionLayer::bgmCallback(void * sender)
{
	m_bbgm = !m_bbgm;
	musicControl::getInstance()->setSwitchBGM(m_bbgm);
}

void optionLayer::effectCallback(void * sender)
{
	m_beffect = !m_beffect;
	musicControl::getInstance()->setSwitchEffect(m_beffect);
}

void optionLayer::resumeCallback(void * sender)
{
	getParent()->resumeSchedulerAndActions();
	getParent()->removeChild(this);
}