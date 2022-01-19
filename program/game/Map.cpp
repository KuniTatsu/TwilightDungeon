#include "Map.h"
#include"GameManager.h"
#include"DxLib.h"
#include"Camera.h"

extern GameManager* gManager;

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
			return room[5];
			break;//����H
		}
	}

	return -1;
}



Map::Map(int Width, int Height)
{
	mapChip[0] = gManager->LoadGraphEx("graphics/PassWay_20.png");
	mapChip[1] = gManager->LoadGraphEx("graphics/Wall_20.png");
	mapChip[2] = gManager->LoadGraphEx("graphics/Stairs.png");

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
	int x = 0;
	int y = 0;
	for (auto i : ground) {
		for (auto k : i) {
			if (k == WALL) {
				DrawRotaGraph(x - gManager->camera->cameraPos.x, y - gManager->camera->cameraPos.y, gManager->graphEx, 0, mapChip[1], false);
			}
			else if (k == PASSWAY) {
				DrawRotaGraph(x - gManager->camera->cameraPos.x, y - gManager->camera->cameraPos.y, gManager->graphEx, 0, mapChip[0], false);
			}
			else if (k == STAIRS) {
				DrawRotaGraph(x - gManager->camera->cameraPos.x, y - gManager->camera->cameraPos.y, gManager->graphEx, 0, mapChip[2], false);
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
}

void Map::AreaDivide()
{
	int lowerWidth = 0;
	int upperWidth = width - 1;

	int lowerHeight = 0;
	int upperHeight = height - 1;

	int roomId = 0;

	/*bool isVertical;*/
	//if (GetRand(1) == 1) {
	//	isVertical = true;
	//}
	//else {
	//	isVertical = false;
	//}

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
		else {

			int left = dumpDivideArea[roomId - 1][0];
			int up = dumpDivideArea[roomId - 1][1];
			int right = dumpDivideArea[roomId - 1][2];
			int down = dumpDivideArea[roomId - 1][3];

			//���������c���傫��������
			if ((right - left) > (down - up)) {
				//���̂ǂ����ŏc�ɐ؂�
				dividePoint = gManager->GetRandEx((left + 2 + roomMinWidth), (right - roomMinWidth - 2));

				//������荶�Ȃ� �����̕������������A�E�̕����͍L��
				if (dividePoint < ((right + left) / 2)) {
					//����������o�^����
					SetDivideArea(left + 1, up + 1, dividePoint - 2, down - 1, roomId);
					//�傫������o�^����
					SetLargeDivideArea(dividePoint + 1, up - 1, right - 1, down - 1, roomId);
				}
				//�������E�Ȃ� ���E�̕������������A���̕����͍L��
				else {
					//����������o�^����
					SetDivideArea(dividePoint + 2, up + 1, right - 1, down - 1, roomId);
					//�傫������o�^����
					SetLargeDivideArea(left + 1, up - 1, dividePoint - 1, down - 1, roomId);
				}
				//������������
				SetDivideLine(dividePoint, up + 1, dividePoint, down - 1, VERTICAL);
			}
			//�����c�������傫��������
			else {
				//�c�̂ǂ����ŉ��ɂ���
				dividePoint = gManager->GetRandEx((up + 2 + roomMinHeight), (down - roomMinHeight - 2));

				//��������Ȃ� ����̕������������A���̕����͍L��
				if (dividePoint < ((down + up) / 2)) {
					//����������o�^����
					SetDivideArea(left + 1, up + 1, right - 1, dividePoint - 2, roomId);
					//�傫������o�^����
					SetLargeDivideArea(left + 1, dividePoint + 1, right - 1, down - 1, roomId);

				}
				//������艺�Ȃ� ����̕������傫���A���̕����͏�����
				else {
					//����������o�^����
					SetDivideArea(left + 1, dividePoint + 2, right - 1, down - 1, roomId);
					//�傫������o�^����
					SetLargeDivideArea(left + 1, up + 1, right - 1, dividePoint - 1, roomId);
				}

				//������������
				SetDivideLine(left + 1, dividePoint, right - 1, dividePoint, HORIZONTAL);
			}

			++roomId;
			continue;
		}

	}
#if 0

	if (isVertical) {
		//�����œ���郉���_���̒l���A�O�񕪊������Ƃ��̑傫�����̒l��������ǂ��Ȃ�H
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
#endif
}

void Map::CreateRoom()
{
	for (auto area : divideArea) {

		int left = area[0];
		int up = area[1];
		int right = area[2];
		int down = area[3];
		int id = area[4];

		int roomLeft = gManager->GetRandEx(left, right - roomMinWidth + 1);
		int roomRight = gManager->GetRandEx(roomLeft + roomMinWidth - 1, right);
		int roomUp = gManager->GetRandEx(up, down - roomMinHeight + 1);
		int roomDown = gManager->GetRandEx(roomUp + roomMinHeight - 1, down);

		SetDivideRoom(roomLeft, roomUp, roomRight, roomDown,id);
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
					SetAllChip(leftAfter, passWayAfter, passWayBefore, passWayAfter);
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
					SetAllChip(passWayBefore, passWayAfter, leftBefore, passWayBefore);
					//after��������_�܂�
					SetAllChip(passWayAfter, downAfter, passWayBefore, passWayAfter);
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
	//���镔�����猩�ď㉺���E�ɗׂ荇�����������邩�m�F����
	//�ׂ荇������������ꍇ
	//�������̕����ƌq�����Ă����return����
	//�����q�����ĂȂ���΂��̕����Ǝ����̕����ɂȂ���|�C���g�����
	//�|�C���g�����ԋ߂�divideline������o��
	//divideline�Ɍ������ĐL�΂�
	//�ʘH�Ŗ��߂�

}

bool Map::CheckPassWay(int roomPos_set, int roomPos_moveStart, int roomPos_moveGoal, int dir)
{
	bool check = true;
	for (int i = roomPos_moveStart; i < roomPos_moveGoal; ++i) {
		//�ʘH�������for���𔲂���
		if (dir == 0) {
			//������E�֒���
			if (GetChip(roomPos_set, i-1) == PASSWAY) {
				check = false;
				break;
			}
		}
		else if (dir == 1) {
			//�ォ�牺�֒���
			if (GetChip(i+1, roomPos_set) == PASSWAY) {
				check = false;
				break;
			}
		}
		else if (dir == 2) {
			//������E�֒���
			if (GetChip(roomPos_set, i + 1) == PASSWAY) {
				check = false;
				break;
			}
		}
		else if (dir == 3) {
			//�ォ�牺�֒���
			if (GetChip(i - 1, roomPos_set) == PASSWAY) {
				check = false;
				break;
			}
		}
	}
	return check;
}
