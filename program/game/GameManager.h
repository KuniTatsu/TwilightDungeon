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

class GameManager {

public:

	GameManager();
	~GameManager();

	/*SceneManager* sManager = nullptr;
	FadeControl* fControl = nullptr;*/
	//Sound* sound = nullptr;
	Map* map = nullptr;

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
	//map�̂ǂ̃`�b�v���擾����
	t2k::Vector3 WorldToLocal(t2k::Vector3 Pos);
	//�}�b�v�̃`�b�v�̏����擾����
	int GetMapChip(t2k::Vector3 PInChip);

private:
	
	//�A�C�e������excel����ǂݎ��֐�
	void loadItem();

	std::vector<std::vector<std::string>> loadItemCsv;

	//�}�b�v�̕�(�`�b�v��)
	const int MAPWIDTH = 50;
	//�}�b�v�̏c��
	const int MAPHEIGHT = 50;


	void Zoom(double* zoomEx);
	
};