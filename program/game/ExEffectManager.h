///Description
///����Ȍ��ʂ���������֐����ЂƂ܂Ƃ߂ɂ����N���X
///�A�C�e����X�L���Ȃǂ���������Ă�(�\�肾����)
///�D�揇�ʂ̓s����J�����f
///

#pragma once
#include<vector>
#include<string>
#include<unordered_map>
#include<functional>
#include"../library/t2klib.h"

class Item;

//**********�܂��g�p���Ȃ��N���X�̂��߈�U�X�V��~*********//

class ExEffectManager
{
public:
	//key�ň����Ԃ�l�Ȃ��̊֐����������A���s����֐�
	std::function<void()> DoEvent(std::string key);
	//key�ň���t2k::Vector3,�Ԃ�l�Ȃ��̊֐����������A���s����֐�
	std::function<void(t2k::Vector3)> DoPosEvent(std::string key);

private:
	ExEffectManager();
	~ExEffectManager();

	std::vector<std::string>loadSkill;
	//�e����ꏈ���̌��ʂ����s����֐�����ꂽmap(�����Ԃ�l�Ȃ�)
	std::unordered_map<std::string, std::function<void()>> exEffects;

	//�e����ꏈ���̌��ʂ����s����֐�����ꂽmap(���� t2k::vector3)
	std::unordered_map<std::string, std::function<void(t2k::Vector3)>> exPosEffect;

	void SetMap(std::string key, void hoge());
	void SetPosMap(std::string key, void hoge(t2k::Vector3));

	//*****�������牺�͓�����ʂ̎�������*****//
	//�S�����̒����烉���_���Ȉʒu�Ƀ��[�v����֐� ���[�v�Ώۂ̈��������
	void Warp(t2k::Vector3& Pos);

	void NextFloor();

	//void CreateWay(t2k::Vector3 Pos);

};

//void CreateWay(t2k::Vector3 Pos) {};