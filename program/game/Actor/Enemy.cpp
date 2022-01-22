#include "Enemy.h"
#include"DxLib.h"
#include "../GameManager.h"
#include"../Player.h"


extern GameManager* gManager;
Enemy::Enemy(int Id, int Type, std::string Name, int Hp, int Atack, int Defence, int Speed, std::string Gh, int Exp)
{
	id = Id;
	type = Type;
	name = Name;

	hp = Hp;
	atack = Atack;
	defence = Defence;
	speed = Speed;

	LoadDivGraph(Gh.c_str(), 12, 3, 4, 24, 32, gh);

	exp = Exp;
	//while (1) {
	//	pos = gManager->SetStartPos(0);
	//	//他のenemyと同じ座標でなければ抜ける
	//	
	//	
	//}
}

Enemy::~Enemy()
{
}

void Enemy::Move()
{
	//向きに関係なく左に進めるなら進む


	//}
	//キャラの位置がマップ上のどのチップか特定する
	t2k::Vector3 myNowPos = gManager->WorldToLocal(pos);
	if (--moveTimer > 0)return;
	moveTimer = MOVETIME;

	//t2k::Vector3 Left = t2k::Vector3(myNowPos.x - 1, myNowPos.y, 0);
	////左が壁じゃなかったら
	//if (gManager->GetMapChip(Left) != 0) {
	//	//左に進む
	//	pos.x -= 20;
	//	mydir = LEFT;
	//}
	////左が壁だったら
	//else {
	//	//向きを取得する
	//	//2.1 向きが左のとき
	//	//下側を見る
	//	//下も壁だったら上に向かう

	//	//2.2 向きが上のとき
	//	//まっすぐ進めるか見る
	//	//まっすぐがダメなら右に向かう

	//	//2.3 向きが右のとき
	//	//まっすぐ進めるか見る
	//	//まっすぐがダメなら

	//	//2.4 向きが下のとき
	//	//まっすぐ進めるか見る
	//	//まっすぐがダメなら右に向かう
	//}


#if 1
	int myleft = GetMyLeft(mydir);
	//もし自分の向いている向きの左が壁じゃなかったら
	//そっちに進む

	//今向いている向きが右のとき
	//上を確認
	if (myleft == UP) {
		//上が壁じゃなかったらそっちに進む
		t2k::Vector3 nextPos = t2k::Vector3(myNowPos.x, myNowPos.y - 1, 0);
		if (gManager->GetMapChip(nextPos) != 0) {
			pos.y -= 20;
			mydir = UP;
		}
		//壁だったら
		else {
			//前が壁じゃなかったら真っ直ぐ進む
			t2k::Vector3 Front = t2k::Vector3(myNowPos.x + 1, myNowPos.y, 0);
			if (gManager->GetMapChip(Front) != 0) {
				pos.x += 20;
			}
			//前が壁だったら
			else {
				//右を取得
				t2k::Vector3 right = t2k::Vector3(myNowPos.x, myNowPos.y - 1, 0);
				//右が壁なら進まない
				if (gManager->GetMapChip(right) == 0)return;
				//右に進む
				pos.y += 20;
				mydir = DOWN;
			}
		}
	}
	//今向いている向きが下のとき
	//右を確認
	else if (myleft == RIGHT) {
		//右が壁じゃなかったらそっちに進む
		t2k::Vector3 nextPos = t2k::Vector3(myNowPos.x + 1, myNowPos.y, 0);
		if (gManager->GetMapChip(nextPos) != 0) {
			pos.x += 20;
			mydir = RIGHT;
		}
		//壁だったら
		else {
			//前が壁じゃなかったら真っ直ぐ進む
			t2k::Vector3 Front = t2k::Vector3(myNowPos.x, myNowPos.y + 1, 0);
			if (gManager->GetMapChip(Front) != 0) {
				pos.y += 20;
			}
			//前が壁だったら
			else {
				//右を取得
				t2k::Vector3 right = t2k::Vector3(myNowPos.x - 1, myNowPos.y, 0);
				//右が壁なら進まない
				if (gManager->GetMapChip(right) == 0)return;
				//右に進む
				pos.x -= 20;
				mydir = LEFT;
			}
		}
	}
	//今向いている向きが左のとき
	//下を確認
	else if (myleft == DOWN) {
		//下が壁じゃなかったらそっちに進む
		t2k::Vector3 nextPos = t2k::Vector3(myNowPos.x, myNowPos.y + 1, 0);
		if (gManager->GetMapChip(nextPos) != 0) {
			pos.y += 20;
			mydir = DOWN;
		}
		//壁だったら
		else {
			//前が壁じゃなかったら真っ直ぐ進む
			t2k::Vector3 Front = t2k::Vector3(myNowPos.x - 1, myNowPos.y, 0);
			if (gManager->GetMapChip(Front) != 0) {
				pos.x -= 20;
			}
			//前が壁だったら
			else {
				//右を取得
				t2k::Vector3 right = t2k::Vector3(myNowPos.x, myNowPos.y - 1, 0);
				//右が壁なら進まない
				if (gManager->GetMapChip(right) == 0)return;
				//右に進む
				pos.y -= 20;
				mydir = UP;
			}
		}
	}
	//今向いている向きが上のとき
	//左を確認
	else if (myleft == LEFT) {
		//左が壁じゃなかったらそっちに進む
		t2k::Vector3 nextPos = t2k::Vector3(myNowPos.x - 1, myNowPos.y, 0);
		if (gManager->GetMapChip(nextPos) != 0) {
			pos.x -= 20;
			mydir = LEFT;
		}
		//壁だったら
		else {
			//前が壁じゃなかったら真っ直ぐ進む
			t2k::Vector3 Front = t2k::Vector3(myNowPos.x, myNowPos.y - 1, 0);
			if (gManager->GetMapChip(Front) != 0) {
				pos.y -= 20;
			}
			//前が壁だったら
			else {
				//右を取得
				t2k::Vector3 right = t2k::Vector3(myNowPos.x + 1, myNowPos.y, 0);
				//右が壁なら進まない
				if (gManager->GetMapChip(right) == 0)return;
				//右に進む
				pos.x += 20;
				mydir = RIGHT;
			}
		}
	}

#endif

#if 0
	int rand = GetRand(3);

	if (rand == 3) {
		//キャラのチップの左のチップがWALLなら移動しない
		//int hoge = gManager->GetMapChip(myNowPos + t2k::Vector3(-1, 0, 0));
		if ((gManager->GetMapChip(myNowPos + t2k::Vector3(-1, 0, 0))) == 0)return;
		//キャラのチップの左のチップがPASSWAYなら移動する
		mydir = LEFT;
		pos.x -= 20;

	}
	else if (rand == 0) {

		//キャラのチップの左のチップがWALLなら移動しない
		if ((gManager->GetMapChip(myNowPos + t2k::Vector3(0, -1, 0))) == 0)return;
		//キャラのチップの左のチップがPASSWAYなら移動する
		mydir = UP;
		pos.y -= 20;
	}
	else if (rand == 1) {
		//キャラのチップの左のチップがWALLなら移動しない
		if ((gManager->GetMapChip(myNowPos + t2k::Vector3(1, 0, 0))) == 0)return;
		//キャラのチップの左のチップがPASSWAYなら移動する
		mydir = RIGHT;
		pos.x += 20;
	}
	else if (rand == 2) {
		//キャラのチップの左のチップがWALLなら移動しない
		if ((gManager->GetMapChip(myNowPos + t2k::Vector3(0, 1, 0))) == 0)return;
		//キャラのチップの左のチップがPASSWAYなら移動する
		mydir = DOWN;
		pos.y += 20;
	}
#endif
}

int Enemy::GetMyLeft(int MyDir)
{
	//上,右,下,左 0,1,2,3
	//もしいま向いている向きが上だったら
	if (MyDir == 0) {
		//向いている方向からみて左は左側
		return LEFT;
	}
	//右を向いていたら 左は上側
	//下を向いていたら 左は右側
	//左を向いていたら 左は下側

	return (MyDir - 1) % 4;
}


