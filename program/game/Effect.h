#pragma once
#pragma once


class BattleEffect {


public:
	BattleEffect();
	~BattleEffect();

	/*int sword_effect_gh[9] = { 0 };
	int sword_after_effect_gh[8] = { 0 };

	int bow_effect_gh[15] = { 0 };
	int bow_after_effect_gh[8] = { 0 };

	int spear_effect_gh[7] = { 0 };
	int spear_after_effect_gh[16] = { 0 };

	int battle_Draw_effect[10] = { 0 };
	int battle_Draw_effect_after[8] = { 0 };*/

	int* sword_effect_gh = nullptr;
	int* sword_after_effect_gh = nullptr;

	int* bow_effect_gh = nullptr;
	int* bow_after_effect_gh = nullptr;

	int* spear_effect_gh = nullptr;
	int* spear_after_effect_gh = nullptr;

	int* battle_Draw_effect = nullptr;
	int* battle_Draw_effect_after = nullptr;

	float time_count = 0;
	int anim_frame = 0;
	int repeatCount = 0;

	bool done_EffectFlag = false;

	//DrawRotaGraphの中心x,y,拡大率,画像,最大枚数,アニメーション速度 gh=sword_effect_gh,sword_after_effect_gh,bow_effect_gh,bow_after_effect_gh,spear_effect_gh,spear_after_effect_gh
	void EffectDraw(int x, int y, double ExRate, int gh[], int max_num, float count_speed);


};