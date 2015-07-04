#ifndef __MUSICCONTROL_H_
#define __MUSICCONTROL_H_

#include "cocos2d.h"

/*
	单独模式下的音乐播放器
*/
class musicControl {
public:
	static musicControl * getInstance(void);

	/* 播放开始背景音乐 */
	void playStartBackground(std::string name);
	
	/* 播放游戏背景音乐 */
	void playGameBackground(std::string name);

	/* 播放音效音乐 */
	void playEffect(std::string name);

	/* 停止播放音乐 */
	void stopPlay(int musicId);

	/* 暂停停播放音乐 */
	void pausePlay(int musicId);

	/* 恢复播放音乐 */
	void resumePlay(int musicId);

	void setSwitchEffect(bool sw);
	void setSwitchBGM(bool sw);
	/* 背景音乐的ID号, 用于暂停 恢复 播放效果 */
	CC_PROPERTY_READONLY(int, m_GameBgmID, gameBgmID);

	CC_PROPERTY_READONLY(int, m_StartBgmID, startBgmID);
private:
	
	bool m_bMusicEffect;		// 音效 开关
	bool m_bMusicBGM;			// BGM 开关
	/* 自定义音效控件 */
	static musicControl * m_sMusicControl;
};

#endif