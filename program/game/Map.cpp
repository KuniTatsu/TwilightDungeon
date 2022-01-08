#include "Map.h"
#include"GameManager.h"
#include"DxLib.h"

extern GameManager* gManager;

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
	mapChip[0] = gManager->LoadGraphEx("graphics/canWalk.png");
	mapChip[1] = gManager->LoadGraphEx("graphics/Wall.png");

	width = Width;
	height = Height;

	//�c�̏�����
	ground.resize(height);
	for (auto v : ground)v.resize(width);
	for (int i = 0; i < height; ++i) {
		ground[i].resize(width);
	}
	//���ׂĂ�ǂɂ���
	for (int i = 0; i < height; ++i) {
		for (int k = 0; k < width; ++k) {
			ground[i][k] = WALL;
		}
	}

}

void Map::MapDraw()
{
	int y = 0;
	for (auto i : ground) {
		for (auto k : i) {
			int x = 0;
			if (k == WALL) {
				DrawRotaGraph(x, y, 1, 0, mapChip[1], false);
			}
			else {
				DrawRotaGraph(x, y, 1, 0, mapChip[0], false);
			}
			x += 50;
		}
		y += 50;
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
	//���ɏ����i�[
	divideArea[size - 1].emplace_back(Left);
	divideArea[size - 1].emplace_back(Up);
	divideArea[size - 1].emplace_back(Right);
	divideArea[size - 1].emplace_back(Down);
	divideArea[size - 1].emplace_back(Id);

}

void Map::SetDivideLine(int Start_x, int Start_y, int Goal_x, int Goal_y, int Dir)
{
	divideLine.emplace_back();
	int size = divideLine.size();
	//�������̏����i�[
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
	//�������̏����i�[
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
		//�����̍Œᕝ��苷�������番�����Ȃ�
		if (upperWidth - lowerWidth < 2 * roomMinWidth + 4)break;
		if (upperHeight - lowerHeight < 2 * roomMinHeight + 4)break;
		//�ő啔�����ɒB�����番������߂�
		if (roomId > roomMaxNum)break;

		//�������W
		int dividePoint = 0;
		if (isVertical) {

		}
		else {


		}

	}

}

void Map::CreateRoom()
{
}

void Map::CreatePassWay()
{
}
