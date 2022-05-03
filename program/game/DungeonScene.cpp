#include "DungeonScene.h"
#include"Map.h"
#include"GameManager.h"
#include"SoundManager.h"
#include"../support/Support.h"
#include "Player.h"
#include"MenuWindow.h"
#include"Actor/EnemyManager.h"
#include"Actor/Enemy.h"
#include"Skill.h"
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
	delete topUI;

	delete shop;
	delete shopDesc;
	//delete inventory;	シーンを変えても所持アイテムのデータは引き　引き継ぎ処理を実装予定:優先度低
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
	//gManager->SetLiveEnemyList(eManager->liveEnemyList);
	//y座標でソート
	gManager->SortEntityList();
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

	//イベントの実行

	//エネルギーが溜まっていたらキャンプに戻る
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_C)) {
		//デバッグ
		if (gManager->isDebug) {
			ReturnCamp();
			return;
		}

		if (gManager->IsOverMax()) {
			ReturnCamp();
			return;
		}
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

	//プレイヤーとアイテムの衝突判定
	if (DetectItem()) {
		for (auto item : dropItems) {
			if (item->GetIsLive())continue;
			if (gManager->PopDetectItem(item, dropItems))break;
		}
	}

	//アニメーション更新
	UpdateAnimation();
	//アニメーションの生死判定
	CheckAnimLive();

	//player生死判定
	if (gManager->player->GetStatus(0) <= 0) {
		WhenDeadPlayer();
	}
}

void DungeonScene::Draw()
{
	//階層間のフェード中でなければ描画する
	if (nowSeq != sequence::FADEDESC) {
		//マップの描画
		gManager->MapDraw();
		//ドロップしているアイテムの描画
		DrawPopItem();
		//playerとenemyの描画
		for (auto actor : gManager->liveEntityList) {
			actor->Draw();
		}

		//HPバーの描画
		player->HpVarDraw();

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
		else if (gManager->GetMapChip(playerPos) == 4 && nowSeq != sequence::SHOP) {
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

		//****画面上部のUI背景描画
		topUI->Menu_Draw();

		DrawTopUI();

		//****

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
	if (!gManager->liveEnemyList.empty()) {
		gManager->liveEnemyList.clear();
	}
	//player以外のentityをactorリストから消去する
	if (!gManager->liveEntityList.empty()) {

		std::list<std::shared_ptr<Actor>>::iterator entity;

		for (entity = gManager->liveEntityList.begin(); entity != gManager->liveEntityList.end(); ) {
			if ((*entity)->GetActId() != 0) {
				entity = gManager->liveEntityList.erase(entity);
				continue;
			}
			entity++;
		}
	}

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
		//int random = rand() % gManager->GetItemNum();
		int random = gManager->GetItemWithWeight(player->GetPlayerLevel());
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
	gManager->ScaleChange();
	gManager->CalcScale();

	EButton = gManager->LoadGraphEx("graphics/button_E.png");
	button_1 = gManager->LoadGraphEx("graphics/button_1.png");
	button_2 = gManager->LoadGraphEx("graphics/button_2.png");
	button_3 = gManager->LoadGraphEx("graphics/button_3.png");

	numButtons[0] = button_1;
	numButtons[1] = button_2;
	numButtons[2] = button_3;

	miniEnemy = gManager->LoadGraphEx("graphics/mini_Enemy.png");


	//std::bind(static_cast<void(&)()>(DungeonScene::InventoryOpen));

	////メニュー関数格納
	//std::bind(&hoge::aaa, this)

	//menues.emplace_back(std::bind(&DungeonScene::InventoryOpen, this));
	//menues.emplace_back(std::bind(&DungeonScene::CheckFootPoint, this));
	//menues.emplace_back(std::bind(&DungeonScene::Save, this));
	//menues.emplace_back(std::bind(&DungeonScene::BackTitle, this));
	//menues.emplace_back(std::bind(&DungeonScene::MenuClose, this));



	//*************UI関連のインスタンス確保***************
	nextLevelWindow = new Menu(300, 300, 300, 200, "graphics/WindowBase_01.png");
	gameOver = new Menu(100, 100, 924, 668, "graphics/WindowBase_01.png");

	shopIn = new Menu(300, 300, 380, 200, "graphics/WindowBase_01.png");
	shopMyInv = new Menu(60, 90, 320, 340, "graphics/WindowBase_01.png");
	shop = new Menu(380, 90, 320, 340, "graphics/WindowBase_01.png");
	shopDesc = new Menu(720, 90, 300, 180, "graphics/WindowBase_01.png");
	shopCoin = new Menu(720, 280, 300, 200, "graphics/WindowBase_01.png");

	menuOpen = new Menu(20, 160, 100, 100, "graphics/WindowBase_01.png");
	topUI = new Menu(10, 10, 800, 150, "graphics/WindowBase_01.png");

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

	//playerのポインタを取得
	player = gManager->GetPlayer();

	//敵をランダムで生成し配置
	RandEnemyCreate(5);

	//アイテムをランダムに生成し配置
	for (int i = 0; i < 5; ++i) {
		//int random = rand() % gManager->GetItemNum();
		int random = gManager->GetItemWithWeight(player->GetPlayerLevel());
		SpawnItem(random);
	}
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


void DungeonScene::SelectItemUseMenu(int SelectNum)
{
	//使うでEnterを押したら
	if (SelectNum == 0) {
		gManager->sound->System_Play(gManager->sound->system_select);
		ItemUse(drawInventoryPage);
		use_usable->menu_live = false;
		itemBuf = nullptr;
		ChangeSequence(sequence::INVENTORY_OPEN);
	}
	//投げるでEnterを押したら
	else if (SelectNum == 1) {
		gManager->sound->System_Play(gManager->sound->system_select);
		firstMenu->menu_live = false;
		ItemThrow(drawInventoryPage);
		ChangeSequence(sequence::THROWITEMMOVE);
	}
	//やめるでEnterを押したら
	else if (SelectNum == 2) {
		gManager->sound->System_Play(gManager->sound->system_cancel);
		use_usable->menu_live = false;
		itemBuf = nullptr;
		ChangeSequence(sequence::INVENTORY_OPEN);
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
		for (auto enemy : gManager->liveEnemyList) {
			enemy->ChangeStatus(2, 10, 0);
		}
	}
	////ショップシークエンスに移動
	//if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_S)) {
	//	ChangeSequence(sequence::SHOP);
	//	return true;
	//}

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
	else
	{
		//スキルの発動チェック
		//スキルが発動したらこのシークエンスをスキップする
		if (ActivateSkillCheck())return true;
	}


	//もしPlayerが動いたら もしくはスキップしたら
	if (gManager->player->Move() || player->skip) {


		itemGetFlag = true;
		playerPos = gManager->WorldToLocal(gManager->player->pos);
		//部屋の中に入ったならミニマップの部屋を表示させる
		gManager->UpdateMiniMap(playerPos);

		//帰還石のエネルギー充填
		gManager->AddEnergyToStone(CHARGEENERGY[static_cast<uint32_t>(CHARGE::WALK)]);

		ChangeSequence(sequence::ENEMYACT);
		if (player->skip)player->skip = false;
		return true;
	}

	return true;
}

bool DungeonScene::SeqPlayerAttack(const float deltatime)
{
	if (lastUseSkill != nullptr) {
		player->SkillAttack(lastUseSkill);
	}
	else {
		player->Attack();
	}
	//敵死亡チェック
	for (auto enemy : gManager->liveEnemyList) {
		if (enemy->GetStatus(0) <= 0) {
			//もしレベルアップしたら
			if (player->AddExp(enemy->GetExp())) {
				//Animationクラスをnew
				std::shared_ptr<Animation>anim = std::make_shared<Animation>("graphics/levelUpEffect_new.png", player->pos);
				//描画リストに登録
				drawAnimationList.emplace_back(anim);
			}
			gManager->addLog(enemy->GetName() + "を倒した!!");
			//コインのドロップ
			player->ChangeHaveCoin(DROPCOIN);

			//帰還石のエネルギー充填
			gManager->AddEnergyToStone(CHARGEENERGY[static_cast<uint32_t>(CHARGE::ENEMY)]);

			//アイテムのポップ判定
			int odds = rand() % 100;
			if (odds < DROPODDS) {
				/*
				//落ちるアイテムの判定
				int random = rand() % gManager->GetItemNum();
				//SpawnItem(random);
				*/

				int random = gManager->GetItemWithWeight(player->GetPlayerLevel());
				t2k::Vector3 dropPos = enemy->pos;
				DropItem(random, dropPos);
			}
			enemy->isLive = false;
		}
	}

	//もしスキル使用後だったら
	if (lastUseSkill != nullptr)lastUseSkill = nullptr;

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
		for (auto liveEnemy : gManager->liveEnemyList) {
			//playerとenemyが隣り合っているならリストにいれて順番に実行する
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

		//y座標でactorをソート
		gManager->SortEntityList();
	}
	/*
	if (mainSequence.getProgressTime() > 1.0f) {
		for (auto liveEnemy : gManager->liveEnemyList) {
			//playerとenemyが隣り合っているならリストにいれて順番に実行する
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

		//y座標でactorをソート
		gManager->SortEntityList();
		*/

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
	////std::functionを使ってメニューを管理したかった
	//if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
	//	//menues[firstMenu->SelectNum] ();
	//	return true;
	//}

	//インベントリを開く
	if (firstMenu->SelectNum == 0 && t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
		gManager->sound->System_Play(gManager->sound->system_select);
		//menuの上下を操作出来なくする
		firstMenu->manageSelectFlag = false;

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
		if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
			SelectItemUseMenu(use_usable->SelectNum);
			return true;
		}
	}
	//装備アイテムなら
	else {
		equipItem* eItem = static_cast<equipItem*> (itemBuf);
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
				//効果音再生
				gManager->sound->System_Play(gManager->sound->system_select);
				//アイテム使用処理
				ItemUse(drawInventoryPage);
				//メニューを閉じる
				use_equip->menu_live = false;
				//アイテムの一時情報を破棄
				itemBuf = nullptr;
				//シークエンスを移動
				ChangeSequence(sequence::INVENTORY_OPEN);
				return true;
			}//投げるでEnterを押したら
			//装備していない　かつ　投げるでEnterを押したら
			else if (use_equip->SelectNum == 1 && t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
				//効果音再生
				gManager->sound->System_Play(gManager->sound->system_select);
				//メニューを閉じる
				firstMenu->menu_live = false;
				//アイテム投擲処理
				ItemThrow(drawInventoryPage);
				//アイテム投擲中シークエンスに移動
				ChangeSequence(sequence::THROWITEMMOVE);
				return true;
			}
			//やめるでEnterを押したら
			else if (use_equip->SelectNum == 2 && t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {
				//効果音再生
				gManager->sound->System_Play(gManager->sound->system_cancel);
				//メニューを閉じる
				use_equip->menu_live = false;
				//アイテムの一時情報破棄
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
	for (auto liveEnemy : gManager->liveEnemyList) {
		t2k::Vector3 localPos = gManager->WorldToLocal(liveEnemy->pos);
		if (gManager->CheckCanDraw(localPos)) {
			//ミニマップにプレイヤーの位置を描画
			DrawRotaGraph(localPos.x * 8 + 150, localPos.y * 8 + 130, 0.4, 0, miniEnemy, true);
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

			//スキルならスキルの情報を取得する
			if (lastUseSkill != nullptr) {
				int index = lastUseSkill->GetGraphicAllNum();

				//Animationクラスをnew
				std::shared_ptr<Animation>anim = std::make_shared<Animation>(lastUseSkill->GetGraphicHandle(), player->effectPos, lastUseSkill->GetActSpeed(), index);
				//描画リストに登録
				drawAnimationList.emplace_back(anim);
				return true;
			}

			//アニメーション画像の最大Index番号の取得
			int index = gManager->GetMaxIndex(GameManager::index::ATTACK);
			//Animationクラスをnew
			std::shared_ptr<Animation>anim = std::make_shared<Animation>("graphics/AttackAnim_30.png", player->effectPos, ATTACKEFFECTSPEED, index, 5, 1, 30, 30);
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
				std::shared_ptr<Animation>anim = std::make_shared<Animation>("graphics/AttackAnim_30.png", player->pos, ATTACKEFFECTSPEED, index, 5, 1, 30, 30);
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

bool DungeonScene::ActivateSkillCheck()
{

	//int t[5] = { t2k::Input::KEYBORD_1, t2k::Input::KEYBORD_2 };
	//各キーが押されたか確認
	for (int i = 0; i < 3; ++i) {
		if (t2k::Input::isKeyDownTrigger(static_cast<t2k::Input::eKeys>(skillKeys[i]))) {
			//もしスキルリストが空じゃないなら
			if (!player->GetSkillList().empty()) {
				//スキルを登録
				lastUseSkill = player->GetSkillList()[i];
				//アニメーションシークエンスに飛ばす
				ChangeSequence(sequence::ANIMATION);
				return true;
			}
			//スキルがない状態でスキルを使おうとすると通常攻撃する
			else {
				ChangeSequence(sequence::ANIMATION);
				return true;
			}
		}
	}

	return false;
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
	for (auto enemy : gManager->liveEnemyList) {

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
	//アイテムデータをマスターから取得
	Item* popItem = gManager->GetItemData(ItemId);
	if (popItem == nullptr)return;

	std::vector<int> intData = popItem->GetAllIntData();
	std::vector<std::string> stringData = popItem->GetAllStringData();


	//取得したデータでアイテムを新規生成
	Item* newItem = new Item(intData[0], intData[1], stringData[0], intData[2], intData[3], intData[4], intData[5], stringData[1], stringData[2]);

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
	newItem->SetPos(popPos);
	dropItems.emplace_back(newItem);
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

	//ここで新しくitemBufと同じデータでアイテムを生成する
	std::vector<int> intData = itemBuf->GetAllIntData();
	std::vector<string> stringData = itemBuf->GetAllStringData();

	std::shared_ptr<Item>throwedItem = std::make_shared<Item>(intData[0], intData[1], stringData[0], intData[2], intData[3], intData[4], intData[5], stringData[1], stringData[2]);

	throwedItem->SetPos(player->pos);
	throwedItem->SetGoalPos(player->mydir);

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
	auto itr1 = gManager->liveEnemyList.begin();
	auto itr2 = gManager->liveEntityList.begin();

	for (int i = 0; i < gManager->liveEnemyList.size(); ++i) {
		if ((*itr1)->isLive == false) {
			itr1 = gManager->liveEnemyList.erase(itr1);
		}
		else {
			itr1++;
		}
	}
	for (int i = 0; i < gManager->liveEntityList.size(); ++i) {
		if ((*itr2)->isLive == false) {
			itr2 = gManager->liveEntityList.erase(itr2);
		}
		else {
			itr2++;
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

//firstMenuのselect関数郡
void DungeonScene::InventoryOpen()
{
	t2k::debugTrace("\nmenu1\n");
	//gManager->sound->System_Play(gManager->sound->system_select);
	////menuの上下を操作出来なくする
	//firstMenu->manageSelectFlag = false;

	////InventoryOpenシークエンスに移動する
	//ChangeSequence(sequence::INVENTORY_OPEN);
}

void DungeonScene::CheckFootPoint()
{
	t2k::debugTrace("\nmenu2\n");
}

void DungeonScene::Save()
{
	t2k::debugTrace("\nmenu3\n");
}

void DungeonScene::BackTitle()
{
	t2k::debugTrace("\nmenu4\n");
}

void DungeonScene::MenuClose()
{
	t2k::debugTrace("\nmenu5\n");
	/*gManager->sound->System_Play(gManager->sound->system_cancel);
	firstMenu->menu_live = false;
	ChangeSequence(sequence::MAIN);*/
}
void DungeonScene::menu5Select()
{
	t2k::debugTrace("hoge");
}

bool DungeonScene::DetectItem()
{

	//アイテム当たり判定感知
	for (auto item : dropItems) {
		//アイテムとプレイヤーが重なったら
		if (item->DetectOnPlayer(playerPos)) {
			//アイテムをすでに拾ってなければ
			if (itemGetFlag) {
				gManager->AddItemToInventory(item->GetItemId(), gManager->inventories, gManager->inventoryNum);
				item->SetIsLiveFalse();
				itemGetFlag = false;

				return true;
			}
		}
	}
	return false;
}

void DungeonScene::DropItem(const int ItemId, const t2k::Vector3 DropPos)
{
	Item* dropItem = gManager->GetItemData(ItemId);

	dropItem->SetPos(DropPos);
	dropItems.emplace_back(dropItem);
}

void DungeonScene::DrawTopUI()
{
	std::vector<Skill*>playerSkills = player->GetSkillList();

	DrawStringEx(topUI->menu_x + 300, topUI->menu_y + 15, -1, "スキル");

	for (int i = 0; i < playerSkills.size(); ++i) {
		//スキル名描画
		DrawStringEx(topUI->menu_x + 380 + i * 130, topUI->menu_y + 15, -1, "%s", playerSkills[i]->GetSkillName().c_str());
		//アイコン画像描画
		DrawRotaGraph(topUI->menu_x + 400 + i * 150, topUI->menu_y + topUI->menu_height / 2, 1.5, 0, playerSkills[i]->GetSkillIconGh(), true);
	}
	//キー画像描画
	for (int i = 0; i < 3; ++i) {
		DrawRotaGraph(topUI->menu_x + 370 + i * 130, topUI->menu_y + 23, 1, 0, numButtons[i], false);
	}

	//画面上部のプレイヤーHPバー
	//player->TopHpVarDraw(100, 80);
	player->TopHpVarDraw(topUI->menu_x + 20, topUI->menu_y + 80);

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


