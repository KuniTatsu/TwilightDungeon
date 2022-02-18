#pragma once
#include<vector>
#include<string>
#include<unordered_map>
#include "../library/t2klib.h"
#include<memory>
#include<list>
#include<string>
class SceneManager;

class Item;
class FadeControl;
class Sound;
class Map;
class Player;
class Camera;
class Enemy;
class ItemManager;
class HaveItem;
class Inventory;
class Actor;

class GameManager {

public:

	GameManager();
	~GameManager();

	//FadeControl* fControl = nullptr;
	Sound* sound = nullptr;
	Map* map = nullptr;

	std::shared_ptr<Player>player = nullptr;

	Camera* camera = nullptr;
	ItemManager* iManager = nullptr;
	HaveItem* haveItem = nullptr;
	Inventory* inventory = nullptr;

	//std::shared_ptr<Inventory>shared_inventory;

	//debug
	bool minimapDraw = true;
	//

	//LoadDivGraph��MaxIndex�f�[�^ attack,player,enemy
	std::vector<std::vector<std::string>>maxIndex;
	//�eLoadDivGraph��MaxIndex�ǂݍ���
	void LoadMaxIndex();

	enum class index : uint32_t {
		ATTACK,
		PLAYER,
		ENEMY
	};

	inline const int& GetMaxIndex(index IndexName) {
		std::string hoge = maxIndex[static_cast<uint32_t>(IndexName)][1].c_str();
		return std::atoi(maxIndex[static_cast<uint32_t>(IndexName)][1].c_str());
	}

	//�C���x���g�����������z��
	std::vector<Inventory*> inventories;

	//std::vector<std::shared_ptr<Inventory>>sharedInventories;

	//�A�C�e����ǉ�����ۂ̊i�[�\�ȃC���x���g���z��ԍ�
	int inventoryNum = 0;

	//�C���x���g���ɃA�C�e����ǉ�����֐�
	void AddItemToInventory(const int itemId);

	//�C���x���g������A�C�e�����폜����֐�
	void PopItemFromInventory(const int NowInventoryId);

	const int GRAPHICSIZE = 20;
	float nowGraphicSize = GRAPHICSIZE;

	double graphEx = 1;

	//�}�b�v�̕�(�`�b�v��)
	const int MAPWIDTH = 60;//default:50
	//�}�b�v�̏c��
	const int MAPHEIGHT = 48;//default:36

	//��x�ǂݍ���gh��ۑ�����map
	std::unordered_map<std::string, int> ghmap;

	std::list<int> haveItemList;
	//itemId����Item*���擾����֐�
	Item* GetItemData(const int ItemId);

	//ItemId�����݂��邩�ǂ����m�F����֐�
	bool OutOfRangeInItem(const int ItemId);

	//�h���b�v�A�C�e�����X�g�������̃A�C�e����pop����֐�
	bool PopDetectItem(Item* item, std::list<Item*>& list);

	float deitatime_;

	void Update();
	void Draw();
	//�}�b�v�̕`��
	void MapDraw();

	//GameManager�̏�����
	void initGameManager();

	//�摜��ǂݍ����map�ɓ����֐�
	//���łɂ���gh�Ȃ炻���Ԃ�
	int LoadGraphEx(const std::string gh);

	//a~b�̊Ԃ̗����擾
	int GetRandEx(const int a, const int b);

	//�����̍Đ���
	void ReCreate();

	//�����I��Map�ɋ��n������֐��Q��Map�̃|�C���^�����n���Ă��Ƃ͏���ɂ����Ċ����̊֐��ɂ܂Ƃ߂���

	//map�̂ǂ̃`�b�v���擾���� �`����W����}�b�v���W�ւ̕ϊ�
	t2k::Vector3 WorldToLocal(const t2k::Vector3 Pos);
	//�}�b�v���W����`����W�ւ̕ϊ�
	t2k::Vector3 LocalToWorld(const int MapX, const int MapY);

	//�}�b�v�̃`�b�v�̏����擾����
	int GetMapChip(const t2k::Vector3 PInChip);

	//����̕`����W���W�������̂ǂ����ɂ��邩�m�F����֐�
	int CheckIsThereFromWorld(const t2k::Vector3 Pos);

	//����̃}�b�v���W�������̂ǂ����ɂ��邩�m�F����֐�
	int CheckIsThere(const t2k::Vector3 Pos);

	//�J�����̈ړ�
	void CameraMove();

	enum class setStartPosType {
		PLAYER,
		STAIR,
		ENEMY,
		ITEM
	};

	//�}�b�v���̃����_���ȕ������擾�������̒��̃����_���ȍ��W���擾�����W��`����W�ɕϊ����ĕԂ�
	//setType 0:�v���C���[�������W,1:�K�i,2:enemy
	t2k::Vector3 SetStartPos(setStartPosType num);


	//�����ԍ����Ƃ̒ʘH�̍��W���i�[����vector
	std::vector< std::vector<t2k::Vector3>>wayPoint;

	//wayPointVector������������֐�
	void InitWayPointVector(const int initroomNum);

	////�����̊O��1�}�X�O����T������vector�Ɋi�[����֐� //�g��Ȃ�
	//void CheckRoomWayPoint(int roomId);

	//�ʘH�쐬����vector�Ɋi�[����֐�
	void SetRoomWayPoint(const t2k::Vector3 pos, const int roomId);

	//�����ԍ��ƈ����̍��W�����ԉ��������̏o�����擾����֐�
	t2k::Vector3 GetFarPoint(const int roomId, const t2k::Vector3 pos);

	//����͈̔͂̃`�b�v��ύX����֐�
	void SetChip(const int Left, const int Up, const int Right, const int Down);

	//�����̑傫���擾
	t2k::Vector3 GetRoomValue(const int roomNum);

	//�����Enemy��Player���ׂ荇���Ă��邩�m�F����֐�
	bool CheckNearByPlayer(std::shared_ptr<Enemy>enemy);
	//�ׂ荇����Player�Ƃ̑��Έʒu���擾����֐� 0:��,1:�E,2:��,3:��
	int GetPlayerVec(std::shared_ptr<Enemy>enemy);

	t2k::Vector3 GetRoomStartPos(const int roomNum);

	//enemylist
	std::list<std::shared_ptr<Enemy>> liveEnemyList;

	//�������Ă���enemy�𓯊�����֐�
	void SetLiveEnemyList(std::list<std::shared_ptr<Enemy>> list);

	//�S�Ă�enemy��player�����͈͓��ɂ��邩�ǂ����m�F����֐�
	bool CheckNearByPlayerToAllEnemy(const int range);
	//����̍��W��enemy�����邩�ǂ����m�F����֐�
	bool CheckIsThereEnemyToDir(const t2k::Vector3 Pos);
	//����̍��W��enemy���擾����֐�
	std::shared_ptr<Enemy> GetIsThereEnemyToDir(const t2k::Vector3 Pos);

	inline void ForceInventoryChange(int& num) {
		num--;
		isDeleteInventory = false;
	}
	bool isDeleteInventory = false;


	//�ڂ̑O�̑ΏۂɃ_���[�W��^����
	void TakeDamageToTarget(Actor* hoge, const t2k::Vector3 Pos);


	//debug�؂�ւ�
	bool isDebug = true;

	int playerRoomNum = 0;

	void setPlayerRoomNum(const int roomNum);

	std::shared_ptr<Player> GetPlayer();

	//�Z�`�Ƃ̃}�E�X�N���b�N���m
	bool CheckMousePointToRect(const int MouseX, const int MouseY, const int RectLeftTopX, const  int RectWidth, const int RectTopY, const int RectHeight);

	//�A�C�e�����������
	void SetItemNum(const int num);
	//���[�h�����A�C�e���̑������擾����֐�
	int GetItemNum();

	//�_���[�W�v�Z
	float CalcDamage(const int Attack, const int Defence);

	//�_���[�W����
	void RunDamageEvent(const float Damage, std::shared_ptr<Actor>actor);

	//���O�֘A

	void addLog(const std::string log);
	void LogDraw(const int x, const int y);
	void DrawStringLows(const int lowNum);

	const t2k::Vector3 Vectores[4]{
		t2k::Vector3(0,-1,0),//��
		t2k::Vector3(1,0,0),//�E
		t2k::Vector3(0,1,0),//��
		t2k::Vector3(-1,0,0),//��
	};

	enum Vec {
		UP,
		RIGHT,
		DOWN,
		LEFT
	};

	inline const t2k::Vector3& GetVecter(int vec) {
		return Vectores[vec];
	}
	inline const t2k::Vector3 GetMultipleVector(int vec, int multiple) {
		const float x = Vectores[vec].x * multiple;
		const float y = Vectores[vec].y * multiple;
		return t2k::Vector3(x, y, 0);
	}
	inline Camera* GetCamera() {
		return camera;
	}

private:
	std::string Log[7] = {};

	const t2k::Vector3 WINDOWCENTER = t2k::Vector3(512, 384, 0);

	int test = 0;
	//�A�C�e���̑���
	int itemNum = 0;
	//std::vector<int> haveItemList;

	void Zoom();

	//test
	std::string strings[5][3]{
		{"����ɂ���","���̖��O��","hogehoge�ł�"},
		{"","",""},
		{"","",""},
		{"","",""},
		{"","",""}
	};

};