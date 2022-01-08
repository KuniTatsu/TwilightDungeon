#include "SoundManager.h"
#include"DxLib.h"

Sound::Sound()
{
	bgm_title = LoadSoundMem("sound/title_bgm.mp3");
	bgm_training = LoadSoundMem("sound/localmap_bgm.mp3");

	system_select = LoadSoundMem("sound/system_select.mp3");
	system_move = LoadSoundMem("sound/system_move.mp3");
	system_cancel = LoadSoundMem("sound/system_cancel.mp3");
	system_statusUp = LoadSoundMem("sound/system_statusUp.mp3");
}

Sound::~Sound()
{
}

void Sound::BGM_Play(int bgm)
{
	PlaySoundMem(bgm, DX_PLAYTYPE_LOOP);
}

void Sound::System_Play(int sound)
{
	PlaySoundMem(sound, DX_PLAYTYPE_BACK);
}
