#pragma once
#include "../library/t2klib.h"

void DrawStringEx(int x, int y, int color, char* _str, ...);
void DrawLineEx(t2k::Vector3& p, t2k::Vector3& v, float length, int color = -1);
