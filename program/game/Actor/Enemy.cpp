#include "Enemy.h"
#include"DxLib.h"
#include "../GameManager.h"
#include"../Player.h"
#include"../Map.h"

extern GameManager* gManager;
Enemy::Enemy(int Id, int Type, std::string Name, int Hp, int Attack, int Defence, int Speed, std::string Gh, int Exp, int Floor, int ActId)
{
	actId = ActId;
	id = Id;
	type = Type;
	name = Name;
	//補正値 constにする
	exHp = (Floor - 1) * 10;
	exAttack = (Floor - 1) * 2;
	exDefence = (Floor - 1) * 2;
	exSpeed = (Floor - 1) * 2;
	exExp = (Floor - 1) * 5;

	//csvから読み込む基礎値
	baseHp = Hp;
	baseAttack = Attack;
	baseDefence = Defence;
	baseSpeed = Speed;

	//描画ステータスの算出
	hp = baseHp + exHp;
	attack = baseAttack + exAttack;
	defence = baseDefence + exDefence;
	speed = baseSpeed + exSpeed;

	LoadDivGraph(Gh.c_str(), 12, 3, 4, 24, 32, gh);
	LoadDivGraph("graphics/AttackAnim_30.png", 5, 5, 1, 30, 30, effectGh);

	baseExp = Exp;
	exp = baseExp + exExp;
	nowHp = hp;

	maxMotionIndex = gManager->GetMaxIndex(GameManager::index::ENEMY);
}

Enemy::~Enemy()
{
	t2k::debugTrace("\n敵が死亡しました\n");
}

void Enemy::TimeUpdate()
{
	--moveTimer;
	if (moveTimer < 0)moveTimer = 0;
}

bool Enemy::Move()
{
	//キャラの位置がマップ上のどのチップか特定する
	myNowPos = gManager->WorldToLocal(pos);

	//今いる場所が部屋のどこかなら部屋の番号を取得する
	roomNum = gManager->CheckIsThere(myNowPos);

	//同じ部屋にいるかチェック
	if (roomNum == gManager->playerRoomNum && roomNum != -1) {
		sameRoom = true;
	}
	else {
		sameRoom = false;
	}
	//プレイヤーを見つけていたら
	if (detectFrag) {
		//プレイヤーの座標に向かう
		ChasePoint = gManager->WorldToLocal(gManager->player->pos);
		//chasepointに向かって移動する
		MoveChasePoint();
		//chasepoint到着判定
		CheckDoneChase();
		//追跡範囲判定
		ResetDetectFrag();
		return true;

	}

	//目的地がセットされていればそちらへ向かう
	if (isSetChasePoint && !sameRoom) {
		MoveChasePoint();
		CheckDoneChase();
		return true;
	}
	//同じ部屋にいるなら
	else if (isSetChasePoint && sameRoom) {
		//playerを追尾するようになる
		ChasePoint = gManager->WorldToLocal(gManager->player->pos);
		isSetChasePoint = true;
		detectFrag = true;

		MoveChasePoint();
		CheckDoneChase();
		return true;
	}

#if 0
	//enemyとplayerが同じ部屋にいるかつ通路ではないなら
	if (sameRoom) {


		//A*で経路探索
		//経路のlistがn個以上残っていれば行わない
		if (willMove.size() < 5) {
			//経路探索 今回は部屋内に障害物がないためまずは単純な経路探索を実装する
			MoveToPlayer();
		}
		//willMoveリストの一番初めのNodeに向かう

		//willMoveリストにはNode*型の変数が入っている
		//Node*の変数から中身のPos(マップ座標系)を取得し、目的地にする


		t2k::Vector3 chasePoint = willMove.front();


		//リセット
		ChasePoint = { 0,0,0 };
		//ChasePointの更新
		ChasePoint = chasePoint;
		//移動し終わったらリストの先頭をpopする
		willMove.pop_front();

		isSetChasePoint = true;
		//MoveChasePoint();
		//return;
	}
#endif


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
		return true;
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
				if (rand() % 2 == 0)DegradedMoveToDir(GetDir(mydir, CheckDir::LEFT));
				else DegradedMoveToDir(GetDir(mydir, CheckDir::RIGHT));
			}
			//左にすすめるか確認
			else if (CheckCanMoveToDir(mydir, myNowPos, CheckDir::LEFT))DegradedMoveToDir(GetDir(mydir, CheckDir::LEFT));
			//もし左に進めないなら右を確認して進めるなら進む
			//else DegradedMoveToDir(GetDir(mydir, CheckDir::RIGHT));
			else if (CheckCanMoveToDir(mydir, myNowPos, CheckDir::RIGHT))DegradedMoveToDir(GetDir(mydir, CheckDir::RIGHT));
			//それでも進めないなら
			else {
				//戻る
				DegradedMoveToDir(GetDir(mydir, CheckDir::DOWN));
			}
		}
	}


	return true;
}


void Enemy::MoveChasePoint()
{
	//目的地に向かって移動する
	//辺の長い方を先に進む

	//目的地までの距離を計算(絶対値で求める)
	int disX = abs(ChasePoint.x - myNowPos.x);
	int disY = abs(ChasePoint.y - myNowPos.y);

	//t2k::Vector3 v[4] ;
	//v[dir::RIGHT]	= { 1, 0, 0 };
	//v[dir::LEFT]	= { -1, 0, 0 };
	//v[dir::UP]		= { 0, 1, 0 };
	//v[dir::DOWN]	= { 0, -1, 0 };

	//bool is_not_wall[dir::MAX] ;
	//for (int i = 0; i < dir::MAX; ++i) is_not_wall[i] = gManager->GetMapChip(myNowPos + v[i]);

	//bool is_there_enemy[dir::MAX];
	//for (int i = 0; i < dir::MAX; ++i) is_there_enemy[i] = gManager->CheckIsThereEnemyToDir(myNowPos + v[i]);

	//int Dir = (disX > disY) ? (myNowPos.x < ChasePoint.x) ? dir::RIGHT : dir::LEFT : (myNowPos.y < ChasePoint.y) ? dir::DOWN : dir::UP ;


	//xのほうが長い場合
	if (disX > disY) {
		//x方向に進む
		//自分のx座標が目的地より左なら右に進む
		if (myNowPos.x < ChasePoint.x) {

			//右が壁じゃないかつ敵がいないなら
			if (gManager->GetMapChip(myNowPos + t2k::Vector3(1, 0, 0)) != 0 && !gManager->CheckIsThereEnemyToDir(myNowPos + t2k::Vector3(1, 0, 0))) {
				//左に進む
				pos.x += gManager->nowGraphicSize;
				mydir = dir::RIGHT;
				return;
			}
			//右に進めないなら上下を考える
			else {
				//enemyが上にいるなら→下に進みたい
				if (ChasePoint.y > myNowPos.y) {
					//下に敵がいなければ
					if (!gManager->CheckIsThereEnemyToDir(myNowPos + t2k::Vector3(0, 1, 0))) {
						//壁か敵がいるなら進まない
						if (gManager->GetMapChip(myNowPos + t2k::Vector3(0, 1, 0)) == 0 || gManager->CheckIsThereEnemyToDir(myNowPos + t2k::Vector3(0, 1, 0)))return;
						pos.y += gManager->nowGraphicSize;
						mydir = dir::DOWN;
						return;
					}
					//下もだめなら何もしない
					else {
						return;
					}
				}
				//enemyが下にいるなら→上に進みたい
				else {
					if (!gManager->CheckIsThereEnemyToDir(myNowPos + t2k::Vector3(0, -1, 0))) {
						//壁か敵がいるなら進まない
						if (gManager->GetMapChip(myNowPos + t2k::Vector3(0, -1, 0)) == 0 || gManager->CheckIsThereEnemyToDir(myNowPos + t2k::Vector3(0, -1, 0)))return;
						pos.y -= gManager->nowGraphicSize;
						mydir = dir::UP;
						return;
					}
					//上もだめなら何もしない
					else {
						return;
					}
				}
			}
		}//左に進む
		else {
			//左が壁じゃないかつ敵がいないなら
			if (gManager->GetMapChip(myNowPos + t2k::Vector3(-1, 0, 0)) != 0 && !gManager->CheckIsThereEnemyToDir(myNowPos + t2k::Vector3(-1, 0, 0))) {
				//左に進む
				pos.x -= gManager->nowGraphicSize;
				mydir = dir::LEFT;
				return;
			}
			//左に進めないなら上下を考えるs
			else {
				//enemyが上にいるなら→下に進みたい
				if (ChasePoint.y > myNowPos.y) {
					//下に敵がいなければ
					if (!gManager->CheckIsThereEnemyToDir(myNowPos + t2k::Vector3(0, 1, 0))) {
						//壁か敵がいるなら進まない
						if (gManager->GetMapChip(myNowPos + t2k::Vector3(0, 1, 0)) == 0 || gManager->CheckIsThereEnemyToDir(myNowPos + t2k::Vector3(0, 1, 0)))return;
						pos.y += gManager->nowGraphicSize;
						mydir = dir::DOWN;
						return;
					}
					//下もだめなら何もしない
					else {
						return;
					}
				}
				//enemyが下にいるなら→上に進みたい
				else {
					if (!gManager->CheckIsThereEnemyToDir(myNowPos + t2k::Vector3(0, -1, 0))) {
						//壁か敵がいるなら進まない
						if (gManager->GetMapChip(myNowPos + t2k::Vector3(0, -1, 0)) == 0 || gManager->CheckIsThereEnemyToDir(myNowPos + t2k::Vector3(0, -1, 0)))return;
						pos.y -= gManager->nowGraphicSize;
						mydir = dir::UP;
						return;
					}
					//上もだめなら何もしない
					else {
						return;
					}
				}
			}
			/*pos.x -= gManager->nowGraphicSize;
			mydir = dir::LEFT;*/
		}
	}//yのほうが長い場合
	else {
		//もしxとyが同じでここに入り
		//目的地に向かって下側か上側が壁の時
		//x方向に進ませる


		//y方向に進む
		//自分のy座標が目的地より上なら下に進む
		if (myNowPos.y < ChasePoint.y) {
			//下が壁じゃないかつ敵がいないなら
			if (gManager->GetMapChip(myNowPos + t2k::Vector3(0, 1, 0)) != 0 && !gManager->CheckIsThereEnemyToDir(myNowPos + t2k::Vector3(0, 1, 0))) {
				//左に進む
				pos.y += gManager->nowGraphicSize;
				mydir = dir::DOWN;
				return;
			}
			//下に進めないなら左右を考える
			else {
				//自分のx座標が目的地より左なら右に進む
				if (myNowPos.x < ChasePoint.x) {
					//壁か敵がいるなら何もしない
					if (gManager->GetMapChip(myNowPos + t2k::Vector3(1, 0, 0)) == 0 || gManager->CheckIsThereEnemyToDir(myNowPos + t2k::Vector3(1, 0, 0)))return;

					pos.x += gManager->nowGraphicSize;
					mydir = dir::RIGHT;
					return;
				}//左に進む
				else {
					//壁か敵がいるなら何もしない
					if (gManager->GetMapChip(myNowPos + t2k::Vector3(-1, 0, 0)) == 0 || gManager->CheckIsThereEnemyToDir(myNowPos + t2k::Vector3(-1, 0, 0)))return;

					pos.x -= gManager->nowGraphicSize;
					mydir = dir::LEFT;
					return;
				}

			}
		}
		//上に進む
		else {
			//上が壁じゃないかつ敵がいないなら
			if (gManager->GetMapChip(myNowPos + t2k::Vector3(0, -1, 0)) != 0 && !gManager->CheckIsThereEnemyToDir(myNowPos + t2k::Vector3(0, -1, 0))) {
				//左に進む
				pos.y -= gManager->nowGraphicSize;
				mydir = dir::UP;
				return;
			}
			//上に進めないなら左右を考える
			else {
				//自分のx座標が目的地より左なら右に進む
				if (myNowPos.x < ChasePoint.x) {
					//壁か敵がいるなら何もしない
					if (gManager->GetMapChip(myNowPos + t2k::Vector3(1, 0, 0)) == 0 || gManager->CheckIsThereEnemyToDir(myNowPos + t2k::Vector3(1, 0, 0)))return;

					pos.x += gManager->nowGraphicSize;
					mydir = dir::RIGHT;
					return;
				}//左に進む
				else {
					//壁か敵がいるなら何もしない
					if (gManager->GetMapChip(myNowPos + t2k::Vector3(-1, 0, 0)) == 0 || gManager->CheckIsThereEnemyToDir(myNowPos + t2k::Vector3(-1, 0, 0)))return;

					pos.x -= gManager->nowGraphicSize;
					mydir = dir::LEFT;
					return;
				}

			}
		}
	}


}
int Enemy::GetDir(const int dir, const int getDir)
{
	//今の向きが上
	if (dir == 0) {
		//左は左側
		if (getDir == CheckDir::LEFT)return CheckDir::LEFT;
		//右は右側
		else if (getDir == CheckDir::RIGHT)return CheckDir::RIGHT;

		else if (getDir == CheckDir::DOWN)return CheckDir::DOWN;
	}
	//今の向きが右
	else if (dir == 1) {
		//左は上側
		if (getDir == CheckDir::LEFT)return CheckDir::UP;
		//右は下側
		else if (getDir == CheckDir::RIGHT)return CheckDir::DOWN;
		else if (getDir == CheckDir::DOWN)return CheckDir::LEFT;
	}
	//今の向きが下
	else if (dir == 2) {
		//左は右側
		if (getDir == CheckDir::LEFT)return CheckDir::RIGHT;
		//右は左側
		else if (getDir == CheckDir::RIGHT)return CheckDir::LEFT;
		else if (getDir == CheckDir::DOWN)return CheckDir::UP;
	}
	//今の向きが左
	else if (dir == 3) {
		//左は下側
		if (getDir == CheckDir::LEFT)return CheckDir::DOWN;
		//右は上側
		else if (getDir == CheckDir::RIGHT)return CheckDir::UP;
		else if (getDir == CheckDir::DOWN)return CheckDir::RIGHT;
	}
	return -1;
}

bool Enemy::MoveToDir(const int dir, t2k::Vector3& nowPos)
{
	//dir方向の1マス先が移動不可なら移動しない
	if (!CheckCanMove(dir, nowPos))return false;
	//上
	if (dir == 0) {
		pos.y -= gManager->nowGraphicSize;
	}
	//右
	else if (dir == 1) {
		pos.x += gManager->nowGraphicSize;
	}
	//下
	else if (dir == 2) {
		pos.y += gManager->nowGraphicSize;
	}
	//左
	else if (dir == 3) {
		pos.x -= gManager->nowGraphicSize;
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
		pos.y -= gManager->nowGraphicSize;
		mydir = dir::UP;
	}
	//右
	else if (dir == 1) {
		pos.x += gManager->nowGraphicSize;
		mydir = dir::RIGHT;
	}
	//下
	else if (dir == 2) {
		pos.y += gManager->nowGraphicSize;
		mydir = dir::DOWN;
	}
	//左
	else if (dir == 3) {
		pos.x -= gManager->nowGraphicSize;
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
	if (gManager->GetMapChip(nextPos) == 0 || gManager->CheckIsThereEnemyToDir(nextPos))return false;

	return true;
}

void Enemy::CheckDoneChase()
{
	//もし今の自分の位置と目的地が一致していれば
	if (pos.x / gManager->nowGraphicSize == ChasePoint.x && pos.y / gManager->nowGraphicSize == ChasePoint.y) {
		//isSetChasePointをfalseにする
		isSetChasePoint = false;
		//目的地を破棄する
		ChasePoint = { 0,0,0 };
	}
}

void Enemy::ResetDetectFrag()
{
	t2k::Vector3 PlayerLocalPos = gManager->WorldToLocal(gManager->player->pos);

	float x = abs(PlayerLocalPos.x - myNowPos.x);
	float y = abs(PlayerLocalPos.y - myNowPos.y);

	if (x + y > DETECTRANGE)detectFrag = false;
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


	//部屋の左上の座標を取得→nodes[0][0]
	t2k::Vector3 LeftTop = gManager->GetRoomStartPos(roomNum);
	int x = myNowPos.x - LeftTop.x;
	int y = myNowPos.y - LeftTop.y;

	// スタートとゴールの位置を取得
	start = Point(x, y);

	nodes = new Node * [MH];
	for (int i = 0; i < MH; i++)
	{
		nodes[i] = new Node[MW];
	}


	//スタートは自分自身,ゴールはplayer
	//for (int i = 0; i < chips.size(); ++i) {
	//	for (int k = 0; k < chips[i].size(); ++k) {
	//		//2==Playerだったらゴールにする →本当に取れてるか不明
	//		if (2 == chips[i][k]) goal = Point(k, i);
	//	}
	//}

	//プレイヤーの座標から直接ゴールを指定する
	t2k::Vector3 playerPos = gManager->WorldToLocal(gManager->player->pos);
	int goalx = playerPos.x - LeftTop.x;
	int goaly = playerPos.y - LeftTop.y;
	goal = Point(goalx, goaly);


	// ノードデータの初期設定
	for (int i = 0; i < MH; ++i) {
		for (int k = 0; k < MW; ++k) {
			nodes[i][k].pos = Point(k, i);
			if (k == start.x && i == start.y)nodes[i][k].status = 4;
			else if (k == goal.x && i == goal.y)nodes[i][k].status = 5;
			else nodes[i][k].status = chips[i][k];
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

	auto a = &nodes[start.y][start.x];

	if (a->status == START) {
		int b = 0;
		++b;
	}

	// 経路探索実行
	bool is_success = aster(tmp_nodes, &nodes[start.y][start.x], &willMove, LeftTop);

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

bool Enemy::isEnableMapPosition(Point pos, Node** const _nodes)
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
Enemy::Node* Enemy::getSmallScoreNodeFromOpenNodes()
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

bool Enemy::aster(Node** _nodes, Node* _now, std::list<t2k::Vector3>* _route, t2k::Vector3 LeftTop)
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
			/*_route->push_back(&_nodes[next.y][next.x]);*/
			int x = next.x + LeftTop.x;
			int y = next.y + LeftTop.y;

			(*_route).push_back(t2k::Vector3(x, y, 0));

			// ゴール一歩手前から自分の親ノードを遡って記録
			// この記録が最短経路となる
			auto p = _now;
			while (nullptr != p) {
				/*_route->push_back(p);*/
				int x = p->pos.x + LeftTop.x;
				int y = p->pos.y + LeftTop.y;
				(*_route).push_back(t2k::Vector3(x, y, 0));
				p = p->parent;
			}

			// ゴールが見つかったので true
			return true;
		}

		// ４方向のノードに対するオープンとスコア計算処理
		_nodes[next.y][next.x].status = OPEN;
		//openvectorに格納
		openNodes.emplace_back(&_nodes[next.y][next.x]);
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
	return aster(_nodes, node, _route, LeftTop);
	}

#endif