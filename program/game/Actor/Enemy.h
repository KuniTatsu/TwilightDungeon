#pragma once
#include"Actor.h"
#include<list>
#include<vector>

//// �T���͈�
//const int MH = 21;
//const int MW = 21;

class Enemy :public Actor {

public:
	//id(int)	EnemyType(int)	Enemy_Name(std::string)	HP(int)	Atack(int)	Defence(int)	Speed(int)	Gh(std::string)	Exp(int) level(int)

	Enemy(int Id, int Type, std::string Name, int Hp, int Atack, int Defence, int Speed, std::string Gh, int Exp);
	~Enemy()override;

	void Move()override;

	//���̌������猩�č����̕������擾����֐�
	int GetMyLeft(int MyDir);

private:
	//�ړI�n
	t2k::Vector3 ChasePoint = {};

	//�������镔�� �����ɂ��Ȃ��ꍇ-1
	int roomNum = -1;

	//�ړI�n�Z�b�g�t���O
	bool isSetChasePoint = false;

	t2k::Vector3 myNowPos = {};
	//�ݒ肳�ꂽ�ړI�n�Ɍ������֐�
	void MoveChasePoint();

	//dir����݂ē���̕�������΍��W�n�̂ǂ̕������擾����֐� �Ƃ肠�������E�����ق���
	int GetDir(const int dir, const int getDir);

	//dir������1�i�ފ֐�(�ړ��\���肠��)
	bool MoveToDir(const int dir, t2k::Vector3& nowPos);

	//�g�p����
	//dir������1�i�ފ֐�(�ړ��\����Ȃ� �g�p����ۂ͈ړ��\�Ȃ��Ƃ�ۏ؂��邱��)
	void DegradedMoveToDir(const int dir);

	//dir�̕�����1�i�񂾃}�X���ړ��\���ǂ����m�F����֐�
	bool CheckCanMove(const int dir, const t2k::Vector3 nowPos);


	enum /*class*/ CheckDir {
		UP,
		RIGHT,
		DOWN,
		LEFT
	};

	//������dir���猩�ē���̕�����1�i�񂾃}�X���ړ��\���ǂ����m�F����֐�
	bool CheckCanMoveToDir(const int dir, const t2k::Vector3 nowPos, const int checkDir);

	//�L�����N�^�[�Ɍ������Ĉړ�����AI�֐�
	void MoveToPlayer();


	

	

};
//�{����EnemyClass�̃����o�ɂ�����



//*****�������牺�͌o�H�T���p�̃N���X,�֐�*****//

// �T���͈�
 int MH = 0;
 int MW = 0;



	// �}�b�v�X�e�[�^�X
enum {
	WALL,      // ��
	SPACE,     // ���
	OPEN,	   //�T���\point
	CLOSED,	   //�T���I��point
	START,     // �X�^�[�g enemy���g
	GOAL       // �S�[���@player
};

// XY���ЂƂ܂Ƃ߂Ɉ�������
class Point {
public:
	//�R���X�g���N�^
	Point() : x(0), y(0) {}
	//�����R���X�g���N�^
	Point(int _x, int _y) : x(_x), y(_y) {}
	//�epoint�̍��W
	int x;
	int y;
	//point���m�̑����Z
	Point operator + (Point p) {
		return Point(x + p.x, y + p.y);
	}
	//point���m�ňʒu�֌W�]��
	bool operator == (Point p) {
		if (x == p.x && y == p.y) return true;
		return false;
	}
};

// �o�H�T���p�m�[�h
class Node {
public:
	Node() :
		status(SPACE),//��Open�ȋ�Ԃŏ�����
		cost_real(0),//�܂������Ă��Ȃ��̂�0
		cost_guess(0),//�S�[�������܂������Ɍ���
		score(0),
		parent(nullptr)//�X�^�[�g��null
	{}

	Point pos;         // 2�����z���̍��W
	int status;        // OPEN ��� CLOSED ���
	int cost_real;     // ���R�X�g ���܂łɈړ��Ɏg��������
	int cost_guess;    // ����R�X�g �S�[���܂ōŒZ�Ői�񂾎��̕���
	int score;         // �X�R�A ���R�X�g+����R�X�g
	Node* parent;      // �e�m�[�h( �Ō�ɃS�[������H�邽�� )

	//�X�R�A���m�ő召�]��
	bool operator < (const Node& node) const {
		return score > node.score;
	}
};



//�T���͈͂̃m�[�h
//�T���J�n���Ɏ����𒆐S�Ƃ���20�}�X�͈͂𒲂ׂ�
//Node nodes[MH][MW];

Node** nodes;

//enemy�̈ړ��\�胊�X�g Aster�̈����ɓ����
std::list<Node*>willMove;

// �w����W���L����( OPEN �\�� )�}�b�v�ʒu���ǂ������� 2�����z��̒��g���ق����Ƃ��͈�����**���g��
bool isEnableMapPosition(Point pos, std::vector<Node*> _nodes);

//�I�[�v���ς݂̃m�[�h���i�[����vector
//std::vector<Node*>openNodes;
//�I�[�v���ς݂̃m�[�h���i�[����list
std::list<Node*>openNodes;


//�I�[�v���ς݂̃m�[�h����X�R�A����ԏ������m�[�h���擾����֐�
Node* getSmallScoreNodeFromOpenNodes();

// �o�H�T�� A*
//bool aster(Node** _nodes, Node* _now, std::list<Node*>* _route);
bool aster(std::vector<Node*> _nodes, Node* _now, std::list<Node*>* _route);