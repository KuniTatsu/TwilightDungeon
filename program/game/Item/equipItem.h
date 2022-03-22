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

	//settype:0->RandomRange,1->���̂܂܂̒l�Ő���
	equipItem(int Id, int ItemType, std::string ItemName, int Saturation, int Heal, int HitDamage,
		std::string Gh, int SubId, int SellPrice, std::string Desc, int Hp, int Attack, int Defence, int Speed,int SetType);

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
	//subId�擾
	inline int GetSubId(){
		return subId;
	}

	//�C���x���g�����̑����A�C�e���̃X�e�[�^�X�`��
	//arg1:�`��X���W
	//arg2:�`��Y���W
	//arg3:�I�𒆃A�C�e����subId
	void DrawEquipItemStatus(int x, int y,int subId);

private:

	int hp = 0;
	int attack = 0;
	int defence = 0;
	int speed = 0;

	int subId = 0;

	bool isEquiped = false;

	//�`��p�Ԋu�̒萔
	const int DIS = 30;

	enum class STATUS : uint32_t {
		HP,
		ATTACK,
		DEFENCE,
		SPEED,
		STATUSMAX
	};
	//�A�C�e���X�e�[�^�X�̔z��
	int equipStatus[static_cast<uint32_t>(STATUS::STATUSMAX)];


	//�A�C�e���̃X�e�[�^�X�����͈͓��Ń����_���ɂ���֐�
	int SetRandomStatus(int CenterNum);

	//�������A�C�e���Ƃ̍����擾����֐�
	void SetDifNumEquipment(int subId);
	//�������A�C�e���Ƃ̍�
	int difNum[static_cast<uint32_t>(STATUS::STATUSMAX)] = {0,0,0,0};
};