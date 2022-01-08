#include"MenuWindow.h"
#include "../library/t2klib.h"
#include "../support/Support.h"
#include"SoundManager.h"
#include"GameManager.h"

//extern Sound* sound;
extern GameManager* gManager;

Menu::Menu(int menu_window_x, int menu_window_y, int menu_window_width, int menu_window_height, std::string gh_path) {

	menu_x = menu_window_x;
	menu_y = menu_window_y;
	menu_width = menu_window_width;
	menu_height = menu_window_height;
	int n = LoadDivGraph(gh_path.c_str(), 9, 3, 3, 16, 16, menu_gh);
	menu_live = false;


}

void Menu::Menu_Draw()
{
	DrawGraph(menu_x, menu_y, menu_gh[0], TRUE);
	DrawExtendGraph(menu_x + 16, menu_y, menu_x + menu_width - 16, menu_y + 16, menu_gh[1], TRUE);
	DrawGraph(menu_x + menu_width - 16, menu_y, menu_gh[2], TRUE);

	DrawExtendGraph(menu_x, menu_y + 16, menu_x + 16, menu_y + menu_height - 16, menu_gh[3], TRUE);
	DrawExtendGraph(menu_x + 16, menu_y + 16, menu_x + menu_width - 16, menu_y + menu_height - 16, menu_gh[4], TRUE);
	DrawExtendGraph(menu_x + menu_width - 16, menu_y + 16, menu_x + menu_width, menu_y + menu_height - 16, menu_gh[5], TRUE);

	DrawGraph(menu_x, menu_y + menu_height - 16, menu_gh[6], TRUE);
	DrawExtendGraph(menu_x + 16, menu_y + menu_height - 16, menu_x + menu_width - 16, menu_y + menu_height, menu_gh[7], TRUE);
	DrawGraph(menu_x + menu_width - 16, menu_y + menu_height - 16, menu_gh[8], TRUE);
}

MenuWindow::MenuWindow(int menu_window_x, int menu_window_y, int menu_window_width, int menu_window_height, std::string gh_path, MenuElement_t* elements, int elements_num)

	: Menu(menu_window_x, menu_window_y, menu_window_width, menu_window_height, gh_path)//メニューの大きさを決める(開始座標:左上,横幅,縦幅)

	, SelectNum(0)
	, m_IsStartOpened(false) // コンストラクタでは、メンバ変数の初期化はこう書くこともできる
	, read_menu_x(menu_window_x)
	, read_menu_y(menu_window_y)
	, read_menu_element_num_(elements_num)
	, read_menu_element(elements)
{



	//外で使う場合はこんな感じで呼び出す
	/*MenuElement_t* p = new MenuElement_t[]{
		{40,30,"ステータス",0},
		{40,80,"持ち物",1},
		{40,130,"必殺技",2},
		{40,180,"マップ",3},
		{40,230,"タイトルへ",4},
	};

	MenuWindow* p = new MenuWindow(10, 10, 10, 10, p, 5);*/


	MenuElement = elements;
	elements_num_ = elements_num;
	String_Color_Black = GetColor(0, 0, 0);

}

MenuWindow::~MenuWindow()
{
	if (MenuElement) {
		delete[] MenuElement;	// 配列を破棄する場合の書き方
	}

}

void MenuWindow::Open() {


	menu_live = true;
	m_IsStartOpened = true;
	SelectNum = 0;
	menuInit = false;


}

void MenuWindow::Read() {
	/*bool readinit = false;
	if (readinit == false) {

		readinit = true;
	}*/


	if (menuInit == false) {
		if (SelectNum == 0) {
			MenuElement[0].x = read_menu_x + 10;
			for (int i = 1; i < elements_num_; ++i) {
				MenuElement[i].x = read_menu_x + 20;
			}
			//MenuElement[2].x = read_menu_x + 20;
		}
		menuInit = true;
	}
	if (m_IsStartOpened == true)return;
	if (manageSelectFlag != false) {


		if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_DOWN)) {
			SelectNum = (SelectNum + 1) % read_menu_element_num_;//一つ下にずれる
			gManager->sound->System_Play(gManager->sound->system_move);
			//sound->System_Play(sound->system_move);
		}
		else if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_UP)) {
			SelectNum = (SelectNum + (read_menu_element_num_ - 1)) % read_menu_element_num_;//一つ上にずれる
			gManager->sound->System_Play(gManager->sound->system_move);

			//sound->System_Play(sound->system_move);
		}
		/*if (menuExitFlag == true) {
			if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_ESCAPE)) {
				menu_live = false;
				SelectNum = 0;
				menuInit = false;
				player->m_ActionFlag_Menu = true;

				secondMenuFlag = false;
			}
		}*/

		if (t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_DOWN) || t2k::Input::isKeyDownTrigger(t2k::Input::KEYBORD_UP)) {

			for (int i = 0; i < elements_num_; i++) {
				if (i == SelectNum) {
					MenuElement[i].x = read_menu_x + 10;
				}
				else {
					MenuElement[i].x = read_menu_x + 20;
				}
			}

		}
	}
	for (int i = 0; i < elements_num_; i++) {
		DrawFormatString(MenuElement[i].x, MenuElement[i].y, GetColor(0, 0, 0), MenuElement[i].name.c_str());
	}

}



void MenuWindow::All() {
	if (menu_live) {
		Menu_Draw();
		Read();
		m_IsStartOpened = false;

	}
	else {

	}

}


