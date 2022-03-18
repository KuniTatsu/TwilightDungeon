///*****Description*****
///音声に関するクラス
///各種音声データと再生関数を持つ
///*********************
#pragma once


class Sound {


public:
	int bgm_town = 0;
	int bgm_tower = 0;
	int bgm_forest = 0;

	int bgm_title = 0;

	int system_select = 0;
	int system_move = 0;
	int system_cancel = 0;
	int system_attack = 0;

	int system_statusUp = 0;



	Sound();
	~Sound();



	void BGM_Play(int bgm);
	void System_Play(int sound);



};
