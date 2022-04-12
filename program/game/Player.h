///*****Description*****
///�v���C���[�Ɋւ���N���X
///�v���C���[�̈ړ��Ƒ�����X�e�[�^�X�̕ύX���s��
/// Actor���p�����Ă���B
///*********************

#pragma once
#include"../library/t2klib.h"
#include"Actor/Actor.h"

class equipItem;
class Player : public Actor
{
public:
	Player(){}
	Player(const t2k::Vector3& StartPos, float Hp, int Attack, int Defence, int Speed, int ActId);
	~Player();

	//�����݃t���O
	bool skip = false;

	t2k::Vector3 localPos = {};


	//�����x��HP�̕ύX�֐�(��ɐH���A�C�e���̎g�p�Ŏg��)
	void ChangeBaseStatus(const int ManpukuMove, const int HpMove);

	//�������̕ύX�֐�
	void ChangeEquipItem(equipItem* item);

	//�������O���֐�
	void RemoveEquipItem(equipItem* item);

	//�������Ă��邩�ǂ����Ԃ��֐�
	inline bool CheckEquip(int subId) {
		if (myEquip[subId - 1] != nullptr)return true;
		return false;
	}

	//�����ɂ��X�e�[�^�X�㏸�l���l������֐�
	void GetSumStatusFromEquipment();

	//����̑����̃X�e�[�^�X���擾����֐�
	//arg2:0->hp,1->attack,2->defence,3->speed
	int GetStatusFromEquipment(int subId,int statusType);

	inline t2k::Vector3 GetPlayerLocalPos() {
		return playerInMap;
	}

	void SetPlayerLocalPos();

	//�ړ��֐�
	bool Move()override;
	//Camp�ł̈ړ��֐�
	void TownMove(dir nextDir);
	//HP�o�[�̕`��
	void HpVarDraw();

	//�o���l�̊l���֐�
	bool AddExp(const int num);

	//�f�o�b�O�p���x���A�b�v�֐�
	void LevelUp();

	//�v���C���[�̃X�e�[�^�X,������`�悷��֐�
	void DrawPlayerStatus(int x,int y,int width,int height);

	//�v���C���[���S���̏���
	void DeadPlayer();

	//��������ω�������֐�
	inline void ChangeHaveCoin(int MoveNum) {
		coin += MoveNum;
		if (coin < 0)coin = 0;
	}
	//���������擾����֐�
	inline int GetHaveCoin() {
		return coin;
	}
private:

	t2k::Vector3 playerInMap;
	//�����x �@�\������
	int manPuku = 100;
	//���ێ����Ă���o���l
	int nowExp = 0;
	//���̃��x���܂ł̕K�v�o���l ���񂾂񑽂��Ȃ�
	int nextLevelExp = 100;
	//���x�����Ƃ̕K�v�o���l�{��
	const int needExpParLevel = 50;

	//���x�����Ƃ̊�{�㏸�X�e�[�^�X�{��
	const int statusParLevel = 2;
	//���x�����Ƃ̗̑͏㏸�{��
	const int hpParLevel = 20;

	//�\����y���W�Ԋu
	const int yBuf = 20;

	//���x���ɂ��㏸�X�e�[�^�X�̐ݒ�֐�
	void SetLevelStatus();
	//���̃��x���܂ł̕K�v�o���l���X�V����֐�
	void SetNextExp();
	//�����A�C�e��
	equipItem* myEquip[6] = { nullptr,nullptr, nullptr, nullptr, nullptr, nullptr };

	const std::string equipName[6] = { "WEAPON","HEAD","CHEST","GLOVE","BOOTS","SHIELD" };
	//hp,attack,defence,speed
	int statuses[4] = { 0,0,0,0 };

	int coin = 10000000;

	//�˂�����܂ő���t���O
	bool left = false;
	bool up = false;
	bool right = false;
	bool down = false;
	bool isDash = false;


	//�_�b�V�����Ȃ�ړ�����֐�
	//bool DetectDash();

	void DashToDir(const int dir, const t2k::Vector3 mapPos);
	void MoveToDir(const int dir, const t2k::Vector3 mapPos);

	//���������ׂĊO���֐�
	void RemoveAllEquip();

};
