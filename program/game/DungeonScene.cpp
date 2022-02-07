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

	eManager = std::make_shared<EnemyManager>();

	MenuWindow::MenuElement_t* menu_0 = new MenuWindow::MenuElement_t[]{
		{70,80,"持ち物",0},
		{70,110,"足元",1},
		{70,140,"セーブ",2},
		{70,170,"タイトルへ戻る",3},
		{70,200,"Esc|メニューを閉じる",4}
	};
	// メニューウィンドウのインスタンス化
	firstMenu = new MenuWindow(30, 50, 220, 210, "graphics/WindowBase_02.png", menu_0, 5);

	//firstMenu->menu_live = true;
	/*for (int i = 0; i < 5; ++i) {
		int rand = GetRand(100) % 6+100;
		eManager->CreateEnemy(rand);
	}*/
	RandEnemyCreate(5);
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
	gManager->player->Draw();
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

	firstMenu->All();
	if (nowSeq == sequence::MAIN) {
		menuOpen->Menu_Draw();
	}
	else if (nowSeq == sequence::INVENTORY_OPEN) {
		inventory->Menu_Draw();
		DrawInventory();
		
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
		gManager->player->ChangeStatus(1, 50);
		for (auto enemy : eManager->liveEnemyList) {
			enemy->ChangeStatus(2, 10);
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
		}
	}

	//足踏み(Playerは移動せずにターンは経過する
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_SPACE)) {
		skip = true;
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
	////enemyの移動インターバル更新
	//for (auto hoge : eManager->liveEnemyList) {
	//	hoge->TimeUpdate();
	//}

	//もしPlayerが動いたら もしくはスキップしたら
	if (gManager->player->Move() || skip) {

		playerPos = gManager->WorldToLocal(gManager->player->pos);

		ChangeSequence(sequence::ENEMYACT);
		if (skip)skip = false;
		return true;
	}

	return true;
}

bool DungeonScene::Seq_EnemyAct(const float deltatime)
{
	//enemyの移動/攻撃
	for (auto hoge : eManager->liveEnemyList) {
		//playerとenemyが隣り合っているなら
		if (gManager->CheckNearByPlayer(hoge))
		{
			//攻撃関数を呼ぶ
		}
		else {
			//移動関数を呼ぶ
			hoge->Move();
		}
		//hoge->Update();
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
	ChangeInventory();


	return true;
}

bool DungeonScene::Seq_CameraMove(const float deltatime)
{
	gManager->camera->CameraMove();


	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_ESCAPE)) {
		ChangeSequence(sequence::MAIN);
		gManager->CameraMove(gManager->player);
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
	if (gManager->inventories[inventoryPage]->inventory[0] == nullptr)return;
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
	if (gManager->inventories[inventoryPage]->inventory[0] == nullptr)return;
	//上下移動
	gManager->inventories[inventoryPage]->CursorMove();
}




