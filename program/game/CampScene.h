///*****Description*****
///ダンジョン入場前の全てを行うシーンクラス
///UpdateとDraw関数がSceneManagerで行われる
///UpdateとDraw関数の中ではシークエンスで毎フレーム行われる処理が分岐する
///各シークエンス間はChangeSequence関数によって次フレームから走るシークエンスを移動する
///*********************

#pragma once
#include"Scene.h"
#include "../library/t2klib.h"
#include<memory>
#include<string>

class Player;
class MenuWindow;

class CampScene :public BaseScene {

public:
	CampScene();
	~CampScene();


	void Update();

	void Draw();

	inline int canMoveThisPoint(int x, int y) {
		return collisionData[y][x];
	}

private:
	//黄昏のかけらのグラフィックハンドル
	int fragmentsGh[6];
	//背景
	int fragBackGround = 0;

	std::shared_ptr<Player>player;

	std::vector<std::vector<std::string>>loadGroundMap;
	std::vector<std::vector<std::string>>loadSurfaceMap;
	std::vector<std::vector<std::string>>loadCollision;

	std::vector<std::vector<int>>groundMapData;
	std::vector<std::vector<int>>surfaceMapData;
	std::vector<std::vector<int>>collisionData;

	MenuWindow* dungeonIn = nullptr;

	bool nowFade = false;

	//mapchip_night_20.pngの読み込み 横x縦=30x20
	int campGraphic[600];
	//画像ハンドル取得関数
	int GetGraphicHandle(int num);

	//特定の座標の画像番号を返す関数
	inline int GetGraphicNum(int x, int y) {
		return groundMapData[y][x];
	}
	inline int GetSurfaceGraphicNum(int x, int y) {
		return surfaceMapData[y][x];
	}

	//範囲外エラー防止用関数
	inline bool checkOutOfRange(t2k::Vector3 nowLocalPos) {
		if (nowLocalPos.x < 1)return false;
		else if (nowLocalPos.y < 1)return false;
		else if (nowLocalPos.x > groundMapData[0].size()-1)return false;
		else if (nowLocalPos.y > groundMapData.size()-1)return false;
		else return true;
	}

	void LoadMap(std::string fileName, std::vector<std::vector<std::string>>& stringData, std::vector<std::vector<int>>& intData);
	void DrawMap(std::vector<std::vector<int>>intData);

	const int PORTALPOINTNUM[5] = { 67,187,307,421,427 };

	/*TOWER,
		FOREST,
		WATERWAY,
		DUNGEONMAX*/
	//侵入するか出したメニューのダンジョン
	int selectDungeon = 0;

	const std::string DUNGEONNAME[5] = { "塔のダンジョン","森のダンジョン","灼熱のダンジョン","滝のダンジョン","石のダンジョン" };
	
	t2k::Sequence<CampScene*> mainSequence =
		t2k::Sequence<CampScene*>(this, &CampScene::SeqFadeIn);

	bool SeqMain(const float deltatime);
	bool SeqDungeonInMenu(const float deltatime);
	//フェードインシークエンス
	bool SeqFadeIn(const float deltatime);

	//シークエンスの列挙体
	enum class sequence {
		MAIN,
		DUNGEONIN,
		FADEIN,
	};
	sequence nowSeq = sequence::MAIN;
	//Sequenceを移動させる関数,enumも一緒に変更する
	void ChangeSequence(const sequence seq);

	bool drawFrag = false;
};