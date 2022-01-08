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

	static void ChangeScene(SCENE scene);     //シーンを引数によって変える関数
	static void Update();                     //現在のシーンの更新関数
	static void Render();                     //現在のシーンの描画関数

	static BaseScene* m_pScene;               //現在のシーン

	static void Save();
	static void Load();

};