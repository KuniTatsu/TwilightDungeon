///*****Description*****
///カメラに関するクラス
///デバッグ時のカメラ自由移動と
///通常描画時の画面補正のための変数を持つ
///*********************
#pragma once
#include "../library/t2klib.h"
#include"../support/Support.h"

class Camera {

public:
	t2k::Vector3 cameraPos = {0,0,0};

	void CameraMove();

};