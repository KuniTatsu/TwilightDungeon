#pragma once
#include"../library/t2klib.h"
#include"Actor/Actor.h"

class equipItem;
class Player : public Actor
{
public:
	Player(){}
	Player(const t2k::Vector3& StartPos, float Hp, int Atack, int Defence, int Speed, int ActId);
	~Player();

	//t2k::Vector3 pos = {};
	bool skip = false;


	//�����x��HP�̕ύX�֐�(��ɐH���A�C�e���̎g�p�Ŏg��)
	void ChangeBaseStatus(const int ManpukuMove, const int HpMove);

	//�������̕ύX�֐�
	void ChangeEquipItem(equipItem* item);

	//�������O���֐�
	void RemoveEquipItem(equipItem* item);

	//�����ɂ��X�e�[�^�X�㏸�l���l������֐�
	void GetSumStatusFromEquipment();


	bool Move()override;

	//void Draw()override;
	void HpVarDraw();

	//�o���l�̊l���֐�
	void AddExp(const int num);

	//debug
	void DrawPlayerStatus();
private:

	int p_gh = 0;

	int manpuku = 100;

	int nowExp = 0;
	int nextLevelExp = 100;

	void SetLevelStatus();
	void SetNextExp();

	equipItem* myEquip[6] = { nullptr,nullptr, nullptr, nullptr, nullptr, nullptr };

	const std::string equipName[6] = { "WEAPON","HEAD","CHEST","GLOVE","BOOTS","SHIELD" };
	//hp,attack,defence,speed
	int statuses[4] = { 0,0,0,0 };

	//�˂�����܂ő���t���O
	bool left = false;
	bool up = false;
	bool right = false;
	bool down = false;

	enum Dir {
		LEFT,
		UP,
		RIGHT,
		DOWN
	};

	void DashToDir(const int dir, const t2k::Vector3 mapPos);
	void MoveToDir(const int dir, const t2k::Vector3 mapPos);



};
