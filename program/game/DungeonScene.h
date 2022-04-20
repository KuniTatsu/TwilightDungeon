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
class Skill;

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
	//�N���A�t���O
	bool dungeonClear = false;

	//�N���A���̏���
	void DungeonClear();

	//1�K�w�ɐݒu����A�C�e����
	const int spawnItemNum = 5;
	//�G��|���Ď�ɓ���R�C���@������CSV����ǂݍ���
	const int DROPCOIN = 500;

	//�O���t�B�b�N
	int EButton = 0;
	int button_1 = 0;
	int button_2 = 0;
	int button_3 = 0;

	int numButtons[3] = {};

	//���߂ɔ��������X�L��
	Skill* lastUseSkill = nullptr;


	/*int alfa = 0;*/
	//����enemy�̍s���܂ł̃C���^�[�o���t���[��
	const int ENEMYACTINTERVAL = 30;
	//�X�V�C���^�[�o���t���[��
	int enemyActTimer = 30;

	std::list<std::shared_ptr<Enemy>>attackEnemies;
	std::list<std::shared_ptr<Enemy>>::iterator itr;

	//****************************************************
	//UI�֘A�̃N���X�̃C���X�^���X

	Menu* nextLevelWindow = nullptr;
	Menu* menuOpen = nullptr;
	Menu* topUI = nullptr;
	Menu* inventory = nullptr;
	Menu* log = nullptr;
	Menu* desc = nullptr;
	Menu* status = nullptr;
	Menu* playerStatus = nullptr;
	Menu* gameOver = nullptr;

	Menu* shopIn = nullptr;
	Menu* shopMyInv = nullptr;
	Menu* shop = nullptr;
	Menu* shopDesc = nullptr;
	Menu* shopCoin = nullptr;

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
	//�G�Ǘ��}�l�[�W��
	std::shared_ptr<EnemyManager>eManager = nullptr;
	//�`�悷��A�j���[�V�������X�g
	std::list<std::shared_ptr<Animation>>drawAnimationList;
	//�_���W�����V�[��������
	void initDungeonScene();
	//�A�j���[�V�����X�V
	void UpdateAnimation();
	//�A�j���[�V�����`��
	void DrawAnimation();
	//�A�j���[�V�����I���m�F
	void CheckAnimLive();
	//�ʏ�U���̃A�j���[�V�����^�C�}�[
	const int ATTACKEFFECTSPEED = 10;
	//�X�L���U���̃A�j���[�V�����^�C�}�[
	//const int SKILLANIMSPEED = 10;

	//�X�L�������p�L�[
	int skillKeys[3] = { t2k::Input::KEYBORD_1, t2k::Input::KEYBORD_2, t2k::Input::KEYBORD_3 };

	//�X�L�������֐�
	bool ActivateSkillCheck();

	//int ATTACKEFFECTINDEX = 5;
	//debug �L�����v�ɖ߂�
	void ReturnCamp();
	//�G�̃~�j�}�b�v�`�� ������ЂƂɂ���
	void DrawMiniEnemy();
	//�~�j�}�b�v�G�l�~�[gh
	int miniEnemy = 0;

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

	//�V���b�v���������j���[�V�[�N�G���X
	bool SeqShopMain(const float deltatime);



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
		SHOP,
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

	//����ȃ}�b�v�`�b�v�𓥂�ł��邩�m�F����֐�
	bool CheckExtraOnTile();

	//Sequence���ړ�������֐�,enum���ꏏ�ɕύX����
	void ChangeSequence(const sequence seq);
	//�V�[�P���X�m�F�֐�
	void DrawNowSequence(const sequence seq);

	//num�̓G�����֐�
	void RandEnemyCreate(const int num);


	//��ʏ㕔��UI�`��֐�
	void DrawTopUI();

	//enemy�̃f�[�^�̊m�F�֐�
	void DrawEnemyData();
	//�`�撆�̃C���x���g���y�[�W
	int drawInventoryPage = 0;
	//�I�𒆂̃C���x���g����`�悷��֐�
	void DrawInventory(int x, int y);
	//�C���x���g���̕ʃy�[�W�Ɉړ�����֐�
	void ChangeInventory();

	int mouseX = 0;
	int mouseY = 0;

	//�A�C�e�����X�|�[��������֐�
	void SpawnItem(const int ItemId);

	//�G����A�C�e�����h���b�v����m��
	const int DROPODDS = 80;
	//�G����A�C�e�����h���b�v������֐�
	void DropItem(const int ItemId, const t2k::Vector3 DropPos);

	//�A�C�e�����E���t���O
	bool itemGetFlag = true;

	//�����Ă���A�C�e�����X�g    //GameManager�ł��̒��g���g������
	std::list<Item*>dropItems;

	//�����Ă���A�C�e���̕`��
	void DrawPopItem();

	//�A�C�e���擾����
	bool DetectItem();

	//�C���x���g���őI�񂾃A�C�e�����ꎞ�I�Ɏ擾����ϐ�
	Item* itemBuf = nullptr;

	//�A�C�e���̎g�p�֐�
	void ItemUse(const int inventoryPage);
	//�A�C�e�������֐�
	void ItemThrow(const int inventoryPage);
	//���񂾓G�̏���
	void DeleteDeadEnemy();
	//�v���C���[���S����
	void WhenDeadPlayer();

	//�V���b�v�̃A�C�e������֐�
	void SetShopItem(int SetNum, int ItemType);
	//�V���b�v�̃A�C�e���y�[�W��
	int shopPage = 0;

	//���̕`�撆�̃V���b�v�y�[�W
	int currentDrawPage = 0;
	//�V���b�v�̃y�[�W�����Ă����z��
	std::vector<Inventory*> shopPages;

	enum class ItemType :uint32_t {
		CONSUME,
		THROW,
		WEAPON,
		ARMOR,
		MAXNUM
	};



};