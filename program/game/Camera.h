///*****Description*****
///�J�����Ɋւ���N���X
///�f�o�b�O���̃J�������R�ړ���
///�ʏ�`�掞�̉�ʕ␳�̂��߂̕ϐ�������
///*********************
#pragma once
#include "../library/t2klib.h"
#include"../support/Support.h"

class Camera {

public:
	t2k::Vector3 cameraPos = {0,0,0};

	void CameraMove();

};