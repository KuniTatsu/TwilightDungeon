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
	virtual ~Item();

	int GetItemId();
	void DrawItemData(int x,int y);

	void DrawPopItem();
	 
	//ColumnId: 0->id,1->type,2->manpukudo,3->heal,4->hitdamage
	virtual int getItemData(int ColumnId);
	std::string getItemName();

	void SetPos(t2k::Vector3 Pos);

	bool DetectOnPlayer(t2k::Vector3 Pos);
	
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
	//–¼‘O‚Æà–¾•¶‚Ì“ü‚Á‚½”z—ñ
	std::string itemDesc[2] = {};

	t2k::Vector3 pos = { 0,0,0 };



};

