#include "Camera.h"

void Camera::CameraMove()
{
	if (t2k::Input::isKeyDown(t2k::Input::KEYBORD_LEFT)) {
		cameraPos.x -= 5;
	}
	if (t2k::Input::isKeyDown(t2k::Input::KEYBORD_RIGHT)) {
		cameraPos.x += 5;
	}
	if (t2k::Input::isKeyDown(t2k::Input::KEYBORD_UP)) {
		cameraPos.y -= 5;
	}
	if (t2k::Input::isKeyDown(t2k::Input::KEYBORD_DOWN)) {
		cameraPos.y += 5;
	}

}
