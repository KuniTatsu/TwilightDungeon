#include"SceneManager.h"
//#include"Title.h"

#include <cstddef>
#include"DxLib.h"
#include"GuildMainScene.h"
#include"TrainingScene.h"
#include"BattleScene.h"
#include"TitleScene.h"

BaseScene* SceneManager::m_pScene = NULL;       //�n�߂̓V�[���͋�

void SceneManager::ChangeScene(SCENE scene) {

    if (m_pScene != NULL) {
        delete m_pScene;
    }

    switch (scene) {          //�����̃V�[��
    case SCENE::TITLE:
        m_pScene = new TitleScene();      //�^�C�g���V�[�������݂̃V�[���ɂ���
        break;
    case SCENE::GUILDMAIN:
        m_pScene = new GuildMainScene();       //�O���[�o���}�b�v�V�[�������݂̃V�[���ɂ���
        break;
    case SCENE::TRAINING:
        m_pScene = new TrainingScene();     //�o�g���V�[�������݂̃V�[���ɂ���
        break;
    case SCENE::BATTLE:
        m_pScene = new BattleScene();     //�o�g���V�[�������݂̃V�[���ɂ���
        break;
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

void SceneManager::Save()
{
    m_pScene->Save();


}

void SceneManager::Load()
{
    m_pScene->Load();
}
