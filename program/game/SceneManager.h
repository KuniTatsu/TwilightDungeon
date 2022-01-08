#pragma once
#include "Scene.h"

class Scene;

class SceneManager {
public:
	enum SCENE {
		TITLE,
		GUILDMAIN,
		//GUILDYEAR,
		TRAINING,
		BATTLE,


	};

	static void ChangeScene(SCENE scene);     //�V�[���������ɂ���ĕς���֐�
	static void Update();                     //���݂̃V�[���̍X�V�֐�
	static void Render();                     //���݂̃V�[���̕`��֐�

	static BaseScene* m_pScene;               //���݂̃V�[��

	static void Save();
	static void Load();

};