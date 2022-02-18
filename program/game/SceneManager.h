#pragma once
#include "Scene.h"

class Scene;

class SceneManager {
public:
	enum SCENE {
		TITLE,
		DUNGEON,
	};

	static void ChangeScene(SCENE scene);     //
	static void Update();                     //
	static void Render();                     //

	static BaseScene* m_pScene;               //

	//BaseScene*型から派生クラス*型を取得する関数
	 template<class T >
	 static T* CastScene() {
		T* scene = dynamic_cast<T*>(m_pScene);
		return scene;
	}
	//DungeonScene* castScene() {
	//	DungeonScene* scene = dynamic_cast<DungeonScene*>(m_pScene);
	//	return scene;
	//}


	static void Save();
	static void Load();

};