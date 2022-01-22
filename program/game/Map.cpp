#include "Map.h"
#include"GameManager.h"
#include"DxLib.h"
#include"Camera.h"

extern GameManager* gManager;

Map::Map(int Width, int Height)
{
	mapChip[0] = gManager->LoadGraphEx("graphics/PassWay_20.png");
	mapChip[1] = gManager->LoadGraphEx("graphics/Wall_20.png");
	mapChip[2] = gManager->LoadGraphEx("graphics/Stairs_.png");
	mapChip[3] = gManager->LoadGraphEx("graphics/EXPASSWAY.png");

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

	CreatePassWay();

	for (auto room : divideRoom) {
		SetAllChip(room[0], room[1], room[2], room[3]);
	}
}

t2k::Vector3 Map::WorldToMap(int WorldX, int WorldY)
{
	int mapX = WorldX / 20;
	int mapY = WorldY / 20;

	return t2k::Vector3(mapX, mapY, 0);
}

t2k::Vector3 Map::MapToWorld(int MapX, int MapY)
{
	int worldX = MapX * 20;
	int worldY = MapY * 20;

	return t2k::Vector3(worldX, worldY, 0);
}

int Map::GetRoomNum()
{
	return divideRoom.size() - 1;
}

vector<int> Map::GetRoom(int roomNum)
{
	return divideRoom[roomNum];
}

int Map::CheckIsThere(int x, int y)
{
	//�S�Ă̕����̍��W���`�F�b�N
	//x,y�����͈͓̔��ɂ���Ȃ炻�̕����ɑ��݂���
	for (auto room : divideRoom) {
		if (x > room[2])continue;
		if (y > room[3])continue;
		if (x >= room[0] && y >= room[1]) {
			return room[4];
			break;//����H
		}
	}

	return -1;
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
	int x = 0;
	int y = 0;
	for (auto i : ground) {
		for (auto k : i) {
			/*if (k == WALL) {
				DrawRotaGraph(x - gManager->camera->cameraPos.x, y - gManager->camera->cameraPos.y, gManager->graphEx, 0, mapChip[1], false);
			}
			else*/ if (k == PASSWAY) {
				DrawRotaGraph(x - gManager->camera->cameraPos.x, y - gManager->camera->cameraPos.y, gManager->graphEx, 0, mapChip[0], false);
			}
			else if (k == STAIRS) {
				DrawRotaGraph(x - gManager->camera->cameraPos.x, y - gManager->camera->cameraPos.y, gManager->graphEx, 0, mapChip[2], false);
			}
			else if (k == EXPASSWAY) {
				DrawRotaGraph(x - gManager->camera->cameraPos.x, y - gManager->camera->cameraPos.y, gManager->graphEx, 0, mapChip[3], false);
			}
			/*if (k == WALL) {
				DrawRotaGraph(x, y, gManager->graphEx, 0, mapChip[1], false);
			}
			else {
				DrawRotaGraph(x, y, gManager->graphEx, 0, mapChip[0], false);
			}*/
			x += 20;
		}
		x = 0;
		y += 20;
	}
	//debug
	if (gManager->isDebug) {
		DrawAllRoomPos(divideRoom);
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

		////�����̍Œᕝ��苷�������番�����Ȃ�
		//if (upperWidth - lowerWidth < 2 * roomMinWidth + 4) {
		//	break;
		//}
		//if (upperHeight - lowerHeight < 2 * roomMinHeight + 4) {
		//	break;
		//}
		////�ő啔�����ɒB�����番������߂�
		//if (roomId > roomMaxNum) {
		//	break;
		//}

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

		if (dir == VERTICAL) {
			//��������ʘH�𐶂₷�ʒu������
			int passWayBefore = gManager->GetRandEx(upBefore + 1, downBefore - 1);
			int passWayAfter = gManager->GetRandEx(upAfter + 1, downAfter - 1);

			//�������ƕ����̑��Έʒu�ŏꍇ����
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
			//��������ʘH�𐶂₷�ʒu������
			int passWayBefore = gManager->GetRandEx(leftBefore + 1, rightBefore - 1);
			int passWayAfter = gManager->GetRandEx(leftAfter + 1, rightAfter - 1);

			//�������ƕ����̑��Έʒu�ŏꍇ����
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
#if 0
	int count_Jump = 0;

	while (count_Jump + 2 < divideRoom.size()) {
		//�����̏��̎擾
		vector<int> roomBefore = divideRoom[count_Jump];
		vector<int> roomAfter = divideRoom[count_Jump + 2];

		int leftBefore = roomBefore[0];
		int upBefore = roomBefore[1];
		int rightBefore = roomBefore[2];
		int downBefore = roomBefore[3];

		int leftAfter = roomAfter[0];
		int upAfter = roomAfter[1];
		int rightAfter = roomAfter[2];
		int downAfter = roomAfter[3];

		int passWayBefore = 0;
		int passWayAfter = 0;

		//�ǂ����̓_����ɕ����̑��Έʒu���擾������
		//�����Ⴂ���̕�������Ȃ�(����������)
		if (upBefore < upAfter) {
			//�Ⴂ���̕������E�������ŏꍇ����

			//�Ⴂ�ق������Ȃ�
			if (rightBefore < leftAfter) {
				//�����̑��Έʒu�͍���ɎႢ����,�E���ɔN��̕���
				//�ʘH������Ă��Ȃ��ӂɒʘH����肽��
				//��ӂ�S�Ē��߂�WALL�ȊO����������ʘH�����݂���

				int before = 0;
				int after = 0;

				//*****before�̕����̓�������
				//�����ʘH���Ȃ����:true
				if (CheckPassWay(rightBefore, upBefore, downBefore, 1)) {
					//�E�ӂ̂ǂ����ɓ���������
					passWayBefore = gManager->GetRandEx(upBefore + 1, downBefore - 1);
					before = 1;
				}
				else if (CheckPassWay(downBefore, leftBefore, rightBefore, 2)) {
					//���ӂ̂ǂ����ɓ���������
					passWayBefore = gManager->GetRandEx(leftBefore + 1, rightBefore - 1);
					before = 2;
				}
				else {
					before = -1;
				}
				//*****

				//*****After�̕����̓��������
				if (CheckPassWay(upAfter, leftAfter, rightAfter, 0)) {
					//��ӂ̂ǂ����ɓ���������
					passWayAfter = gManager->GetRandEx(leftAfter + 1, rightAfter - 1);
					after = 1;
				}
				else if (CheckPassWay(leftAfter, upAfter, downAfter, 3)) {
					//���ӂ̂ǂ����ɓ���������
					passWayBefore = gManager->GetRandEx(upAfter + 1, downAfter - 1);
					after = 2;
				}
				else {
					after = -1;
				}
				//*****
				//��������ǂ��炩����ł����Ȃ������玟�̃��[�v�֔�����
				if (before == -1 || after == -1)
				{
					count_Jump += 1;
					continue;
				}

				//������������o���Ă�����
				if (before == 1 && after == 1) {
					//�ʘH���Ȃ�
					//before��������_�܂�
					SetAllChip(rightBefore, passWayBefore, passWayAfter, passWayBefore);
					//�����_����after�܂�
					SetAllChip(passWayAfter, passWayBefore, passWayAfter, upAfter);
					count_Jump += 1;
					continue;
				}
				else if (before == 2 && after == 2) {
					//�ʘH���Ȃ�
					//before��������_�܂�
					SetAllChip(passWayBefore, downBefore, passWayBefore, passWayAfter);
					//�����_����after
					SetAllChip(passWayBefore, passWayAfter, leftAfter, passWayAfter);
					count_Jump += 1;
					continue;
				}
				else {
					//�ʘH���������Ȃ��ꍇ�͂���Ȃ�
					count_Jump += 1;
					continue;
				}
			}
			//�Ⴂ�����E�Ȃ�
			else {
				//�����̑��Έʒu�͉E��ɎႢ����,�����ɔN��̕���
				int before = 0;
				int after = 0;

				//*****before�̕����̓�������
				//�����ʘH���Ȃ����:true
				if (CheckPassWay(leftBefore, upBefore, downBefore, 3)) {
					//���ӂ̂ǂ����ɓ���������
					passWayBefore = gManager->GetRandEx(upBefore + 1, downBefore - 1);
					before = 1;
				}
				else if (CheckPassWay(downBefore, leftBefore, rightBefore, 2)) {
					//���ӂ̂ǂ����ɓ���������
					passWayBefore = gManager->GetRandEx(leftBefore + 1, rightBefore - 1);
					before = 2;
				}
				else {
					before = -1;
				}
				//*****

				//*****After�̕����̓��������
				if (CheckPassWay(upAfter, leftAfter, rightAfter, 0)) {
					//��ӂ̂ǂ����ɓ���������
					passWayAfter = gManager->GetRandEx(leftAfter + 1, rightAfter - 1);
					after = 1;
				}
				else if (CheckPassWay(rightAfter, upAfter, downAfter, 1)) {
					//�E�ӂ̂ǂ����ɓ���������
					passWayBefore = gManager->GetRandEx(upAfter + 1, downAfter - 1);
					after = 2;
				}
				else {
					after = -1;
				}
				//*****
				//��������ǂ��炩����ł����Ȃ������玟�̃��[�v�֔�����
				if (before == -1 || after == -1)
				{
					count_Jump += 1;
					continue;
				}

				//������������o���Ă�����
				if (before == 1 && after == 1) {
					//�ʘH���Ȃ�
					//�����_����before�܂�
					SetAllChip(passWayAfter, passWayBefore, leftBefore, passWayBefore);
					//�����_����after�܂�
					SetAllChip(passWayAfter, passWayBefore, passWayAfter, upAfter);
					count_Jump += 1;
					continue;
				}
				else if (before == 2 && after == 2) {
					//�ʘH���Ȃ�
					//before��������_�܂�
					SetAllChip(passWayBefore, downBefore, passWayBefore, passWayAfter);
					//after��������_�܂�
					SetAllChip(rightAfter, passWayAfter, passWayBefore, passWayAfter);
					count_Jump += 1;
					continue;
				}
				else {
					//�ʘH���������Ȃ��ꍇ�͂���Ȃ�
					count_Jump += 1;
					continue;
				}
			}//��ō��E�̏ꍇ�I���
		}
		//�Ⴂ���������Ȃ�
		else {
			//�Ⴂ���̕������E�������ŏꍇ����
			//�Ⴂ�ق������Ȃ�
			if (rightBefore < leftAfter) {
				//�����̑��Έʒu�͍����ɎႢ����,�E��ɔN��̕���
				int before = 0;
				int after = 0;

				//*****before�̕����̓�������
				//�����ʘH���Ȃ����:true
				if (CheckPassWay(upBefore, leftBefore, rightBefore, 0)) {
					//��ӂ̂ǂ����ɓ���������
					passWayBefore = gManager->GetRandEx(leftBefore + 1, rightBefore - 1);
					before = 1;
				}
				else if (CheckPassWay(rightBefore, upBefore, downBefore, 1)) {
					//�E�ӂ̂ǂ����ɓ���������
					passWayBefore = gManager->GetRandEx(upBefore + 1, downBefore - 1);
					before = 2;
				}
				else {
					before = -1;
				}
				//*****

				//*****After�̕����̓��������
				if (CheckPassWay(leftAfter, upAfter, downAfter, 3)) {
					//���ӂ̂ǂ����ɓ���������
					passWayAfter = gManager->GetRandEx(upAfter + 1, downAfter - 1);
					after = 1;
				}
				else if (CheckPassWay(downAfter, leftAfter, rightAfter, 2)) {
					//���ӂ̂ǂ����ɓ���������
					passWayBefore = gManager->GetRandEx(leftAfter + 1, rightAfter - 1);
					after = 2;
				}
				else {
					after = -1;
				}
				//*****
				//��������ǂ��炩����ł����Ȃ������玟�̃��[�v�֔�����
				if (before == -1 || after == -1)
				{
					count_Jump += 1;
					continue;
				}

				//������������o���Ă�����
				if (before == 1 && after == 1) {
					//�ʘH���Ȃ�
					//�����_����before�܂�
					SetAllChip(passWayBefore, passWayAfter, passWayBefore, upBefore);
					//�����_����after�܂�
					SetAllChip(passWayBefore, passWayAfter, leftAfter, passWayAfter);
					count_Jump += 1;
					continue;
				}
				else if (before == 2 && after == 2) {
					//�ʘH���Ȃ�
					//before��������_�܂�
					SetAllChip(rightBefore, passWayBefore, passWayAfter, passWayBefore);
					//after��������_�܂�
					SetAllChip(passWayAfter, downAfter, passWayAfter, passWayBefore);
					count_Jump += 1;
					continue;
				}
				else {
					//�ʘH���������Ȃ��ꍇ�͂���Ȃ�
					count_Jump += 1;
					continue;
				}
			}
			//�Ⴂ�����E�Ȃ�
			else {
				//�����̑��Έʒu�͉E���ɎႢ����,����ɔN��̕���
				int before = 0;
				int after = 0;

				//*****before�̕����̓�������
				//�����ʘH���Ȃ����:true
				if (CheckPassWay(upBefore, leftBefore, rightBefore, 0)) {
					//��ӂ̂ǂ����ɓ���������
					passWayBefore = gManager->GetRandEx(leftBefore + 1, rightBefore - 1);
					before = 1;
				}
				else if (CheckPassWay(leftBefore, upBefore, downBefore, 3)) {
					//���ӂ̂ǂ����ɓ���������
					passWayBefore = gManager->GetRandEx(upBefore + 1, downBefore - 1);
					before = 2;
				}
				else {
					before = -1;
				}
				//*****

				//*****After�̕����̓��������
				if (CheckPassWay(rightAfter, upAfter, downAfter, 1)) {
					//�E�ӂ̂ǂ����ɓ���������
					passWayAfter = gManager->GetRandEx(upAfter + 1, downAfter - 1);
					after = 1;
				}
				else if (CheckPassWay(downAfter, leftAfter, rightAfter, 2)) {
					//���ӂ̂ǂ����ɓ���������
					passWayBefore = gManager->GetRandEx(leftAfter + 1, rightAfter - 1);
					after = 2;
				}
				else {
					after = -1;
				}
				//*****
				//��������ǂ��炩����ł����Ȃ������玟�̃��[�v�֔�����
				if (before == -1 || after == -1)
				{
					count_Jump += 1;
					continue;
				}

				//������������o���Ă�����
				if (before == 1 && after == 1) {
					//�ʘH���Ȃ�
					//�����_����before�܂�
					SetAllChip(passWayBefore, passWayAfter, passWayBefore, upBefore);
					//after��������_�܂�
					SetAllChip(rightAfter, passWayAfter, passWayBefore, passWayAfter);
					count_Jump += 1;
					continue;
				}
				else if (before == 2 && after == 2) {
					//�ʘH���Ȃ�
					//�����_����before�܂�
					SetAllChip(passWayAfter, passWayBefore, leftBefore, passWayBefore);
					//after��������_�܂�
					SetAllChip(passWayAfter, downAfter, passWayAfter, passWayBefore);
					count_Jump += 1;
					continue;
				}
				else {
					//�ʘH���������Ȃ��ꍇ�͂���Ȃ�
					count_Jump += 1;
					continue;
				}
			}
		}

	}
#endif
#if 1
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


#endif
	//���镔�����猩�ď㉺���E�ɗׂ荇�����������邩�m�F����
	//�ׂ荇������������ꍇ
	//�������̕����ƌq�����Ă����return����
	//�����q�����ĂȂ���΂��̕����Ǝ����̕����ɂȂ���|�C���g�����
	//�|�C���g�����ԋ߂�divideline������o��
	//divideline�Ɍ������ĐL�΂�
	//�ʘH�Ŗ��߂�

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
				return t2k::Vector3(pointX, point, 0);
			}
		}//���Ȃ�ォ��
		else {
			//��������y���W�ƕ�����y���W���ׂ�
			//��������蕔���̏オ�傫����Ε������͏�ɂ���
			if (divideLine[roomId - 2][1] < divideRoom[roomId][1]) {
				int point = 0;
				while (1) {
					//�����̏�[�̂ǂ�����y���W
					point = gManager->GetRandEx(divideRoom[roomId][0], divideRoom[roomId][2]);
					if ((GetChip(point, divideRoom[roomId][1] - 1)) == 0 && (GetChip(point - 1, divideRoom[roomId][1] - 1)) == 0 && (GetChip(point + 1, divideRoom[roomId][1] - 1)) == 0) break;
				}
				dir = 0;
				int pointY = divideRoom[roomId][1];
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
				if ((GetChip(divideRoom[roomId][0] - 1, point)) == 0) break;
			}
			dir = 3;
			int pointX = divideRoom[roomId][0];
			return t2k::Vector3(pointX, point, 0);
		}
		else {
			int point = 0;
			while (1) {
				//�����̉E�[�̂ǂ�����y���W
				point = gManager->GetRandEx(divideRoom[roomId][1], divideRoom[roomId][3]);
				if ((GetChip(divideRoom[roomId][0] + 1, point)) == 0) break;
			}
			dir = 1;
			int pointX = divideRoom[roomId][2];
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
				if ((GetChip(point, divideRoom[roomId][1] - 1)) == 0) break;
			}
			dir = 0;
			int pointY = divideRoom[roomId][1];
			return t2k::Vector3(point, pointY, 0);
		}
		else {
			int point = 0;
			while (1) {
				//�����̉��[�̂ǂ�����y���W
				point = gManager->GetRandEx(divideRoom[roomId][0], divideRoom[roomId][2]);
				if ((GetChip(point, divideRoom[roomId][3] + 1)) == 0) break;
			}
			dir = 2;
			int pointY = divideRoom[roomId][3];
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
		SetChip(setX, setY, EXPASSWAY);
		//chip = GetChip(x, y - 1);
	}
	else if (dir == 1) {
		setX = x + 1;
		setY = y;
		SetChip(setX, setY, EXPASSWAY);
		//chip = GetChip(x + 1, y);
	}
	else if (dir == 2) {
		setX = x;
		setY = y + 1;
		SetChip(setX, setY, EXPASSWAY);
		//chip = GetChip(x, (y + 1));
	}
	else if (dir == 3) {
		setX = x - 1;
		setY = y;
		SetChip(setX, setY, EXPASSWAY);
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
	if (nextDir == 0) {
		if (GetChip(x - 1, y) == 1)return true;
		else if (GetChip(x + 1, y) == 1)return true;
		else if (GetChip(x, y - 1) == 1)return true;
	}
	else if (nextDir == 1) {
		if (GetChip(x, y - 1) == 1)return true;
		else if (GetChip(x, y + 1) == 1)return true;
		else if (GetChip(x + 1, y) == 1)return true;
	}
	else if (nextDir == 2) {
		if (GetChip(x - 1, y) == 1)return true;
		else if (GetChip(x + 1, y) == 1)return true;
		else if (GetChip(x, y + 1) == 1)return true;
	}
	else if (nextDir == 3) {
		if (GetChip(x, y - 1) == 1)return true;
		else if (GetChip(x, y + 1) == 1)return true;
		else if (GetChip(x - 1, y) == 1)return true;
	}
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
