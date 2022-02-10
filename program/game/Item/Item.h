#pragma once
#include<string>
#include<vector>
#include"../../library/t2klib.h"
#include"../../support/Support.h"

class Item
{
public:
	Item() {};
	Item(int Id,int ItemType,std::string ItemName,int Saturation,int Heal,int HitDamage,std::string Gh,std::string Desc);
	explicit Item(int ItemId);
	virtual ~Item();
	//�A�C�e��ID�̎擾
	int GetItemId();
	//�A�C�e���f�[�^�̕`��
	void DrawItemData(int x,int y);
	//pop���Ă���A�C�e���̕`��
	void DrawPopItem();
	 
	//ColumnId: 0->id,1->type,2->manpukudo,3->heal,4->hitdamage
	virtual int getItemData(int ColumnId);
	std::string getItemName();

	void SetPos(t2k::Vector3 Pos);
	//�v���C���[������ł��邩�ǂ����̎擾�֐�
	bool DetectOnPlayer(t2k::Vector3 Pos);

	//�|�b�v��Ԃ��ǂ����擾
	bool GetIsLive();
	//�A�C�e�����擾�������Ƀ|�b�v��Ԃł͂Ȃ�����֐�
	void SetIsLiveFalse();
	//�A�C�e�����W�̎擾
	t2k::Vector3 GetItemDrawPos();
	

	//�ړ��⊮ �ړ���ݒ�
	void SetGoalPos(int dir);

	bool ThrowItem(int dir);

	//�������Ă���A�C�e���̕`��֐�
	void DrawThrowItem();
	
protected:

	//id(int)	ItemType(int)	Item_Name(std::string)	AmountOfSaturation(int)	AmountOfHeal(int)	HitDamage(int) Gh(std::string)

	int id = 0;
	int itemType = 0;
	std::string itemName = "";
	int amountOfSaturation = 0;
	int amountOfHeal = 0;
	int hitDamage = 0;

	int gh = 0;
	std::string desc = {};
	//���O�Ɛ������̓������z��
	std::string itemDesc[2] = {};
	//�`����W
	t2k::Vector3 pos = { 0,0,0 };
	//�}�b�v���W
	t2k::Vector3 mapPos = { 0,0,0 };

	bool isLive = true;

	//�ړ��⊮�n
	t2k::Vector3 goalPos;
	t2k::Vector3 nowPos;
	t2k::Vector3 move;

	bool init = false;
	
};

