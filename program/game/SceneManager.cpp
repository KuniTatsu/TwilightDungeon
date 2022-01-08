#include"SceneManager.h"
//#include"Title.h"

#include <cstddef>
#include"DxLib.h"
#include"GuildMainScene.h"
#include"TrainingScene.h"
#include"BattleScene.h"
#include"TitleScene.h"

BaseScene* SceneManager::m_pScene = NULL;       //始めはシーンは空

void SceneManager::ChangeScene(SCENE scene) {

    if (m_pScene != NULL) {
        delete m_pScene;
    }

    switch (scene) {          //引数のシーン
    case SCENE::TITLE:
        m_pScene = new TitleScene();      //タイトルシーンを現在のシーンにする
        break;
    case SCENE::GUILDMAIN:
        m_pScene = new GuildMainScene();       //グローバルマップシーンを現在のシーンにする
        break;
    case SCENE::TRAINING:
        m_pScene = new TrainingScene();     //バトルシーンを現在のシーンにする
        break;
    case SCENE::BATTLE:
        m_pScene = new BattleScene();     //バトルシーンを現在のシーンにする
        break;
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

void SceneManager::Save()
{
    m_pScene->Save();


}

void SceneManager::Load()
{
    m_pScene->Load();
}
