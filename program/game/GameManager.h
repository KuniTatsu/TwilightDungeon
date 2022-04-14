///*****Description*****
///�Q�[���@�\�S�ʂɊւ���N���X
///�Q�[���S�̂ɑ΂��Ċ֘A���鏈���y�уC���X�^���X��ۗL����N���X
/// �V�[���}�l�[�W����Update,Draw�����̃N���X��Update,Draw�ōs���AgameMain�ŌĂяo�����ƂŃQ�[���𓮂���
///*********************
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
class Inventory;
class Actor;
class ResourceManager;
class SkillManager;
class Skill;

class GameManager {

public:

	GameManager();
	~GameManager();

	FadeControl* fControl = nullptr;
	Sound* sound = nullptr;
	Map* map = nullptr;

	std::shared_ptr<Player>player = nullptr;

	Camera* camera = nullptr;
	ItemManager* iManager = nullptr;
	Inventory* inventory = nullptr;
	ResourceManager* resource = nullptr;

	SkillManager* sManager = nullptr;

	//�X�L����^����֐�
	void SetActorSkill(std::vector<Skill*>& SkillList, int SkillType, int SkillId);

	//debug
	bool minimapDraw = true;
	//
	//�����F
	int red = 0;
	int blue = 0;

	//�}�b�v�`�b�v�̈ꖇ�̑傫��
	const int GRAPHICSIZE = 20;
	float nowGraphicSize = GRAPHICSIZE;

	//rotaGraph�p�{��
	double graphEx = 1.0;
	//�`����@
	enum class ScaleMode :uint32_t {
		NORMAL,
		WIDE
	};
	//���̕`����@
	ScaleMode nowScale = ScaleMode::NORMAL;
	//�`����@���Ƃ̃}�b�v�`�b�v�̑傫��
	const float scale[2] = { 20.0f,40.0f };

	//debug�p�g��k���֐�
	void ScaleChange();

	//scaleChange�p�v�Z�֐�
	void CalcScale();

	//��ʒ��S
	const t2k::Vector3 WINDOWCENTER = t2k::Vector3(512, 384, 0);

	//�}�b�v�̕�(�`�b�v��)
	const int MAPWIDTH = 60;//default:50
	//�}�b�v�̏c��
	const int MAPHEIGHT = 48;//default:36


	//LoadDivGraph��MaxIndex�f�[�^ attack,player,enemy
	std::vector<std::vector<std::string>>maxIndex;
	//�eLoadDivGraph��MaxIndex�ǂݍ���
	void LoadMaxIndex();

	enum class index : uint32_t {
		ATTACK,
		PLAYER,
		ENEMY
	};
	//�ő�C���f�b�N�X�����擾����֐�
	inline const int& GetMaxIndex(index IndexName) {
		return std::atoi(maxIndex[static_cast<uint32_t>(IndexName)][1].c_str());
	}

	//�C���x���g�����������z��
	std::vector<Inventory*> inventories;

	//std::vector<std::shared_ptr<Inventory>>sharedInventories;

	//�A�C�e����ǉ�����ۂ̊i�[�\�ȃC���x���g���z��ԍ�
	int inventoryNum = 0;

	//�C���x���g���ɃA�C�e����ǉ�����֐�
	//�A�C�e��Id,�ǉ�����C���x���g��,���݂̃C���x���g���ԍ�
	void AddItemToInventory(const int ItemId, std::vector<Inventory*>& Inventories, int& InventoryNum);

	//�V���b�v�̃A�C�e�����C���x���g���ɒǉ�����֐�
	void AddItemFromShop(Item* ShopItem);

	//�C���x���g������A�C�e�����폜����֐�
	void PopItemFromInventory(const int NowInventoryId);

	//���ݑ������̃A�C�e���ƃC���x���g�����̃A�C�e���̃X�e�[�^�X�̍����擾����֐�
	int GetDifStatus(int subId, int equipType, int amount);

	//�������Ă��邩�m�F����֐�
	bool CheckEquipItem(int subId);

	//�A�C�e���^�C�v���烉���_���ȃA�C�e��Id���擾����֐�
	int GetRandItemData(int ItemType);

	//�A�C�e���|�C���^����A�C�e��Id���擾����֐�
	int GetItemId(Item* Item);

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

	enum class SpawnScene {
		Camp,
		Dungeon
	};

	//�v���C���[�̐���
	void MakePlayer(SpawnScene nowScene);

	//camp�pplayer�������W�ݒ�
	void SetCampStartPlayerPos();

	//�X�L���l��
	void SetSkill(std::vector<Skill*>SkillList, int SkillId);
	//�_���W�����ꗗ
	enum class Dungeon : uint32_t {
		TOWER,
		FOREST,
		WATERWAY,
		DUNGEONMAX
	};
	Dungeon nowDungeon;
	//�_���W�����̖��O
	const std::string DUNGEONNAME[3] = { "���̃_���W����","�X�̃_���W����","�ܔM�̃_���W����" };

	//�_���W�����̖��O���擾����֐�
	inline std::string GetDungeonName(Dungeon dungeonEnum) {
		//if (dungeonEnum > Dungeon::DUNGEONMAX)return "�͈͊O";
		return DUNGEONNAME[static_cast<uint32_t>(dungeonEnum)];
	}
	//�_���W�����}�b�v���Ŏg���摜�n���h���̎擾
	std::vector<int>& GetGraphicHandles(Dungeon dungeonName);
	//�_���W�����̐���
	void CreateDungeon(Dungeon dungeonName);

	//�_���W�����}�b�v�̐���
	void CreateMap(Dungeon dungeonName);

	//�_���W�����}�b�v���Ƃ�BGM�Đ�
	void RunDungeonBgm();

	//BGM��~
	void StopBgm();

	//���ʉ�
	void RunSystemSound();

	//�}�b�v�̕`��
	void MapDraw();
	//�~�j�}�b�v�`��
	void MiniMapDraw();

	//�~�j�}�b�v�X�V
	void UpdateMiniMap(t2k::Vector3 localPos);

	//�~�j�}�b�v�`��\�m�F�֐�
	bool CheckCanDraw(t2k::Vector3 localPos);

	//�����摜
	int arrowButton = 0;
	int RButton = 0;
	int EnterButton = 0;
	int howToPlayBack = 0;

	//�����摜�`��
	void DrawHowTo();

	//GameManager�̏�����
	void initGameManager();

	//�摜��ǂݍ����map�ɓ����֐�
	//���łɂ���gh�Ȃ炻���Ԃ�
	int LoadGraphEx(const std::string gh);

	//�������̘A�Ȃ����摜��vector�Ɋi�[����֐�
	void LoadDivGraphEx(const std::string Gh, const int AllNum, const int WidthNum, const int HeightNum,
		int XSize, int YSize, std::vector<int>& GhVector);



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

	//�J�������L�����𒆐S�Ƀ��Z�b�g //�⊮���g���ďu�Ԉړ����Ȃ�������
	void CameraReset();

	//�J�����̈ړ�
	void CameraMove(int width, int height, int dir);

	//����ȃE�B���h�E�̕\���֐�
	//arg1 windowX �ő�ɊJ�������̉��̑傫��
	//arg2 windowY �ő�ɊJ�������̏c�̑傫��
	//arg3 xSpeed  ���ɊJ���ۂ̃^�C�}�[�X�s�[�h
	//arg4 ySpeed�@�c�ɊJ���ۂ̃^�C�}�[�X�s�[�h
	//arg5 centerX �摜�`�撆�SX���W
	//arg6 centerY �摜�`�撆�SY���W
	//arg7 gh	   �\������摜�̃O���t�B�b�N�n���h��
	void DrawOpenWindow(float windowX, float windowY, int xCount, float yCount, int gh);
	//�E�B���h�E���J���ۂ̃^�C�}�[
	float openTimer = 0;

	enum class setStartPosType {
		PLAYER,
		STAIR,
		ENEMY,
		ITEM
	};

	//�}�b�v���̃����_���ȕ������擾�������̒��̃����_���ȍ��W���擾�����W��`����W�ɕϊ����ĕԂ�
	//setType 0:�v���C���[�������W,1:�K�i,2:enemy
	t2k::Vector3 SetStartPos(setStartPosType num);

	const t2k::Vector3 spawnPos = { 720,360,0 };

	//camp�̓���̍��W�Ƀv���C���[��ݒu
	inline const t2k::Vector3& SpawnPlayerCamp() {
		return spawnPos;
	}
	//�v���C���[���W�ꎞ�ۊǗp
	t2k::Vector3 playerPosBuf = {};


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
	//actor�^�̐����҃��X�g
	std::list<std::shared_ptr<Actor>>liveEntityList;

	//entity���X�g��y���W�Ń\�[�g����֐�
	void SortEntityList();

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
	void DealDamageToTarget(Actor* hoge, const t2k::Vector3 Pos);

	//�ڂ̑O�̑ΏۂɃX�L���ɂ��_���[�W��^����
	void DealSkillDamageToTarget(Actor* Actor, const t2k::Vector3 Pos,Skill* ActivateSkill);


	//debug�؂�ւ�
	bool isDebug = false;
	//player�������镔���̔ԍ�
	int playerRoomNum = 0;
	//�v���C���[�̕����ԍ���ύX����@
	void setPlayerRoomNum(const int roomNum);
	//�v���C���[�̎擾
	std::shared_ptr<Player> GetPlayer();
	t2k::Vector3 GetPlayerLocalPos();

	//�Z�`�Ƃ̃}�E�X�N���b�N���m
	bool CheckMousePointToRect(const int MouseX, const int MouseY, const int RectLeftTopX, const  int RectWidth, const int RectTopY, const int RectHeight);

	//�A�C�e�����������
	void SetItemNum(const int num);
	//���[�h�����A�C�e���̑������擾����֐�
	int GetItemNum();

	//�_���[�W�v�Z
	float CalcDamage(const int Attack, const int Defence);
	//�X�L���_���[�W�v�Z
	float CalcSkillDamage(const float SkillRate, const int Attack, const int Defence);

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
	inline int GetFragmentNum() {
		return haveTwilightFragment;
	}
	inline void SetFragmentNum(int num) {
		haveTwilightFragment += num;
		if (haveTwilightFragment > 5)haveTwilightFragment = 5;
		else if (haveTwilightFragment < 0)haveTwilightFragment = 0;
	}

	//�v���C���[���S���ɌĂԊ֐�
	void PlayerDead();
	//���O�̏���
	inline void ResetLog() {
		for (int i = 0; i < 7; ++i) {
			Log[i] = "";
		}
	}
	//���}�b�v�Ŕ����������t���O�̍X�V
	void SetDoneBuy();
	//���}�b�v�Ŕ����������t���O�̎擾
	bool GetDoneBuy();

private:
	std::string Log[7] = {};

	int test = 0;
	//�A�C�e���̑���
	int itemNum = 0;

	//�������Ă��鉩���̂�����̐� ���̂����z��łǂ��̃_���W��������ǂꂪ��ɓ��邩���߂邩��
	int haveTwilightFragment = 0;

	void Zoom();



};