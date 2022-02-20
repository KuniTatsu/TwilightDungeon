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