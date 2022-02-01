#include "DungeonScene.h"
#include"Map.h"
#include"GameManager.h"

#include"../support/Support.h"
#include "Player.h"
#include"MenuWindow.h"
#include"Actor/EnemyManager.h"
#include"Actor/Enemy.h"
#include"Camera.h"

extern GameManager* gManager;

DungeonScene::DungeonScene()
{
	alfa = gManager->LoadGraphEx("graphics/old/test.png");
	nextLevelWindow = new Menu(300, 300, 300, 200, "graphics/WindowBase_01.png");
	eManager = std::make_shared<EnemyManager>();
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
		if (!eManager->liveEnemyList.empty())eManager->liveEnemyList.clear();
		if (!gManager->hoge.empty())gManager->hoge.clear();
		dungeonLevel++;
		gManager->ReCreate();
		RandEnemyCreate(5);
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
}

bool DungeonScene::Seq_Main(const float deltatime)
{
	//debug
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_ESCAPE)) {
		ChangeSequence(sequence::CAMERA);
		return true;
	}

	//足踏み(Playerは移動せずにターンは経過する
	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_SPACE)) {
		skip = true;
	}

	//毎フレーム取得する必要はなさそう→移動後に確認,エリア移動後に1回だけ確認するように変更したい
	playerPos = gManager->WorldToLocal(gManager->player->pos);

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
	else if (seq == sequence::CAMERA) {
		DrawStringEx(800, 300, -1, "CAMERASequence");
	}
}

void DungeonScene::DrawEnemyData()
{
	int i = 0;
	for (auto enemy : eManager->liveEnemyList) {

		DrawStringEx(450 + 120 * i, 480, -1, "%s", enemy->name.c_str());
		DrawStringEx(450 + 120 * i, 500, -1, "%d", enemy->hp);
		DrawStringEx(450 + 120 * i, 520, -1, "%d", enemy->atack);
		DrawStringEx(450 + 120 * i, 540, -1, "%d", enemy->defence);
		DrawStringEx(450 + 120 * i, 560, -1, "%d", enemy->speed);
		DrawStringEx(450 + 120 * i, 580, -1, "%d", enemy->exp);
		++i;
	}

}


