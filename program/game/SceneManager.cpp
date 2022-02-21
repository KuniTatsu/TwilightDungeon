#include"SceneManager.h"
#include <cstddef>
#include"DxLib.h"
#include"TitleScene.h"
#include"DungeonScene.h"
#include"CampScene.h"

BaseScene* SceneManager::m_pScene = NULL;       //始めはシーンは空

void SceneManager::ChangeScene(SCENE scene) {

    if (m_pScene != NULL) {
        delete m_pScene;
    }

    switch (scene) {          //引数のシーン
    case SCENE::TITLE:
        m_pScene = new TitleScene();      //タイトルシーンを現在のシーンにする
        break;
    case SCENE::DUNGEON:
        m_pScene = new DungeonScene();       //ダンジョンシーンを現在のシーンにする
        break;
    case SCENE::CAMP:
        m_pScene = new CampScene();     //バトルシーンを現在のシーンにする
        break;
    //case SCENE::BATTLE:
    //    m_pScene = new BattleScene();     //バトルシーンを現在のシーンにする
    //    break;
    default:
        break;
    }

}

void SceneManager::Update() {
    m_pScene->Update();         //現在のシーンの更新関数
}

void SceneManager::Render() {
    m_pScene->Draw();           //現在のシーンの描画関数
}


