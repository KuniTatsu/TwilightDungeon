#include "DungeonScene.h"
#include"Map.h"
#include"GameManager.h"

#include"../support/Support.h"
#include "Player.h"
#include"MenuWindow.h"
#include"Actor/EnemyManager.h"
#include"Actor/Enemy.h"
#include"Camera.h"
#include"Item/Item.h"
#include"Item/Inventory.h"


extern GameManager* gManager;

DungeonScene::DungeonScene()
{
	alfa = gManager->LoadGraphEx("graphics/old/test.png");
	nextLevelWindow = new Menu(300, 300, 300, 200, "graphics/WindowBase_01.png");
	menuOpen = new Menu(20, 20, 100, 100, "graphics/WindowBase_01.png");
	inventory = new Menu(255, 50, 420, 340, "graphics/WindowBase_01.png");
	log = new Menu(10, 580, 1000, 180, "graphics/WindowBase_01.png");
	desc = new Menu(680, 300, 320, 90, "graphics/WindowBase_01.png");
	playerStatus = new Menu(10, 10, 500, 200, "graphics/WindowBase_01.png");

	MenuWindow::MenuElement_t* menu_usable = new MenuWindow::MenuElement_t[]{
		{670,450,"使う",0},
		{670,480,"投げる",1},
		{670,510,"やめる",2}
	};
	use_usable = new MenuWindow(640, 440, 90, 100, "graphics/WindowBase_02.png", menu_usable, 3, 0.15);

	MenuWindow::MenuElement_t* menu_equip = new MenuWindow::MenuElement_t[]{
		{670,450,"装備する",0},
		{670,480,"投げる",1},
		{670,510,"やめる",2}
	};
	use_equip = new MenuWindow(640, 440, 90, 100, "graphics/WindowBase_02.png", menu_equip, 3, 0.15);

	eManager = std::make_shared<EnemyManager>();

	MenuWindow::MenuElement_t* menu_0 = new MenuWindow::MenuElement_t[]{
		{70,80,"持ち物",0},
		{70,110,"足元",1},
		{70,140,"セーブ",2},
		{70,170,"タイトルへ戻る",3},
		{70,200,"Esc|メニューを閉じる",4}
	};
	// メニューウィンドウのインスタンス化
	firstMenu = new MenuWindow(30, 50, 220, 210, "graphics/WindowBase_02.png", menu_0, 5, 0.45);

	//firstMenu->menu_live = true;
	/*for (int i = 0; i < 5; ++i) {
		int rand = GetRand(100) % 6+100;
		eManager->CreateEnemy(rand);
	}*/
	RandEnemyCreate(5);
	for (int i = 0; i < 5; ++i) {
		int rand = GetRand(100) % gManager->GetItemNum();
		SpawnItem(rand);
	}
	player = gManager->GetPlayer();
}

DungeonScene::~DungeonScene()
{
}

void DungeonScene::RandEnemyCreate(int num)
{
	for (int i = 0; i < num; ++i) {
		int rand = GetRand(100) % 6 + 100;
		eManager->CreateEnemy(rand, dungeonLevel);
	}
}

void DungeonScene::Update()
{
	GetMousePoint(&mouseX, &mouseY);

	main_sequence.update(gManager->deitatime_);

	//デバッグ切り替え
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_F2)) {
		if (gManager->isDebug)gManager->isDebug = false;
		else gManager->isDebug = true;
	}

	//デバッグ用マップ再生成
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_F5)) {

		/*for (auto enemy : eManager->liveEnemyList) {
			enemy->isLive = false;
		}*/
		MoveLevel(1);
	}

	//アイテム当たり判定感知

	for (auto item : dropItems) {
		//アイテムとプレイヤーが重なったら
		if (item->DetectOnPlayer(playerPos)) {
			gManager->AddItemToInventory(item->GetItemId());
			item->SetIsLiveFalse();
			//gManager->PopDetectItem(item, dropItems);
		}
	}
	for (auto item : dropItems) {
		if (item->GetIsLive())continue;
		if (gManager->PopDetectItem(item, dropItems))break;
	}
	//isLiveがfalseな敵をリストから外したい
	/*std::list<std::shared_ptr<Enemy>>::iterator hoge = eManager->liveEnemyList.begin();
	for (int i = 0; i < eManager->liveEnemyList.size(); ++i) {
		if ((*it)->isLive == false) {
			eManager->liveEnemyList.erase(it);
		}
		it++;
	}*/


	////スクリーン全体をズーム,ズームアウトしたかった

}

void DungeonScene::Draw()
{
	gManager->map->MapDraw();
	DrawPopItem();
	gManager->player->Draw();
	gManager->player->HpVarDraw();

	for (auto hoge : eManager->liveEnemyList) {
		hoge->Draw();
	}

	if (gManager->isDebug) {
		DrawStringEx(100, 280, -1, "現在の階層:%d", dungeonLevel);

		DrawStringEx(100, 300, -1, "PlayerMapChipX:%d", (int)playerPos.x);
		DrawStringEx(100, 320, -1, "PlayerMapChipY:%d", (int)playerPos.y);
		DrawEnemyData();
	}
	if (gManager->GetMapChip(playerPos) == 3) {
		nextLevelWindow->Menu_Draw();
		DrawStringEx(nextLevelWindow->menu_x + 10, nextLevelWindow->menu_y + 100, -1, "Enterで次の階へ");
	}
	for (auto hoge : gManager->wayPoint) {
		for (auto hage : hoge) {
			int X = (int)hage.x;
			int Y = (int)hage.y;
			DrawCircle(X * 20 - gManager->camera->cameraPos.x, Y * 20 - gManager->camera->cameraPos.y, 10, -1, true);
		}
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawRotaGraph(gManager->player->pos.x - gManager->camera->cameraPos.x, gManager->player->pos.y - gManager->camera->cameraPos.y, 1, 0, alfa, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	//gManager->map->DrawAllRoomPos(gManager->map.)

	DrawNowSequence(nowSeq);
	log->Menu_Draw();
	gManager->LogDraw(log->menu_x, log->menu_y);

	firstMenu->All();
	if (nowSeq == sequence::MAIN || nowSeq == sequence::ENEMYACT) {
		menuOpen->Menu_Draw();
		DrawStringEx(menuOpen->menu_x + 10, menuOpen->menu_y + 10, -1, "Menuを開く");
	}
	else if (nowSeq == sequence::INVENTORY_OPEN || nowSeq == sequence::INVENTORY_USE) {
		inventory->Menu_Draw();
		DrawInventory();
		if (nowSeq == sequence::INVENTORY_USE) {
			if (usetype == USABLE)use_usable->All();
			else if (usetype == EQUIP)use_equip->All();
		}
	}
	else if (nowSeq == sequence::THROWITEMMOVE) {
		if (!throwedItemList.empty()) {
			for (auto item : throwedItemList) {
				item->DrawThrowItem();
			}
		}
	}

	if (t2k::Input::isKeyDown(t2k::Input::KEYBORD_P)) {
		playerStatus->Menu_Draw();
		player->DrawPlayerStatus();
	}
}

int DungeonScene::GetDungeonLevel()
{
	return dungeonLevel;
}

void DungeonScene::SetDungeonLevel(int addLevel)
{
	dungeonLevel += addLevel;
}

void DungeonScene::MoveLevel(int addLevel)
{
	if (!eManager->liveEnemyList.empty())eManager->liveEnemyList.clear();
	if (!gManager->hoge.empty())gManager->hoge.clear();
	dungeonLevel++;
	gManager->ReCreate();
	RandEnemyCreate(5);
	dropItems.clear();
	for (int i = 0; i < 5; ++i) {
		int rand = GetRand(100) % gManager->GetItemNum();
		SpawnItem(rand);
	}
}

bool DungeonScene::Seq_Main(const float deltatime)
{
	//debug
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_ESCAPE)) {
		ChangeSequence(sequence::CAMERA);
		return true;
	}
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_D)) {
		gManager->player->TakeHpEffect(-20);
	}
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_U)) {
		gManager->player->ChangeStatus(1, 50, 0);
		for (auto enemy : eManager->liveEnemyList) {
			enemy->ChangeStatus(2, 10, 0);
		}
	}
	//

	//menuを開く
	//もしmenuボタンの上にマウスがいたら
	if (gManager->CheckMousePointToRect(mouseX, mouseY, menuOpen->menu_x, menuOpen->menu_width, menuOpen->menu_y, menuOpen->menu_height)) {
		//マウスクリックしていたら
		if (t2k::Input::isMouseTrigger(t2k::Input::MOUSE_RELEASED_LEFT)) {
			t2k::debugTrace("\n押されたよ\n");//ok
			firstMenu->Open();
			ChangeSequence(sequence::FIRSTMENU);
			return true;
		}
	}
	//Eを押してもメニューが開く
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_E)) {
		firstMenu->Open();
		ChangeSequence(sequence::FIRSTMENU);
		return true;
	}



	//足踏み(Playerは移動せずにターンは経過する
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_SPACE)) {
		player->skip = true;
	}

	//もしplayerが階段の上にいたら
	//windowを出す
	//enterで次の階へ
	if (gManager->GetMapChip(playerPos) == 3) {

		if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
			dungeonLevel += 1;
			if (!eManager->liveEnemyList.empty())eManager->liveEnemyList.clear();
			gManager->ReCreate();
			RandEnemyCreate(5);
		}
	}
	//左クリックかRボタンで攻撃
	//プレイヤーの攻撃は目の前に対象がいなかった場合skipと同じ処理を行う
	if (t2k::Input::isMouseTrigger(t2k::Input::MOUSE_RELEASED_LEFT) || t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_R)) {
		player->Atack();
		//死亡チェック
		for (auto enemy : eManager->liveEnemyList) {
			if (enemy->GetStatus(0) <= 0)enemy->isLive = false;
		}

		DeleteDeadEnemy();
		if (!player->skip) {
			ChangeSequence(sequence::ENEMYACT);
			return true;
		}
	}

	//もしPlayerが動いたら もしくはスキップしたら
	if (gManager->player->Move() || player->skip) {

		playerPos = gManager->WorldToLocal(gManager->player->pos);

		ChangeSequence(sequence::ENEMYACT);
		if (player->skip)player->skip = false;
		return true;
	}

	return true;
}

bool DungeonScene::Seq_EnemyAct(const float deltatime)
{
	if (main_sequence.isStart()) {
		for (auto liveEnemy : eManager->liveEnemyList) {
			//playerとenemyが隣り合っているなら
			if (gManager->CheckNearByPlayer(liveEnemy))
			{
				//もしプレイヤーの方向を向いていない場合は向かせる
				//プレイヤーの位置とenemyの位置との相対方向を取得する
				int vec = gManager->GetPlayerVec(liveEnemy);

				if (liveEnemy->mydir != vec)liveEnemy->setDir(vec);
				//リストに入れる
				atackEnemies.emplace_back(liveEnemy);
			}
			else {
				//動く関数は全て同時でいい
				liveEnemy->Move();
			}
		}
		itr = atackEnemies.begin();
	}
	//攻撃する敵リストがからじゃないなら
	if (!atackEnemies.empty()) {
		//もし敵が一体だけならインターバルを0にする
		if (atackEnemies.size() == 1)enemyActTimer = ENEMYACTINTERVAL;
		//一体ずつ攻撃させるためのインターバル計測
		if (++enemyActTimer > ENEMYACTINTERVAL) {
			(*itr)->Atack();
			enemyActTimer = 0;
			itr = atackEnemies.erase(itr);
		}
		//すべての敵が攻撃し終えるまでこのシークエンスを出ない
		if (!atackEnemies.empty())return true;
	}
	ChangeSequence(sequence::MAIN);
	return true;
}

bool DungeonScene::Seq_FirstMenu(const float deltatime)
{
	if (firstMenu->SelectNum == 0 && t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
		//menuの上下を操作出来なくする
		firstMenu->manageSelectFlag = false;
		//gManager->sound->System_Play(gManager->sound->system_select);

		//InventoryOpenシークエンスに移動する
		ChangeSequence(sequence::INVENTORY_OPEN);
		return true;
	}
	else if (firstMenu->SelectNum == 4 && t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
		firstMenu->menu_live = false;
		ChangeSequence(sequence::MAIN);
		return true;
	}


	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_ESCAPE))
	{
		firstMenu->menu_live = false;
		ChangeSequence(sequence::MAIN);
		return true;
	}
	return true;
}

bool DungeonScene::Seq_InventoryOpen(const float deltatime)
{

	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_ESCAPE))
	{
		firstMenu->manageSelectFlag = true;
		ChangeSequence(sequence::FIRSTMENU);
		return true;
	}
	//インベントリの操作
	ChangeInventory();
	//もしインベントリが空ならreturn
	if (gManager->inventories[inventoryPage]->inventoryList.empty())return true;
	//現在選択中のカーソル位置を取得
	int selectNum = gManager->inventories[inventoryPage]->GetCursorNum();


	//もしインベントリを開いている時にenterが押されたら
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
		//現在のカーソルの位置のアイテムを取得する
		auto itr = gManager->inventories[inventoryPage]->inventoryList.begin();
		for (int i = 0; i < selectNum; ++i) {
			itr++;
		}
		Item* selectedItem = (*itr);
		//一時的にアイテムデータを保管する
		itemBuf = selectedItem;
		//そのアイテムのtypeを取得する
		int type = selectedItem->getItemData(1);
		if (type < 2) {
			use_usable->Open();
			usetype = USABLE;
			ChangeSequence(sequence::INVENTORY_USE);
			return true;
		}
		else {
			use_equip->Open();
			usetype = EQUIP;
			ChangeSequence(sequence::INVENTORY_USE);
			return true;
		}
	}

	return true;
}

bool DungeonScene::Seq_InventoryUse(const float deltatime)
{
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_ESCAPE))
	{
		use_usable->menu_live = false;
		use_equip->menu_live = false;
		itemBuf = nullptr;
		ChangeSequence(sequence::INVENTORY_OPEN);
		return true;
	}

	int type = itemBuf->getItemData(1);
	//消費アイテムなら
	if (type < 2) {
		//使うでEnterを押したら
		if (use_usable->SelectNum == 0 && t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
			ItemUse(inventoryPage);
			use_usable->menu_live = false;
			itemBuf = nullptr;
			ChangeSequence(sequence::INVENTORY_OPEN);
			return true;
		}
		//投げるでEnterを押したら
		else if (use_usable->SelectNum == 1 && t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
			firstMenu->menu_live = false;
			ItemThrow(inventoryPage);
			ChangeSequence(sequence::THROWITEMMOVE);
			return true;
		}
		//やめるでEnterを押したら
		else if (use_usable->SelectNum == 2 && t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
			use_usable->menu_live = false;
			itemBuf = nullptr;
			ChangeSequence(sequence::INVENTORY_OPEN);
			return true;
		}
	}
	//装備アイテムなら
	else {
		//使うでEnterを押したら
		if (use_equip->SelectNum == 0 && t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
			ItemUse(inventoryPage);
			use_equip->menu_live = false;
			itemBuf = nullptr;
			ChangeSequence(sequence::INVENTORY_OPEN);
			return true;
		}//投げるでEnterを押したら
		else if (use_equip->SelectNum == 1 && t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
			firstMenu->menu_live = false;
			ItemUse(inventoryPage);
			ChangeSequence(sequence::THROWITEMMOVE);
			return true;
		}
		//やめるでEnterを押したら
		else if (use_equip->SelectNum == 2 && t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
			use_equip->menu_live = false;
			itemBuf = nullptr;
			ChangeSequence(sequence::INVENTORY_OPEN);
			return true;
		}
	}
	return true;
}

bool DungeonScene::Seq_ThrowItemMove(const float deltatime)
{
	if (throwedItemList.empty())return true;
	auto itr = throwedItemList.begin();

	if ((*itr)->ThrowItem(player->mydir)) {
		throwedItemList.erase(itr);
		ChangeSequence(sequence::MAIN);
		return true;
	}
	return true;
}

bool DungeonScene::Seq_CameraMove(const float deltatime)
{
	gManager->camera->CameraMove();


	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_ESCAPE)) {
		ChangeSequence(sequence::MAIN);
		gManager->CameraMove();
		return true;
	}

	return true;
}

void DungeonScene::ChangeSequence(sequence seq)
{
	nowSeq = seq;
	if (seq == sequence::MAIN) {
		main_sequence.change(&DungeonScene::Seq_Main);
	}
	else if (seq == sequence::ENEMYACT) {
		main_sequence.change(&DungeonScene::Seq_EnemyAct);
	}
	else if (seq == sequence::FIRSTMENU) {
		main_sequence.change(&DungeonScene::Seq_FirstMenu);
	}
	else if (seq == sequence::INVENTORY_OPEN) {
		main_sequence.change(&DungeonScene::Seq_InventoryOpen);
	}
	else if (seq == sequence::INVENTORY_USE) {
		main_sequence.change(&DungeonScene::Seq_InventoryUse);
	}
	else if (seq == sequence::THROWITEMMOVE) {
		main_sequence.change(&DungeonScene::Seq_ThrowItemMove);
	}
	else if (seq == sequence::CAMERA) {
		main_sequence.change(&DungeonScene::Seq_CameraMove);
	}

}

void DungeonScene::DrawNowSequence(sequence seq)
{
	if (seq == sequence::MAIN) {
		DrawStringEx(800, 300, -1, "MAINSequence");
	}
	else if (seq == sequence::ENEMYACT) {
		DrawStringEx(800, 300, -1, "ENEMYACTSequence");
	}
	else if (seq == sequence::FIRSTMENU) {
		DrawStringEx(800, 300, -1, "FIRSTMENUSequence");
	}
	else if (seq == sequence::INVENTORY_OPEN) {
		DrawStringEx(800, 300, -1, "INVENTORYOPENSequence");
	}
	else if (seq == sequence::INVENTORY_USE) {
		DrawStringEx(800, 300, -1, "INVENTORYUSESequence");
	}
	else if (seq == sequence::THROWITEMMOVE) {
		DrawStringEx(800, 300, -1, "ITEMMOVESequence");
	}
	else if (seq == sequence::CAMERA) {
		DrawStringEx(800, 300, -1, "CAMERASequence");
	}
}

void DungeonScene::DrawEnemyData()
{
	int i = 0;
	for (auto enemy : eManager->liveEnemyList) {

		DrawStringEx(450 + 120 * i, 480, -1, "%s", enemy->GetName().c_str());
		DrawStringEx(450 + 120 * i, 500, -1, "%d", enemy->GetStatus(0));
		DrawStringEx(450 + 120 * i, 520, -1, "%d", enemy->GetStatus(1));
		DrawStringEx(450 + 120 * i, 540, -1, "%d", enemy->GetStatus(2));
		DrawStringEx(450 + 120 * i, 560, -1, "%d", enemy->GetStatus(3));
		DrawStringEx(450 + 120 * i, 580, -1, "%d", enemy->GetExp());
		++i;
	}

}

void DungeonScene::DrawInventory()
{
	if (gManager->inventories[inventoryPage]->inventoryList.empty())return;
	desc->Menu_Draw();
	SetFontSize(25);
	gManager->inventories[inventoryPage]->DrawInventory(inventory->menu_x, inventory->menu_y);
	gManager->inventories[inventoryPage]->DrawItemData(desc->menu_x + 10, desc->menu_y + 10);

	SetFontSize(16);
}

void DungeonScene::ChangeInventory()
{
	//インベントリを切り替える
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RIGHT)) {
		inventoryPage = (inventoryPage + 1) % (gManager->inventoryNum + 1);
		gManager->inventories[inventoryPage]->CursorReset();
	}
	else if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_LEFT)) {
		inventoryPage = (inventoryPage + (gManager->inventoryNum)) % (gManager->inventoryNum + 1);
		gManager->inventories[inventoryPage]->CursorReset();
	}
	if (gManager->inventories[inventoryPage]->inventoryList.empty())return;
	//上下移動
	gManager->inventories[inventoryPage]->CursorMove();
}

void DungeonScene::SpawnItem(int ItemId)
{
	Item* popItem = gManager->GetItemData(ItemId);
	t2k::Vector3 popPos;
	if (dropItems.empty()) {
		popPos = gManager->SetStartPos(0);
	}
	else {
		//すでにリスト内にあるアイテムのポジションとかぶっていたらもう一度座標を取得する
		while (1) {
			popPos = gManager->SetStartPos(0);
			bool set = false;
			for (auto item : dropItems) {
				t2k::Vector3 alreadySetPos = item->GetPos();
				if (popPos.x == alreadySetPos.x && popPos.y == alreadySetPos.y) {
					continue;
				}
				else {
					set = true;
					break;
				}
			}
			if (set)break;
		}
	}
	popItem->SetPos(popPos);
	dropItems.emplace_back(popItem);
}

void DungeonScene::DrawPopItem()
{
	if (dropItems.empty())return;
	for (auto popItem : dropItems) {
		popItem->DrawPopItem();
	}
}

void DungeonScene::ItemUse(/*int selectNum, Inventory* inventory,*/ int inventoryPage)
{
	int type = itemBuf->getItemData(1);

	//アイテムの処理を行う
	//もし使ったアイテムが回復消費アイテムだったら
	if (type == 0) {
		int manpuku = itemBuf->getItemData(2);
		int heal = itemBuf->getItemData(3);
		player->ChangeBaseStatus(manpuku, heal);
		gManager->addLog(itemBuf->getItemName() + "を使った");
		//インベントリからの消去
		gManager->PopItemFromInventory(inventoryPage);
	}//投擲消費アイテムだったら
	else if (type == 1) {
		//投げるアイテムをpopアイテムとして描画する
		//投げる関数を呼ぶ
		ItemThrow(inventoryPage);


		//投擲アイテムは使うでも投げるでもアイテムを射出する

	}//装備アイテムだったらs
	else if (type == 2 || type == 3) {
		equipItem* item = (equipItem*)itemBuf;
		player->ChangeEquipItem(item);
	}
}

void DungeonScene::ItemThrow(int inventoryPage)
{
	//itemBuf->SetPos(player->pos);

	//Item* throwedItem = itemBuf;
	//ここで新しくitemBufと同じデータでアイテムを生成する
	std::vector<int> intData = itemBuf->GetAllIntData();
	std::vector<string> stringData = itemBuf->GetAllStringData();

	//Item* throwedItem = new Item(intData[0], intData[1], stringData[0], intData[2], intData[3], intData[4], stringData[1], stringData[2]);
	std::shared_ptr<Item>throwedItem = std::make_shared<Item>(intData[0], intData[1], stringData[0], intData[2], intData[3], intData[4], stringData[1], stringData[2]);

	throwedItem->SetPos(player->pos);
	throwedItem->SetGoalPos(player->mydir);
	//throwItem.emplace_back(throwedItem);

	throwedItemList.emplace_back(throwedItem);
	//インベントリからの消去
	gManager->PopItemFromInventory(inventoryPage);
}
void DungeonScene::DeleteDeadEnemy()
{
	auto itr = eManager->liveEnemyList.begin();

	for (int i = 0; i < eManager->liveEnemyList.size(); ++i) {
		if (!(*itr)->isLive) {
			itr = eManager->liveEnemyList.erase(itr);
			--i;
		}
	}

	/*for (auto liveEnemy : eManager->liveEnemyList) {
		if(!liveEnemy->isLive)
	}*/

}

////ログを生成する関数,古い方から消える
//void DungeonScene::addLog(const std::string log)
//{
//	if (!Log[8].empty()) {
//		Log[0] = Log[1];
//		Log[1] = Log[2];
//		Log[2] = Log[3];
//		Log[3] = Log[4];
//		Log[4] = Log[5];
//		Log[5] = Log[6];
//		Log[6] = Log[7];
//		Log[7] = Log[8];
//		Log[8] = log;
//		return;
//	}
//	for (int i = 0; i < 10; i++) {
//
//		if (Log[i].empty()) {
//
//			Log[i] = log;
//			return;
//		}
//	}
//
//}
////生成したログを表示する関数
//void DungeonScene::LogDraw()
//{
//	for (int i = 0; i < 9; ++i) {
//
//		DrawStringEx(log->menu_x + 20, log->menu_y + 40 + (i * 20), -1, "%s", Log[i].c_str());
//	}
//}


