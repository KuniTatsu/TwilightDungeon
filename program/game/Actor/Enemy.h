#pragma once
#include"Actor.h"
#include<list>
#include<vector>

//// 探索範囲
//const int MH = 21;
//const int MW = 21;

class Enemy :public Actor {

public:
	//id(int)	EnemyType(int)	Enemy_Name(std::string)	HP(int)	Atack(int)	Defence(int)	Speed(int)	Gh(std::string)	Exp(int) level(int)

	Enemy(int Id, int Type, std::string Name, int Hp, int Atack, int Defence, int Speed, std::string Gh, int Exp);
	~Enemy()override;

	void Move()override;

	//今の向きから見て左側の方向を取得する関数
	int GetMyLeft(int MyDir);

private:
	//目的地
	t2k::Vector3 ChasePoint = {};

	//所属する部屋 部屋にいない場合-1
	int roomNum = -1;

	//目的地セットフラグ
	bool isSetChasePoint = false;

	t2k::Vector3 myNowPos = {};
	//設定された目的地に向かう関数
	void MoveChasePoint();

	//dirからみて特定の方向が絶対座標系のどの方向か取得する関数 とりあえず左右だけほしい
	int GetDir(const int dir, const int getDir);

	//dir方向に1進む関数(移動可能判定あり)
	bool MoveToDir(const int dir, t2k::Vector3& nowPos);

	//使用注意
	//dir方向に1進む関数(移動可能判定なし 使用する際は移動可能なことを保証すること)
	void DegradedMoveToDir(const int dir);

	//dirの方向に1進んだマスが移動可能かどうか確認する関数
	bool CheckCanMove(const int dir, const t2k::Vector3 nowPos);


	enum /*class*/ CheckDir {
		UP,
		RIGHT,
		DOWN,
		LEFT
	};

	//自分のdirから見て特定の方向に1進んだマスが移動可能かどうか確認する関数
	bool CheckCanMoveToDir(const int dir, const t2k::Vector3 nowPos, const int checkDir);

	//キャラクターに向かって移動するAI関数
	void MoveToPlayer();


	

	

};
//本当はEnemyClassのメンバにしたい



//*****ここから下は経路探索用のクラス,関数*****//

// 探索範囲
 int MH = 0;
 int MW = 0;



	// マップステータス
enum {
	WALL,      // 壁
	SPACE,     // 空間
	OPEN,	   //探索可能point
	CLOSED,	   //探索終了point
	START,     // スタート enemy自身
	GOAL       // ゴール　player
};

// XYをひとまとめに扱うため
class Point {
public:
	//コンストラクタ
	Point() : x(0), y(0) {}
	//引数コンストラクタ
	Point(int _x, int _y) : x(_x), y(_y) {}
	//各pointの座標
	int x;
	int y;
	//point同士の足し算
	Point operator + (Point p) {
		return Point(x + p.x, y + p.y);
	}
	//point同士で位置関係評価
	bool operator == (Point p) {
		if (x == p.x && y == p.y) return true;
		return false;
	}
};

// 経路探索用ノード
class Node {
public:
	Node() :
		status(SPACE),//未Openな空間で初期化
		cost_real(0),//まだ動いていないので0
		cost_guess(0),//ゴールが決まった時に決定
		score(0),
		parent(nullptr)//スタートはnull
	{}

	Point pos;         // 2次元配列上の座標
	int status;        // OPEN やら CLOSED やら
	int cost_real;     // 実コスト 今までに移動に使った歩数
	int cost_guess;    // 推定コスト ゴールまで最短で進んだ時の歩数
	int score;         // スコア 実コスト+推定コスト
	Node* parent;      // 親ノード( 最後にゴールから辿るため )

	//スコア同士で大小評価
	bool operator < (const Node& node) const {
		return score > node.score;
	}
};



//探索範囲のノード
//探索開始時に自分を中心として20マス範囲を調べる
//Node nodes[MH][MW];

Node** nodes;

//enemyの移動予定リスト Asterの引数に入れる
std::list<Node*>willMove;

// 指定座標が有効な( OPEN 可能な )マップ位置かどうか判定 2次元配列の中身がほしいときは引数に**を使う
bool isEnableMapPosition(Point pos, std::vector<Node*> _nodes);

//オープン済みのノードを格納するvector
//std::vector<Node*>openNodes;
//オープン済みのノードを格納するlist
std::list<Node*>openNodes;


//オープン済みのノードからスコアが一番小さいノードを取得する関数
Node* getSmallScoreNodeFromOpenNodes();

// 経路探索 A*
//bool aster(Node** _nodes, Node* _now, std::list<Node*>* _route);
bool aster(std::vector<Node*> _nodes, Node* _now, std::list<Node*>* _route);