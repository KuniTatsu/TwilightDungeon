#pragma once
#include<DxLib.h>
#include<string>

class Menu {
public:

	Menu() {}
	virtual ~Menu() {}
	int menu_x, menu_y, menu_width, menu_height = 0;
	int menu_gh[9] = { 0 };
	bool menu_live;


	//newするときに大きさとghのpathを引数で渡す→いずれ大きさとpathを両方excelからいじりたい
	Menu(int menu_window_x, int menu_window_y, int menu_window_width, int menu_window_height,std::string gh_path);

	void Menu_Draw();
};

class MenuWindow :public Menu {
public:

	typedef struct {
		int x;
		int y;
		std::string name;
		int menu_num;
	}MenuElement_t;

public:

	int SelectNum;				// 現在の選択番号
	bool menuInit = false;

	int read_menu_x = 0;
	int read_menu_y = 0;
	int read_menu_element_num_ = 0;

	MenuElement_t* read_menu_element = nullptr;

	//bool menu_layer_flag_0 = false;
	bool secondMenuFlag = false;
	bool manageSelectFlag = true;//Read関数のSelectNumを動かすかどうか trueなら動かす

	MenuWindow(int menu_window_x, int menu_window_y, int menu_window_width, int menu_window_height, std::string gh_path,MenuElement_t* elements, int elements_num);
	~MenuWindow();
	//メニューを開く
	void Open();

	void Read();

	void All();

private:

	int String_Color_Black = 0;;
	int elements_num_;
	bool m_IsStartOpened;		// ESCキーで開いた1フレーム目
	MenuElement_t* MenuElement = nullptr;

};

