#include "Enemy.h"
#include"DxLib.h"
#include "../GameManager.h"
#include"../Player.h"
#include"../Map.h"

extern GameManager* gManager;
Enemy::Enemy(int Id, int Type, std::string Name, int Hp, int Atack, int Defence, int Speed, std::string Gh, int Exp, int Floor)
{
	id = Id;
	type = Type;
	name = Name;

	exHp = (Floor - 1) * 10;
	exAtack = (Floor - 1) * 2;
	exDefence = (Floor - 1) * 2;
	exSpeed = (Floor - 1) * 2;
	exExp = (Floor - 1) * 5;

	hp = Hp + exHp;
	atack = Atack + exAtack;
	defence = Defence + exDefence;
	speed = Speed + exSpeed;

	LoadDivGraph(Gh.c_str(), 12, 3, 4, 24, 32, gh);

	exp = Exp + exExp;
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
void Enemy::TimeUpdate()
{
	--moveTimer;
	if (moveTimer < 0)moveTimer = 0;
}
//void Enemy::Update() {
//	main_sequence.update(gManager->deitatime_);
//}
//
//bool Enemy::SeqMove(const float deltatime)
//{
//	//移動処理
//	Move();
//
//	//もし隣にplayerがいるなら攻撃シークエンスに映る
//
//	return true;
//}
//
//bool Enemy::SeqAttack(const float deltatime)
//{
//	//隣のplayerに向かって攻撃するシークエンス
//
//	//攻撃関数を実行
//	//アニメーションを実行
//
//
//
//	return true;
//}
void Enemy::Move()
{
	////動ける状態じゃなければ動かない
	//if (moveTimer > 0)return;
	/*moveTimer = MOVETIME;*/

	//キャラの位置がマップ上のどのチップか特定する
	myNowPos = gManager->WorldToLocal(pos);

	//今いる場所が部屋のどこかなら部屋の番号を取得する
	roomNum = gManager->CheckIsThere(myNowPos);

#if 0
	//enemyとplayerが同じ部屋にいるなら
	if (roomNum == gManager->playerRoomNum) {
		//A*で経路探索
		//経路のlistがn個以上残っていれば行わない
		if (willMove.size() < 5) {
			//経路探索
			MoveToPlayer();
		}
		//willMoveリストの一番初めのNodeに向かう

		//willMoveリストにはNode*型の変数が入っている
		//Node*の変数から中身のPos(マップ座標系)を取得し、目的地にする


		t2k::Vector3 chasePoint = t2k::Vector3(willMove.front()->pos.x, willMove.front()->pos.y, 0);

		//リセット
		ChasePoint = { 0,0,0 };
		//ChasePointの更新
		ChasePoint = chasePoint;
		//移動し終わったらリストの戦闘をpopする
		willMove.pop_front();

		isSetChasePoint = true;
		//MoveChasePoint();
		//return;
	}

#endif
	//目的地がセットされていればそちらへ向かう
	if (isSetChasePoint) {
		MoveChasePoint();
		return;
	}


	//部屋のどこかにいるなら
	if (roomNum != -1) {
		//その部屋の出口の中から自分から一番遠い出口を取得する
		t2k::Vector3 wayPoint = gManager->GetFarPoint(roomNum, myNowPos);

		//CHasePointのリセット
		ChasePoint = (0, 0, 0);
		//取得した出口を目的地にセットする
		ChasePoint = wayPoint;

		isSetChasePoint = true;
		MoveChasePoint();
		return;
	}
	//通路にいるなら
	else {
		//進めるなら自分のdirの方向に進む
		bool canMove = MoveToDir(mydir, myNowPos);
		//もし進めなければdirから見て左側に行けないか確認する
		if (!canMove) {
			//もし左右にすすめるならランダムで進む
			if (CheckCanMoveToDir(mydir, myNowPos, CheckDir::LEFT) && CheckCanMoveToDir(mydir, myNowPos, CheckDir::RIGHT))
			{
				if (GetRand(10) % 2 == 0)DegradedMoveToDir(GetDir(mydir, CheckDir::LEFT));
				else DegradedMoveToDir(GetDir(mydir, CheckDir::RIGHT));
			}
			//左にすすめるか確認
			else if (CheckCanMoveToDir(mydir, myNowPos, CheckDir::LEFT))DegradedMoveToDir(GetDir(mydir, CheckDir::LEFT));
			//もし左に進めないなら右に進む
			else DegradedMoveToDir(GetDir(mydir, CheckDir::RIGHT));
		}
	}


#if 0
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
			mydir = dir::UP;
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
				mydir = dir::DOWN;
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
			mydir = dir::RIGHT;
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
				mydir = dir::LEFT;
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
			mydir = dir::DOWN;
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
				mydir = dir::UP;
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
			mydir = dir::LEFT;
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
				mydir = dir::RIGHT;
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

void Enemy::MoveChasePoint()
{
	//目的地に向かって移動する
	//辺の長い方を先に進む

	//目的地までの距離を計算(絶対値で求める)
	int disX = abs(ChasePoint.x - myNowPos.x);
	int disY = abs(ChasePoint.y - myNowPos.y);

	////もし今の自分の位置と目的地が一致していれば
	//if (myNowPos.x == ChasePoint.x && myNowPos.y == ChasePoint.y) {
	//	//isSetChasePointをfalseにする
	//	isSetChasePoint = false;
	//	//目的地を破棄する
	//	ChasePoint = { 0,0,0 };
	//}

	//xのほうが長い場合
	if (disX > disY) {
		//x方向に進む
		//自分のx座標が目的地より左なら右に進む
		if (myNowPos.x < ChasePoint.x) {
			if (gManager->GetMapChip(myNowPos + t2k::Vector3(1, 0, 0)) == 0)return;
			pos.x += 20;
			mydir = dir::RIGHT;
		}//左に進む
		else {
			if (gManager->GetMapChip(myNowPos + t2k::Vector3(-1, 0, 0)) == 0)return;
			pos.x -= 20;
			mydir = dir::LEFT;
		}
	}//yのほうが長い場合
	else {
		//もしxとyが同じでここに入り
		//目的地に向かって下側か上側が壁の時
		//x方向に進ませる


		//y方向に進む
		//自分のy座標が目的地より上なら下に進む
		if (myNowPos.y < ChasePoint.y) {
			//ひとつ下が壁のとき
			if (gManager->GetMapChip(myNowPos + t2k::Vector3(0, 1, 0)) == 0) {
				//自分のx座標が目的地より左なら右に進む
				if (myNowPos.x < ChasePoint.x) {
					if (gManager->GetMapChip(myNowPos + t2k::Vector3(1, 0, 0)) == 0)return;
					pos.x += 20;
					mydir = dir::RIGHT;
				}//左に進む
				else {
					if (gManager->GetMapChip(myNowPos + t2k::Vector3(-1, 0, 0)) == 0)return;
					pos.x -= 20;
					mydir = dir::LEFT;
				}
				return;
			}
			//if (gManager->GetMapChip(myNowPos + t2k::Vector3(0, 1, 0)) == 0)return;
			pos.y += 20;
			mydir = dir::DOWN;
		}//上に進む
		else {
			//ひとつ上が壁のとき
			if (gManager->GetMapChip(myNowPos + t2k::Vector3(0, -1, 0)) == 0) {
				//自分のx座標が目的地より左なら右に進む
				if (myNowPos.x < ChasePoint.x) {
					if (gManager->GetMapChip(myNowPos + t2k::Vector3(1, 0, 0)) == 0)return;
					pos.x += 20;
					mydir = dir::RIGHT;
				}//左に進む
				else {
					if (gManager->GetMapChip(myNowPos + t2k::Vector3(-1, 0, 0)) == 0)return;
					pos.x -= 20;
					mydir = dir::LEFT;
				}
				return;
			}
			//if (gManager->GetMapChip(myNowPos + t2k::Vector3(0, -1, 0)) == 0)return;
			pos.y -= 20;
			mydir = dir::UP;
		}
	}

	//もし今の自分の位置と目的地が一致していれば
	if (pos.x / 20 == ChasePoint.x && pos.y / 20 == ChasePoint.y) {
		//isSetChasePointをfalseにする
		isSetChasePoint = false;
		//目的地を破棄する
		ChasePoint = { 0,0,0 };
	}

}
int Enemy::GetDir(const int dir, const int getDir)
{
	//今の向きが上
	if (dir == 0) {
		//左は左側
		if (getDir == CheckDir::LEFT)return CheckDir::LEFT;
		//右は右側
		if (getDir == CheckDir::RIGHT)return CheckDir::RIGHT;
	}
	//今の向きが右
	else if (dir == 1) {
		//左は上側
		if (getDir == CheckDir::LEFT)return CheckDir::UP;
		//右は下側
		if (getDir == CheckDir::RIGHT)return CheckDir::DOWN;
	}
	//今の向きが下
	else if (dir == 2) {
		//左は右側
		if (getDir == CheckDir::LEFT)return CheckDir::RIGHT;
		//右は左側
		if (getDir == CheckDir::RIGHT)return CheckDir::LEFT;
	}
	//今の向きが左
	else if (dir == 3) {
		//左は下側
		if (getDir == CheckDir::LEFT)return CheckDir::DOWN;
		//右は上側
		if (getDir == CheckDir::RIGHT)return CheckDir::UP;
	}
	return -1;
}
/*
enum dir{
		UP,
		RIGHT,
		DOWN,
		LEFT
	};
*/
/*
t2k::Vector3 Enemy::MoveToDir(int dir)
{
	//上
	if (dir == 0) {
		return t2k::Vector3(0, -1, 0);
	}
	//右
	else if (dir == 1) {
		return t2k::Vector3(1, 0, 0);
	}
	//下
	else if (dir == 2) {
		return t2k::Vector3(0, 1, 0);
	}
	//左
	else if (dir == 3) {
		return t2k::Vector3(-1, 0, 0);
	}
	return t2k::Vector3(-1, -1, -1);
}
*/

bool Enemy::MoveToDir(const int dir, t2k::Vector3& nowPos)
{
	//dir方向の1マス先が移動不可なら移動しない
	if (!CheckCanMove(dir, nowPos))return false;
	//上
	if (dir == 0) {
		pos.y -= 20;
	}
	//右
	else if (dir == 1) {
		pos.x += 20;
	}
	//下
	else if (dir == 2) {
		pos.y += 20;
	}
	//左
	else if (dir == 3) {
		pos.x -= 20;
	}
	else {
		t2k::debugTrace("\n移動エラー\n");
	}
	return true;
}

void Enemy::DegradedMoveToDir(const int dir)
{
	//上
	if (dir == 0) {
		pos.y -= 20;
		mydir = dir::UP;
	}
	//右
	else if (dir == 1) {
		pos.x += 20;
		mydir = dir::RIGHT;
	}
	//下
	else if (dir == 2) {
		pos.y += 20;
		mydir = dir::DOWN;
	}
	//左
	else if (dir == 3) {
		pos.x -= 20;
		mydir = dir::LEFT;
	}
	else {
		t2k::debugTrace("\n移動エラー\n");
	}
}

bool Enemy::CheckCanMove(const int dir, const t2k::Vector3 nowPos)
{
	t2k::Vector3 nextPos = { 0,0,0 };
	//上
	if (dir == 0) {
		nextPos = nowPos + t2k::Vector3(0, -1, 0);
	}
	//右
	else if (dir == 1) {
		nextPos = nowPos + t2k::Vector3(1, 0, 0);
	}
	//下
	else if (dir == 2) {
		nextPos = nowPos + t2k::Vector3(0, 1, 0);
	}
	//左
	else if (dir == 3) {
		nextPos = nowPos + t2k::Vector3(-1, 0, 0);
	}
	//壁ならfalseを返す
	if (gManager->GetMapChip(nextPos) == 0)return false;

	return true;
}

bool Enemy::CheckCanMoveToDir(const int dir, const t2k::Vector3 nowPos, const int checkDir)
{
	//今向いているのが上の場合
	if (dir == 0) {
		//確認したい方向がdirから見て上の時
		if (checkDir == CheckDir::UP)return CheckCanMove(CheckDir::UP, nowPos);
		//右の時
		if (checkDir == CheckDir::RIGHT)return CheckCanMove(CheckDir::RIGHT, nowPos);
		//下の時
		if (checkDir == CheckDir::DOWN)return CheckCanMove(CheckDir::DOWN, nowPos);
		//左の時
		if (checkDir == CheckDir::LEFT)return CheckCanMove(CheckDir::LEFT, nowPos);
	}
	//今向いているのが右の場合
	else if (dir == 1) {
		//向かって左側は上方向
		if (checkDir == CheckDir::LEFT)return CheckCanMove(CheckDir::UP, nowPos);
		//向かって右側は下方向
		if (checkDir == CheckDir::RIGHT)return CheckCanMove(CheckDir::DOWN, nowPos);
	}
	//今向いているのが下の場合
	else if (dir == 2) {
		//向かって左側は右方向
		if (checkDir == CheckDir::LEFT)return CheckCanMove(CheckDir::RIGHT, nowPos);
		//向かって右側は左方向
		if (checkDir == CheckDir::RIGHT)return CheckCanMove(CheckDir::LEFT, nowPos);
	}
	//今向いているのが左の場合
	else if (dir == 3) {
		//向かって左側は下方向
		if (checkDir == CheckDir::LEFT)return CheckCanMove(CheckDir::DOWN, nowPos);
		//向かって右側は上方向
		if (checkDir == CheckDir::RIGHT)return CheckCanMove(CheckDir::UP, nowPos);
	}

	return false;
}

#if 0

//経路探索
void Enemy::MoveToPlayer()
{
	Point goal;
	Point start;


	if (nodes != nullptr)
	{
		for (int i = 0; i < MH; i++)
		{
			delete[] nodes[i];
		}

		delete[] nodes;
		nodes = nullptr;
	}

	//自分が今いる部屋を取得する →roomNum

	std::vector<std::vector<int>>chips;
	//chipsに今いる部屋の中のchipデータを収納
	gManager->map->GetAllChip(roomNum, chips);
	//部屋の大きさ
	t2k::Vector3 hoge = gManager->GetRoomValue(roomNum);
	MH = hoge.y;
	MW = hoge.x;

	nodes = new Node * [MH];
	for (int i = 0; i < MH; i++)
	{
		nodes[i] = new Node[MW];
	}

	// スタートとゴールの位置を取得
	start = Point(myNowPos.x, myNowPos.y);
	//スタートは自分自身,ゴールはplayer
	//for (int i = 0; i < chips.size(); ++i) {
	//	for (int k = 0; k < chips[i].size(); ++k) {
	//		//2==Playerだったらゴールにする →本当に取れてるか不明
	//		if (2 == chips[i][k]) goal = Point(k, i);
	//	}
	//}

	//プレイヤーの座標から直接ゴールを指定する
	t2k::Vector3 playerPos = gManager->WorldToLocal(gManager->player->pos);
	goal = Point(playerPos.x, playerPos.y);

	// ノードデータの初期設定
	for (int i = 0; i < MH; ++i) {
		for (int k = 0; k < MW; ++k) {
			nodes[i][k].pos = Point(k, i);
			nodes[i][k].status = chips[i][k];
			nodes[i][k].cost_guess = abs((goal.x + goal.y) - (i + k));
		}
	}

	// 二次元配列のアドレスを引数に渡す為の準備
	//Node* tmp_nodes[MH];

	// nodes
	//TODO: 開放すること
	Node** tmp_nodes;
	tmp_nodes = new Node * [MH];
	for (int i = 0; i < MH; i++)
	{
		tmp_nodes[i] = new Node[MW];
		for (int j = 0; j < MW; j++)
		{
			tmp_nodes[i][j] = nodes[i][j];
		}
	}

	// 経路探索実行
	bool is_success = aster(tmp_nodes, &nodes[start.y][start.x], &willMove);

	// false が帰ってきたら到達不能
	if (!is_success) {
		printf("到達不能\n");
		return;
	}
	if (nodes != nullptr)
	{
		for (int i = 0; i < MH; i++)
		{
			delete[] nodes[i];
		}

		delete[] nodes;
		nodes = nullptr;
	}
	if (tmp_nodes != nullptr)
	{
		for (int i = 0; i < MH; i++)
		{
			delete[] tmp_nodes[i];
		}

		delete[] tmp_nodes;
		tmp_nodes = nullptr;
	}
}

//void Enemy::ChangeSequence(sequence seq)
//{
//	nowSeq = seq;
//	if (seq == sequence::MOVE) {
//		main_sequence.change(&Enemy::SeqMove);
//	}
//	else if (seq == sequence::ATTACK) {
//		main_sequence.change(&Enemy::SeqAttack);
//	}
//}

bool isEnableMapPosition(Point pos, Node** const _nodes)
{
	//探索範囲外ならfalse
	if (pos.x < 0) return false;
	if (pos.y < 0) return false;
	if (pos.x >= MW) return false;
	if (pos.y >= MH) return false;
	//スタートは再度調べない
	if (START == _nodes[pos.y][pos.x].status) return false;
	//空間またはゴールなら探索可能
	if (SPACE == _nodes[pos.y][pos.x].status) return true;
	if (GOAL == _nodes[pos.y][pos.x].status) return true;
	//壁は探索不可
	return false;
}
Node* getSmallScoreNodeFromOpenNodes()
{
	Node* p = nullptr;

	std::list<Node*>::iterator itr = openNodes.begin();
	for (auto node : openNodes) {
		//もし検索の最初ならはじめのノードを入れる
		if (nullptr == p) {
			p = *itr;
			++itr;
			continue;
		}
		//もしスコアが小さいものが見つかればpにいれる
		if (p->score > (*itr)->score) p = *itr;
		//もしスコアが同じなら
		else if (p->score == (*itr)->score) {
			//実コストを比べて小さいならpに入れる
			if (p->cost_real > (*itr)->cost_real) p = *itr;
		}
		++itr;
	}
	return p;
}

bool aster(Node** _nodes, Node* _now, std::list<Node*>* _route)
{
	// スタート地点のスコア計算
	if (START == _now->status) {
		_now->score = _now->cost_real + _now->cost_guess;
	}

	// ４方向の座標
	Point dir[4] = { Point(0, 1), Point(1, 0), Point(0, -1), Point(-1, 0) };

	// 周り４方向を調べて可能ならオープン
	for (int i = 0; i < 4; ++i) {
		Point next = _now->pos + dir[i];

		// 調べ先がオープン可能かどうか
		if (!isEnableMapPosition(next, _nodes)) continue;

		// オープン予定の座標がゴールだった
		if (GOAL == _nodes[next.y][next.x].status) {

			// ゴールを保存して
			auto goal = &_nodes[next.y][next.x];
			_route->push_back(&_nodes[next.y][next.x]);

			// ゴール一歩手前から自分の親ノードを遡って記録
			// この記録が最短経路となる
			auto p = _now;
			while (nullptr != p) {
				_route->push_back(p);
				p = p->parent;
			}

			// ゴールが見つかったので true
			return true;
		}

		// ４方向のノードに対するオープンとスコア計算処理
		_nodes[next.y][next.x].status = OPEN;
		//openvectorに格納
		openNodes.emplace_back(_nodes[next.y][next.x]);
		_nodes[next.y][next.x].parent = _now;
		_nodes[next.y][next.x].cost_real = _now->cost_real + 1;
		_nodes[next.y][next.x].score = _nodes[next.y][next.x].cost_real + _nodes[next.y][next.x].cost_guess;
	}

	// 周りのオープンが終わったので自分はクローズ
	if (START != _now->status) {
		_now->status = CLOSED;

		//opennodesのなかから_nowと同じものを見つけてpopする
		for (auto itr = openNodes.begin(); itr != openNodes.end();) {
			if (*itr == _now) {
				itr = openNodes.erase(itr);
				break;
			}
			++itr;
		}
	}

	// 現在オープンしているノードで一番スコアの小さいものが基準ノード
	Node* node = getSmallScoreNodeFromOpenNodes();

	// ノードが見つからなければ到達不能
	if (nullptr == node) return false;

	// 再帰的に調べていく
	return aster(_nodes, node, _route);
}

#endif