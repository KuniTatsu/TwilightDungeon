#include "equipItem.h"
#include"../GameManager.h"

extern GameManager* gManager;

equipItem::equipItem(int Id, int ItemType, std::string ItemName, int Saturation, int Heal, int HitDamage, std::string Gh, int SubId, int SellPrice, std::string Desc, int Hp, int Attack, int Defence, int Speed, int SetType)
	: Item(Id, ItemType, ItemName, Saturation, Heal, HitDamage, SellPrice, Gh, Desc)
{
	if (SetType == 0) {
		hp = SetRandomStatus(Hp);
		attack = SetRandomStatus(Attack);
		defence = SetRandomStatus(Defence);
		speed = SetRandomStatus(Speed);
	}
	else {
		hp = Hp;
		attack = Attack;
		defence = Defence;
		speed = Speed;
	}

	subId = SubId;
	equipStatus[0] = hp;
	equipStatus[1] = attack;
	equipStatus[2] = defence;
	equipStatus[3] = speed;

}

equipItem::~equipItem()
{
}

int equipItem::getItemData(int ColumnId)
{
	if (ColumnId == 0)return id;
	else if (ColumnId == 1)return itemType;
	else if (ColumnId == 2)return amountOfSaturation;
	else if (ColumnId == 3)return amountOfHeal;
	else if (ColumnId == 4)return hitDamage;
	else if (ColumnId == 5)return hp;
	else if (ColumnId == 6)return attack;
	else if (ColumnId == 7)return defence;
	else if (ColumnId == 8)return speed;
	else if (ColumnId == 9)return subId;
}

std::vector<int> equipItem::GetAllIntData()
{
	std::vector<int>data;
	data.emplace_back(id);
	data.emplace_back(itemType);
	data.emplace_back(amountOfSaturation);
	data.emplace_back(amountOfHeal);
	data.emplace_back(hitDamage);

	data.emplace_back(subId);
	data.emplace_back(sellingPrice);
	data.emplace_back(hp);
	data.emplace_back(attack);
	data.emplace_back(defence);
	data.emplace_back(speed);

	return data;
}

void equipItem::ChangeEquip()
{

	if (isEquiped)isEquiped = false;
	else isEquiped = true;

}

void equipItem::DrawEquipItemStatus(int x, int y, int subId)
{
	DrawStringEx(x, y, -1, "HP:%d上昇", hp);
	DrawStringEx(x, y + DIS, -1, "攻撃力%d上昇", attack);
	DrawStringEx(x, y + DIS * 2, -1, "防御力:%d上昇", defence);
	DrawStringEx(x, y + DIS * 3, -1, "速度:%d上昇", speed);

	//装備中アイテムとの差を取得する関数
	//装備していなければ実行しない
	if (gManager->CheckEquipItem(subId)) {
		SetDifNumEquipment(subId);
		int k = 0;
		for (int i = 0; i < static_cast<uint32_t>(STATUS::STATUSMAX); ++i) {
			//int color = 0;
			//差が0以上なら青で表示
			if (difNum[i] >= 0) {
				DrawStringEx(x + 180, y + DIS * k, gManager->blue, "差:+%d", difNum[i]);
			}
			//差がマイナスなら赤で表示
			else {
				DrawStringEx(x + 180, y + DIS * k, gManager->red, "差:%d", difNum[i]);
			}
			//DrawStringEx(x + 100, y + DIS * k, color, "差:%d", difNum[i]);
			k++;
		}
	}
}

int equipItem::SetRandomStatus(int CenterNum)
{
	return gManager->GetRandEx(CenterNum * 0.8, CenterNum * 1.2);
}

void equipItem::SetDifNumEquipment(int subId)
{
	for (int i = 0; i < static_cast<uint32_t>(STATUS::STATUSMAX); ++i) {
		difNum[i] = gManager->GetDifStatus(subId, i, equipStatus[i]);
	}
}


