///*****Description*****
///�_���W�������̑S�Ă��s���V�[���N���X
///Update��Draw�֐���SceneManager�ōs����
///Update��Draw�֐��̒��ł̓V�[�N�G���X�Ŗ��t���[���s���鏈�������򂷂�
///�e�V�[�N�G���X�Ԃ�ChangeSequence�֐��ɂ���Ď��t���[�����瑖��V�[�N�G���X���ړ�����
///*********************

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
class Animation;

class DungeonScene :public BaseScene {

public:
	DungeonScene();
	~DungeonScene();


	void Update();

	void Draw();

	int GetDungeonLevel();
	void SetDungeonLevel(const int addLevel);

	void MoveLevel(const int addLevel);

private:
	bool firstIn = true;

	//�K�w
	int dungeonLevel = 1;

	//1�K�w�ɐݒu����A�C�e����
	const int spawnItemNum = 5;

	int EButton = 0;

	/*int alfa = 0;*/
	//����enemy�̍s���܂ł̃C���^�[�o���t���[��
	const int ENEMYACTINTERVAL = 30;
	//�X�V�C���^�[�o���t���[��
	int enemyActTimer = 30;

	std::list<std::shared_ptr<Enemy>>atackEnemies;
	std::list<std::shared_ptr<Enemy>>::iterator itr;

	//****************************************************
	//UI�֘A�̃N���X�̃C���X�^���X
	
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
	MenuWindow* firstMenu = nullptr;
	//****************************************************

	enum useType {
		USABLE,
		EQUIP,
		NOWEQUIP
	};
	useType usetype = USABLE;
	std::shared_ptr<Player> player = nullptr;

	//���ł���A�C�e�����X�g
	std::list<Item*> throwItem;

	std::list<std::shared_ptr<Item>>throwedItemList;

	std::shared_ptr<EnemyManager>eManager = nullptr;

	std::list<std::shared_ptr<Animation>>drawAnimationList;
	void initDungeonScene();

	void UpdateAnimation();
	void DrawAnimation();

	void CheckAnimLive();
	const int ATTACKEFFECTSPEED = 10;
	//int ATTACKEFFECTINDEX = 5;

	void ReturnCamp();

	t2k::Vector3 playerPos = {};

	t2k::Sequence<DungeonScene*> mainSequence =
		t2k::Sequence<DungeonScene*>(this, &DungeonScene::SeqDescFade);

	//�v���C���[����V�[�N�G���X
	bool SeqMain(const float deltatime);
	//�v���C���[�U���V�[�N�G���X
	bool SeqPlayerAttack(const float deltatime);
	//�G�l�~�[����V�[�N�G���X
	bool SeqEnemyAct(const float deltatime);
	//�G�l�~�[�U���V�[�N�G���X
	bool SeqEnemyAttack(const float deltatime);
	//�P�ڂ̃��j���[����
	bool SeqFirstMenu(const float deltatime);
	//Inventory�`��V�[�N�G���X
	bool SeqInventoryOpen(const float deltatime);
	//Inventory�A�C�e���g�p�m�F�V�[�N�G���X
	bool SeqInventoryUse(const float deltatime);
	//�������ł���V�[�N�G���X
	bool SeqThrowItemMove(const float deltatime);
	//�A�j���[�V�����V�[�N�G���X
	bool SeqAnimation(const float deltatime);
	//�t�F�[�h�C���V�[�N�G���X
	bool SeqFadeIn(const float deltatime);
	//�t�F�[�h�A�E�g�V�[�N�G���X
	bool SeqFadeOut(const float deltatime);
	//�G���A�ړ����̕����t�F�[�h�V�[�N�G���X
	bool SeqDescFade(const float deltatime);

	//debug
	bool SeqCameraMove(const float deltatime);
	//�V�[�N�G���X�̗񋓑�
	enum class sequence {
		MAIN,
		PLAYERATTACK,
		ENEMYACT,
		ENEMYATTACK,
		FIRSTMENU,
		INVENTORY_OPEN,
		INVENTORY_USE,
		THROWITEMMOVE,
		ANIMATION,
		FADEIN,
		FADEOUT,
		FADEDESC,
		CAMERA

	};
	sequence nowSeq = sequence::FADEDESC;
	sequence lastSeq = sequence::FADEDESC;

	enum class DescType {
		DUNGEONIN,
		MOVELEVEL

	};

	//���b�f���^�^�C���𑫂�
	float descFadeCount = 0;
	//������`�悷�鎞��
	const float DESCFADETIME = 3;
	//���̃_���W����������
	std::string nowDungeonName = "";

	//drawstring�p�̒��S���W�␳
	const float OFFSET = 30.0f;

	//fadeDesc�V�[�N�G���X�ŕ�����`�悷��֐�
	void DrawFadeDesc();


	//Sequence���ړ�������֐�,enum���ꏏ�ɕύX����
	void ChangeSequence(const sequence seq);
	//�V�[�P���X�m�F�֐�
	void DrawNowSequence(const sequence seq);

	//num�̓G�����֐�
	void RandEnemyCreate(const int num);

	//enemy�̃f�[�^�̊m�F�֐�
	void DrawEnemyData();
	//�`�撆�̃C���x���g���y�[�W
	int drawInventoryPage = 0;
	//�I�𒆂̃C���x���g����`�悷��֐�
	void DrawInventory();
	//�C���x���g���̕ʃy�[�W�Ɉړ�����֐�
	void ChangeInventory();

	int mouseX = 0;
	int mouseY = 0;

	//�A�C�e�����X�|�[��������֐�
	void SpawnItem(const int ItemId);

	//�����Ă���A�C�e�����X�g
	std::list<Item*>dropItems;

	//�����Ă���A�C�e���̕`��
	void DrawPopItem();

	//�C���x���g���őI�񂾃A�C�e�����ꎞ�I�Ɏ擾����ϐ�
	Item* itemBuf = nullptr;

	//�A�C�e���̎g�p�֐�
	void ItemUse(const int inventoryPage);

	void ItemThrow(const int inventoryPage);

	void DeleteDeadEnemy();

};