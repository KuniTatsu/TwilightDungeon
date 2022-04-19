#include "Item.h"
#include"../GameManager.h"
#include"DxLib.h"
#include"../../library/t2klib.h"
#include"../../support/Support.h"
#include"../Camera.h"

extern GameManager* gManager;

Item::Item(int Id, int ItemType, std::string ItemName, int Saturation, int Heal, int HitDamage, int SellPrice, std::string Gh, std::string Desc)
{
	id = Id;
	itemType = ItemType;
	itemName = ItemName;
	amountOfSaturation = Saturation;
	amountOfHeal = Heal;
	hitDamage = HitDamage;

	ghData = Gh;

	gh = gManager->LoadGraphEx(Gh);
	desc = Desc;

	itemDesc[0] = itemName;
	itemDesc[1] = desc;

	//sellingPrice = SellPrice;
	sellingPrice = gManager->GetRandEx(SellPrice,SellPrice*1.5);

	//固有IDの設定
	uniqueId = rand();

}



Item::Item(int Id)
{
	//Idから
}

Item::~Item()
{
	t2k::debugTrace("\nアイテムが破棄されました\n");
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
	else if (ColumnId == 5)return sellingPrice;
	else return -1;
}

const std::string Item::getItemName()
{
	return itemName;
}

std::vector<int> Item::GetAllIntData()
{
	std::vector<int>data;
	data.emplace_back(id);
	data.emplace_back(itemType);
	data.emplace_back(amountOfSaturation);
	data.emplace_back(amountOfHeal);
	data.emplace_back(hitDamage);
	data.emplace_back(sellingPrice);

	return data;
}

std::vector<std::string> Item::GetAllStringData()
{
	std::vector<std::string>data;
	data.emplace_back(itemName);
	data.emplace_back(ghData);
	data.emplace_back(desc);

	return data;
}

void Item::SetPos(t2k::Vector3 Pos)
{
	pos = Pos;
	mapPos = gManager->WorldToLocal(pos);
}

t2k::Vector3& Item::GetPos()
{
	return pos;
}

bool Item::DetectOnPlayer(t2k::Vector3 Pos)
{
	t2k::Vector3 myMapPos = gManager->WorldToLocal(pos);
	if (myMapPos.x == Pos.x && myMapPos.y == Pos.y) {
		return true;
	}
	return false;
}

bool Item::GetIsLive()
{
	return isLive;
}

void Item::SetIsLiveFalse()
{
	isLive = false;
}

t2k::Vector3 Item::GetItemDrawPos()
{
	return pos;
}

void Item::SetGoalPos(int dir)
{
	if (!init) {
		nowPos = pos;
		goalPos = pos + gManager->GetMultipleVector(dir, gManager->nowGraphicSize);
		init = true;
	}
	else goalPos = nowPos + gManager->GetMultipleVector(dir, gManager->nowGraphicSize);
	//1フレームあたりの移動量
	move = (goalPos - nowPos) * moveSpeed;
}

bool Item::ThrowItem(int dir)
{
	nowPos += move;
	//アイテムを描画する

	//マップ座標
	t2k::Vector3 mapPos = gManager->WorldToLocal(nowPos);
	t2k::Vector3 nextPos;


	nextPos = mapPos + gManager->GetVecter(dir);

	//壁なら
	if (gManager->GetMapChip(nextPos) == 0) {
		return true;
	}//敵なら
	else if (gManager->CheckIsThereEnemyToDir(nextPos)) {
		return true;
	}
	//ゴールまでの距離が5以下になったら
	if ((goalPos - nowPos).length() < 5) {
		SetGoalPos(dir);
	}
	return false;
}

void Item::DrawThrowItem()
{
	DrawRotaGraph(nowPos.x - gManager->camera->cameraPos.x, nowPos.y - gManager->camera->cameraPos.y, gManager->graphEx, 0, gh, true);
}


void Item::DrawItemStringData(int x, int y)
{
	DrawStringEx(x, y, -1, "%s", itemDesc[0].c_str());
	DrawStringEx(x, y + 30, -1, "%s", itemDesc[1].c_str());
}

void Item::DrawPopItem()
{
	DrawRotaGraph(pos.x - gManager->camera->cameraPos.x, pos.y - gManager->camera->cameraPos.y, gManager->graphEx, 0, gh, true);
}
