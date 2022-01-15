#include "Map.h"
#include"GameManager.h"
#include"DxLib.h"
#include"Camera.h"

extern GameManager* gManager;

void Map::SetAllChip(int Left, int Up, int Right, int Down)
{
	for (int i = Up; i <= Down; ++i) {
		for (int k = Left; k <= Right; ++k) {
			SetChip(k, i, ROUTE);
		}
	}


}

void Map::DivideStart(int Width, int Height, Map* map)
{
	AreaDivide();

	CreateRoom();

	CreatePassWay();

	for (auto room : divideRoom) {
		SetAllChip(room[0], room[1], room[2], room[3]);
	}

}

Map::Map(int Width, int Height)
{
	mapChip[0] = gManager->LoadGraphEx("graphics/PassWay_20.png");
	mapChip[1] = gManager->LoadGraphEx("graphics/Wall_20.png");

	camera = new Camera();

	width = Width;
	height = Height;

	//縦の初期化
	ground.resize(height);
	for (auto v : ground)v.resize(width);
	for (int i = 0; i < height; ++i) {
		ground[i].resize(width);
	}
	//すべてを壁にする
	for (int i = 0; i < height; ++i) {
		for (int k = 0; k < width; ++k) {
			ground[i][k] = WALL;
		}
	}

}

int Map::GetChip(int x, int y)
{
	if (IsOutOfRange(x, y))return outOfRange;
	return ground[y][x];
}

void Map::SetChip(int x, int y, int SetChip)
{
	if (IsOutOfRange(x, y))return;
	ground[y][x] = SetChip;
}

void Map::MapDraw()
{
	camera->CameraMove();
	int x = 0;
	int y = 0;
	for (auto i : ground) {
		for (auto k : i) {
			if (k == WALL) {
				DrawRotaGraph(x - camera->cameraPos.x, y - camera->cameraPos.y, gManager->graphEx, 0, mapChip[1], false);
			}
			else {
				DrawRotaGraph(x - camera->cameraPos.x, y - camera->cameraPos.y, gManager->graphEx, 0, mapChip[0], false);
			}
			x += 20;
		}
		x = 0;
		y += 20;
	}


}

bool Map::IsOutOfRange(int x, int y)
{
	if (x <= -1 || width < x)return true;
	if (y <= -1 || height < y)return true;
	return false;
}

void Map::SetDivideArea(int Left, int Up, int Right, int Down, int Id)
{
	divideArea.emplace_back();
	int size = divideArea.size();
	//区画に情報を格納
	divideArea[size - 1].emplace_back(Left);
	divideArea[size - 1].emplace_back(Up);
	divideArea[size - 1].emplace_back(Right);
	divideArea[size - 1].emplace_back(Down);
	divideArea[size - 1].emplace_back(Id);

}

void Map::setLargeDivideArea(int Left, int Up, int Right, int Down, int Id)
{
	dumpDivideArea.emplace_back();
	int size = dumpDivideArea.size();
	//区画に情報を格納
	dumpDivideArea[size - 1].emplace_back(Left);
	dumpDivideArea[size - 1].emplace_back(Up);
	dumpDivideArea[size - 1].emplace_back(Right);
	dumpDivideArea[size - 1].emplace_back(Down);
	dumpDivideArea[size - 1].emplace_back(Id);
}

void Map::SetDivideLine(int Start_x, int Start_y, int Goal_x, int Goal_y, int Dir)
{
	divideLine.emplace_back();
	int size = divideLine.size();
	//分割線の情報を格納
	divideLine[size - 1].emplace_back(Start_x);
	divideLine[size - 1].emplace_back(Start_y);
	divideLine[size - 1].emplace_back(Goal_x);
	divideLine[size - 1].emplace_back(Goal_y);
	divideLine[size - 1].emplace_back(Dir);
}

void Map::SetDivideRoom(int Left, int Up, int Right, int Down)
{
	divideRoom.emplace_back();
	int size = divideRoom.size();
	//分割線の情報を格納
	divideRoom[size - 1].emplace_back(Left);
	divideRoom[size - 1].emplace_back(Up);
	divideRoom[size - 1].emplace_back(Right);
	divideRoom[size - 1].emplace_back(Down);
}

void Map::AreaDivide()
{
	int lowerWidth = 0;
	int upperWidth = width - 1;

	int lowerHeight = 0;
	int upperHeight = height - 1;

	int roomId = 0;

	bool isVertical;
	if (GetRand(1) == 1) {
		isVertical = true;
	}
	else {
		isVertical = false;
	}

	while (1) {
		//部屋の最低幅より狭かったら分割しない
		if (upperWidth - lowerWidth < 2 * roomMinWidth + 4) {
			break;
		}
		if (upperHeight - lowerHeight < 2 * roomMinHeight + 4) {
			break;
		}
		//最大部屋数に達したら分割をやめる
		if (roomId > roomMaxNum) {
			break;
		}

		//分割座標
		int dividePoint = 0;
		if (isVertical) {
			//ここで入れるランダムの値が、前回分割したときの大きい方の値だったら良くなる？
			dividePoint = gManager->GetRandEx((lowerWidth + 2 + roomMinWidth), (upperWidth - roomMinWidth - 2));

			if ((upperWidth + lowerWidth) / 2 < dividePoint) {
				SetDivideArea(dividePoint + 2, lowerHeight + 1, upperWidth - 1, upperHeight - 1, roomId);
				//setLargeDivideArea(dividePoint + 2, lowerHeight + 1, upperWidth - 1, upperHeight - 1, roomId);
				upperWidth = dividePoint - 1;
			}
			else {
				SetDivideArea(dividePoint + 1, lowerHeight + 1, upperWidth - 2, upperHeight - 1, roomId);
				//setLargeDivideArea(dividePoint + 1, lowerHeight + 1, upperWidth - 2, upperHeight - 1, roomId);
				lowerWidth = dividePoint + 1;
			}
			SetDivideLine(dividePoint, lowerHeight + 1, dividePoint, upperHeight - 1, VERTICAL);
			isVertical = false;
		}
		else {
			dividePoint = gManager->GetRandEx((lowerHeight + 2 + roomMinHeight), (upperHeight - roomMinHeight - 2));

			if ((upperHeight + lowerHeight) / 2 < dividePoint) {
				SetDivideArea(lowerWidth + 1, dividePoint + 2, upperWidth - 1, upperHeight - 1, roomId);
				//setLargeDivideArea(lowerWidth + 1, dividePoint + 2, upperWidth - 1, upperHeight - 1, roomId);
				upperHeight = dividePoint - 1;
			}
			else {
				SetDivideArea(lowerWidth + 1, lowerHeight + 1, upperWidth - 1, dividePoint - 2, roomId);
				//setLargeDivideArea(lowerWidth + 1, lowerHeight + 1, upperWidth - 1, dividePoint - 2, roomId);
				lowerHeight = dividePoint + 1;
			}
			SetDivideLine(lowerWidth + 1, dividePoint, upperWidth - 1, dividePoint, HORIZONTAL);
			isVertical = true;

		}
		++roomId;

	}

}

void Map::CreateRoom()
{
	for (auto area : divideArea) {

		int left = area[0];
		int up = area[1];
		int right = area[2];
		int down = area[3];

		int roomLeft = gManager->GetRandEx(left, right - roomMinWidth + 1);
		int roomRight = gManager->GetRandEx(roomLeft + roomMinWidth - 1, right);
		int roomUp = gManager->GetRandEx(up, down - roomMinHeight + 1);
		int roomDown = gManager->GetRandEx(roomUp + roomMinHeight - 1, down);

		SetDivideRoom(roomLeft, roomUp, roomRight, roomDown);
	}

}

//通路の数は部屋の数+1
void Map::CreatePassWay()
{
	int count = 0;
	int size = divideLine.size() - 1;

	for (auto line : divideLine) {

		if (count >= size)break;

		int startX = line[0];
		int startY = line[1];
		int goalX = line[2];
		int goalY = line[3];
		int dir = line[4];

		//部屋の情報の取得
		vector<int> roomBefore = divideRoom[count];
		vector<int> roomAfter = divideRoom[count + 1];

		int leftBefore = roomBefore[0];
		int upBefore = roomBefore[1];
		int rightBefore = roomBefore[2];
		int downBefore = roomBefore[3];

		int leftAfter = roomAfter[0];
		int upAfter = roomAfter[1];
		int rightAfter = roomAfter[2];
		int downAfter = roomAfter[3];

		if (dir == VERTICAL) {
			//部屋から通路を生やす位置を決定
			int passWayBefore = gManager->GetRandEx(upBefore + 1, downBefore - 1);
			int passWayAfter = gManager->GetRandEx(upAfter + 1, downAfter - 1);

			//分割線と部屋の相対位置で場合分け
			if (leftBefore < leftAfter) {
				SetAllChip(rightBefore, passWayBefore, startX, passWayBefore);
				SetAllChip(startX, passWayAfter, leftAfter, passWayAfter);
			}
			else {
				SetAllChip(rightAfter, passWayAfter, startX, passWayAfter);
				SetAllChip(startX, passWayBefore, leftBefore, passWayBefore);
			}
			if (passWayBefore > passWayAfter)swap(passWayBefore, passWayAfter);
			SetAllChip(startX, passWayBefore, startX, passWayAfter);
		}
		else {
			//部屋から通路を生やす位置を決定
			int passWayBefore = gManager->GetRandEx(leftBefore + 1, rightBefore - 1);
			int passWayAfter = gManager->GetRandEx(leftAfter + 1, rightAfter - 1);

			//分割線と部屋の相対位置で場合分け
			if (upBefore < upAfter) {
				SetAllChip(passWayBefore, downBefore, passWayBefore, startY);
				SetAllChip(passWayAfter, startY, passWayAfter, upAfter);
			}
			else {
				SetAllChip(passWayAfter, downAfter, passWayAfter, startY);
				SetAllChip(passWayBefore, startY, passWayBefore, upBefore);
			}
			if (passWayBefore > passWayAfter)swap(passWayBefore, passWayAfter);
			SetAllChip(passWayBefore, startY, passWayAfter, startY);
		}
		count++;
	}



}
