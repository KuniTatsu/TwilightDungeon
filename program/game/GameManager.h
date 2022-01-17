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
	//Sound* sound = nullptr;
	Map* map = nullptr;
	Player* player = nullptr;
	Camera* camera = nullptr;

	double graphEx = 1;
	
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

	void ReCreate();
	//map�̂ǂ̃`�b�v���擾���� �`����W����}�b�v���W�ւ̕ϊ�
	t2k::Vector3 WorldToLocal(t2k::Vector3 Pos);
	//�}�b�v���W����`����W�ւ̕ϊ�
	t2k::Vector3 LocalToWorld(int MapX,int MapY);

	//�}�b�v�̃`�b�v�̏����擾����
	int GetMapChip(t2k::Vector3 PInChip);

	//�J�����̈ړ�
	void CameraMove(Player* p);

private:
	
	//�A�C�e������excel����ǂݎ��֐�
	void loadItem();

	std::vector<std::vector<std::string>> loadItemCsv;

	//�}�b�v�̕�(�`�b�v��)
	const int MAPWIDTH = 50;
	//�}�b�v�̏c��
	const int MAPHEIGHT = 38;
	//�}�b�v���̃����_���ȕ������擾�������̒��̃����_���ȍ��W���擾�����W��`����W�ɕϊ����ĕԂ�
	t2k::Vector3 SetStartPos();


	void Zoom(double* zoomEx);
	
};