#pragma once
#include"../library/t2klib.h"
#include"../support/Support.h"
#include<string>

class Camera;

class Animation {
public:
	Animation(std::string Gh,t2k::Vector3 Pos,int ActSpeed,int MaxIndex);
	~Animation();

	void Update();
	void Draw();

	inline bool GetIsAlive() {
		return isAlive;
	}

private:

	Camera* camera = nullptr;

	//std::vector<int>gh;
	//エフェクトの元画像
	int gh[5];
	//描画座標
	t2k::Vector3 pos;
	//コマ更新フレーム数
	int actSpeed = 0;
	//コマ最大数
	int maxMotionIndex = 0;
	//現在のフレームカウント
	int actWait = actSpeed;
	//選択中の画像インデックス
	int actIndex = 0;
	//描画する画像
	int drawGh = 0;


	bool isAlive = true;
};