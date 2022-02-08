#include "Item.h"
#include"../GameManager.h"
#include"DxLib.h"
#include"../../library/t2klib.h"
#include"../../support/Support.h"
#include"../Camera.h"

extern GameManager* gManager;

Item::Item(int Id, int ItemType, std::string ItemName, int Saturation, int Heal, int HitDamage, std::string Gh, std::string Desc)
{
	id = Id;
	itemType = ItemType;
	itemName = ItemName;
	amountOfSaturation = Saturation;
	amountOfHeal = Heal;
	hitDamage = HitDamage;

	gh = gManager->LoadGraphEx(Gh);
	desc = Desc;

	itemDesc[0] = itemName;
	itemDesc[1] = desc;
}

Item::~Item()
{
}

int Item::GetItemId()
{
	return id;
}

int Item::getItemData(int ColumnId)
{
	if (ColumnId == 0)return id;
	else if (ColumnId == 1)return itemType;
	else if (ColumnId == 2)return amountOfSaturation;
	else if (ColumnId == 3)return amountOfHeal;
	else if (ColumnId == 4)return hitDamage;
}

std::string Item::getItemName()
{
	return itemName;
}

void Item::SetPos(t2k::Vector3 Pos)
{
	pos = Pos;
}

bool Item::DetectOnPlayer(t2k::Vector3 Pos)
{
	t2k::Vector3 myMapPos = gManager->WorldToLocal(pos);
	if (myMapPos.x == Pos.x && myMapPos.y == Pos.y) {
		return true;
	}
	return false;
}


void Item::DrawItemData(int x, int y)
{
	DrawStringEx(x, y, -1, "%s", itemDesc[0].c_str());
	DrawStringEx(x, y+30, -1, "%s", itemDesc[1].c_str());
}

void Item::DrawPopItem()
{
	DrawRotaGraph(pos.x - gManager->camera->cameraPos.x, pos.y - gManager->camera->cameraPos.y, 1, 0, gh, true);
}
