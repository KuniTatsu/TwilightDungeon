///*****Description*****
///スキルに関するクラス
///プレイヤーとエネミー双方が持つことが出来るスキルの情報,処理を保持する
///*********************
#pragma once
#include<string>
#include<vector>
class Skill
{
public:
	Skill(int SkillId, int SkillType,std::string SkillName, float DamageRate, float HealRate, std::string GhPass, int AllNum
		, int XNum, int YNum, int XSize, int YSize,int ActSpeed, std::string IconGhPass);
	~Skill();

	//ダメージと回復量のデータを取得する
	inline const float* GetSkillAmount() {
		return floatData;
	}
	//スキルの名前を取得する
	inline const std::string& GetSkillName() {
		return name;
	}
	//アニメーション画像総枚数を取得する
	inline const int& GetGraphicAllNum() {
		return graphicAllNum;
	}
	//アニメーションタイマー秒数を取得する
	inline const int& GetActSpeed() {
		return actSpeed;
	}
	//アニメーション画像のハンドルVectorを取得する
	inline const std::vector<int>&GetGraphicHandle() {
		return animGh;
	}
	//スキルアイコン画像のGhを取得する
	inline const int& GetSkillIconGh() {
		return iconGh;
	}

private:
	//スキルId
	int skillId = 0;
	//スキルタイプ
	int skillType = 0;
	//スキルの名前
	std::string name = "";

	//攻撃力にかかる倍率
	float damageRate = 0;
	//ダメージにかかる倍率
	float healRate = 0;
	//上２つを纏めた配列
	float floatData[2];

	//フォルダパス
	std::string ghPass = "";
	//DivGraphで分割した画像ハンドル
	std::vector<int>animGh;

	//iconフォルダパス
	std::string iconPass="";
	//iconGh
	int iconGh = 0;

	//画像の情報
	int graphicAllNum = 0;
	int xNum = 0;
	int yNum = 0;

	int xSize = 0;
	int ySize = 0;

	int actSpeed = 0;
};

