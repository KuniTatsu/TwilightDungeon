///*****Description*****
///�����A�C�e���Ɋւ���N���X
///�A�C�e�����p�����Ă���
/// �����A�C�e���ŗL�̃X�e�[�^�X�ێ��ƁA
/// �����󋵂̕ύX�֐�������
///*********************
#pragma once
#include"Item.h"

class equipItem :public Item {

public:
	//HP(int)	Atack(int)	Defence(int)	Speed(int)

	equipItem(int Id, int ItemType, std::string ItemName, int Saturation, int Heal, int HitDamage, std::string Gh, int SubId, std::string Desc, int Hp, int Attack, int Defence, int Speed);

	~equipItem()override;
	//ColumnId: 0->id,1->type,2->manpukudo,3->heal,4->hitdamage,5->hp,6->attack,7->defence,8->speed,9->subid,
	int getItemData(int ColumnId)override;

	//�������ɕK�v��int�^�A�C�e���f�[�^��vector�ŕԂ��֐� Id,ItemType,Saturation,Heal,HitDamage,subId,hp,attack,defence,speed,
	std::vector<int> GetAllIntData()override;

	//�A�C�e���̑����󋵂�ύX����֐�
	void ChangeEquip();
	inline bool GetIsEquiped() {
		return isEquiped;
	}
	
private:

	int hp = 0;
	int attack = 0;
	int defence = 0;
	int speed = 0;

	int subId = 0;

	bool isEquiped = false;
};