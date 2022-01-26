#pragma once
#include<vector>
#include<string>
#include<unordered_map>
#include "../library/t2klib.h"
class SceneManager;

class Item;
class FadeControl;
class Sound;
class Map;
class Player;
class Camera;

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

	double graphEx = 1;

	//�}�b�v�̕�(�`�b�v��)
	const int MAPWIDTH = 60;//default:50
	//�}�b�v�̏c��
	const int MAPHEIGHT = 48;//default:36
	
	//��x�ǂݍ���gh��ۑ�����map
	std::unordered_map<std::string, int> ghmap;

	//[itemid][haveItemNum]�A�C�e���������Ă��鐔��id�̓񎟌��z��
	std::vector<std::vector<int>> haveItem;
	//item���ׂĂ̔z��
	std::vector<std::vector<Item*>> itemList;

	float deitatime_;

	
	void Update();
	void Draw();

	//GameManager�̏�����
	void initGameManager();

	//�摜��ǂݍ����map�ɓ����֐�
	//���łɂ���gh�Ȃ炻���Ԃ�
	int LoadGraphEx(std::string gh);

	//�A�C�e���̏��������������߂�֐�
	void haveItemInit();

	void setitem(int ItemId, int addNum);

	//a~b�̊Ԃ̗����擾
	int GetRandEx(int a, int b);

	//�����̍Đ���
	void ReCreate();

	//map�̂ǂ̃`�b�v���擾���� �`����W����}�b�v���W�ւ̕ϊ�
	t2k::Vector3 WorldToLocal(t2k::Vector3 Pos);
	//�}�b�v���W����`����W�ւ̕ϊ�
	t2k::Vector3 LocalToWorld(int MapX,int MapY);

	//�}�b�v�̃`�b�v�̏����擾����
	int GetMapChip(t2k::Vector3 PInChip);

	//����̃}�b�v���W�������̂ǂ����ɂ��邩�m�F����֐�
	int CheckIsThere(t2k::Vector3 Pos);

	//�J�����̈ړ�
	void CameraMove(Player* p);

	//�}�b�v���̃����_���ȕ������擾�������̒��̃����_���ȍ��W���擾�����W��`����W�ɕϊ����ĕԂ�
	//setType 0:�v���C���[�������W,1:�K�i
	t2k::Vector3 SetStartPos(int setType);

	//�����ԍ����Ƃ̒ʘH�̍��W���i�[����vector
	std::vector< std::vector<t2k::Vector3>>wayPoint;

	//wayPointVector������������֐�
	void InitWayPointVector(int initroomNum);

	//�����̊O��1�}�X�O����T������vector�Ɋi�[����֐� //�g��Ȃ�
	void CheckRoomWayPoint(int roomId);

	//�ʘH�쐬����vector�Ɋi�[����֐�
	void SetRoomWayPoint(t2k::Vector3 pos,int roomId);



	//debug�؂�ւ�
	bool isDebug = true;

private:
	
	//�A�C�e������excel����ǂݎ��֐�
	void loadItem();

	std::vector<std::vector<std::string>> loadItemCsv;

	
	int test = 0;



	void Zoom(double* zoomEx);
	
};