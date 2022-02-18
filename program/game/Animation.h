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
	//�G�t�F�N�g�̌��摜
	int gh[5];
	//�`����W
	t2k::Vector3 pos;
	//�R�}�X�V�t���[����
	int actSpeed = 0;
	//�R�}�ő吔
	int maxMotionIndex = 0;
	//���݂̃t���[���J�E���g
	int actWait = actSpeed;
	//�I�𒆂̉摜�C���f�b�N�X
	int actIndex = 0;
	//�`�悷��摜
	int drawGh = 0;


	bool isAlive = true;
};