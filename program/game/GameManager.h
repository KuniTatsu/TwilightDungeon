#pragma once
#include<vector>
#include<string>
#include<unordered_map>
#include "../library/t2klib.h"
#include<memory>
#include<list>
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

class GameManager {

public:

	GameManager();
	~GameManager();

	/*SceneManager* sManager = nullptr;
	FadeControl* fControl = nullptr;*/
	Sound* sound = nullptr;
	Map* map = nullptr;
	Player* player = nullptr;
	Camera* camera = nullptr;
	ItemManager* iManager = nullptr;
	HaveItem* haveItem = nullptr;
	Inventory* inventory = nullptr;

	//�C���x���g�����������z��
	std::vector<Inventory*> inventories;
	//�A�C�e����ǉ�����ۂ̊i�[�\�ȃC���x���g���z��ԍ�
	int inventoryNum = 0;

	//�C���x���g���ɃA�C�e����ǉ�����֐�
	void AddItemToInventory(int itemId);

	double graphEx = 1;

	//�}�b�v�̕�(�`�b�v��)
	const int MAPWIDTH = 60;//default:50
	//�}�b�v�̏c��
	const int MAPHEIGHT = 48;//default:36

	//��x�ǂݍ���gh��ۑ�����map
	std::unordered_map<std::string, int> ghmap;

	//item���ׂĂ̔z��
	//std::list<Item*> itemList;

	std::list<int> haveItemList;
	//itemId����Item*���擾����֐�
	Item* GetItemData(int ItemId);

	//ItemId�����݂��邩�ǂ����m�F����֐�
	bool OutOfRangeInItem(int ItemId);


	//�h���b�v�A�C�e�����X�g�������̃A�C�e����pop����֐�
	bool PopDetectItem(Item* item, std::list<Item*>& list);

	float deitatime_;


	void Update();
	void Draw();

	//GameManager�̏�����
	void initGameManager();

	//�摜��ǂݍ����map�ɓ����֐�
	//���łɂ���gh�Ȃ炻���Ԃ�
	int LoadGraphEx(std::string gh);

	//a~b�̊Ԃ̗����擾
	int GetRandEx(int a, int b);

	//�����̍Đ���
	void ReCreate();

	//�����I��Map�ɋ��n������֐��Q��Map�̃|�C���^�����n���Ă��Ƃ͏���ɂ����Ċ����̊֐��ɂ܂Ƃ߂���

	//map�̂ǂ̃`�b�v���擾���� �`����W����}�b�v���W�ւ̕ϊ�
	t2k::Vector3 WorldToLocal(t2k::Vector3 Pos);
	//�}�b�v���W����`����W�ւ̕ϊ�
	t2k::Vector3 LocalToWorld(int MapX, int MapY);

	//�}�b�v�̃`�b�v�̏����擾����
	int GetMapChip(t2k::Vector3 PInChip);

	//����̕`����W���W�������̂ǂ����ɂ��邩�m�F����֐�
	int CheckIsThereFromWorld(t2k::Vector3 Pos);

	//����̃}�b�v���W�������̂ǂ����ɂ��邩�m�F����֐�
	int CheckIsThere(t2k::Vector3 Pos);

	//�J�����̈ړ�
	void CameraMove(Player* p);

	//�}�b�v���̃����_���ȕ������擾�������̒��̃����_���ȍ��W���擾�����W��`����W�ɕϊ����ĕԂ�
	//setType 0:�v���C���[�������W,1:�K�i,2:enemy
	t2k::Vector3 SetStartPos(int setType);

	//�����ԍ����Ƃ̒ʘH�̍��W���i�[����vector
	std::vector< std::vector<t2k::Vector3>>wayPoint;

	//wayPointVector������������֐�
	void InitWayPointVector(int initroomNum);

	////�����̊O��1�}�X�O����T������vector�Ɋi�[����֐� //�g��Ȃ�
	//void CheckRoomWayPoint(int roomId);

	//�ʘH�쐬����vector�Ɋi�[����֐�
	void SetRoomWayPoint(t2k::Vector3 pos, int roomId);

	//�����ԍ��ƈ����̍��W�����ԉ��������̏o�����擾����֐�
	t2k::Vector3 GetFarPoint(int roomId, t2k::Vector3 pos);

	//����͈̔͂̃`�b�v��ύX����֐�
	void SetChip(int Left, int Up, int Right, int Down);

	//�����̑傫���擾
	t2k::Vector3 GetRoomValue(int roomNum);

	//�����Enemy��Player���ׂ荇���Ă��邩�m�F����֐�
	bool CheckNearByPlayer(std::shared_ptr<Enemy>enemy);

	t2k::Vector3 GetRoomStartPos(int roomNum);

	//enemylist
	std::list<std::shared_ptr<Enemy>> hoge;

	//�S�Ă�enemy��player�����͈͓��ɂ��邩�ǂ����m�F����֐�
	bool CheckNearByPlayerToAllEnemy(int range);


	//debug�؂�ւ�
	bool isDebug = true;

	int playerRoomNum = 0;

	void setPlayerRoomNum(int roomNum);

	Player* GetPlayer();

	//�Z�`�Ƃ̃}�E�X�N���b�N���m
	bool CheckMousePointToRect(int MouseX, int MouseY, int RectLeftTopX, int RectWidth, int RectTopY, int RectHeight);

	//�A�C�e�������擾
	void SetItemNum(int num);

	int GetItemNum();

private:

	int test = 0;
	//�A�C�e���̑���
	int itemNum=0;
	//std::vector<int> haveItemList;

	void Zoom(double* zoomEx);

};