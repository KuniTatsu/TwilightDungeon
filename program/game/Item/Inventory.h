#pragma once
#include<list>
class Item;

class Inventory
{
public:
	Inventory();
	~Inventory();

	Item* inventory[10] = { nullptr,nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr };

	std::list<Item*> inventoryList;
	//�C���x���g���ɃA�C�e����ǉ�����֐�
	void AddInventory(Item* item);
	//�J�[�\�����㉺�ɓ������֐�
	void CursorMove();
	//�J�[�\������ԏ�ɖ߂��֐�
	void CursorReset();
	//�C���x���g�����̃A�C�e������`�悷��֐�
	void DrawInventory(int x,int y);
	//�J�[�\���őI�𒆂̃A�C�e���̐�����`�悷��֐�
	void DrawItemData(int x,int y);

private:
	//�I�𒆂̃A�C�e�����w���J�[�\���̈ʒu
	int selectCursor = 0;
	//�J�[�\��gh
	int cursorGh = 0;
	//�C���x���g�����̃A�C�e����
	int itemNum = 0;


};

