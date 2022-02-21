#include"SceneManager.h"
#include <cstddef>
#include"DxLib.h"
#include"TitleScene.h"
#include"DungeonScene.h"
#include"CampScene.h"

BaseScene* SceneManager::m_pScene = NULL;       //�n�߂̓V�[���͋�

void SceneManager::ChangeScene(SCENE scene) {

    if (m_pScene != NULL) {
        delete m_pScene;
    }

    switch (scene) {          //�����̃V�[��
    case SCENE::TITLE:
        m_pScene = new TitleScene();      //�^�C�g���V�[�������݂̃V�[���ɂ���
        break;
    case SCENE::DUNGEON:
        m_pScene = new DungeonScene();       //�_���W�����V�[�������݂̃V�[���ɂ���
        break;
    case SCENE::CAMP:
        m_pScene = new CampScene();     //�o�g���V�[�������݂̃V�[���ɂ���
        break;
    //case SCENE::BATTLE:
    //    m_pScene = new BattleScene();     //�o�g���V�[�������݂̃V�[���ɂ���
    //    break;
    default:
        break;
    }

}

void SceneManager::Update() {
    m_pScene->Update();         //���݂̃V�[���̍X�V�֐�
}

void SceneManager::Render() {
    m_pScene->Draw();           //���݂̃V�[���̕`��֐�
}


