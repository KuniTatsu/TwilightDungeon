///*****Description*****
///�C���x���g���Ɋւ���N���X
///�v���C���[�̏��L����A�C�e���̏���ێ�����
/// �C���x���g�����̃A�C�e���̏������s��
///*********************
#pragma once
#include<list>
#include<memory>
class Item;

class Inventory
{
public:
	Inventory(int MyInventoryNum);
	~Inventory();

	//std::shared_ptr<Item>inventory[10] = { nullptr,nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr };
	
	//������Item��10����list
	std::list<Item*> inventoryList;
	//std::list<std::shared_ptr<Item>> inventorySharedList;

	//�C���x���g���ɃA�C�e����ǉ�����֐�
	void AddInventory(Item* item);

	//void AddSharedInventory(std::shared_ptr<Item>item);
	// 
	//�J�[�\�����㉺�ɓ������֐�
	void CursorMove();
	//�J�[�\������ԏ�ɖ߂��֐�
	void CursorReset();
	//�C���x���g�����̃A�C�e������`�悷��֐�
	void DrawInventory(const int x, const int y);
	//�J�[�\���őI�𒆂̃A�C�e���̐�����`�悷��֐�
	void DrawItemData(const int x, const int y);

	//�J�[�\���̈ʒu���擾����֐�
	int GetCursorNum();

	//�J�[�\���̈ʒu��ύX����֐� type:0->���Z�ړ�,1->�w��ʒu�ړ�
	void SetCursorNum(const int MoveNum);

	//�C���x���g���ԍ����擾����֐�
	inline int GetInventoryNum() {
		return myInventoryNum;
	}

	inline void SetItemNum(const int num) {
		itemNum += num;
	}

private:
	//�I�𒆂̃A�C�e�����w���J�[�\���̈ʒu
	int selectCursor = 0;
	//�J�[�\��gh
	int cursorGh = 0;
	//�I�𒆃A�C�e���̔w�i
	int selectItemBackGh = 0;
	//�C���x���g�����̃A�C�e����
	int itemNum = 0;

	//�C���x���g���ԍ�
	int myInventoryNum = 0;


};

