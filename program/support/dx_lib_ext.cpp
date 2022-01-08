#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include "DxLib.h"
#include "dx_lib_ext.h"

void DrawStringEx(int x, int y, int color, char* _str, ...) {

	char buff[255] = { 0 };
	va_list argptr;

	va_start(argptr, _str);
	vsprintf(buff, _str, argptr);
	va_end(argptr);

	DrawString(x, y, buff, color);

}

void DrawLineEx(t2k::Vector3& p, t2k::Vector3& v, float length, int color) {
	t2k::Vector3 n = v;
	n.normalize();
	n *= length;
	DrawLine(p.x, p.y, p.x + n.x, p.y + n.y, color, false);
}
