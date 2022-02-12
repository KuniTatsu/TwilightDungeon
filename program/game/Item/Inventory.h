#pragma once
#include<list>
#include<memory>
class Item;

class Inventory
{
public:
	Inventory();
	~Inventory();

	//Item* inventory[10] = { nullptr,nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr };

	//std::shared_ptr<Item>inventory[10] = { nullptr,nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr ,nullptr };

	std::list<Item*> inventoryList;
	std::list<std::shared_ptr<Item>> inventorySharedList;

	//�C���x���g���ɃA�C�e����ǉ�����֐�
	void AddInventory(Item* item);

	void AddSharedInventory(std::shared_ptr<Item>item);
	//�J�[�\�����㉺�ɓ������֐�
	void CursorMove();
	//�J�[�\������ԏ�ɖ߂��֐�
	void CursorReset();
	//�C���x���g�����̃A�C�e������`�悷��֐�
	void DrawInventory(int x, int y);
	//�J�[�\���őI�𒆂̃A�C�e���̐�����`�悷��֐�
	void DrawItemData(int x, int y);

	//�J�[�\���̈ʒu���擾����֐�
	int GetCursorNum();

	//�J�[�\���̈ʒu��ύX����֐�
	void SetCursorNum(int MoveNum);

private:
	//�I�𒆂̃A�C�e�����w���J�[�\���̈ʒu
	int selectCursor = 0;
	//�J�[�\��gh
	int cursorGh = 0;
	//�I�𒆃A�C�e���̔w�i
	int selectItemBackGh = 0;
	//�C���x���g�����̃A�C�e����
	int itemNum = 0;


};

