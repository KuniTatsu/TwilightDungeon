#pragma once
#include<list>
#include<vector>

class HaveItem
{
public:
	HaveItem();
	~HaveItem();
	//ItemId n�Ԃ������Ă���A�C�e�����X�g�ɓo�^
	void setItemToInventory(int ItemId);
	//inventory�̒��g���O����vector�Ɉڂ��`�Ŏ擾����֐�
	std::vector<int>* getItemFromInventory(std::vector<int>*HaveItemIds);

private:

	std::list<int>inventory;
};

