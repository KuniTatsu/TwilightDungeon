#pragma once
#include<vector>
#include<string>
#include<unordered_map>
class SceneManager;

class Item;
class FadeControl;
class Sound;
class Map;

class GameManager {

public:

	GameManager();
	~GameManager();

	SceneManager* sManager = nullptr;
	FadeControl* fControl = nullptr;
	Sound* sound = nullptr;
	Map* map = nullptr;


	
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

	

	//gh��Ԃ��֐�
	std::vector<int> SetCharaGh(int num);

	//�A�C�e���̏��������������߂�֐�
	void haveItemInit();

	void setitem(int ItemId, int addNum);

	
private:
	
	//�A�C�e������excel����ǂݎ��֐�
	void loadItem();

	std::vector<std::vector<std::string>> loadItemCsv;

	//�}�b�v�̕�
	const int MAPWIDTH = 30;
	//�}�b�v�̏c��
	const int MAPHEIGHT = 30;
};