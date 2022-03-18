///*****Description*****
///�A�j���[�V�����Ɋւ���N���X
///�C���X�^���X�������ɃA�j���[�V�����̏����擾
/// ���̃A�j���[�V�����̍X�V�A�`�揈�����s��
///*********************#pragma once
#include"../library/t2klib.h"
#include"../support/Support.h"
#include<string>

class Camera;

class Animation {
public:
	Animation(std::string Gh,t2k::Vector3 Pos,int ActSpeed,int MaxIndex);
	//���x���A�b�v�p
	Animation(std::string Gh, t2k::Vector3 Pos);
	~Animation();

	void Update();
	void Draw();
	//�A�j���[�V�������I�����Ă��邩�擾����֐�
	inline bool GetIsAlive() {
		return isAlive;
	}

private:

	Camera* camera = nullptr;

	//std::vector<int>gh;
	//�G�t�F�N�g�̌��摜
	int gh[5] = {};
	//���x���A�b�v���摜
	int levelUpGh[10] = {};
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

	int animationType = 0;

	//�A�j���[�V�����̏I���m�F�t���O
	bool isAlive = true;
};