#pragma once
#include"Scene.h"

class Menu;

class TitleScene : public BaseScene {

public:
	TitleScene();
	~TitleScene();



	void Update();

	void Draw();
private:
	Menu* titleBack = nullptr;

	Menu* title_Menu = nullptr;

	bool init = false;
	int title_gh = 0;
	int enter_gh = 0;

	int title_Name_gh = 0;
	int title_Background_gh = 0;

	int String_Color_Black;
	bool nowFade = false;


};