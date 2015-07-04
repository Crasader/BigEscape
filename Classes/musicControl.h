#ifndef __MUSICCONTROL_H_
#define __MUSICCONTROL_H_

#include "cocos2d.h"

/*
	����ģʽ�µ����ֲ�����
*/
class musicControl {
public:
	static musicControl * getInstance(void);

	/* ���ſ�ʼ�������� */
	void playStartBackground(std::string name);
	
	/* ������Ϸ�������� */
	void playGameBackground(std::string name);

	/* ������Ч���� */
	void playEffect(std::string name);

	/* ֹͣ�������� */
	void stopPlay(int musicId);

	/* ��ͣͣ�������� */
	void pausePlay(int musicId);

	/* �ָ��������� */
	void resumePlay(int musicId);

	void setSwitchEffect(bool sw);
	void setSwitchBGM(bool sw);
	/* �������ֵ�ID��, ������ͣ �ָ� ����Ч�� */
	CC_PROPERTY_READONLY(int, m_GameBgmID, gameBgmID);

	CC_PROPERTY_READONLY(int, m_StartBgmID, startBgmID);
private:
	
	bool m_bMusicEffect;		// ��Ч ����
	bool m_bMusicBGM;			// BGM ����
	/* �Զ�����Ч�ؼ� */
	static musicControl * m_sMusicControl;
};

#endif