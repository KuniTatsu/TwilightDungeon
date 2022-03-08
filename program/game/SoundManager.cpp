#include "SoundManager.h"
#include"DxLib.h"

Sound::Sound()
{
	bgm_town = LoadSoundMem("sound/bgm_town.mp3");
	bgm_tower = LoadSoundMem("sound/bgm_tower.mp3");
	bgm_forest = LoadSoundMem("sound/bgm_forest.mp3");
	bgm_title = LoadSoundMem("sound/bgm_title.mp3");

	system_select = LoadSoundMem("sound/system_select.mp3");
	system_move = LoadSoundMem("sound/system_move.mp3");
	system_cancel = LoadSoundMem("sound/system_cancel.mp3");
	system_attack = LoadSoundMem("sound/system_attack.mp3");

}

Sound::~Sound()
{
}

void Sound::BGM_Play(int bgm)
{
	ChangeVolumeSoundMem(255 * 80 / 100, bgm);
	PlaySoundMem(bgm, DX_PLAYTYPE_LOOP);
}

void Sound::System_Play(int sound)
{
	PlaySoundMem(sound, DX_PLAYTYPE_BACK);
}
