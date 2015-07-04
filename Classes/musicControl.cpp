/*
	���� ��Ч����
*/
#include "audio/include/AudioEngine.h"
#include "cocos2d.h"
#include "musicControl.h"

USING_NS_CC;
using namespace cocos2d::experimental;

musicControl * musicControl::m_sMusicControl = nullptr;	// �Զ�����Ч�ؼ�

/* ���ſ�ʼҳ�汳����Ч */
void musicControl::playStartBackground(const std::string name)
{
	if (m_bMusicBGM)
		m_StartBgmID = AudioEngine::play2d(name, true);
}

/* ������Ϸҳ������Ч*/
void musicControl::playGameBackground(const std::string name)
{
	if (m_bMusicBGM)
		m_GameBgmID = AudioEngine::play2d(name, true);
}

/* ����Ч����Ч */
void musicControl::playEffect(const std::string name)
{
	if (m_bMusicEffect)
		AudioEngine::play2d(name);
}

/*	�����Զ�����Чģʽ  */
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

/*  ֹͣ����  */
void musicControl::stopPlay(int musicId)
{
	AudioEngine::stop(musicId);
}

/* ֹͣ���� */
void musicControl::pausePlay(int musicId)
{
	AudioEngine::pause(musicId);
}

/* �ָ����� */
void musicControl::resumePlay(int musicId)
{
	AudioEngine::resume(musicId);
}

/* ��ȡ��ʼ������Ч */
int musicControl::getstartBgmID() const
{
	return m_StartBgmID;
}

/* ��ȡ��Ϸ������ЧID */
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
