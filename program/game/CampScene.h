///*****Description*****
///�_���W��������O�̑S�Ă��s���V�[���N���X
///Update��Draw�֐���SceneManager�ōs����
///Update��Draw�֐��̒��ł̓V�[�N�G���X�Ŗ��t���[���s���鏈�������򂷂�
///�e�V�[�N�G���X�Ԃ�ChangeSequence�֐��ɂ���Ď��t���[�����瑖��V�[�N�G���X���ړ�����
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

	//mapchip_night_20.png�̓ǂݍ��� ��x�c=30x20
	int campGraphic[600];

	int GetGraphicHandle(int num);

	void LoadMap(std::string fileName, std::vector<std::vector<std::string>>& stringData, std::vector<std::vector<int>>& intData);
	void DrawMap(std::vector<std::vector<int>>intData);

	

};