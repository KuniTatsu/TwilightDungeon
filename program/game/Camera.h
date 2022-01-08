#pragma once
#include "../library/t2klib.h"
#include"../support/Support.h"

class Camera {

public:
	

	t2k::Vector3 cameraPos = {};

	void CameraMove();

};