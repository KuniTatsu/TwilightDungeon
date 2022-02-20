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
class CampScene :public BaseScene {

public:
	CampScene();
	~CampScene();


	void Update();

	void Draw();

	inline bool canMoveThisPoint(int x, int y) {
		return collisionData[y][x];
	}

private:

	std::shared_ptr<Player>player;

	std::vector<std::vector<std::string>>loadGroundMap;
	std::vector<std::vector<std::string>>loadSurfaceMap;
	std::vector<std::vector<std::string>>loadCollision;

	std::vector<std::vector<int>>groundMapData;
	std::vector<std::vector<int>>surfaceMapData;
	std::vector<std::vector<int>>collisionData;

	//mapchip_night_20.pngの読み込み 横x縦=30x20
	int campGraphic[600];

	int GetGraphicHandle(int num);

	void LoadMap(std::string fileName, std::vector<std::vector<std::string>>& stringData, std::vector<std::vector<int>>& intData);
	void DrawMap(std::vector<std::vector<int>>intData);

	

};