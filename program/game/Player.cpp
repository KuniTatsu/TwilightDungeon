#include "Player.h"
#include"GameManager.h"
#include"DxLib.h"
#include"Camera.h"
#include"Item/equipItem.h"

extern GameManager* gManager;

Player::Player(const t2k::Vector3& StartPos, float Hp, int Atack, int Defence, int Speed, int ActId)
{
	actId = ActId;
	pos = StartPos;
	//p_gh = gManager->LoadGraphEx("graphics/PlayerTest.png");

	LoadDivGraph("graphics/Character_1.png", 12, 3, 4, 32, 32, gh);
	LoadDivGraph("graphics/AttackAnim_30.png", 5, 5, 1, 30, 30, effectGh);


	nowHpVar_gh = gManager->LoadGraphEx("graphics/haveHpVar.png");
	hpVar_gh = gManager->LoadGraphEx("graphics/AllHpVar.png");

	name = "Player";

	baseHp = Hp;
	baseAtack = Atack;
	baseDefence = Defence;
	baseSpeed = Speed;

	hp = baseHp;
	atack = baseAtack;
	defence = baseDefence;
	speed = baseSpeed;

	nowHp = hp;
	nowHpVarWidth = nowHp / hp;
	maxMotionIndex = gManager->GetMaxIndex(GameManager::index::PLAYER);
	playerInMap = gManager->WorldToLocal(pos);
}

Player::~Player()
{
}
//体力と満腹度を変更する関数
void Player::ChangeBaseStatus(int ManpukuMove, int HpMove)
{
	manPuku += ManpukuMove;
	if (manPuku < 0)manPuku = 0;
	else if (manPuku > 100)manPuku = 100;

	TakeHpEffect(HpMove);

}
//装備を変更する関数
void Player::ChangeEquipItem(equipItem* item)
{
	int subId = item->getItemData(9);
	//1->武器,2->head,3->chest,4->glove,5->boots,6->shield

		//もうすでに装備しているものがあれば
	if (myEquip[subId - 1] != nullptr) {
		myEquip[subId - 1]->ChangeEquip();
		//myEquip[subId - 1] = nullptr;
		//入れ替える(本来はここでinventoryにあるアイテムの[E]マークを切り替えたい)
		item->ChangeEquip();
		myEquip[subId - 1] = item;
	}
	else {
		item->ChangeEquip();
		myEquip[subId - 1] = item;
	}
	for (int i = 0; i < 4; ++i) {
		ChangeStatus(i, item->getItemData(i + 5), 1);
	}
}
//装備を外す関数
void Player::RemoveEquipItem(equipItem* item)
{
	int subId = item->getItemData(9);
	//1->武器,2->head,3->chest,4->glove,5->boots,6->shield
	myEquip[subId - 1]->ChangeEquip();
	myEquip[subId - 1] = nullptr;

	GetSumStatusFromEquipment();
	for (int i = 0; i < 4; ++i) {
		ChangeStatus(i, statuses[i], 1);
	}
}
//装備アイテムの合計ステータスを格納する関数
void Player::GetSumStatusFromEquipment()
{
	//自分が装備しているアイテムの HP/atack/defence/speedをそれぞれ合計して配列にしまいたい
	//0に戻す
	for (int k = 0; k < 4; ++k) {
		statuses[k] = 0;
	}
	for (int i = 0; i < 4; ++i) {
		for (int k = 0; k < 6; ++k) {
			if (myEquip[k] == nullptr)continue;
			statuses[i] = myEquip[k]->getItemData(i + 5);
		}
	}
}
int Player::GetStatusFromEquipment(int subId, int statusType)
{
	//subIdに対応する装備アイテムの持つ、statusTypeに対応するステータスを返す
	return myEquip[subId - 1]->getItemData(statusType + 5);
}
//リファクタリング必須　あとで変える:優先度高
bool Player::Move()
{
	//キャラの位置がマップ上のどのチップか特定する
	t2k::Vector3 playerInMap = gManager->WorldToLocal(pos);

	if (left)
	{
		DashToDir(LEFT, playerInMap);
		return true;
	}
	else if (up)
	{
		DashToDir(UP, playerInMap);
		return true;
	}
	else if (right)
	{
		DashToDir(RIGHT, playerInMap);
		return true;
	}
	else if (down)
	{
		DashToDir(DOWN, playerInMap);
		return true;
	}

	//もしshiftも一緒に押していたら壁か敵に当たるまでダッシュする
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_LEFT) && t2k::Input::isKeyDown(t2k::Input::KEYBORD_LSHIFT)) {
		mydir = Actor::LEFT;
		DashToDir(LEFT, playerInMap);
		left = true;
		gManager->setPlayerRoomNum(gManager->CheckIsThere(playerInMap));
		return true;
		//もしshiftも一緒に押していたら壁か敵に当たるまでダッシュする
	}
	else if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_UP) && t2k::Input::isKeyDown(t2k::Input::KEYBORD_LSHIFT)) {
		mydir = Actor::UP;
		DashToDir(UP, playerInMap);
		up = true;
		gManager->setPlayerRoomNum(gManager->CheckIsThere(playerInMap));
		return true;
	}
	else if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RIGHT) && t2k::Input::isKeyDown(t2k::Input::KEYBORD_LSHIFT)) {
		mydir = Actor::RIGHT;
		DashToDir(RIGHT, playerInMap);
		right = true;
		gManager->setPlayerRoomNum(gManager->CheckIsThere(playerInMap));
		return true;
	}
	else if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_DOWN) && t2k::Input::isKeyDown(t2k::Input::KEYBORD_LSHIFT)) {
		mydir = Actor::DOWN;
		DashToDir(DOWN, playerInMap);
		down = true;
		gManager->setPlayerRoomNum(gManager->CheckIsThere(playerInMap));
		return true;
	}


	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_LEFT)) {
		mydir = Actor::LEFT;
		//キャラのチップの左のチップがWALLなら移動しない
		if ((gManager->GetMapChip(playerInMap + t2k::Vector3(-1, 0, 0))) == 0)return false;
		//敵がその位置にいれば移動しない
		if (gManager->CheckIsThereEnemyToDir(playerInMap + t2k::Vector3(-1, 0, 0)))return false;

		//キャラのチップの左のチップがPASSWAYなら移動する
		MoveToDir(LEFT, playerInMap);
		return true;
	}
	else if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_UP)) {
		mydir = Actor::UP;
		//キャラのチップの左のチップがWALLなら移動しない
		if ((gManager->GetMapChip(playerInMap + t2k::Vector3(0, -1, 0))) == 0)return false;
		//敵がその位置にいれば移動しない
		if (gManager->CheckIsThereEnemyToDir(playerInMap + t2k::Vector3(0, -1, 0)))return false;

		//キャラのチップの左のチップがPASSWAYなら移動する
		MoveToDir(UP, playerInMap);
		return true;
	}
	else if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RIGHT)) {
		mydir = Actor::RIGHT;
		//キャラのチップの左のチップがWALLなら移動しない
		if ((gManager->GetMapChip(playerInMap + t2k::Vector3(1, 0, 0))) == 0)return false;
		//敵がその位置にいれば移動しない
		if (gManager->CheckIsThereEnemyToDir(playerInMap + t2k::Vector3(1, 0, 0)))return false;

		//キャラのチップの左のチップがPASSWAYなら移動する
		MoveToDir(RIGHT, playerInMap);
		return true;
	}
	else if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_DOWN)) {
		mydir = Actor::DOWN;
		//キャラのチップの左のチップがWALLなら移動しない
		if ((gManager->GetMapChip(playerInMap + t2k::Vector3(0, 1, 0))) == 0)return false;
		//敵がその位置にいれば移動しない
		if (gManager->CheckIsThereEnemyToDir(playerInMap + t2k::Vector3(0, 1, 0)))return false;
		//キャラのチップの左のチップがPASSWAYなら移動する
		MoveToDir(DOWN, playerInMap);
		return true;
	}
	return false;
}

void Player::TownMove(dir nextDir)
{
	playerInMap = gManager->WorldToLocal(pos);
	mydir = nextDir;
	//MoveToDir(nextDir, playerInMap);
	t2k::Vector3 nextPos = gManager->GetMultipleVector(nextDir, 20);
	pos.x += nextPos.x;
	pos.y += nextPos.y;
}
//HPバーの描画
void Player::HpVarDraw()
{
	DrawRotaGraph(pos.x - gManager->camera->cameraPos.x, pos.y - gManager->camera->cameraPos.y - 30, 1, 0, hpVar_gh, false);
	DrawRotaGraph3(pos.x - gManager->camera->cameraPos.x - 15, pos.y - gManager->camera->cameraPos.y - 30 - 5, 0, 0,
		nowHpVarWidth, 1, 0, nowHpVar_gh, false);
	DrawStringEx(pos.x - gManager->camera->cameraPos.x - 10, pos.y - gManager->camera->cameraPos.y - 50, -1, "%.0f", nowHp);
}
//経験値取得とレベルアップ処理
bool Player::AddExp(const int num)
{
	nowExp += num;
	if (nowExp >= nextLevelExp) {
		level++;
		nowExp = nowExp - nextLevelExp;
		SetLevelStatus();
		SetNextExp();
		return true;
	}
	return false;
}

void Player::LevelUp()
{
	level++;
	SetLevelStatus();
	SetNextExp();
}

//近くに敵がいなければ指定方向に進む関数
void Player::DashToDir(int dir, t2k::Vector3 mapPos)
{
	if (dir == LEFT) {
		if ((gManager->GetMapChip(mapPos + t2k::Vector3(-1, 0, 0))) == 0 || gManager->CheckNearByPlayerToAllEnemy(2))
		{
			left = false;
			return;
		}
		pos.x -= 20;
		//gManager->CameraMove(gManager->MAPWIDTH * 20, gManager->MAPHEIGHT * 20,dir);
		gManager->CameraReset();
	}
	else if (dir == UP) {
		if ((gManager->GetMapChip(mapPos + t2k::Vector3(0, -1, 0))) == 0 || gManager->CheckNearByPlayerToAllEnemy(2))
		{
			up = false;
			return;
		}
		pos.y -= 20;
		//gManager->CameraMove(gManager->MAPWIDTH * 20, gManager->MAPHEIGHT * 20, dir);
		gManager->CameraReset();
	}
	else if (dir == RIGHT) {
		if ((gManager->GetMapChip(mapPos + t2k::Vector3(1, 0, 0))) == 0 || gManager->CheckNearByPlayerToAllEnemy(2))
		{
			right = false;
			return;
		}
		pos.x += 20;
		//gManager->CameraMove(gManager->MAPWIDTH * 20, gManager->MAPHEIGHT * 20, dir);
		gManager->CameraReset();
	}
	else if (dir == DOWN) {
		if ((gManager->GetMapChip(mapPos + t2k::Vector3(0, 1, 0))) == 0 || gManager->CheckNearByPlayerToAllEnemy(2))
		{
			down = false;
			return;
		}
		pos.y += 20;
		//gManager->CameraMove(gManager->MAPWIDTH * 20, gManager->MAPHEIGHT * 20, dir);
		gManager->CameraReset();
	}

	gManager->setPlayerRoomNum(gManager->CheckIsThere(mapPos));
}
//指定方向に1マス進む関数
void Player::MoveToDir(int dir, t2k::Vector3 mapPos)
{
	if (dir == LEFT) {
		if ((gManager->GetMapChip(mapPos + t2k::Vector3(-1, 0, 0))) == 0)return;
		pos.x -= 20;
		//gManager->CameraMove(gManager->MAPWIDTH * 20, gManager->MAPHEIGHT * 20, dir);
		gManager->CameraReset();
	}
	else if (dir == UP) {
		if ((gManager->GetMapChip(mapPos + t2k::Vector3(0, -1, 0))) == 0)return;

		pos.y -= 20;
		//gManager->CameraMove(gManager->MAPWIDTH * 20, gManager->MAPHEIGHT * 20, dir);
		gManager->CameraReset();
	}
	else if (dir == RIGHT) {
		if ((gManager->GetMapChip(mapPos + t2k::Vector3(1, 0, 0))) == 0)return;
		pos.x += 20;
		//gManager->CameraMove(gManager->MAPWIDTH * 20, gManager->MAPHEIGHT * 20, dir);
		gManager->CameraReset();
	}
	else if (dir == DOWN) {
		if ((gManager->GetMapChip(mapPos + t2k::Vector3(0, 1, 0))) == 0)return;
		pos.y += 20;
		//gManager->CameraMove(gManager->MAPWIDTH * 20, gManager->MAPHEIGHT * 20, dir);
		gManager->CameraReset();
	}

	gManager->setPlayerRoomNum(gManager->CheckIsThere(mapPos));
}
void Player::RemoveAllEquip()
{
	for (int i = 0; i < 6; ++i) {
		myEquip[i] = nullptr;
	}
	GetSumStatusFromEquipment();
	for (int i = 0; i < 4; ++i) {
		ChangeStatus(i, statuses[i], 1);
	}
}
//リファクタリング必須　あとで変える:優先度中
//プレイヤーステータスの描画
void Player::DrawPlayerStatus(int x, int y, int width, int height)
{

	DrawStringEx(x + 10, y + yBuf, -1, "現在のレベル:%d", level);//20
	DrawStringEx(x + 10, y + yBuf * 2, -1, "体力:%.0f/%d", nowHp, hp);	//40
	DrawStringEx(x + 10, y + yBuf * 3, -1, "攻撃力:%d", atack);	//60
	DrawStringEx(x + 10, y + yBuf * 4, -1, "防御力:%d", defence);	//80
	DrawStringEx(x + 10, y + yBuf * 5, -1, "素早さ:%d", speed);	//100
	for (int i = 0; i < 6; ++i) {
		DrawStringEx(x + 160, y + 20 + 30 * i, -1, "%s:", equipName[i].c_str());
	}
	for (int i = 0; i < 6; ++i) {
		if (myEquip[i] == nullptr) {
			DrawStringEx(x + 230, y + 20 + 30 * i, -1, "装備なし");
		}
		else {
			DrawStringEx(x + 230, y + 20 + 30 * i, -1, "%s", myEquip[i]->getItemName().c_str());
		}
	}

	DrawStringEx(x + 10, y + 120, -1, "経験値:%d/%d", nowExp, nextLevelExp);

}

void Player::DeadPlayer()
{
	nowExp = 0;
	level = 1;
	//ステータスの初期化
	SetLevelStatus();
	//必要経験値の初期化
	SetNextExp();
	//装備アイテムの破棄
	RemoveAllEquip();
	//hpのリセット
	nowHp = hp;
	nowHpVarWidth = nowHp / hp;
	mydir = dir::UP;
}

void Player::SetLevelStatus()
{
	int lvHp = (level - 1) * hpParLevel;
	int lvAttack = (level - 1) * statusParLevel;
	int lvDefence = (level - 1) * statusParLevel;
	int lvSpeed = (level - 1) * statusParLevel;
	int lvStatus[4] = { lvHp,lvAttack,lvDefence,lvSpeed };

	for (int i = 0; i < 4; ++i) {
		ChangeStatus(i, lvStatus[i], 0);
	}
}

void Player::SetNextExp()
{
	nextLevelExp = 100 + (level - 1) * needExpParLevel; //1lv上がるごとに必要経験値が50増える
}
