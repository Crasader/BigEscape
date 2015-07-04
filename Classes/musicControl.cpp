/*
	精简 音效引擎
*/
#include "audio/include/AudioEngine.h"
#include "cocos2d.h"
#include "musicControl.h"

USING_NS_CC;
using namespace cocos2d::experimental;

musicControl * musicControl::m_sMusicControl = nullptr;	// 自定义音效控件

/* 播放开始页面背景音效 */
void musicControl::playStartBackground(const std::string name)
{
	if (m_bMusicBGM)
		m_StartBgmID = AudioEngine::play2d(name, true);
}

/* 播放游戏页背景音效*/
void musicControl::playGameBackground(const std::string name)
{
	if (m_bMusicBGM)
		m_GameBgmID = AudioEngine::play2d(name, true);
}

/* 播放效果音效 */
void musicControl::playEffect(const std::string name)
{
	if (m_bMusicEffect)
		AudioEngine::play2d(name);
}

/*	返回自定义音效模式  */
musicControl * musicControl::getInstance()
{
	static bool isInit = false;
	if (false == isInit)
	{
		auto control = new musicControl;
		m_sMusicControl = control;
		control->setSwitchBGM(true);
		control->setSwitchEffect(true);
		isInit = true;
		return control;
	}
	else
		return m_sMusicControl;
}

/*  停止播放  */
void musicControl::stopPlay(int musicId)
{
	AudioEngine::stop(musicId);
}

/* 停止播放 */
void musicControl::pausePlay(int musicId)
{
	AudioEngine::pause(musicId);
}

/* 恢复播放 */
void musicControl::resumePlay(int musicId)
{
	AudioEngine::resume(musicId);
}

/* 获取开始背景音效 */
int musicControl::getstartBgmID() const
{
	return m_StartBgmID;
}

/* 获取游戏背景音效ID */
int musicControl::getgameBgmID() const
{
	return m_GameBgmID;
}

void  musicControl::setSwitchEffect(bool sw)
{
	m_bMusicEffect = sw;
}

void  musicControl::setSwitchBGM(bool sw)
{
	if (sw)
	{
		resumePlay(m_StartBgmID);
		m_bMusicBGM = true;
	}
	else
	{
		pausePlay(m_StartBgmID);
		m_bMusicBGM = false;
	}
}
