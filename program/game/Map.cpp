#include "Map.h"
#include"GameManager.h"
#include"DxLib.h"
#include"Camera.h"
#include"Player.h"	

extern GameManager* gManager;

Map::Map(int Width, int Height)
{
	mapChip[0] = gManager->LoadGraphEx("graphics/floor.png");//��
	mapChip[1] = gManager->LoadGraphEx("graphics/Wall.png");//�����̕�
	mapChip[2] = gManager->LoadGraphEx("graphics/Stairs_.png");//�K�i

	ue = gManager->LoadGraphEx("graphics/Wall_top.png");//top
	shita = gManager->LoadGraphEx("graphics/Wall_bottom.png");//bottom
	hidari = gManager->LoadGraphEx("graphics/Wall_leftside.png");//left
	migi = gManager->LoadGraphEx("graphics/Wall_rightside.png");//right

	voidGh = gManager->LoadGraphEx("graphics/void.png");

	//mapChip[7] = gManager->LoadGraphEx("graphics/EXPASSWAY.png");//debug

	miniMapChip[0] = gManager->LoadGraphEx("graphics/mini_PassWay.png");
	miniMapChip[1] = gManager->LoadGraphEx("graphics/mini_Stair.png");

	miniPlayer = gManager->LoadGraphEx("graphics/PlayerTest.png");
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

void Map::SetAllChip(int Left, int Up, int Right, int Down)
{
	for (int i = Up; i <= Down; ++i) {
		for (int k = Left; k <= Right; ++k) {
			SetChip(k, i, PASSWAY);
		}
	}
}

void Map::DivideStart(int Width, int Height, Map* map)
{
	AreaDivide();

	CreateRoom();

	gManager->InitWayPointVector(GetRoomNum());

	CreatePassWay();

	for (auto room : divideRoom) {
		SetAllChip(room[0], room[1], room[2], room[3]);
	}
	sumRoomNum = divideRoom.size();
}

t2k::Vector3 Map::GetRoomValue(int roomNum)
{
	int x = abs(divideRoom[roomNum][2] - divideRoom[roomNum][0]);
	int y = abs(divideRoom[roomNum][3] - divideRoom[roomNum][1]);
	return t2k::Vector3(x + 1, y + 1, 0);
}

t2k::Vector3 Map::GetRoomStartPos(int roomNum)
{
	int Left = divideRoom[roomNum][0];
	int Up = divideRoom[roomNum][1];
	return t2k::Vector3(Left, Up, 0);
}

int Map::CheckIsThere(int x, int y)
{
	bool isThere = false;
	//�S�Ă̕����̍��W���`�F�b�N
	//x,y�����͈͓̔��ɂ���Ȃ炻�̕����ɑ��݂���
	for (auto room : divideRoom) {
		if (x > room[2])continue;
		if (y > room[3])continue;
		if (x >= room[0] && y >= room[1]) {
			return room[4];
			isThere = true;
		}
	}
	/*if (!isThere)*/return -1;

}

void Map::GetAllChip(int roomNum, std::vector<std::vector<int>>& chips)
{

	t2k::Vector3 hoge = GetRoomValue(roomNum);
	//�z�����U���Z�b�g
	chips.clear();

	//�����̏c�̑傫���Ŕz��̑傫����ݒ�
	chips.resize(hoge.y);

	int Left = divideRoom[roomNum][0];
	int Up = divideRoom[roomNum][1];
	int Right = divideRoom[roomNum][2];
	int Down = divideRoom[roomNum][3];

	int g = 0;
	for (int i = Up; i <= Down; ++i) {
		for (int k = Left; k <= Right; ++k) {
			chips[g].emplace_back(GetChip(k, i));
		}
		++g;
	}
	/*t2k::Vector3 hoge = GetRoomValue(roomNum);
	for (int i = 0; i < hoge.x; ++i) {
		for (int k = 0; k < hoge.y; ++k) {

			chips[k][i] = ground[divideRoom[roomNum][0] + i][divideRoom[roomNum][1] + k];
			chips[k][i] = ground[divideRoom[roomNum][0] + i][divideRoom[roomNum][1] + k];
		}
	}*/

}

void Map::SetChip(int x, int y, int SetChip)
{
	if (IsOutOfRange(x, y))return;
	ground[y][x] = SetChip;
}

void Map::MapDraw()
{
	//**debug
	if (gManager->isDebug) {
		int count = 0;
		for (auto hoge : divideArea) {

			int x1 = (hoge[0] - 1) * 20 - gManager->camera->cameraPos.x;
			int y1 = (hoge[1] - 1) * 20 - gManager->camera->cameraPos.y;
			int x2 = (hoge[2] + 1) * 20 - gManager->camera->cameraPos.x;
			int y2 = (hoge[3] + 1) * 20 - gManager->camera->cameraPos.y;

			DrawBox(x1, y1, x2, y2, colors[count], true);

			count = (count + 1) % 5;
		}
		for (auto hoge : divideLine) {
			int x1 = hoge[0] * 20 - gManager->camera->cameraPos.x;
			int y1 = hoge[1] * 20 - gManager->camera->cameraPos.y;
			int x2 = hoge[2] * 20 - gManager->camera->cameraPos.x;
			int y2 = hoge[3] * 20 - gManager->camera->cameraPos.y;


			DrawLine(x1, y1, x2, y2, -1);
		}
		//**
	}
	for (int i = 0; i < ground.size(); ++i) {
		for (int k = 0; k < ground[i].size(); ++k) {
			if (ground[i][k] == WALL) {
				if (!CheckAround(i, k))continue; //DrawRotaGraph(k * 20 - gManager->camera->cameraPos.x, i * 20 - gManager->camera->cameraPos.y, gManager->graphEx, 0, voidGh, false);
				//DrawRotaGraphF(k * gManager->nowGraphicSize - gManager->camera->cameraPos.x, i * gManager->nowGraphicSize - gManager->camera->cameraPos.y, gManager->graphEx, 0, mapChip[1], false);

				int iti = CheckAroundWay(i, k);
				if (iti == ROOMWALL)				DrawRotaGraph(k * 20 - gManager->camera->cameraPos.x, i * 20 - gManager->camera->cameraPos.y, gManager->graphEx, 0, mapChip[1], false);
				else if (iti == ROOMTOP)			DrawRotaGraph(k * 20 - gManager->camera->cameraPos.x, i * 20 - gManager->camera->cameraPos.y, gManager->graphEx, 0, ue, false);
				else if (iti == ROOMRIGHT)			DrawRotaGraph(k * 20 - gManager->camera->cameraPos.x, i * 20 - gManager->camera->cameraPos.y, gManager->graphEx, 0, migi, true);
				else if (iti == ROOMBOTTOM)			DrawRotaGraph(k * 20 - gManager->camera->cameraPos.x, i * 20 - gManager->camera->cameraPos.y, gManager->graphEx, 0, shita, true);
				else if (iti == ROOMLEFT)			DrawRotaGraph(k * 20 - gManager->camera->cameraPos.x, i * 20 - gManager->camera->cameraPos.y, gManager->graphEx, 0, hidari, true);
				else if (iti == ROOMLEFTTOP)		DrawRotaGraph(k * 20 - gManager->camera->cameraPos.x, i * 20 - gManager->camera->cameraPos.y, gManager->graphEx, 0, mapChip[1], false);
				else if (iti == ROOMRIGHTTOP)		DrawRotaGraph(k * 20 - gManager->camera->cameraPos.x, i * 20 - gManager->camera->cameraPos.y, gManager->graphEx, 0, mapChip[1], false);
				else if (iti == ROOMLEFTBOTTOM)		DrawRotaGraph(k * 20 - gManager->camera->cameraPos.x, i * 20 - gManager->camera->cameraPos.y, gManager->graphEx, 0, mapChip[1], false);
				else if (iti == ROOMRIGHTBOTTOM)	DrawRotaGraph(k * 20 - gManager->camera->cameraPos.x, i * 20 - gManager->camera->cameraPos.y, gManager->graphEx, 0, mapChip[1], false);
			}
			else if (ground[i][k] == PASSWAY)DrawRotaGraph(k * 20 - gManager->camera->cameraPos.x, i * 20 - gManager->camera->cameraPos.y, gManager->graphEx, 0, mapChip[0], false);
			else if (ground[i][k] == STAIRS)DrawRotaGraph(k * 20 - gManager->camera->cameraPos.x, i * 20 - gManager->camera->cameraPos.y, gManager->graphEx, 0, mapChip[2], false);
			//else if (ground[i][k] == EXPASSWAY)DrawRotaGraph(k * 20 - gManager->camera->cameraPos.x, i * 20 - gManager->camera->cameraPos.y, gManager->graphEx, 0, mapChip[3], false);

		}
	}

	//debug
	if (gManager->isDebug) {
		DrawAllRoomPos(divideRoom);
	}
}

void Map::MiniMapDraw()
{
	int x = 150;
	int y = 50;
	for (auto i : ground) {
		for (auto k : i) {
			if (k == PASSWAY) {
				DrawRotaGraph(x, y, 0.5, 0, miniMapChip[0], false);
			}
			else if (k == STAIRS) {
				DrawRotaGraph(x, y, 0.5, 0, miniMapChip[0], false);
				DrawRotaGraph(x, y, 0.5, 0, miniMapChip[1], false);
			}
			else if (k == EXPASSWAY) {
				DrawRotaGraph(x, y, 0.5, 0, miniMapChip[0], false);
			}

			x += 10;
		}
		x = 150;
		y += 10;
	}
	t2k::Vector3 PlayerPos = gManager->WorldToLocal(player->pos);
	DrawRotaGraph(PlayerPos.x * 10 + 150, PlayerPos.y * 10 + 50, 0.5, 0, miniPlayer, true);
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

void Map::SetLargeDivideArea(int Left, int Up, int Right, int Down, int Id)
{
	dumpDivideArea.emplace_back();
	int size = dumpDivideArea.size();
	//���ɏ����i�[
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
	//�������̏����i�[
	divideLine[size - 1].emplace_back(Start_x);
	divideLine[size - 1].emplace_back(Start_y);
	divideLine[size - 1].emplace_back(Goal_x);
	divideLine[size - 1].emplace_back(Goal_y);
	divideLine[size - 1].emplace_back(Dir);
}

void Map::SetDivideRoom(int Left, int Up, int Right, int Down, int RoomId)
{
	divideRoom.emplace_back();
	int size = divideRoom.size();
	//�����̏����i�[
	divideRoom[size - 1].emplace_back(Left);
	divideRoom[size - 1].emplace_back(Up);
	divideRoom[size - 1].emplace_back(Right);
	divideRoom[size - 1].emplace_back(Down);
	divideRoom[size - 1].emplace_back(RoomId);
	//SetAllChip(Left, Up, Right, Down);
}

void Map::AreaDivide()
{
	int lowerWidth = 0;
	int upperWidth = width - 1;

	int lowerHeight = 0;
	int upperHeight = height - 1;

	int roomId = 0;



	while (1) {
		if (roomId > 0) {
			//�����̍Œᕝ��苷�������番�����Ȃ�
			if (dumpDivideArea[roomId - 1][2] - dumpDivideArea[roomId - 1][0] < 2 * roomMinWidth + 4) {
				break;
			}
			if (dumpDivideArea[roomId - 1][3] - dumpDivideArea[roomId - 1][1] < 2 * roomMinHeight + 4) {
				break;
			}
		}
		//�ő啔�����ɒB�����番������߂�
		if (roomId > roomMaxNum) {
			break;
		}


		//�������W
		int dividePoint = 0;

		//���������畔���܂ł͍Œ�2����

		//�ŏ��̕�����������
		if (!doneFirstDivide) {
			//���������c���傫��������
			if ((upperWidth - lowerWidth) > (upperHeight - lowerHeight)) {
				//���̂ǂ����ŏc�ɐ؂�
				dividePoint = gManager->GetRandEx((lowerWidth + 2 + roomMinWidth), (upperWidth - roomMinWidth - 2));

				//������荶�Ȃ� �����̕������������A�E�̕����͍L��
				if (dividePoint < (upperWidth / 2)) {
					//����������o�^����
					SetDivideArea(lowerWidth + 1, lowerHeight + 1, dividePoint - 2, upperHeight - 1, roomId);
					//�傫������o�^����
					SetLargeDivideArea(dividePoint + 1, lowerHeight - 1, upperWidth - 1, upperHeight - 1, roomId);
				}
				//�������E�Ȃ� ���E�̕������������A���̕����͍L��
				else {
					//����������o�^����
					SetDivideArea(dividePoint + 2, lowerHeight + 1, upperWidth - 1, upperHeight - 1, roomId);
					//�傫������o�^����
					SetLargeDivideArea(lowerWidth + 1, lowerHeight - 1, dividePoint - 1, upperHeight - 1, roomId);
				}
				//������������
				SetDivideLine(dividePoint, lowerHeight + 1, dividePoint, upperHeight - 1, VERTICAL);
			}
			//�����c�������傫��������
			else {
				//�c�̂ǂ����ŉ��ɂ���
				dividePoint = gManager->GetRandEx((lowerHeight + 2 + roomMinHeight), (upperHeight - roomMinHeight - 2));
				//��������Ȃ� ����̕������������A���̕����͍L��
				if (dividePoint < (upperHeight / 2)) {
					//����������o�^����
					SetDivideArea(lowerWidth + 1, lowerHeight + 1, upperWidth - 1, dividePoint - 2, roomId);
					//�傫������o�^����
					SetLargeDivideArea(lowerWidth + 1, dividePoint + 1, upperWidth - 1, upperHeight - 1, roomId);

				}
				//������艺�Ȃ� ����̕������傫���A���̕����͏�����
				else {
					//����������o�^����
					SetDivideArea(lowerWidth + 1, dividePoint + 2, upperWidth - 1, upperHeight - 1, roomId);
					//�傫������o�^����
					SetLargeDivideArea(lowerWidth + 1, lowerHeight + 1, upperWidth - 1, dividePoint - 1, roomId);
				}

				//������������
				SetDivideLine(lowerWidth + 1, dividePoint, upperWidth - 1, dividePoint, HORIZONTAL);
			}
			//�ŏ��̕������I������@�ȍ~��SetLargeDivideArea�ɓo�^���ꂽ�������X�ɕ������Ă���
			doneFirstDivide = true;
			++roomId;
			t2k::debugTrace("\nroonId:%d\n", roomId);
			continue;
		}

		else
		{
			//��O�ɓo�^���ꂽ�����������̃f�[�^
			int lastLeft = divideArea[roomId - 1][0];
			int lastUp = divideArea[roomId - 1][1];
			int lastRight = divideArea[roomId - 1][2];
			int lastDown = divideArea[roomId - 1][3];

			int dir = divideLine[roomId - 1][4];

			/*int prev_2_Left = divideArea[roomId - 2][0];
			int prev_2_Up = divideArea[roomId - 2][1];
			int prev_2_Right = divideArea[roomId - 2][2];
			int prev_2_Down = divideArea[roomId - 2][3];*/

			//���񕪊�����G���A�̃f�[�^
			int left = dumpDivideArea[roomId - 1][0];
			int up = dumpDivideArea[roomId - 1][1];
			int right = dumpDivideArea[roomId - 1][2];
			int down = dumpDivideArea[roomId - 1][3];

			//���������c���傫��������
			if ((right - left) > (down - up)) {
				int midPoint = (left + right) / 2;
				//1�O�̃G���A�����ɂ��邩�E�ɂ��邩�m���߂�
				//��O�̕������c�̏ꍇ�͕����_�ŏꍇ����
				//��O���c�̕����@���@�����������O�̃G���A���ׂ荇���悤�ɐ؂�Ȃ���΂����Ȃ�
				if (dir == VERTICAL) {

					//1�O�̃G���A�̉E����������G���A�̍���菬������1�O�̃G���A�͍��ɂ���
					if (lastRight < left) {

						//�����_�͕�������G���A�̍����ɂȂ���΂����Ȃ�
						dividePoint = gManager->GetRandEx((left + 2 + roomMinWidth), (midPoint/* - roomMinWidth - 2*/));
						//�K�������G���A�̍����̕������������Ȃ�
						//�����������̓o�^
						SetDivideArea(left + 1, up + 1, dividePoint - 2, down - 1, roomId);
						//�傫�������̓o�^
						SetLargeDivideArea(dividePoint + 1, up + 1, right - 1, down - 1, roomId);
					}
					//1�O�̃G���A���E�ɂ���
					else {
						//�����_�͉E���ɂȂ���΂����Ȃ�
						dividePoint = gManager->GetRandEx((midPoint /*+ 2 + roomMinWidth*/), (right - roomMinWidth - 2));
						//�K���E���̕������������Ȃ�
						//����������o�^����
						SetDivideArea(dividePoint + 2, up + 1, right - 1, down - 1, roomId);
						//�傫������o�^����
						SetLargeDivideArea(left + 1, up + 1, dividePoint - 1, down - 1, roomId);

					}
				}
				else {
					//��O�����̕����@���@�����������ǂ��؂��Ă��ׂ荇��
					dividePoint = gManager->GetRandEx((left + 2 + roomMinWidth), (right - roomMinWidth - 2));
					//������荶�Ȃ� �����̕������������A�E�̕����͍L��
					if (dividePoint < ((right + left) / 2)) {
						//����������o�^����
						SetDivideArea(left + 1, up + 1, dividePoint - 2, down - 1, roomId);
						//�傫������o�^����
						SetLargeDivideArea(dividePoint + 1, up + 1, right - 1, down - 1, roomId);
					}
					//�������E�Ȃ� ���E�̕������������A���̕����͍L��
					else {
						//����������o�^����
						SetDivideArea(dividePoint + 2, up + 1, right - 1, down - 1, roomId);
						//�傫������o�^����
						SetLargeDivideArea(left + 1, up + 1, dividePoint - 1, down - 1, roomId);
					}
				}
				//������������
				SetDivideLine(dividePoint, up + 1, dividePoint, down - 1, VERTICAL);
			}
			//�����c�������傫��������
			else {
				int midPoint = (up + down) / 2;
				//1�O�̃G���A����ɂ��邩���ɂ��邩�m���߂�
				// 1�O�̕��������Ȃ番���_��1�O�̃G���A���
				if (dir == HORIZONTAL) {


					//��O�̃G���A�̏オ�����G���A�̉������傫����Έ�O�̃G���A�͉��ɂ���
					if (down < lastUp) {
						//�����_�͕�������G���A�̉����ɂȂ���΂����Ȃ�
						dividePoint = gManager->GetRandEx((midPoint + 2 + roomMinHeight), (down - roomMinHeight - 2));
						//�K�������G���A�̉����̕������������Ȃ�
						//�����������̓o�^

						SetDivideArea(left + 1, dividePoint + 2, right - 1, down - 1, roomId);
						//�傫�������̓o�^
						SetLargeDivideArea(left + 1, up + 1, right - 1, dividePoint - 1, roomId);
					}
					//��O�̃G���A����ɂ���
					else {
						//�����_�͕�������G���A�̏㑤�ɂȂ���΂����Ȃ�
						dividePoint = gManager->GetRandEx((up + 2 + roomMinHeight), (midPoint - roomMinHeight - 2));
						//�K���㑤�̕������������Ȃ�
						//����������o�^����
						SetDivideArea(left + 1, up + 1, right - 1, dividePoint - 2, roomId);
						//�傫������o�^����
						SetLargeDivideArea(left + 1, dividePoint + 1, right - 1, down - 1, roomId);

					}
				}
				// 1�O�̕������c�Ȃ番���_�ǂ��ł��G���A�ׂ͗荇��
				else {
					dividePoint = gManager->GetRandEx((up + 2 + roomMinWidth), (down - roomMinWidth - 2));
					//������艺�Ȃ� �����̕������������A��̕����͍L��
					if (dividePoint < ((up + down) / 2)) {
						//����������o�^����
						SetDivideArea(left + 1, up + 1, right - 1, dividePoint - 2, roomId);
						//�傫������o�^����
						SetLargeDivideArea(left + 1, dividePoint + 2, right - 1, down - 1, roomId);
					}
					//��������Ȃ� ����̕������������A���̕����͍L��
					else {
						//����������o�^����
						SetDivideArea(left + 1, dividePoint + 2, right - 1, down - 1, roomId);
						//�傫������o�^����
						SetLargeDivideArea(left + 1, up + 1, right - 1, dividePoint - 2, roomId);
					}
				}
				//������������
				SetDivideLine(left + 1, dividePoint, right - 1, dividePoint, HORIZONTAL);
			}
			++roomId;
			t2k::debugTrace("\nroonId:%d\n", roomId);
			continue;
		}
		
	}
}

void Map::CreateRoom()
{
	for (auto area : divideArea) {

		int left = area[0];
		int up = area[1];
		int right = area[2];
		int down = area[3];
		int id = area[4];

		int roomLeft = gManager->GetRandEx(left, right - roomMinWidth + 2);
		int roomRight = gManager->GetRandEx(roomLeft + roomMinWidth - 2, right);
		int roomUp = gManager->GetRandEx(up, down - roomMinHeight + 2);
		int roomDown = gManager->GetRandEx(roomUp + roomMinHeight - 2, down);

		SetDivideRoom(roomLeft, roomUp, roomRight, roomDown, id);
	}

}

//�ʘH�̐��͕����̐�+1
void Map::CreatePassWay()
{
	int count = 0;
	int size = divideLine.size() - 1;

	for (auto line : divideLine) {
		t2k::debugTrace("\n�ʉ�%d���\n", count);
		if (count >= size)break;

		int startX = line[0];
		int startY = line[1];
		int goalX = line[2];
		int goalY = line[3];
		int dir = line[4];

		//�����̏��̎擾
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

		//���������c�Ȃ�
		if (dir == VERTICAL) {
			//��������ʘH�𐶂₷�ʒu������
			//y���W
			int passWayBefore = gManager->GetRandEx(upBefore + 1, downBefore - 1);
			int passWayAfter = gManager->GetRandEx(upAfter + 1, downAfter - 1);

			//�����̑��Έʒu�ŏꍇ����
			//�����O�̕�����������̕�����荶�ɂ����
			if (leftBefore < leftAfter) {
				//�ʘH�̊J�n�n�_��ۑ�����
				//�O�̕���
				gManager->SetRoomWayPoint(t2k::Vector3(rightBefore + 1, passWayBefore, 0), roomBefore[4]);
				//���Ƃ̕���
				gManager->SetRoomWayPoint(t2k::Vector3(leftAfter - 1, passWayAfter, 0), roomAfter[4]);

				//�O�̕������番�����ɒ��������������
				SetAllChip(rightBefore, passWayBefore, startX, passWayBefore);
				//���������炠�Ƃ̕����֒��������������
				SetAllChip(startX, passWayAfter, leftAfter, passWayAfter);
			}
			else {
				//�ʘH�̊J�n�n�_��ۑ�����
				//�O�̕���
				gManager->SetRoomWayPoint(t2k::Vector3(leftBefore - 1, passWayBefore, 0), roomBefore[4]);
				//���Ƃ̕���
				gManager->SetRoomWayPoint(t2k::Vector3(rightAfter + 1, passWayAfter, 0), roomAfter[4]);

				SetAllChip(rightAfter, passWayAfter, startX, passWayAfter);
				SetAllChip(startX, passWayBefore, leftBefore, passWayBefore);
			}
			if (passWayBefore > passWayAfter)swap(passWayBefore, passWayAfter);
			SetAllChip(startX, passWayBefore, startX, passWayAfter);
		}
		//�����������Ȃ�
		else {
			//��������ʘH�𐶂₷�ʒu������
			//x���W
			int passWayBefore = gManager->GetRandEx(leftBefore + 1, rightBefore - 1);
			int passWayAfter = gManager->GetRandEx(leftAfter + 1, rightAfter - 1);

			//�����̑��Έʒu�ŏꍇ����
			//�O�̕�����肠�Ƃ̕��������ɂ����
			if (upBefore < upAfter) {
				//�ʘH�̊J�n�n�_��ۑ�����
				//�O�̕���
				gManager->SetRoomWayPoint(t2k::Vector3(passWayBefore, downBefore + 1, 0), roomBefore[4]);
				//���Ƃ̕���
				gManager->SetRoomWayPoint(t2k::Vector3(passWayAfter, upAfter - 1, 0), roomAfter[4]);

				SetAllChip(passWayBefore, downBefore, passWayBefore, startY);
				SetAllChip(passWayAfter, startY, passWayAfter, upAfter);
			}
			else {
				//�ʘH�̊J�n�n�_��ۑ�����
				//�O�̕���
				gManager->SetRoomWayPoint(t2k::Vector3(passWayBefore, upBefore - 1, 0), roomBefore[4]);
				//���Ƃ̕���
				gManager->SetRoomWayPoint(t2k::Vector3(passWayAfter, downAfter + 1, 0), roomAfter[4]);

				SetAllChip(passWayAfter, downAfter, passWayAfter, startY);
				SetAllChip(passWayBefore, startY, passWayBefore, upBefore);
			}
			if (passWayBefore > passWayAfter)swap(passWayBefore, passWayAfter);
			SetAllChip(passWayBefore, startY, passWayAfter, startY);
		}
		count++;
	}
	///////////�����Ŗ������[�v�ɓ����Ă�/////////////
	//0:��, 1 : �E, 2 : ��, 3 : ��
	int dir = 0;
	//�ŏ��̕����̏o���_���擾����
	t2k::Vector3 start = RandomPoint(0, dir);
	CreateSecondWay(start.x, start.y, dir, 0);

	//�Ō�̕����̏o���_���擾����
	int lastroomId = divideRoom.back()[4];
	int lastdir = 0;
	t2k::Vector3 laststart = RandomPoint(lastroomId, lastdir);
	CreateSecondWay(laststart.x, laststart.y, lastdir, lastroomId);


}
//0:��, 1 : �E, 2 : ��, 3 : ��
// �Ō�̕����̏ꍇ�͎擾���镪�����͈�O�̕���������Ȃ��Ƃ����Ȃ�
t2k::Vector3 Map::RandomPoint(int roomId, int& dir)
{
	//**********�Ō�̕����̏ꍇ**********
	if (roomId != 0) {
		if (divideLine[roomId - 2][4] == VERTICAL) {
			//��������x���W�ƕ�����x���W���ׂ�
			//��������蕔���̍����傫����Ε������͍��ɂ���
			if (divideLine[roomId - 2][0] < divideRoom[roomId][0]) {
				int point = 0;
				while (1) {
					//�����̍��[�̂ǂ�����y���W
					point = gManager->GetRandEx(divideRoom[roomId][1], divideRoom[roomId][3]);
					if ((GetChip(divideRoom[roomId][0] - 1, point) == 0 && GetChip(divideRoom[roomId][0] - 1, point - 1) == 0 && GetChip(divideRoom[roomId][0] - 1, point + 1)) == 0) break;
				}
				dir = 3;
				int pointX = divideRoom[roomId][0];
				gManager->SetRoomWayPoint(t2k::Vector3(pointX - 1, point, 0), divideRoom[roomId][4]);
				return t2k::Vector3(pointX, point, 0);
			}
			else {
				int point = 0;
				while (1) {
					//�����̉E�[�̂ǂ�����y���W
					point = gManager->GetRandEx(divideRoom[roomId][1], divideRoom[roomId][3]);
					if ((GetChip(divideRoom[roomId][0] + 1, point)) == 0 && (GetChip(divideRoom[roomId][0] + 1, point - 1)) == 0 && (GetChip(divideRoom[roomId][0] + 1, point + 1)) == 0) break;
				}
				dir = 1;
				int pointX = divideRoom[roomId][2];
				gManager->SetRoomWayPoint(t2k::Vector3(pointX + 1, point, 0), divideRoom[roomId][4]);
				return t2k::Vector3(pointX, point, 0);
			}
		}//���Ȃ�ォ��
		else {
			//��������y���W�ƕ�����y���W���ׂ�
			//��������蕔���̏オ�傫����Ε������͏�ɂ���
			if (divideLine[roomId - 2][1] < divideRoom[roomId][1]) {
				int point = 0;
				while (1) {
					//�����̏�[�̂ǂ�����x���W
					point = gManager->GetRandEx(divideRoom[roomId][0], divideRoom[roomId][2]);
					if ((GetChip(point, divideRoom[roomId][1] - 1)) == 0 && (GetChip(point - 1, divideRoom[roomId][1] - 1)) == 0 && (GetChip(point + 1, divideRoom[roomId][1] - 1)) == 0) break;
				}
				dir = 0;
				int pointY = divideRoom[roomId][1];
				gManager->SetRoomWayPoint(t2k::Vector3(point, pointY - 1, 0), divideRoom[roomId][4]);
				return t2k::Vector3(point, pointY, 0);
			}
			else {
				int point = 0;
				while (1) {
					//�����̉��[�̂ǂ�����x���W
					point = gManager->GetRandEx(divideRoom[roomId][0], divideRoom[roomId][2]);
					if ((GetChip(point, divideRoom[roomId][3] + 1)) == 0 && (GetChip(point - 1, divideRoom[roomId][3] + 1)) == 0 && (GetChip(point + 1, divideRoom[roomId][3] + 1)) == 0) break;
				}
				dir = 2;
				int pointY = divideRoom[roomId][3];
				gManager->SetRoomWayPoint(t2k::Vector3(point, pointY + 1, 0), divideRoom[roomId][4]);
				return t2k::Vector3(point, pointY, 0);
			}
		}


	}
	//**********�Ō�̕����̏ꍇ**********

	//**********�ŏ��̕����̏ꍇ**********

	//�����̕������猩�ĕ��������ǂ��ɂ��邩���ׂ�
	//���������c�Ȃ獶���E
	if (divideLine[roomId][4] == VERTICAL) {
		//��������x���W�ƕ�����x���W���ׂ�
		//��������蕔���̍����傫����Ε������͍��ɂ���
		if (divideLine[roomId][0] < divideRoom[roomId][0]) {
			int point = 0;
			while (1) {
				//�����̍��[�̂ǂ�����y���W
				point = gManager->GetRandEx(divideRoom[roomId][1], divideRoom[roomId][3]);
				if ((GetChip(divideRoom[roomId][0] - 1, point)) == 0 && (GetChip(divideRoom[roomId][0] - 1, point + 1)) == 0 && (GetChip(divideRoom[roomId][0] - 1, point - 1)) == 0) break;
			}
			dir = 3;
			int pointX = divideRoom[roomId][0];
			gManager->SetRoomWayPoint(t2k::Vector3(pointX - 1, point, 0), divideRoom[roomId][4]);
			return t2k::Vector3(pointX, point, 0);
		}
		else {
			int point = 0;
			while (1) {
				//�����̉E�[�̂ǂ�����y���W
				point = gManager->GetRandEx(divideRoom[roomId][1], divideRoom[roomId][3]);
				if ((GetChip(divideRoom[roomId][2] + 1, point)) == 0 && (GetChip(divideRoom[roomId][2] + 1, point - 1)) == 0 && (GetChip(divideRoom[roomId][2] + 1, point + 1)) == 0) break;
			}
			dir = 1;
			int pointX = divideRoom[roomId][2];
			gManager->SetRoomWayPoint(t2k::Vector3(pointX + 1, point, 0), divideRoom[roomId][4]);
			return t2k::Vector3(pointX, point, 0);
		}
	}//���Ȃ�ォ��
	else {
		//��������y���W�ƕ�����y���W���ׂ�
		//��������蕔���̏オ�傫����Ε������͏�ɂ���
		if (divideLine[roomId][1] < divideRoom[roomId][1]) {
			int point = 0;
			while (1) {
				//�����̏�[�̂ǂ�����y���W
				point = gManager->GetRandEx(divideRoom[roomId][0], divideRoom[roomId][2]);
				if ((GetChip(point, divideRoom[roomId][1] - 1)) == 0 && (GetChip(point - 1, divideRoom[roomId][1] - 1)) == 0 && (GetChip(point + 1, divideRoom[roomId][1] - 1)) == 0) break;
			}
			dir = 0;
			int pointY = divideRoom[roomId][1];
			gManager->SetRoomWayPoint(t2k::Vector3(point, pointY - 1, 0), divideRoom[roomId][4]);
			return t2k::Vector3(point, pointY, 0);
		}
		else {
			int point = 0;
			while (1) {
				//�����̉��[�̂ǂ�����y���W
				point = gManager->GetRandEx(divideRoom[roomId][0], divideRoom[roomId][2]);
				if ((GetChip(point, divideRoom[roomId][3] + 1)) == 0 && (GetChip(point - 1, divideRoom[roomId][3] + 1)) == 0 && (GetChip(point + 1, divideRoom[roomId][3] + 1)) == 0) break;
			}
			dir = 2;
			int pointY = divideRoom[roomId][3];
			gManager->SetRoomWayPoint(t2k::Vector3(point, pointY + 1, 0), divideRoom[roomId][4]);
			return t2k::Vector3(point, pointY, 0);
		}
	}
	//**********�ŏ��̕����̏ꍇ**********

	//return t2k::Vector3(-1, -1, -1);
}
//0:��, 1 : �E, 2 : ��, 3 : ��
bool Map::CreateSecondWay(int x, int y, int dir, int roomId)
{
	if (x - 1 < 0 || y - 1 < 0)return true;
	if (x + 1 > gManager->MAPWIDTH - 1 || y + 1 > gManager->MAPHEIGHT - 1)return true;
	int chip = 0;
	int setX = 0;
	int setY = 0;

	if (dir == 0) {
		setX = x;
		setY = y - 1;
		SetChip(setX, setY, PASSWAY/* EXPASSWAY*/);
		//chip = GetChip(x, y - 1);
	}
	else if (dir == 1) {
		setX = x + 1;
		setY = y;
		SetChip(setX, setY, PASSWAY/*EXPASSWAY*/);
		//chip = GetChip(x + 1, y);
	}
	else if (dir == 2) {
		setX = x;
		setY = y + 1;
		SetChip(setX, setY, PASSWAY /*EXPASSWAY*/);
		//chip = GetChip(x, (y + 1));
	}
	else if (dir == 3) {
		setX = x - 1;
		setY = y;
		SetChip(setX, setY, PASSWAY/* EXPASSWAY*/);
		//chip = GetChip((x - 1), y);
	}
	//�����ʘH�̉��̂ǂ��炩���A�i�s�����ɒʘH�������
	if (CheckChip(setX, setY, dir))
	{

		return true;
	}

	//��������x,y���W�Ɨׂ̕����̕ӂ̂ǂ�����x,�܂���y���W�������Ȃ璼�p�ɋȂ���
	//�ŏ��̕����Ȃ�
	if (roomId == 0) {
		//�ォ���Ȃ�
		if (dir == 0 || dir == 2) {
			//�����͈͓̔��ɂ��邩�m�F
			if (divideRoom[roomId + 2][1]<setY && divideRoom[roomId + 2][3]>setY) {
				//���������ɂ���Ȃ�
				if (divideRoom[roomId + 2][2] < setX) {
					return CreateSecondWay(setX, setY, 3, roomId);
				}
				else {
					return CreateSecondWay(setX, setY, 1, roomId);
				}
			}
		}
		//�E�����Ȃ�
		else if (dir == 1 || dir == 3) {
			if (divideRoom[roomId + 2][0]<setX && divideRoom[roomId + 2][2]>setX) {
				//��������ɂ���
				if (divideRoom[roomId + 2][3] < setY) {
					return CreateSecondWay(setX, setY, 0, roomId);
				}
				else {
					return CreateSecondWay(setX, setY, 2, roomId);
				}
			}
		}
	}
	//�Ō�̂ւ�Ȃ�
	else if (roomId == divideRoom.back()[4]) {
		if (dir == 0 || dir == 2) {
			//��
			if (divideRoom[roomId - 2][1]<setY && divideRoom[roomId - 2][3]>setY) {
				//���������ɂ���
				if (divideRoom[roomId - 2][2] < setX) {
					return CreateSecondWay(setX, setY, 3, roomId);
				}
				else {
					return CreateSecondWay(setX, setY, 1, roomId);
				}
			}
		}
		else if (dir == 1 || dir == 3) {
			if (divideRoom[roomId - 2][0]<setX && divideRoom[roomId - 2][2]>setX) {
				//��������ɂ���
				if (divideRoom[roomId - 2][3] < setY) {
					return CreateSecondWay(setX, setY, 0, roomId);
				}
				else {
					return CreateSecondWay(setX, setY, 2, roomId);
				}
			}
		}
	}
	return CreateSecondWay(setX, setY, dir, roomId);
}
//0:��, 1 : �E, 2 : ��, 3 : ��
bool Map::CheckChip(int x, int y, int nextDir)
{
	if (x + 1 > gManager->MAPWIDTH - 1 || y + 1 > gManager->MAPHEIGHT - 1 || x <= 1 || y <= 1)return true;
	//if (x >= 49 || y >= 35 || x <= 1 || y <= 1)return true;

	bool isThere = false;
	if (nextDir == 0) {
		if (GetChip(x - 1, y) == 1)isThere = true;
		else if (GetChip(x + 1, y) == 1)isThere = true;
		else if (GetChip(x, y - 1) == 1)isThere = true;

		int num = 0;
		if (num = CheckIsThere(x - 1, y) != -1)
		{
			//if (num != 0)gManager->SetRoomWayPoint(t2k::Vector3(x, y, 0), num - 1);
			//else gManager->SetRoomWayPoint(t2k::Vector3(x, y, 0), num);
			isThere = true;
		}
		else if (num = CheckIsThere(x + 1, y) != -1)
		{
			//if (num != 0)gManager->SetRoomWayPoint(t2k::Vector3(x, y, 0), num - 1);
			//else gManager->SetRoomWayPoint(t2k::Vector3(x, y, 0), num);
			isThere = true;
		}
		else if (num = CheckIsThere(x, y - 1) != -1)
		{
			//if (num != 0)gManager->SetRoomWayPoint(t2k::Vector3(x, y, 0), num - 1);
			//else gManager->SetRoomWayPoint(t2k::Vector3(x, y, 0), num);
			isThere = true;
		}
	}
	else if (nextDir == 1) {
		if (GetChip(x, y - 1) == 1)isThere = true;
		else if (GetChip(x, y + 1) == 1)isThere = true;
		else if (GetChip(x + 1, y) == 1)isThere = true;

		int num = 0;
		if (num = CheckIsThere(x, y - 1) != -1)
		{
			//if (num != 0)gManager->SetRoomWayPoint(t2k::Vector3(x, y, 0), num - 1);
			//else gManager->SetRoomWayPoint(t2k::Vector3(x, y, 0), num);
			isThere = true;
		}
		else if (num = CheckIsThere(x, y + 1) != -1)
		{
			//if (num != 0)gManager->SetRoomWayPoint(t2k::Vector3(x, y, 0), num - 1);
			//else gManager->SetRoomWayPoint(t2k::Vector3(x, y, 0), num);
			isThere = true;
		}
		else if (num = CheckIsThere(x + 1, y) != -1)
		{
			//if (num != 0)gManager->SetRoomWayPoint(t2k::Vector3(x, y, 0), num - 1);
			//else gManager->SetRoomWayPoint(t2k::Vector3(x, y, 0), num);
			isThere = true;
		}
	}
	else if (nextDir == 2) {
		if (GetChip(x - 1, y) == 1)isThere = true;
		else if (GetChip(x + 1, y) == 1)isThere = true;
		else if (GetChip(x, y + 1) == 1)isThere = true;

		int num = 0;
		if (num = CheckIsThere(x - 1, y) != -1)
		{
			//if (num != 0)gManager->SetRoomWayPoint(t2k::Vector3(x, y, 0), num - 1);
			//else gManager->SetRoomWayPoint(t2k::Vector3(x, y, 0), num);
			isThere = true;
		}
		else if (num = CheckIsThere(x + 1, y) != -1)
		{
			//if (num != 0)gManager->SetRoomWayPoint(t2k::Vector3(x, y, 0), num - 1);
			//else gManager->SetRoomWayPoint(t2k::Vector3(x, y, 0), num);
			isThere = true;
		}
		else if (num = CheckIsThere(x, y + 1) != -1)
		{
			//if (num != 0)gManager->SetRoomWayPoint(t2k::Vector3(x, y, 0), num - 1);
			//else gManager->SetRoomWayPoint(t2k::Vector3(x, y, 0), num);
			isThere = true;
		}
	}
	else if (nextDir == 3) {
		if (GetChip(x, y - 1) == 1)isThere = true;
		else if (GetChip(x, y + 1) == 1)isThere = true;
		else if (GetChip(x - 1, y) == 1)isThere = true;

		int num = 0;
		if (num = CheckIsThere(x, y - 1) != -1)
		{
			//if (num != 0)gManager->SetRoomWayPoint(t2k::Vector3(x, y, 0), num - 1);
			//else gManager->SetRoomWayPoint(t2k::Vector3(x, y, 0), num);
			isThere = true;
		}
		else if (num = CheckIsThere(x, y + 1) != -1)
		{
			//if (num != 0)gManager->SetRoomWayPoint(t2k::Vector3(x, y, 0), num - 1);
			//else gManager->SetRoomWayPoint(t2k::Vector3(x, y, 0), num);
			isThere = true;
		}
		else if (num = CheckIsThere(x - 1, y) != -1)
		{
			//if (num != 0)gManager->SetRoomWayPoint(t2k::Vector3(x, y, 0), num - 1);
			//else gManager->SetRoomWayPoint(t2k::Vector3(x, y, 0), num);
			isThere = true;
		}
	}
	if (isThere)return true;
	return false;
}
bool Map::CheckPassWay(int roomPos_set, int roomPos_moveStart, int roomPos_moveGoal, int dir)
{
	bool check = true;
	for (int i = roomPos_moveStart; i < roomPos_moveGoal; ++i) {
		//�ʘH�������for���𔲂���
		if (dir == 0) {
			//������E�֒���
			if (roomPos_set - 1 < 0)break;
			if (GetChip(i, roomPos_set - 1) == PASSWAY) {
				check = false;
				break;
			}
		}
		else if (dir == 1) {
			//�ォ�牺�֒���
			if (roomPos_set + 1 > width)break;
			if (GetChip(roomPos_set + 1, i) == PASSWAY) {
				check = false;
				break;
			}
		}
		else if (dir == 2) {
			//������E�֒���
			if (roomPos_set + 1 > height)break;
			if (GetChip(i, roomPos_set + 1) == PASSWAY) {
				check = false;
				break;
			}
		}
		else if (dir == 3) {
			//�ォ�牺�֒���
			if (roomPos_set - 1 < 0)break;
			if (GetChip(roomPos_set - 1, i) == PASSWAY) {
				check = false;
				break;
			}
		}
	}
	return check;
}

std::string Map::GetColorName(int code)
{
	if (code == color_red)return "��";
	else if (code == color_green)return "��";
	else if (code == color_blue)return "��";
	else if (code == color_yellow)return "��";
	else if (code == color_purple)return "��";

	else return "�G���[";
}

bool Map::CheckAround(int x, int y)
{
	int ue = 0;
	int migi = 0;
	int shita = 0;
	int hidari = 0;

	int hidariue = 0;
	int hidarishita = 0;
	int migiue = 0;
	int migishita = 0;

	if (x > 0) {
		if (y > 0)hidariue = ground[x - 1][y - 1];
		hidari = ground[x - 1][y];
		if (y + 1 < gManager->MAPWIDTH)hidarishita = ground[x - 1][y + 1];
	}

	if (y > 0)ue = ground[x][y - 1];
	if (y + 1 < gManager->MAPWIDTH)shita = ground[x][y + 1];

	if (x + 1 < gManager->MAPHEIGHT) {
		if (y > 0)migiue = ground[x + 1][y - 1];
		migi = ground[x + 1][y];
		if (y + 1 < gManager->MAPWIDTH)migishita = ground[x + 1][y + 1];
	}
	if (ue + migi + shita + hidari + hidariue + hidarishita + migiue + migishita >= 1)return true;
	else return false;
}

int Map::CheckAroundWay(int x, int y)
{
	bool ue = false;
	bool migi = false;
	bool shita = false;
	bool hidari = false;

	bool hidariue = false;
	bool hidarishita = false;
	bool migiue = false;
	bool migishita = false;

	//8�ߖT�̃Z�����ʘH�Ȃ�true,�����łȂ����false����
	if (x > 0) {
		if (y > 0)hidariue = CheckThisCell(x - 1, y - 1);
		if (y > 0)hidari = CheckThisCell(x, y - 1);
		if (y + 1 < gManager->MAPWIDTH)hidarishita = CheckThisCell(x - 1, y + 1);
	}

	if (x > 0)ue = CheckThisCell(x - 1, y);
	if (x + 1 < gManager->MAPHEIGHT)shita = CheckThisCell(x + 1, y);

	if (x + 1 < gManager->MAPHEIGHT) {
		if (y > 0)migiue = CheckThisCell(x + 1, y - 1);
		if (y + 1 < gManager->MAPWIDTH) migi = CheckThisCell(x, y + 1);
		if (y + 1 < gManager->MAPWIDTH)migishita = CheckThisCell(x + 1, y + 1);
	}
	if (hidari && hidarishita && hidariue && migi && migiue && migishita)return ROOMWALL;//8 �����̃u���b�N

	else if (ue && migishita)return ROOMWALL;
	else if (ue && hidari)return ROOMWALL;

	//��,�E��,������true->�ʘH�Ȃ� �����̏㑤
	else if (shita)return ROOMTOP;//0

	else if (hidari)return ROOMRIGHT;//1
	else if (ue)return ROOMBOTTOM;//2
	else if (migi)return ROOMLEFT;//3

	else if (migishita && !ue && !shita && !migi && !hidari && !migiue && !hidariue && !hidarishita)return ROOMLEFTTOP;//4
	else if (hidarishita && !ue && !shita && !migi && !hidari && !migiue && !hidariue && !migishita)return ROOMRIGHTTOP;//5
	else if (migiue && !ue && !shita && !migi && !hidari && !hidarishita && !hidariue && !migishita)return ROOMLEFTBOTTOM;//6
	else if (hidariue && !ue && !shita && !migi && !hidari && !migiue && !migishita && !hidarishita)return ROOMRIGHTBOTTOM;//7



	else return -1;
}

bool Map::CheckThisCell(int x, int y)
{
	if (ground[x][y] == PASSWAY || ground[x][y] == STAIRS)return true;
	else return false;
}

void Map::DrawAllRoomPos(vector<vector<int>>RoomList)
{
	int drawPosX = 10;
	int drawPosY = 10;

	//�� �� �E �� id
	for (auto room : RoomList) {
		for (auto area : divideArea) {
			DrawStringEx(drawPosX + 200 * room[4], drawPosY, -1, "roomId:%d", room[4]);
			DrawStringEx(drawPosX + 200 * room[4], drawPosY + 20, -1, "areaColor:%s", GetColorName(colors[room[4]]).c_str());
			DrawStringEx(drawPosX + 200 * room[4], drawPosY + 40, -1, "Left:%d", room[0]);
			DrawStringEx(drawPosX + 200 * room[4], drawPosY + 60, -1, "Up:%d", room[1]);
			DrawStringEx(drawPosX + 200 * room[4], drawPosY + 80, -1, "Right:%d", room[2]);
			DrawStringEx(drawPosX + 200 * room[4], drawPosY + 100, -1, "Down:%d", room[3]);

			DrawStringEx(drawPosX + 200 * area[4], drawPosY + 140, -1, "areaId:%d", area[4]);
			DrawStringEx(drawPosX + 200 * area[4], drawPosY + 160, -1, "Left:%d", area[0]);
			DrawStringEx(drawPosX + 200 * area[4], drawPosY + 180, -1, "Up:%d", area[1]);
			DrawStringEx(drawPosX + 200 * area[4], drawPosY + 200, -1, "Right:%d", area[2]);
			DrawStringEx(drawPosX + 200 * area[4], drawPosY + 220, -1, "Down:%d", area[3]);


		}
	}
}
