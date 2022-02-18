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
#include"Item/equipItem.h"
#include"Item/Inventory.h"
#include"Animation.h"

using namespace std;

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

	MenuWindow::MenuElement_t* menu_nowEquip = new MenuWindow::MenuElement_t[]{
		{670,450,"はずす",0},
		{670,480,"投げる",1},
		{670,510,"やめる",2}
	};
	use_nowEquip = new MenuWindow(640, 440, 90, 100, "graphics/WindowBase_02.png", menu_nowEquip, 3, 0.15);


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
		//int rand = GetRand(100) % gManager->GetItemNum();
		int random = rand() % gManager->GetItemNum();
		SpawnItem(random);
	}
	player = gManager->GetPlayer();
}

DungeonScene::~DungeonScene()
{
}

void DungeonScene::RandEnemyCreate(int num)
{
	for (int i = 0; i < num; ++i) {
		int random = rand() % 6 + 100;
		eManager->CreateEnemy(random, dungeonLevel);
	}
	gManager->SetLiveEnemyList(eManager->liveEnemyList);
}

void DungeonScene::Update()
{
	GetMousePoint(&mouseX, &mouseY);

	mainSequence.update(gManager->deitatime_);

	//デバッグ切り替え
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_F2)) {
		if (gManager->isDebug)gManager->isDebug = false;
		else gManager->isDebug = true;
	}

	//デバッグ用マップ再生成
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_F5)) {
		MoveLevel(1);
		return;
	}
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_M)) {
		if (gManager->minimapDraw)gManager->minimapDraw = false;
		else gManager->minimapDraw = true;
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
	UpdateAnimation();
	CheckAnimLive();
}

void DungeonScene::Draw()
{
	gManager->MapDraw();

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
	//wayPointDebug
	//for (auto hoge : gManager->wayPoint) {
	//	for (auto hage : hoge) {
	//		int X = (int)hage.x;
	//		int Y = (int)hage.y;
	//		DrawCircle(X * gManager->GRAPHICSIZE - gManager->camera->cameraPos.x, Y * gManager->GRAPHICSIZE - gManager->camera->cameraPos.y, 10, -1, true);
	//	}
	//}
	//

	//視界システムテスト
	/*SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawRotaGraph(gManager->player->pos.x - gManager->camera->cameraPos.x, gManager->player->pos.y - gManager->camera->cameraPos.y, 1, 0, alfa, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);*/

	//gManager->map->DrawAllRoomPos(gManager->map.)

	DrawNowSequence(nowSeq);

	DrawAnimation();

	//ここから下はシークエンスごとの描画

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
			else if (usetype == NOWEQUIP)use_nowEquip->All();
		}
	}
	else if (nowSeq == sequence::THROWITEMMOVE) {
		if (!throwedItemList.empty()) {
			for (auto item : throwedItemList) {
				item->DrawThrowItem();
			}
		}
	}
	else if (nowSeq == sequence::ANIMATION) {
		
	}
	log->Menu_Draw();
	gManager->LogDraw(log->menu_x, log->menu_y);

	//debug
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
	//空じゃないならenemyを全て消去する
	if (!eManager->liveEnemyList.empty())eManager->liveEnemyList.clear();
	if (!gManager->liveEnemyList.empty())gManager->liveEnemyList.clear();
	//アイテムの消去
	dropItems.clear();

	//ダンジョンの階層を移動する
	dungeonLevel += addLevel;
	//mapの再生成
	gManager->ReCreate();
	//敵の再生成
	RandEnemyCreate(5);
	//アイテムの再生成
	for (int i = 0; i < spawnItemNum; ++i) {
		int random = rand() % gManager->GetItemNum();
		SpawnItem(random);
	}
}

void DungeonScene::UpdateAnimation()
{
	if (drawAnimationList.empty())return;
	for (auto anim : drawAnimationList) {
		anim->Update();
	}
}

void DungeonScene::DrawAnimation()
{
	if (drawAnimationList.empty())return;
	for (auto anim : drawAnimationList) {
		anim->Draw();
	}
}
void DungeonScene::CheckAnimLive()
{
	auto itr = drawAnimationList.begin();
	while (itr != drawAnimationList.end()) {
		if (!(*itr)->GetIsAlive()) {
			itr = drawAnimationList.erase(itr);
			continue;
		}
		++itr;
	}
}



bool DungeonScene::SeqMain(const float deltatime)
{
	//debug
	//カメラ移動モード
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_ESCAPE)) {
		ChangeSequence(sequence::CAMERA);
		return true;
	}
	//ダメージを受ける
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_D)) {
		gManager->player->TakeHpEffect(-20);
	}
	//ステータス上昇
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
			MoveLevel(1);
			return true;
		}
	}
	//左クリックかRボタンで攻撃
	//プレイヤーの攻撃は目の前に対象がいなかった場合skipと同じ処理を行う
	if (t2k::Input::isMouseTrigger(t2k::Input::MOUSE_RELEASED_LEFT) || t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_R)) {
		//ここをアニメーションシークエンスに飛ばす

		ChangeSequence(sequence::ANIMATION);
		return true;

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

bool DungeonScene::SeqPlayerAttack(const float deltatime)
{
	player->Atack();
	//死亡チェック
	for (auto enemy : eManager->liveEnemyList) {
		if (enemy->GetStatus(0) <= 0) {
			player->AddExp(enemy->GetExp());
			enemy->isLive = false;
		}
	}

	DeleteDeadEnemy();
	if (!player->skip) {
		ChangeSequence(sequence::ENEMYACT);
		return true;
	}
	ChangeSequence(sequence::ENEMYACT);
	return true;
}

bool DungeonScene::SeqEnemyAct(const float deltatime)
{
	if (mainSequence.isStart()) {
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

		//アニメーションシークエンスに飛ばす
		ChangeSequence(sequence::ANIMATION);
		return true;

		/*
		//*****enemyAttackSeqに処理を移動

		////もし敵が一体だけならインターバルを0にする
		//if (atackEnemies.size() == 1)enemyActTimer = ENEMYACTINTERVAL;
		////一体ずつ攻撃させるためのインターバル計測
		//if (++enemyActTimer > ENEMYACTINTERVAL) {
		//	(*itr)->Atack();
		//	enemyActTimer = 0;
		//	itr = atackEnemies.erase(itr);
		//}
		////すべての敵が攻撃し終えるまでこのシークエンスを出ない
		//if (!atackEnemies.empty())return true;
		//*****
		*/
	}
	ChangeSequence(sequence::MAIN);
	return true;
}

bool DungeonScene::SeqEnemyAttack(const float deltatime)
{
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


	//すべての攻撃を終えたらMainSequenceに戻る
	ChangeSequence(sequence::MAIN);
	return true;
}

bool DungeonScene::SeqFirstMenu(const float deltatime)
{
	//インベントリを開く
	if (firstMenu->SelectNum == 0 && t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
		//menuの上下を操作出来なくする
		firstMenu->manageSelectFlag = false;
		//gManager->sound->System_Play(gManager->sound->system_select);

		//InventoryOpenシークエンスに移動する
		ChangeSequence(sequence::INVENTORY_OPEN);
		return true;
	}
	//メニューを閉じる
	else if (firstMenu->SelectNum == 4 && t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
		firstMenu->menu_live = false;
		ChangeSequence(sequence::MAIN);
		return true;
	}

	//Escキーでもメニューを閉じる
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_ESCAPE))
	{
		firstMenu->menu_live = false;
		ChangeSequence(sequence::MAIN);
		return true;
	}
	return true;
}

bool DungeonScene::SeqInventoryOpen(const float deltatime)
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
	if (gManager->inventories[drawInventoryPage]->inventoryList.empty())return true;
	//現在選択中のカーソル位置を取得
	int selectNum = gManager->inventories[drawInventoryPage]->GetCursorNum();


	//もしインベントリを開いている時にenterが押されたら
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
		//現在のカーソルの位置のアイテムを取得する
		auto itr = gManager->inventories[drawInventoryPage]->inventoryList.begin();
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
			equipItem* eItem = (equipItem*)itemBuf;
			if (eItem->GetIsEquiped()) {
				use_nowEquip->Open();
				usetype = NOWEQUIP;
				ChangeSequence(sequence::INVENTORY_USE);
				return true;
			}
			use_equip->Open();
			usetype = EQUIP;
			ChangeSequence(sequence::INVENTORY_USE);
			return true;
		}
	}

	return true;
}

bool DungeonScene::SeqInventoryUse(const float deltatime)
{
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_ESCAPE))
	{
		use_usable->menu_live = false;
		use_equip->menu_live = false;
		use_nowEquip->menu_live = false;
		itemBuf = nullptr;
		ChangeSequence(sequence::INVENTORY_OPEN);
		return true;
	}

	int type = itemBuf->getItemData(1);
	//投擲アイテムと分けるべき
	//消費アイテムなら
	if (type < 2) {
		//使うでEnterを押したら
		if (use_usable->SelectNum == 0 && t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
			ItemUse(drawInventoryPage);
			use_usable->menu_live = false;
			itemBuf = nullptr;
			ChangeSequence(sequence::INVENTORY_OPEN);
			return true;
		}
		//投げるでEnterを押したら
		else if (use_usable->SelectNum == 1 && t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
			firstMenu->menu_live = false;
			ItemThrow(drawInventoryPage);
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
		equipItem* eItem = (equipItem*)itemBuf;
		//装備している時
		if (eItem->GetIsEquiped()) {
			//使うでEnterを押したら
			if (use_nowEquip->SelectNum == 0 && t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
				ItemUse(drawInventoryPage);
				use_equip->menu_live = false;
				itemBuf = nullptr;
				ChangeSequence(sequence::INVENTORY_OPEN);
				return true;
			}
			//装備している　かつ　投げるでEnterを押したら
			else if (use_nowEquip->SelectNum == 1 && t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
				gManager->addLog("装備中のアイテムは投げられません");
				ChangeSequence(sequence::INVENTORY_OPEN);
				return true;
			}
			//やめるでEnterを押したら
			else if (use_nowEquip->SelectNum == 2 && t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
				use_equip->menu_live = false;
				itemBuf = nullptr;
				ChangeSequence(sequence::INVENTORY_OPEN);
				return true;
			}
		}
		else {
			//使うでEnterを押したら
			if (use_equip->SelectNum == 0 && t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
				ItemUse(drawInventoryPage);
				use_equip->menu_live = false;
				itemBuf = nullptr;
				ChangeSequence(sequence::INVENTORY_OPEN);
				return true;
			}//投げるでEnterを押したら
			//装備していない　かつ　投げるでEnterを押したら
			else if (use_equip->SelectNum == 1 && t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
				firstMenu->menu_live = false;
				ItemThrow(drawInventoryPage);
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
	}
	return true;
}

bool DungeonScene::SeqThrowItemMove(const float deltatime)
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

bool DungeonScene::SeqAnimationFirst(const float deltatime)
{
	//アニメーションさせる関数を実行する
	//アニメーションが終わり次第誰から呼ばれたかを確認してシークエンスを移動する
	//プレイヤーの攻撃なら
	if (lastSeq == sequence::MAIN) {
		//最初のフレームのみ実行
		if (mainSequence.isStart()) {
			//アニメーションポジションの決定
			player->SetAnimPos();
			//アニメーション画像の最大Index番号の取得
			int index = gManager->GetMaxIndex(GameManager::index::ATTACK);
			//Animationクラスをnew
			std::shared_ptr<Animation>anim = std::make_shared<Animation>("graphics/AttackAnim_30.png", player->effectPos, ATTACKEFFECTSPEED, index);
			//描画リストに登録
			drawAnimationList.emplace_back(anim);
		}
		//もしアニメーションが終わっているなら
		if (drawAnimationList.empty()) {
			//攻撃シークエンスに移動
			ChangeSequence(sequence::PLAYERATTACK);
			return true;
		}
	}
	//エネミーの攻撃なら
	else if (lastSeq == sequence::ENEMYACT) {



		//もしアニメーションが終わっているなら
		ChangeSequence(sequence::ENEMYATTACK);
		return true;
	}

	return true;
}

bool DungeonScene::SeqAnimationSecond(const float deltatime)
{
	return false;
}

bool DungeonScene::SeqCameraMove(const float deltatime)
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
	lastSeq = nowSeq;
	nowSeq = seq;
	if (seq == sequence::MAIN) {
		mainSequence.change(&DungeonScene::SeqMain);
	}
	else if (seq == sequence::PLAYERATTACK) {
		mainSequence.change(&DungeonScene::SeqPlayerAttack);
	}
	else if (seq == sequence::ENEMYACT) {
		mainSequence.change(&DungeonScene::SeqEnemyAct);
	}
	else if (seq == sequence::ENEMYATTACK) {
		mainSequence.change(&DungeonScene::SeqEnemyAttack);
	}
	else if (seq == sequence::FIRSTMENU) {
		mainSequence.change(&DungeonScene::SeqFirstMenu);
	}
	else if (seq == sequence::INVENTORY_OPEN) {
		mainSequence.change(&DungeonScene::SeqInventoryOpen);
	}
	else if (seq == sequence::INVENTORY_USE) {
		mainSequence.change(&DungeonScene::SeqInventoryUse);
	}
	else if (seq == sequence::THROWITEMMOVE) {
		mainSequence.change(&DungeonScene::SeqThrowItemMove);
	}
	else if (seq == sequence::ANIMATION) {
		mainSequence.change(&DungeonScene::SeqAnimationFirst);
	}
	else if (seq == sequence::CAMERA) {
		mainSequence.change(&DungeonScene::SeqCameraMove);
	}

}

void DungeonScene::DrawNowSequence(sequence seq)
{
	if (seq == sequence::MAIN) {
		DrawStringEx(800, 300, -1, "MAINSequence");
	}
	else if (seq == sequence::ENEMYACT) {
		DrawStringEx(800, 300, -1, "PLAYERATTACKSequence");
	}
	else if (seq == sequence::ENEMYACT) {
		DrawStringEx(800, 300, -1, "ENEMYACTSequence");
	}
	else if (seq == sequence::ENEMYACT) {
		DrawStringEx(800, 300, -1, "ENEMYATTACKSequence");
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
	else if (seq == sequence::ANIMATION) {
		DrawStringEx(800, 300, -1, "ANIMATIONSequence");
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
	DrawStringEx(inventory->menu_x + 300, inventory->menu_y + 10, -1, "ページ:%d", gManager->inventories[drawInventoryPage]->GetInventoryNum());
	if (gManager->inventories[drawInventoryPage]->inventoryList.empty())return;
	desc->Menu_Draw();
	SetFontSize(25);
	gManager->inventories[drawInventoryPage]->DrawInventory(inventory->menu_x, inventory->menu_y);
	gManager->inventories[drawInventoryPage]->DrawItemData(desc->menu_x + 10, desc->menu_y + 10);

	SetFontSize(16);
}

void DungeonScene::ChangeInventory()
{
	//インベントリを切り替える
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RIGHT)) {
		drawInventoryPage = (drawInventoryPage + 1) % (gManager->inventoryNum + 1);
		gManager->inventories[drawInventoryPage]->CursorReset();
	}
	else if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_LEFT)) {
		drawInventoryPage = (drawInventoryPage + (gManager->inventoryNum)) % (gManager->inventoryNum + 1);
		gManager->inventories[drawInventoryPage]->CursorReset();
	}
	if (gManager->inventories[drawInventoryPage]->inventoryList.empty())return;
	//上下移動
	gManager->inventories[drawInventoryPage]->CursorMove();
}

void DungeonScene::SpawnItem(int ItemId)
{
	Item* popItem = gManager->GetItemData(ItemId);
	if (popItem == nullptr)return;
	//ポップさせる座標
	t2k::Vector3 popPos;
	//設置済みのアイテムがない
	if (dropItems.empty()) {
		popPos = gManager->SetStartPos(GameManager::setStartPosType::ITEM);
	}
	//すでに設置済みのアイテムがある
	else {
		bool set = false;
		int count = 0;//debug
		//すでにリスト内にあるアイテムのポジションとかぶっていたらもう一度座標を取得する
		while (!set) {
			popPos = gManager->SetStartPos(GameManager::setStartPosType::ITEM);
			//bool set = false;
			//設置済みの全てのアイテムと比べる
			for (auto item : dropItems) {
				t2k::Vector3 alreadySetPos = item->GetPos();
				if (popPos.x == alreadySetPos.x && popPos.y == alreadySetPos.y) {
					t2k::debugTrace("\nポジションかぶり%d回目\n", count);
					count++;
					continue;
				}
				else {
					set = true;
					break;
				}
			}
			//if (set)break;

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
		//ページ内のアイテムを全て消費してページを消去したあとだったら
		if (gManager->isDeleteInventory) {
			//gManager->ForceInventoryChange(inventoryPage);
			if (drawInventoryPage == 0) {
				gManager->isDeleteInventory = false;
				return;
			}
			drawInventoryPage--;
			gManager->isDeleteInventory = false;
		}
	}//投擲消費アイテムだったら
	else if (type == 1) {
		//投げるアイテムをpopアイテムとして描画する
		//投げる関数を呼ぶ
		ItemThrow(inventoryPage);


		//投擲アイテムは使うでも投げるでもアイテムを射出する

	}//装備アイテムだったらs
	else if (type == 2 || type == 3) {
		equipItem* item = (equipItem*)itemBuf;
		if (item->GetIsEquiped()) {
			player->RemoveEquipItem(item);
			return;
		}
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
	//ページ内のアイテムを全て消費してページを消去したあとだったら
	if (gManager->isDeleteInventory) {
		gManager->ForceInventoryChange(inventoryPage);
	}
}
void DungeonScene::DeleteDeadEnemy()
{
	auto itr = eManager->liveEnemyList.begin();

	for (int i = 0; i < eManager->liveEnemyList.size(); ++i) {
		if ((*itr)->isLive == false) {
			itr = eManager->liveEnemyList.erase(itr);
			gManager->SetLiveEnemyList(eManager->liveEnemyList);
		}
		else {
			itr++;
		}
	}
}




