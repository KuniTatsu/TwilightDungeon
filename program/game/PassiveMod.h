//装備やプレイヤー,敵に付与されるパッシブ効果のクラス
//装備は最大5個,プレイヤーと敵は最大数なし,効果消滅までのターンあり
//倍率値:攻撃力に乗算される,加算値:攻撃力に加算される

#pragma once
#include<string>

class PassiveMod
{
public:
	PassiveMod(int ModId, int ModType, std::string ModName,float AttackRate,float DefRate,float SpeedRate,float AddAttack,float AddDef,float AddSpeed,
		float ExtraDamage,float ExtraHp,int AddGold,int AddExp);
	~PassiveMod();

private:
	//ModId
	int modId = 0;
	//Modタイプ
	int modType = 0;
	//Modの名前
	std::string name = "";

	//攻撃倍率値
	float attackRate = 0;
	//攻撃加算値
	float addAttack = 0;

	//防御倍率値
	float defRate = 0;
	//防御加算値
	float addDef = 0;

	//速度倍率値
	float speedRate = 0;
	//速度加算値
	float addSpeed = 0;

	//追加ダメージ倍率値
	float extraDamageRate = 0;
	//最大Hp追加倍率値
	float extraHpRate = 0;

	//獲得Gold加算
	int addGold = 0;
	//獲得経験値加算
	int addExp = 0;




};
