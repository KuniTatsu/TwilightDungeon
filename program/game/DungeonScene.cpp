#include "DungeonScene.h"
#include"Map.h"
#include"GameManager.h"
#include"SoundManager.h"
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
#include"FadeControl.h"
#include"SceneManager.h"

using namespace std;

extern GameManager* gManager;

DungeonScene::DungeonScene()
{
	initDungeonScene();
}

DungeonScene::~DungeonScene()
{
	delete nextLevelWindow;
	delete gameOver;
	delete menuOpen;

	delete shop;
	delete shopDesc;
	//delete inventory;	シーンを変えても所持アイテムのデータは引き継ぎたい　引き継ぎ処理を実装予定:優先度低
	delete log;
	delete desc;
	delete status;	delete playerStatus;
	delete use_usable;
	delete use_equip;
	delete use_nowEquip;
	delete firstMenu;


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
	//debug キャンプシーンへ移動
	if (dungeonClear) {
		SceneManager::ChangeScene(SceneManager::SCENE::CAMP);
		return;
	}

	//マウス位置取得
	GetMousePoint(&mouseX, &mouseY);
	//現在のシークエンスのUpdate処理を実行
	mainSequence.update(gManager->deitatime_);

	//デバッグ
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_C)) {
		ReturnCamp();
		return;
	}

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
			//アイテムをすでに拾ってなければ
			if (itemGetFlag) {
				gManager->AddItemToInventory(item->GetItemId(), gManager->inventories, gManager->inventoryNum);
				item->SetIsLiveFalse();
				itemGetFlag = false;
			}
		}
	}

	for (auto item : dropItems) {
		if (item->GetIsLive())continue;
		if (gManager->PopDetectItem(item, dropItems))break;
	}
	UpdateAnimation();
	CheckAnimLive();

	if (gManager->player->GetStatus(0) <= 0) {
		WhenDeadPlayer();
	}
}

void DungeonScene::Draw()
{
	//階層間のフェード中でなければ描画する
	if (nowSeq != sequence::FADEDESC) {
		gManager->MapDraw();
		//ドロップしているアイテムの描画
		DrawPopItem();
		//プレイヤーの描画
		player->Draw();
		//HPバーの描画
		player->HpVarDraw();
		//敵の描画
		for (auto enemy : eManager->liveEnemyList) {
			enemy->Draw();
		}
		//ミニマップの描画
		gManager->MiniMapDraw();
		//ミニマップの敵の描画
		DrawMiniEnemy();

		//debugMode
		if (gManager->isDebug) {
			DrawStringEx(100, 280, -1, "現在の階層:%d", dungeonLevel);

			DrawStringEx(100, 300, -1, "PlayerMapChipX:%d", (int)playerPos.x);
			DrawStringEx(100, 320, -1, "PlayerMapChipY:%d", (int)playerPos.y);
			DrawEnemyData();
		}

		//プレイヤーの位置が階段の上ならウィンドウを表示する
		if (gManager->GetMapChip(playerPos) == 3) {
			nextLevelWindow->Menu_Draw();

			DrawStringEx(nextLevelWindow->menu_x + 60, nextLevelWindow->menu_y + 60, -1, "階段を見つけた");

			DrawStringEx(nextLevelWindow->menu_x + 60, nextLevelWindow->menu_y + 100, -1, "次の階へ進みますか？");

			DrawStringEx(nextLevelWindow->menu_x + 60, nextLevelWindow->menu_y + 140, -1, "Enterで次の階へ");

		}
		//プレイヤーがショップの上にいるならウィンドウを表示する
		else if (gManager->GetMapChip(playerPos) == 4&&nowSeq!=sequence::SHOP) {
			shopIn->Menu_Draw();
			DrawStringEx(shopIn->menu_x + 70, shopIn->menu_y + 60, -1, "ショップを見つけた");

			DrawStringEx(shopIn->menu_x + 70, shopIn->menu_y + 100, -1, "売り物を見ますか？");

			SetFontSize(30);
			DrawStringEx(shopIn->menu_x + 20, shopIn->menu_y + 140, -1, "Enterでショップを見る");
			SetFontSize(16);

		}
		//アニメーションリストの描画
		DrawAnimation();

		if (gManager->isDebug) {
			//今のシークエンス名を表示する
			DrawNowSequence(nowSeq);
		}
	}
	//ここから下はシークエンスごとの描画

	firstMenu->All();
	//フェード中以外で描画 画面左上に"メニューを開く"UIを表示
	if (nowSeq == sequence::MAIN || nowSeq == sequence::ENEMYACT || nowSeq == sequence::PLAYERATTACK || nowSeq == sequence::ENEMYATTACK || nowSeq == sequence::ANIMATION) {
		menuOpen->Menu_Draw();
		DrawStringEx(menuOpen->menu_x + 10, menuOpen->menu_y + 10, -1, "Menuを開く");
		DrawStringEx(menuOpen->menu_x + 10, menuOpen->menu_y + menuOpen->menu_height * 5 / 9, -1, "Press");
		DrawRotaGraph(menuOpen->menu_x + menuOpen->menu_width - 20, menuOpen->menu_y + menuOpen->menu_height * 2 / 3, 1, 0, EButton, false);

		//説明画像描画
		gManager->DrawHowTo();
	}
	//インベントリを開いている時に描画
	else if (nowSeq == sequence::INVENTORY_OPEN || nowSeq == sequence::INVENTORY_USE) {
		inventory->Menu_Draw();
		DrawInventory(inventory->menu_x, inventory->menu_y);
		if (nowSeq == sequence::INVENTORY_USE) {
			if (usetype == USABLE)use_usable->All();
			else if (usetype == EQUIP)use_equip->All();
			else if (usetype == NOWEQUIP)use_nowEquip->All();
		}
	}
	//アイテムが投げられ、飛んでいる時に描画
	else if (nowSeq == sequence::THROWITEMMOVE) {
		if (!throwedItemList.empty()) {
			for (auto item : throwedItemList) {
				item->DrawThrowItem();
			}
		}
	}
	//フェード中に描画
	else if (nowSeq == sequence::FADEDESC) {
		SetFontSize(30);
		DrawFadeDesc();
		SetFontSize(16);
	}
	else if (nowSeq == sequence::SHOP) {
		//ショップインベントリの描画
		shop->Menu_Draw();

		//文字サイズ変更
		SetFontSize(25);
		//ショップインベントリ内容の描画
		shopPages[currentDrawPage]->DrawInventory(shop->menu_x + 10, shop->menu_y + 10);
		//文字サイズ変更
		SetFontSize(16);

		//必要コインの描画
		shopCoin->Menu_Draw();
		shopPages[currentDrawPage]->DrawNeedCoin(shopCoin->menu_x, shopCoin->menu_y);
		//アイテムステータスの比較表示
		shopDesc->Menu_Draw();
		SetFontSize(23);
		DrawStringEx(shopDesc->menu_x + 10, shopDesc->menu_y + 10, -1, "現在装備中の装備と比較");
		shopPages[currentDrawPage]->DrawEquipItemStatus(shopDesc->menu_x + 10, shopDesc->menu_y + 50);
		SetFontSize(16);
		//自分のインベントリ表示
		shopMyInv->Menu_Draw();
		DrawInventory(shopMyInv->menu_x, shopMyInv->menu_y);
	}
	if (nowSeq == sequence::FADEDESC)return;
	//ログの背景描画
	log->Menu_Draw();
	//ログの描画
	gManager->LogDraw(log->menu_x, log->menu_y);
	//プレイヤーステータス背景の描画
	playerStatus->Menu_Draw();
	//プレイヤーステータスの描画
	player->DrawPlayerStatus(playerStatus->menu_x, playerStatus->menu_y, playerStatus->menu_width, playerStatus->menu_height);

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

	//ショップアイテムをすべてdeleteする
	for (int i = 0; i < shopPages.size(); ++i) {
		shopPages[i]->inventoryList.clear();
	}

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

void DungeonScene::initDungeonScene()
{
	EButton = gManager->LoadGraphEx("graphics/button_E.png");

	miniEnemy = gManager->LoadGraphEx("graphics/mini_Enemy.png");

	//*************UI関連のインスタンス確保***************
	nextLevelWindow = new Menu(300, 300, 300, 200, "graphics/WindowBase_01.png");
	gameOver = new Menu(100, 100, 924, 668, "graphics/WindowBase_01.png");

	shopIn = new Menu(300, 300, 380, 200, "graphics/WindowBase_01.png");
	shopMyInv = new Menu(60, 90, 320, 340, "graphics/WindowBase_01.png");
	shop = new Menu(380, 90, 320, 340, "graphics/WindowBase_01.png");
	shopDesc = new Menu(720, 90, 300, 180, "graphics/WindowBase_01.png");
	shopCoin = new Menu(720, 280, 300, 200, "graphics/WindowBase_01.png");

	menuOpen = new Menu(20, 20, 100, 100, "graphics/WindowBase_01.png");

	inventory = new Menu(255, 50, 420, 340, "graphics/WindowBase_01.png");
	desc = new Menu(680, 300, 320, 90, "graphics/WindowBase_01.png");

	log = new Menu(12, 560, 1000, 200, "graphics/WindowBase_01.png");
	status = new Menu(680, 100, 320, 190, "graphics/WindowBase_01.png");
	playerStatus = new Menu(512, 560, 500, 200, "graphics/WindowBase_01.png");

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
	use_equip = new MenuWindow(640, 440, 120, 100, "graphics/WindowBase_02.png", menu_equip, 3, 0.2);

	MenuWindow::MenuElement_t* menu_nowEquip = new MenuWindow::MenuElement_t[]{
		{670,450,"はずす",0},
		{670,480,"投げる",1},
		{670,510,"やめる",2}
	};
	use_nowEquip = new MenuWindow(640, 440, 90, 100, "graphics/WindowBase_02.png", menu_nowEquip, 3, 0.15);




	MenuWindow::MenuElement_t* menu_0 = new MenuWindow::MenuElement_t[]{
		{70,80,"持ち物",0},
		{70,110,"足元",1},
		{70,140,"セーブ",2},
		{70,170,"タイトルへ戻る",3},
		{70,200,"Esc|メニューを閉じる",4}
	};
	// メニューウィンドウのインスタンス化
	firstMenu = new MenuWindow(30, 50, 220, 210, "graphics/WindowBase_02.png", menu_0, 5, 0.45);

	//**********************************************************************//

	eManager = std::make_shared<EnemyManager>();
	//playerのインスタンスがなければ生成(debug)
	gManager->MakePlayer(GameManager::SpawnScene::Dungeon);
	//敵をランダムで生成し配置
	RandEnemyCreate(5);

	//アイテムをランダムに生成し配置
	for (int i = 0; i < 5; ++i) {
		int random = rand() % gManager->GetItemNum();
		SpawnItem(random);
	}
	//playerのポインタを取得
	player = gManager->GetPlayer();
	gManager->RunDungeonBgm();
	dungeonClear = false;
	//ショップ用インベントリを生成する
	if (shopPages.empty()) {
		//新しくinventoryのインスタンスを生成する
		Inventory* newPage = new Inventory(shopPage + 1);
		//ショップのページを登録
		shopPages.emplace_back(newPage);
	}
}

void DungeonScene::DrawAnimation()
{
	if (drawAnimationList.empty())return;
	auto itr = drawAnimationList.front();
	itr->Draw();

	/*for (auto anim : drawAnimationList) {
		anim->Draw();
	}*/
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
	//ショップシークエンスに移動
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_S)) {
		ChangeSequence(sequence::SHOP);
		return true;
	}

	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_L)) {
		player->LevelUp();
		//もしレベルアップしたら

			//Animationクラスをnew
		std::shared_ptr<Animation>anim = std::make_shared<Animation>("graphics/levelUpEffect_new.png", player->pos);
		//描画リストに登録
		drawAnimationList.emplace_back(anim);

	}
	//

	//menuを開く
	//もしmenuボタンの上にマウスがいたら
	if (gManager->CheckMousePointToRect(mouseX, mouseY, menuOpen->menu_x, menuOpen->menu_width, menuOpen->menu_y, menuOpen->menu_height)) {
		//マウスクリックしていたら
		if (t2k::Input::isMouseTrigger(t2k::Input::MOUSE_RELEASED_LEFT)) {
			t2k::debugTrace("\n押されたよ\n");//ok
			gManager->sound->System_Play(gManager->sound->system_select);
			firstMenu->Open();
			ChangeSequence(sequence::FIRSTMENU);
			return true;
		}
	}
	//Eを押してもメニューが開く
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_E)) {
		gManager->sound->System_Play(gManager->sound->system_select);
		firstMenu->Open();
		ChangeSequence(sequence::FIRSTMENU);
		return true;
	}



	//足踏み(Playerは移動せずにターンは経過する
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_SPACE)) {
		player->skip = true;
	}
	//階段,ショップの上にプレイヤーがいるかどうか確認
	if (CheckExtraOnTile())return true;

	//左クリックかRボタンで攻撃
	//プレイヤーの攻撃は目の前に対象がいなかった場合skipと同じ処理を行う
	if (t2k::Input::isMouseTrigger(t2k::Input::MOUSE_RELEASED_LEFT) || t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_R)) {
		//ここをアニメーションシークエンスに飛ばす

		ChangeSequence(sequence::ANIMATION);
		return true;

	}

	//もしPlayerが動いたら もしくはスキップしたら
	if (gManager->player->Move() || player->skip) {


		itemGetFlag = true;
		playerPos = gManager->WorldToLocal(gManager->player->pos);
		//部屋の中に入ったならミニマップの部屋を表示させる
		gManager->UpdateMiniMap(playerPos);



		ChangeSequence(sequence::ENEMYACT);
		if (player->skip)player->skip = false;
		return true;
	}

	return true;
}

bool DungeonScene::SeqPlayerAttack(const float deltatime)
{
	player->Attack();
	//死亡チェック
	for (auto enemy : eManager->liveEnemyList) {
		if (enemy->GetStatus(0) <= 0) {
			//もしレベルアップしたら
			if (player->AddExp(enemy->GetExp())) {
				//Animationクラスをnew
				std::shared_ptr<Animation>anim = std::make_shared<Animation>("graphics/levelUpEffect_new.png", player->pos);
				//描画リストに登録
				drawAnimationList.emplace_back(anim);
			}
			//コインのドロップ
			player->ChangeHaveCoin(DROPCOIN);

			//アイテムのポップ判定
			int odds = rand() % 100;
			if (odds < DROPODDS) {
				//落ちるアイテムの判定
				int random = rand() % gManager->GetItemNum();
				//SpawnItem(random);
				t2k::Vector3 dropPos = enemy->pos;
				DropItem(random, dropPos);
			}
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

void DungeonScene::ReturnCamp()
{
	gManager->StopBgm();
	player->pos = gManager->SpawnPlayerCamp();
	SceneManager::ChangeScene(SceneManager::CAMP);
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
				attackEnemies.emplace_back(liveEnemy);
			}
			else {
				//動く関数は全て同時でいい
				liveEnemy->Move();
			}
		}
		itr = attackEnemies.begin();
	}
	//攻撃する敵リストがからじゃないなら
	if (!attackEnemies.empty()) {

		//アニメーションシークエンスに飛ばす
		ChangeSequence(sequence::ANIMATION);
		return true;


	}
	ChangeSequence(sequence::MAIN);
	return true;
}

bool DungeonScene::SeqEnemyAttack(const float deltatime)
{
	//もし敵が一体だけならインターバルを0にする
	if (attackEnemies.size() == 1)enemyActTimer = ENEMYACTINTERVAL;
	//一体ずつ攻撃させるためのインターバル計測
	if (++enemyActTimer > ENEMYACTINTERVAL) {
		(*itr)->Attack();
		enemyActTimer = 0;
		itr = attackEnemies.erase(itr);
	}
	//すべての敵が攻撃し終えるまでこのシークエンスを出ない
	if (!attackEnemies.empty())return true;


	//すべての攻撃を終えたらMainSequenceに戻る
	ChangeSequence(sequence::MAIN);
	return true;
}

bool DungeonScene::SeqFirstMenu(const float deltatime)
{
	//インベントリを開く
	if (firstMenu->SelectNum == 0 && t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
		gManager->sound->System_Play(gManager->sound->system_select);
		//menuの上下を操作出来なくする
		firstMenu->manageSelectFlag = false;
		//gManager->sound->System_Play(gManager->sound->system_select);

		//InventoryOpenシークエンスに移動する
		ChangeSequence(sequence::INVENTORY_OPEN);
		return true;
	}
	//メニューを閉じる
	else if (firstMenu->SelectNum == 4 && t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
		gManager->sound->System_Play(gManager->sound->system_cancel);
		firstMenu->menu_live = false;
		ChangeSequence(sequence::MAIN);
		return true;
	}

	//Escキーでもメニューを閉じる
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_ESCAPE))
	{
		gManager->sound->System_Play(gManager->sound->system_cancel);
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
		gManager->sound->System_Play(gManager->sound->system_cancel);
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
		gManager->sound->System_Play(gManager->sound->system_select);
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
		gManager->sound->System_Play(gManager->sound->system_cancel);
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
			gManager->sound->System_Play(gManager->sound->system_select);
			ItemUse(drawInventoryPage);
			use_usable->menu_live = false;
			itemBuf = nullptr;
			ChangeSequence(sequence::INVENTORY_OPEN);
			return true;
		}
		//投げるでEnterを押したら
		else if (use_usable->SelectNum == 1 && t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
			gManager->sound->System_Play(gManager->sound->system_select);
			firstMenu->menu_live = false;
			ItemThrow(drawInventoryPage);
			ChangeSequence(sequence::THROWITEMMOVE);
			return true;
		}
		//やめるでEnterを押したら
		else if (use_usable->SelectNum == 2 && t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
			gManager->sound->System_Play(gManager->sound->system_cancel);
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
				gManager->sound->System_Play(gManager->sound->system_select);
				ItemUse(drawInventoryPage);
				use_equip->menu_live = false;
				itemBuf = nullptr;
				ChangeSequence(sequence::INVENTORY_OPEN);
				return true;
			}
			//装備している　かつ　投げるでEnterを押したら
			else if (use_nowEquip->SelectNum == 1 && t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
				gManager->sound->System_Play(gManager->sound->system_cancel);
				gManager->addLog("装備中のアイテムは投げられません");
				ChangeSequence(sequence::INVENTORY_OPEN);
				return true;
			}
			//やめるでEnterを押したら
			else if (use_nowEquip->SelectNum == 2 && t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
				gManager->sound->System_Play(gManager->sound->system_cancel);
				use_equip->menu_live = false;
				itemBuf = nullptr;
				ChangeSequence(sequence::INVENTORY_OPEN);
				return true;
			}
		}
		else {
			//使うでEnterを押したら
			if (use_equip->SelectNum == 0 && t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
				gManager->sound->System_Play(gManager->sound->system_select);
				ItemUse(drawInventoryPage);
				use_equip->menu_live = false;
				itemBuf = nullptr;
				ChangeSequence(sequence::INVENTORY_OPEN);
				return true;
			}//投げるでEnterを押したら
			//装備していない　かつ　投げるでEnterを押したら
			else if (use_equip->SelectNum == 1 && t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
				gManager->sound->System_Play(gManager->sound->system_select);
				firstMenu->menu_live = false;
				ItemThrow(drawInventoryPage);
				ChangeSequence(sequence::THROWITEMMOVE);
				return true;
			}
			//やめるでEnterを押したら
			else if (use_equip->SelectNum == 2 && t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
				gManager->sound->System_Play(gManager->sound->system_cancel);
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


void DungeonScene::DrawMiniEnemy()
{

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	for (auto liveEnemy : eManager->liveEnemyList) {
		t2k::Vector3 localPos = gManager->WorldToLocal(liveEnemy->pos);
		if (gManager->CheckCanDraw(localPos)) {
			//ミニマップにプレイヤーの位置を描画
			DrawRotaGraph(localPos.x * 10 + 150, localPos.y * 10 + 50, 0.5, 0, miniEnemy, true);
		}
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

bool DungeonScene::SeqAnimation(const float deltatime)
{
	//アニメーションさせる関数を実行する
	//アニメーションが終わり次第誰から呼ばれたかを確認してシークエンスを移動する
	//プレイヤーの攻撃なら
	if (lastSeq == sequence::MAIN) {
		//最初のフレームのみ実行
		if (mainSequence.isStart()) {
			gManager->sound->System_Play(gManager->sound->system_attack);
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
		if (mainSequence.isStart()) {
			gManager->sound->System_Play(gManager->sound->system_attack);
			//アニメーション画像の最大Index番号の取得
			int index = gManager->GetMaxIndex(GameManager::index::ATTACK);
			for (auto attackEnemy : attackEnemies) {
				//Animationクラスをnew
				std::shared_ptr<Animation>anim = std::make_shared<Animation>("graphics/AttackAnim_30.png", player->pos, ATTACKEFFECTSPEED, index);
				//描画リストに登録
				drawAnimationList.emplace_back(anim);
			}
		}


		if (drawAnimationList.empty()) {
			//もしアニメーションが終わっているなら
			ChangeSequence(sequence::ENEMYATTACK);
			return true;
		}
	}

	return true;
}


//階層移動時などに文字を描画するシークエンス
bool DungeonScene::SeqDescFade(const float deltatime)
{
	//文字描画時間が終わるまでフェードアウトを始めない

		//***フェードイン処理
	if (gManager->fControl->doneFade && descFadeCount < DESCFADETIME) {
		gManager->fControl->FadeIn();
		return true;
	}
	//***
	//文字描画時間の更新
	descFadeCount += deltatime;

	if (descFadeCount < DESCFADETIME)return true;

	if (!gManager->fControl->doneFade) {
		gManager->fControl->FadeOut();
		return true;
	}
	else {
		ChangeSequence(sequence::FADEIN);
		descFadeCount = 0;
		return true;
	}

	return true;
}

//todo
//shopシークエンス中に表示するMenuインスタンスを作る
//Draw関数の中にこのシークエンス中に表示したい インベントリの中身表示関数を実行させる
//ショップのマップチップを用意する


bool DungeonScene::SeqShopMain(const float deltatime)
{
	if (mainSequence.isStart()) {
		if (gManager->GetDoneBuy())return true;
		if (shopPages.empty()) {
			//新しくinventoryのインスタンスを生成する
			Inventory* newPage = new Inventory(shopPage + 1);
			//ショップのページを登録
			shopPages.emplace_back(newPage);
		}
		//ショップページにアイテムを登録(6個,2個,2個で登録)
		SetShopItem(6, static_cast<uint32_t>(ItemType::CONSUME));
		SetShopItem(2, static_cast<uint32_t>(ItemType::WEAPON));
		SetShopItem(2, static_cast<uint32_t>(ItemType::ARMOR));
	}
	//ESCAPEでショップを閉じる
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_ESCAPE)) {

		//ショップを使ったフラグを更新
		gManager->SetDoneBuy();
		//カーソル位置のリセット
		shopPages[currentDrawPage]->CursorReset();

		//ショップを閉じる
		ChangeSequence(sequence::MAIN);
		return true;
	}

	//ページ内のアイテムが空ならreturn
	if (shopPages[currentDrawPage]->inventoryList.empty())return true;
	//ショップアイテムの選択移動
	shopPages[currentDrawPage]->CursorMove();
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
		int cursorNum = shopPages[currentDrawPage]->GetCursorNum();

		//表示中のインベントリの冒頭のアイテムを取得
		auto itr = shopPages[currentDrawPage]->inventoryList.begin();

		//選択されたアイテムまでイテレータ移動
		for (int i = 0; i < cursorNum; ++i) {
			itr++;
		}
		int price = 0;
		if ((*itr)->getItemData(1) >= 2) {
			auto item = static_cast<equipItem*>((*itr));
			price = item->getItemData(10);
		}
		else {
			price = (*itr)->getItemData(5);
		}
		//所持金が足りないときは買わない
		if (player->GetHaveCoin() < price) {
			gManager->addLog("所持金が足りません!");
			return true;
		}

		int itemId = gManager->GetItemId((*itr));
		//インベントリに表示されていたステータスのまま追加する
		//gManager->AddItemToInventory(itemId, gManager->inventories, gManager->inventoryNum, 1);
		gManager->AddItemFromShop((*itr));
		//コインを減らす
		player->ChangeHaveCoin(price * (-1));

		//shopに並んでいる該当アイテムをdeleteする
		delete (*itr);
		itr = shopPages[currentDrawPage]->inventoryList.erase(itr);

		shopPages[currentDrawPage]->SetCursorNum(-1);

	}


	return true;
}

bool DungeonScene::SeqCameraMove(const float deltatime)
{
	gManager->camera->CameraMove();

	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_ESCAPE)) {
		ChangeSequence(sequence::MAIN);
		gManager->CameraReset();
		return true;
	}

	return true;
}
void DungeonScene::DrawFadeDesc() {


	if (mainSequence.isStart()) {
		nowDungeonName = gManager->GetDungeonName(gManager->nowDungeon);
	}
	if (lastSeq == sequence::FADEOUT) {
		DrawStringEx(gManager->WINDOWCENTER.x - OFFSET, gManager->WINDOWCENTER.y, -1, "%s", nowDungeonName.c_str());
		DrawStringEx(gManager->WINDOWCENTER.x - OFFSET, gManager->WINDOWCENTER.y + 50, -1, "%d階", dungeonLevel);
	}
	else {
		DrawStringEx(gManager->WINDOWCENTER.x - OFFSET, gManager->WINDOWCENTER.y, -1, "%s", nowDungeonName.c_str());

	}

}
bool DungeonScene::CheckExtraOnTile()
{
	//踏んでいるタイルが普通の通路だったらreturn false;
	if (gManager->GetMapChip(playerPos) == 1) return false;

	//もしplayerが階段の上にいたら
	//windowを出す
	//enterで次の階へ
	if (gManager->GetMapChip(playerPos) == 3) {

		if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
			gManager->sound->System_Play(gManager->sound->system_select);
			ChangeSequence(sequence::FADEOUT);
			return true;
		}
	}
	//踏んでいるのがショップだったら
	else if (gManager->GetMapChip(playerPos) == 4) {
		//enterでショップシークエンスに入る
		if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
			gManager->sound->System_Play(gManager->sound->system_select);
			ChangeSequence(sequence::SHOP);
			return true;
		}
	}
	return false;
}
bool DungeonScene::SeqFadeIn(const float deltatime)
{
	if (gManager->fControl->doneFade) {
		gManager->fControl->FadeIn();
		return true;
	}
	else {
		ChangeSequence(sequence::MAIN);
		return true;
	}
	return true;
}

bool DungeonScene::SeqFadeOut(const float deltatime)
{
	if (!gManager->fControl->doneFade) {
		gManager->fControl->FadeOut();
		return true;
	}
	else {
		//ゴール階層なら黄昏のかけらを付与してシーンチェンジ
		if (dungeonLevel >= 49) {
			DungeonClear();
			return true;
		}

		//次のmapを生成
		MoveLevel(1);
		playerPos = gManager->WorldToLocal(gManager->player->pos);
		//ChangeSequence(sequence::FADEIN);
		ChangeSequence(sequence::FADEDESC);
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
		mainSequence.change(&DungeonScene::SeqAnimation);
	}
	else if (seq == sequence::FADEIN) {
		mainSequence.change(&DungeonScene::SeqFadeIn);
	}
	else if (seq == sequence::FADEOUT) {
		mainSequence.change(&DungeonScene::SeqFadeOut);
	}
	else if (seq == sequence::FADEDESC) {
		mainSequence.change(&DungeonScene::SeqDescFade);
	}
	else if (seq == sequence::SHOP) {
		mainSequence.change(&DungeonScene::SeqShopMain);
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

void DungeonScene::DrawInventory(int x, int y)
{
	//インベントリが空ならreturn
	if (gManager->inventories[drawInventoryPage]->inventoryList.empty())return;
	//文字サイズ変更
	SetFontSize(25);
	//インベントリ内のアイテムを描画
	gManager->inventories[drawInventoryPage]->DrawInventory(x, y);

	//ショップシークエンスではインベントリ内のアイテムデータは表示しない
	if (nowSeq != sequence::SHOP) {
		//インベントリページ数を描画
		DrawStringEx(x + 300, y + 10, -1, "ページ:%d", gManager->inventories[drawInventoryPage]->GetInventoryNum());
		//アイテム説明描画
		desc->Menu_Draw();
		gManager->inventories[drawInventoryPage]->DrawItemDesc(desc->menu_x + 10, desc->menu_y + 10);
		//ステータス比較描画
		status->Menu_Draw();
		gManager->inventories[drawInventoryPage]->DrawEquipItemStatus(status->menu_x + 10, status->menu_y + 10);
	}
	SetFontSize(16);
}

void DungeonScene::ChangeInventory()
{
	//上下移動
	gManager->inventories[drawInventoryPage]->CursorMove();

	//インベントリが一つだけならreturn
	if (gManager->inventoryNum == 0)return;
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
			//最初のページならやめる
			if (drawInventoryPage == 0) {
				gManager->isDeleteInventory = false;
				return;
			}
			//もしいま描画しているインベントリが最後のページなら描画ページを1減らす
			if (drawInventoryPage == gManager->inventoryNum + 1) {
				drawInventoryPage--;
			}
			//drawInventoryPage--;
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
	std::shared_ptr<Item>throwedItem = std::make_shared<Item>(intData[0], intData[1], stringData[0], intData[2], intData[3], intData[4], intData[5], stringData[1], stringData[2]);

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

void DungeonScene::WhenDeadPlayer()
{
	//プレイヤーとインベントリの初期化
	gManager->PlayerDead();
	//ダンジョンのレベルの初期化
	dungeonLevel = 1;
	//キャンプシーンに戻る
	SceneManager::ChangeScene(SceneManager::SCENE::CAMP);
	return;
}

void DungeonScene::SetShopItem(int SetNum, int ItemType)
{
	//SetNumと同じだけランダムでアイテムを生成しインベントリに入れる
	for (int i = 0; i < SetNum; ++i) {
		//アイテムタイプの中からランダムなアイテムのIdを取得
		int itemId = gManager->GetRandItemData(ItemType);
		//ショップアイテムページにアイテムを追加
		gManager->AddItemToInventory(itemId, shopPages, shopPage);
	}
}

void DungeonScene::DropItem(const int ItemId, const t2k::Vector3 DropPos)
{
	Item* dropItem = gManager->GetItemData(ItemId);

	dropItem->SetPos(DropPos);
	dropItems.emplace_back(dropItem);
}

void DungeonScene::DungeonClear()
{
	//かけら付与
	gManager->SetFragmentNum(1);
	//クリアフラグ
	dungeonClear = true;
	//ログリセット
	gManager->ResetLog();
	player->pos = gManager->SpawnPlayerCamp();
	player->mydir = player->dir::UP;
}


