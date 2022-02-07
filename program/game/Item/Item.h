#pragma once
#include<string>
#include<vector>

class Item
{
public:
	Item() {};
	Item(int Id,int ItemType,std::string ItemName,int Saturation,int Heal,int HitDamage,std::string Gh,std::string Desc);
	virtual ~Item();

	int GetItemId();
	void DrawItemData(int x,int y);
	 
	//ColumnId: 0->id,1->type,2->manpukudo,3->heal,4->hitdamage
	virtual int getItemData(int ColumnId);
	std::string getItemName();
	
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


};

