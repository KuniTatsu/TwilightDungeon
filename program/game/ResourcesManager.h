///*****Description*****
///�_���W�������摜�Ɋւ���N���X
///�S�Ă̎�ނ̃_���W�������Ŏg���O���t�B�b�N��ǂݍ��݁A
/// �n���h����ێ�����
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