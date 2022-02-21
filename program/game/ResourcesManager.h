///*****Description*****
///ダンジョン内画像に関するクラス
///全ての種類のダンジョン内で使うグラフィックを読み込み、
/// ハンドルを保持する
///*********************
#pragma once
#include <vector>
#include<string>

class ResourceManager {

public:

	ResourceManager();
	~ResourceManager();

	void LoadResource();

	
	std::vector<std::vector<std::string>> dungeonMapChip;
	//TOWER,FOREST,WATERWAY
	std::vector<std::vector<int>> dungeonMapChipGh;



private:

};