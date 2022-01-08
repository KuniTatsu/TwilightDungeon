#include "TitleScene.h"
#include"GameManager.h"
#include"DxLib.h"
#include "../library/t2klib.h"
#include "../support/Support.h"
#include"SceneManager.h"
#include"MenuWindow.h"
#include"FadeControl.h"
#include"SoundManager.h"


extern GameManager* gManager;

TitleScene::TitleScene()
{
	gManager->sound->BGM_Play(gManager->sound->bgm_title);
	String_Color_Black = GetColor(0, 0, 0);

	//title_gh = gManager->LoadGraphEx("graphics/sougenna.jpg");

	enter_gh = gManager->LoadGraphEx("graphics/button_Enter.png");
	title_Name_gh = gManager->LoadGraphEx("graphics/Title.png");
	title_Background_gh = gManager->LoadGraphEx("graphics/title_Background_Edit_2.png");

	title_Menu = new Menu(384, 500, 250, 80, "graphics/WindowBase_02.png");
	titleBack = new Menu(200, 40, 650, 120, "graphics/WindowBase_01.png");

}

TitleScene::~TitleScene()
{
	init = false;
	nowFade = false;
	StopSoundMem(gManager->sound->bgm_title);
	delete title_Menu;
	delete titleBack;
}

void TitleScene::Update()
{
	if (!init) {
		//�t�F�[�h�C������
		gManager->fControl->FadeIn();

		//�����t�F�[�h�C�����I������牺�̏����Ɉڂ�,���̏������s��Ȃ�����
		if (gManager->fControl->doneFade == false) {
			init = true;
			return;
		}
		return;
	}


	if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_RETURN)) {

		gManager->sound->System_Play(gManager->sound->system_select);
		nowFade = true;
		/*SceneManager::ChangeScene(SceneManager::SCENE::TRAINING);
		return;*/
	}
	if (nowFade) {
		gManager->fControl->FadeOut();
	}
	//�Q�[���̊J�n����
	//TrainingScene�֔�΂�
	if (gManager->fControl->doneFade != true)return;
	SceneManager::ChangeScene(SceneManager::SCENE::TRAINING);
	return;
	

	//�Ȃɂ��L�[�{�[�h����������or�{�^������������Q�[�����I������
	//�I������O�Ɉȉ��̏����O���t�@�C���ɕۑ�����
	//���݂̍ݐЃL�����N�^�[�̃X�e�[�^�X
	//���ɂ��̐i����
	//�������Ă���J�[�h�̓��ɂ��ƃC�x���g
	//Daycell�̃C�x���gid��gh


}

void TitleScene::Draw()
{

	
	//�w�i
	DrawRotaGraph(512, 384, 1.05, 0, title_Background_gh, true);
	//�^�C�g���̔w�i
	titleBack->Menu_Draw();
	//�^�C�g��
	DrawRotaGraph(512, 100, 1.28, 0, title_Name_gh, true);


	//�^�C�g�����j���[�̕`��
	title_Menu->Menu_Draw();

	//�^�C�g�����j���[�̗v�f
	DrawRotaGraph(title_Menu->menu_x+130, title_Menu->menu_y+20, 1, 0, enter_gh, true);
	DrawStringEx(title_Menu->menu_x + 10, title_Menu->menu_y+50, String_Color_Black, "Enter�������ăQ�[���X�^�[�g");
	//


}
