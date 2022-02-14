#pragma once
#include"Scene.h"
#include "../library/t2klib.h"
#include<memory>

class Map;
class Menu;
class MenuWindow;
class EnemyManager;
class Item;
class Inventory;
class Player;
class Enemy;

class DungeonScene :public BaseScene {

public:
	DungeonScene();
	~DungeonScene();


	//num�̓G�����֐�
	void RandEnemyCreate(int num);

	void Update();

	void Draw();

	int GetDungeonLevel();
	void SetDungeonLevel(int addLevel);

	void MoveLevel(int addLevel);

private:
	//�K�w
	int dungeonLevel = 1;

	int alfa = 0;
	//����enemy�̍s���܂ł̃C���^�[�o���t���[��
	const int ENEMYACTINTERVAL = 30;
	//�X�V�C���^�[�o���t���[��
	int enemyActTimer = 30;

	std::list<std::shared_ptr<Enemy>>atackEnemies;
	std::list<std::shared_ptr<Enemy>>::iterator itr;

	//���̉�ɍs����window
	Menu* nextLevelWindow = nullptr;
	Menu* menuOpen = nullptr;
	Menu* inventory = nullptr;
	Menu* log = nullptr;
	Menu* desc = nullptr;
	Menu* playerStatus = nullptr;
	MenuWindow* use_usable = nullptr;
	MenuWindow* use_equip = nullptr;

	MenuWindow* use_nowEquip = nullptr;

	enum useType {
		USABLE,
		EQUIP,
		NOWEQUIP
	};
	useType usetype = USABLE;

	MenuWindow* firstMenu = nullptr;
	std::shared_ptr<Player> player = nullptr;
	
	//���ł���A�C�e�����X�g
	std::list<Item*> throwItem;

	std::list<std::shared_ptr<Item>>throwedItemList;

	/*
	Menu* hoge=new Menu(30,50,175,390,gh(int))
	Menu* hoge=new Menu(220,50,440,390,gh(int))
	*/

	std::shared_ptr<EnemyManager>eManager = nullptr;

	//**debug
	t2k::Vector3 playerPos = {};

	t2k::Sequence<DungeonScene*> main_sequence =
		t2k::Sequence<DungeonScene*>(this, &DungeonScene::Seq_Main);

	//�v���C���[����V�[�N�G���X
	bool Seq_Main(const float deltatime);
	//�G�l�~�[����V�[�N�G���X
	bool Seq_EnemyAct(const float deltatime);
	//�P�ڂ̃��j���[����
	bool Seq_FirstMenu(const float deltatime);
	//Inventory�`��V�[�N�G���X
	bool Seq_InventoryOpen(const float deltatime);
	//Inventory�A�C�e���g�p�m�F�V�[�N�G���X
	bool Seq_InventoryUse(const float deltatime);
	//�������ł���V�[�N�G���X
	bool Seq_ThrowItemMove(const float deltatime);

	//debug
	bool Seq_CameraMove(const float deltatime);
	//�V�[�N�G���X�̗񋓑�
	enum class sequence {
		MAIN,
		ENEMYACT,
		FIRSTMENU,
		INVENTORY_OPEN,
		INVENTORY_USE,
		THROWITEMMOVE,
		CAMERA

	};
	sequence nowSeq = sequence::MAIN;
	//Sequence���ړ�������֐�,enum���ꏏ�ɕύX����
	void ChangeSequence(sequence seq);
	//�V�[�P���X�m�F�֐�
	void DrawNowSequence(sequence seq);

	//enemy�̃f�[�^�̊m�F�֐�
	void DrawEnemyData();
	//�`�撆�̃C���x���g���y�[�W
	int inventoryPage = 0;
	//�I�𒆂̃C���x���g����`�悷��֐�
	void DrawInventory();
	//�C���x���g���̕ʃy�[�W�Ɉړ�����֐�
	void ChangeInventory();

	int mouseX = 0;
	int mouseY = 0;

	//�A�C�e�����X�|�[��������֐�
	void SpawnItem(int ItemId);

	//�����Ă���A�C�e�����X�g
	std::list<Item*>dropItems;

	//�����Ă���A�C�e���̕`��
	void DrawPopItem();

	//�C���x���g���őI�񂾃A�C�e�����ꎞ�I�Ɏ擾����ϐ�
	Item* itemBuf = nullptr;

	//�A�C�e���̎g�p�֐�
	void ItemUse(/*int selectNum,Inventory* inventory, */int inventoryPage);

	void ItemThrow(int inventoryPage);

	void DeleteDeadEnemy();

	/*std::string Log[9] = {};
	void addLog(const std::string log);
	void LogDraw();*/
	

};