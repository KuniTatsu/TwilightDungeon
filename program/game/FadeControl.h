#pragma once

class FadeControl {

public:

	FadeControl();

	//�摜�����X�ɕ\������֐�
	void imageFadeIn(int x, int y, double rate, int gh);

	//�摜�����X�ɏ����֐�
	void imageFadeOut();


	//��ʂ����X�ɏ����֐�
	void FadeOut();
	//��ʂ����X�ɏo���֐�
	void FadeIn();
	//true�͐^����,false�͖��邢
	bool doneFade = true;


private:

	int imageFade = 0;
	const int FADESPEED = 5;
	const int FADEINSPEED = 4;

	int screenFade = 0;






};