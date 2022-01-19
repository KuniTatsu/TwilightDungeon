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
	//‘S‚Ä‚Ì•”‰®‚ÌÀ•W‚ğƒ`ƒFƒbƒN
	//x,y‚ª‚»‚Ì”ÍˆÍ“à‚É‚ ‚é‚È‚ç‚»‚Ì•”‰®‚É‘¶İ‚·‚é
	for (auto room : divideRoom) {
		if (x > room[2])continue;
		if (y > room[3])continue;
		if (x >= room[0] && y >= room[1]) {
			return room[5];
			break;//‚¢‚éH
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

	//c‚Ì‰Šú‰»
	ground.resize(height);
	for (auto v : ground)v.resize(width);
	for (int i = 0; i < height; ++i) {
		ground[i].resize(width);
	}
	//‚·‚×‚Ä‚ğ•Ç‚É‚·‚é
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
	//‹æ‰æ‚Éî•ñ‚ğŠi”[
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
	//‹æ‰æ‚Éî•ñ‚ğŠi”[
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
	//•ªŠ„ü‚Ìî•ñ‚ğŠi”[
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
	//•”‰®‚Ìî•ñ‚ğŠi”[
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
			//•”‰®‚ÌÅ’á•‚æ‚è‹·‚©‚Á‚½‚ç•ªŠ„‚µ‚È‚¢
			if (dumpDivideArea[roomId - 1][2] - dumpDivideArea[roomId - 1][0] < 2 * roomMinWidth + 4) {
				break;
			}
			if (dumpDivideArea[roomId - 1][3] - dumpDivideArea[roomId - 1][1] < 2 * roomMinHeight + 4) {
				break;
			}
		}
		//Å‘å•”‰®”‚É’B‚µ‚½‚ç•ªŠ„‚ğ‚â‚ß‚é
		if (roomId > roomMaxNum) {
			break;
		}

		////•”‰®‚ÌÅ’á•‚æ‚è‹·‚©‚Á‚½‚ç•ªŠ„‚µ‚È‚¢
		//if (upperWidth - lowerWidth < 2 * roomMinWidth + 4) {
		//	break;
		//}
		//if (upperHeight - lowerHeight < 2 * roomMinHeight + 4) {
		//	break;
		//}
		////Å‘å•”‰®”‚É’B‚µ‚½‚ç•ªŠ„‚ğ‚â‚ß‚é
		//if (roomId > roomMaxNum) {
		//	break;
		//}

		//•ªŠ„À•W
		int dividePoint = 0;

		//•ªŠ„ü‚©‚ç•”‰®‚Ü‚Å‚ÍÅ’á2—£‚·

		//Å‰‚Ì•ªŠ„‚¾‚Á‚½‚ç
		if (!doneFirstDivide) {
			//‚à‚µ‰¡‚ªc‚æ‚è‘å‚«‚©‚Á‚½‚ç
			if ((upperWidth - lowerWidth) > (upperHeight - lowerHeight)) {
				//‰¡‚Ì‚Ç‚±‚©‚Åc‚ÉØ‚é
				dividePoint = gManager->GetRandEx((lowerWidth + 2 + roomMinWidth), (upperWidth - roomMinWidth - 2));

				//”¼•ª‚æ‚è¶‚È‚ç ¨¶‚Ì•”‰®‚ª¬‚³‚­A‰E‚Ì•”‰®‚ÍL‚¢
				if (dividePoint < (upperWidth / 2)) {
					//¬‚³‚¢•û‚ğ“o˜^‚·‚é
					SetDivideArea(lowerWidth + 1, lowerHeight + 1, dividePoint - 2, upperHeight - 1, roomId);
					//‘å‚«‚¢•û‚ğ“o˜^‚·‚é
					SetLargeDivideArea(dividePoint + 1, lowerHeight - 1, upperWidth - 1, upperHeight - 1, roomId);
				}
				//”¼•ª‚æ‚è‰E‚È‚ç ¨‰E‚Ì•”‰®‚ª¬‚³‚­A¶‚Ì•”‰®‚ÍL‚¢
				else {
					//¬‚³‚¢•û‚ğ“o˜^‚·‚é
					SetDivideArea(dividePoint + 2, lowerHeight + 1, upperWidth - 1, upperHeight - 1, roomId);
					//‘å‚«‚¢•û‚ğ“o˜^‚·‚é
					SetLargeDivideArea(lowerWidth + 1, lowerHeight - 1, dividePoint - 1, upperHeight - 1, roomId);
				}
				//•ªŠ„ü‚ğˆø‚­
				SetDivideLine(dividePoint, lowerHeight + 1, dividePoint, upperHeight - 1, VERTICAL);
			}
			//‚à‚µc‚ª‰¡‚æ‚è‘å‚«‚©‚Á‚½‚ç
			else {
				//c‚Ì‚Ç‚±‚©‚Å‰¡‚É‚«‚é
				dividePoint = gManager->GetRandEx((lowerHeight + 2 + roomMinHeight), (upperHeight - roomMinHeight - 2));
				//”¼•ª‚æ‚èã‚È‚ç ¨ã‚Ì•”‰®‚ª¬‚³‚­A‰º‚Ì•”‰®‚ÍL‚¢
				if (dividePoint < (upperHeight / 2)) {
					//¬‚³‚¢•û‚ğ“o˜^‚·‚é
					SetDivideArea(lowerWidth + 1, lowerHeight + 1, upperWidth - 1, dividePoint - 2, roomId);
					//‘å‚«‚¢•û‚ğ“o˜^‚·‚é
					SetLargeDivideArea(lowerWidth + 1, dividePoint + 1, upperWidth - 1, upperHeight - 1, roomId);

				}
				//”¼•ª‚æ‚è‰º‚È‚ç ¨ã‚Ì•”‰®‚ª‘å‚«‚­A‰º‚Ì•”‰®‚Í¬‚³‚¢
				else {
					//¬‚³‚¢•û‚ğ“o˜^‚·‚é
					SetDivideArea(lowerWidth + 1, dividePoint + 2, upperWidth - 1, upperHeight - 1, roomId);
					//‘å‚«‚¢•û‚ğ“o˜^‚·‚é
					SetLargeDivideArea(lowerWidth + 1, lowerHeight + 1, upperWidth - 1, dividePoint - 1, roomId);
				}

				//•ªŠ„ü‚ğˆø‚­
				SetDivideLine(lowerWidth + 1, dividePoint, upperWidth - 1, dividePoint, HORIZONTAL);
			}
			//Å‰‚Ì•ªŠ„‚ğI—¹‚·‚é@ˆÈ~‚ÍSetLargeDivideArea‚É“o˜^‚³‚ê‚½•”‰®‚ğX‚É•ªŠ„‚µ‚Ä‚¢‚­
			doneFirstDivide = true;
			++roomId;
			continue;
		}
		else {

			int left = dumpDivideArea[roomId - 1][0];
			int up = dumpDivideArea[roomId - 1][1];
			int right = dumpDivideArea[roomId - 1][2];
			int down = dumpDivideArea[roomId - 1][3];

			//‚à‚µ‰¡‚ªc‚æ‚è‘å‚«‚©‚Á‚½‚ç
			if ((right - left) > (down - up)) {
				//‰¡‚Ì‚Ç‚±‚©‚Åc‚ÉØ‚é
				dividePoint = gManager->GetRandEx((left + 2 + roomMinWidth), (right - roomMinWidth - 2));

				//”¼•ª‚æ‚è¶‚È‚ç ¨¶‚Ì•”‰®‚ª¬‚³‚­A‰E‚Ì•”‰®‚ÍL‚¢
				if (dividePoint < ((right + left) / 2)) {
					//¬‚³‚¢•û‚ğ“o˜^‚·‚é
					SetDivideArea(left + 1, up + 1, dividePoint - 2, down - 1, roomId);
					//‘å‚«‚¢•û‚ğ“o˜^‚·‚é
					SetLargeDivideArea(dividePoint + 1, up - 1, right - 1, down - 1, roomId);
				}
				//”¼•ª‚æ‚è‰E‚È‚ç ¨‰E‚Ì•”‰®‚ª¬‚³‚­A¶‚Ì•”‰®‚ÍL‚¢
				else {
					//¬‚³‚¢•û‚ğ“o˜^‚·‚é
					SetDivideArea(dividePoint + 2, up + 1, right - 1, down - 1, roomId);
					//‘å‚«‚¢•û‚ğ“o˜^‚·‚é
					SetLargeDivideArea(left + 1, up - 1, dividePoint - 1, down - 1, roomId);
				}
				//•ªŠ„ü‚ğˆø‚­
				SetDivideLine(dividePoint, up + 1, dividePoint, down - 1, VERTICAL);
			}
			//‚à‚µc‚ª‰¡‚æ‚è‘å‚«‚©‚Á‚½‚ç
			else {
				//c‚Ì‚Ç‚±‚©‚Å‰¡‚É‚«‚é
				dividePoint = gManager->GetRandEx((up + 2 + roomMinHeight), (down - roomMinHeight - 2));

				//”¼•ª‚æ‚èã‚È‚ç ¨ã‚Ì•”‰®‚ª¬‚³‚­A‰º‚Ì•”‰®‚ÍL‚¢
				if (dividePoint < ((down + up) / 2)) {
					//¬‚³‚¢•û‚ğ“o˜^‚·‚é
					SetDivideArea(left + 1, up + 1, right - 1, dividePoint - 2, roomId);
					//‘å‚«‚¢•û‚ğ“o˜^‚·‚é
					SetLargeDivideArea(left + 1, dividePoint + 1, right - 1, down - 1, roomId);

				}
				//”¼•ª‚æ‚è‰º‚È‚ç ¨ã‚Ì•”‰®‚ª‘å‚«‚­A‰º‚Ì•”‰®‚Í¬‚³‚¢
				else {
					//¬‚³‚¢•û‚ğ“o˜^‚·‚é
					SetDivideArea(left + 1, dividePoint + 2, right - 1, down - 1, roomId);
					//‘å‚«‚¢•û‚ğ“o˜^‚·‚é
					SetLargeDivideArea(left + 1, up + 1, right - 1, dividePoint - 1, roomId);
				}

				//•ªŠ„ü‚ğˆø‚­
				SetDivideLine(left + 1, dividePoint, right - 1, dividePoint, HORIZONTAL);
			}

			++roomId;
			continue;
		}

	}
#if 0

	if (isVertical) {
		//‚±‚±‚Å“ü‚ê‚éƒ‰ƒ“ƒ_ƒ€‚Ì’l‚ªA‘O‰ñ•ªŠ„‚µ‚½‚Æ‚«‚Ì‘å‚«‚¢•û‚Ì’l‚¾‚Á‚½‚ç—Ç‚­‚È‚éH
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

//’Ê˜H‚Ì”‚Í•”‰®‚Ì”+1
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

		//•”‰®‚Ìî•ñ‚Ìæ“¾
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
			//•”‰®‚©‚ç’Ê˜H‚ğ¶‚â‚·ˆÊ’u‚ğŒˆ’è
			int passWayBefore = gManager->GetRandEx(upBefore + 1, downBefore - 1);
			int passWayAfter = gManager->GetRandEx(upAfter + 1, downAfter - 1);

			//•ªŠ„ü‚Æ•”‰®‚Ì‘Š‘ÎˆÊ’u‚Åê‡•ª‚¯
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
			//•”‰®‚©‚ç’Ê˜H‚ğ¶‚â‚·ˆÊ’u‚ğŒˆ’è
			int passWayBefore = gManager->GetRandEx(leftBefore + 1, rightBefore - 1);
			int passWayAfter = gManager->GetRandEx(leftAfter + 1, rightAfter - 1);

			//•ªŠ„ü‚Æ•”‰®‚Ì‘Š‘ÎˆÊ’u‚Åê‡•ª‚¯
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
		//•”‰®‚Ìî•ñ‚Ìæ“¾
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

		//‚Ç‚±‚©‚Ì“_‚ğŠî€‚É•”‰®‚Ì‘Š‘ÎˆÊ’u‚ğæ“¾‚µ‚½‚¢
		//‚à‚µá‚¢•û‚Ì•”‰®‚ªã‚È‚ç(”‚ª¬‚³‚¢)
		if (upBefore < upAfter) {
			//á‚¢•û‚Ì•”‰®‚ª‰E‚©¶‚©‚Åê‡•ª‚¯

			//á‚¢‚Ù‚¤‚ª¶‚È‚ç
			if (rightBefore < leftAfter) {
				//•”‰®‚Ì‘Š‘ÎˆÊ’u‚Í¶ã‚Éá‚¢•”‰®,‰E‰º‚É”Nã‚Ì•”‰®
				//’Ê˜H‚ªì‚ç‚ê‚Ä‚¢‚È‚¢•Ó‚É’Ê˜H‚ğì‚è‚½‚¢
				//ˆê•Ó‚ğ‘S‚Ä’­‚ß‚ÄWALLˆÈŠO‚ª‚ ‚Á‚½‚ç’Ê˜H‚ª‘¶İ‚·‚é

				int before = 0;
				int after = 0;

				//*****before‚Ì•”‰®‚Ì“üŒûŒŸõ
				//‚à‚µ’Ê˜H‚ª‚È‚¯‚ê‚Î:true
				if (CheckPassWay(rightBefore, upBefore, downBefore, 1)) {
					//‰E•Ó‚Ì‚Ç‚±‚©‚É“ü‚èŒû‚ğì‚é
					passWayBefore = gManager->GetRandEx(upBefore + 1, downBefore - 1);
					before = 1;
				}
				else if (CheckPassWay(downBefore, leftBefore, rightBefore, 2)) {
					//‰º•Ó‚Ì‚Ç‚±‚©‚É“ü‚èŒû‚ğì‚é
					passWayBefore = gManager->GetRandEx(leftBefore + 1, rightBefore - 1);
					before = 2;
				}
				else {
					before = -1;
				}
				//*****

				//*****After‚Ì•”‰®‚Ì“ü‚èŒûŒŸõ
				if (CheckPassWay(upAfter, leftAfter, rightAfter, 0)) {
					//ã•Ó‚Ì‚Ç‚±‚©‚É“ü‚èŒû‚ğì‚é
					passWayAfter = gManager->GetRandEx(leftAfter + 1, rightAfter - 1);
					after = 1;
				}
				else if (CheckPassWay(leftAfter, upAfter, downAfter, 3)) {
					//¶•Ó‚Ì‚Ç‚±‚©‚É“ü‚èŒû‚ğì‚é
					passWayBefore = gManager->GetRandEx(upAfter + 1, downAfter - 1);
					after = 2;
				}
				else {
					after = -1;
				}
				//*****
				//“ü‚èŒû‚ª‚Ç‚¿‚ç‚©ˆê•û‚Å‚àì‚ê‚È‚©‚Á‚½‚çŸ‚Ìƒ‹[ƒv‚Ö”²‚¯‚é
				if (before == -1 || after == -1)
				{
					count_Jump += 1;
					continue;
				}

				//“ü‚èŒû‚ª—¼•ûo—ˆ‚Ä‚¢‚½‚ç
				if (before == 1 && after == 1) {
					//’Ê˜H‚ğ‚Â‚È‚®
					//before‚©‚çŒğ·“_‚Ü‚Å
					SetAllChip(rightBefore, passWayBefore, passWayAfter, passWayBefore);
					//Œğ·“_‚©‚çafter‚Ü‚Å
					SetAllChip(passWayAfter, passWayBefore, passWayAfter, upAfter);
					count_Jump += 1;
					continue;
				}
				else if (before == 2 && after == 2) {
					//’Ê˜H‚ğ‚Â‚È‚®
					//before‚©‚çŒğ·“_‚Ü‚Å
					SetAllChip(passWayBefore, downBefore, passWayBefore, passWayAfter);
					//Œğ·“_‚©‚çafter
					SetAllChip(passWayBefore, passWayAfter, leftAfter, passWayAfter);
					count_Jump += 1;
					continue;
				}
				else {
					//’Ê˜H‚ªŒğ·‚µ‚È‚¢ê‡‚Í‚Â‚­‚ç‚È‚¢
					count_Jump += 1;
					continue;
				}
			}
			//á‚¢•û‚ª‰E‚È‚ç
			else {
				//•”‰®‚Ì‘Š‘ÎˆÊ’u‚Í‰Eã‚Éá‚¢•”‰®,¶‰º‚É”Nã‚Ì•”‰®
				int before = 0;
				int after = 0;

				//*****before‚Ì•”‰®‚Ì“üŒûŒŸõ
				//‚à‚µ’Ê˜H‚ª‚È‚¯‚ê‚Î:true
				if (CheckPassWay(leftBefore, upBefore, downBefore, 3)) {
					//¶•Ó‚Ì‚Ç‚±‚©‚É“ü‚èŒû‚ğì‚é
					passWayBefore = gManager->GetRandEx(upBefore + 1, downBefore - 1);
					before = 1;
				}
				else if (CheckPassWay(downBefore, leftBefore, rightBefore, 2)) {
					//‰º•Ó‚Ì‚Ç‚±‚©‚É“ü‚èŒû‚ğì‚é
					passWayBefore = gManager->GetRandEx(leftBefore + 1, rightBefore - 1);
					before = 2;
				}
				else {
					before = -1;
				}
				//*****

				//*****After‚Ì•”‰®‚Ì“ü‚èŒûŒŸõ
				if (CheckPassWay(upAfter, leftAfter, rightAfter, 0)) {
					//ã•Ó‚Ì‚Ç‚±‚©‚É“ü‚èŒû‚ğì‚é
					passWayAfter = gManager->GetRandEx(leftAfter + 1, rightAfter - 1);
					after = 1;
				}
				else if (CheckPassWay(rightAfter, upAfter, downAfter, 1)) {
					//‰E•Ó‚Ì‚Ç‚±‚©‚É“ü‚èŒû‚ğì‚é
					passWayBefore = gManager->GetRandEx(upAfter + 1, downAfter - 1);
					after = 2;
				}
				else {
					after = -1;
				}
				//*****
				//“ü‚èŒû‚ª‚Ç‚¿‚ç‚©ˆê•û‚Å‚àì‚ê‚È‚©‚Á‚½‚çŸ‚Ìƒ‹[ƒv‚Ö”²‚¯‚é
				if (before == -1 || after == -1)
				{
					count_Jump += 1;
					continue;
				}

				//“ü‚èŒû‚ª—¼•ûo—ˆ‚Ä‚¢‚½‚ç
				if (before == 1 && after == 1) {
					//’Ê˜H‚ğ‚Â‚È‚®
					//Œğ·“_‚©‚çbefore‚Ü‚Å
					SetAllChip(passWayAfter, passWayBefore, leftBefore, passWayBefore);
					//Œğ·“_‚©‚çafter‚Ü‚Å
					SetAllChip(passWayAfter, passWayBefore, passWayAfter, upAfter);
					count_Jump += 1;
					continue;
				}
				else if (before == 2 && after == 2) {
					//’Ê˜H‚ğ‚Â‚È‚®
					//before‚©‚çŒğ·“_‚Ü‚Å
					SetAllChip(passWayBefore, downBefore, passWayBefore, passWayAfter);
					//after‚©‚çŒğ·“_‚Ü‚Å
					SetAllChip(leftAfter, passWayAfter, passWayBefore, passWayAfter);
					count_Jump += 1;
					continue;
				}
				else {
					//’Ê˜H‚ªŒğ·‚µ‚È‚¢ê‡‚Í‚Â‚­‚ç‚È‚¢
					count_Jump += 1;
					continue;
				}
			}//ã‚Å¶‰E‚Ìê‡I‚í‚è
		}
		//á‚¢•”‰®‚ª‰º‚È‚ç
		else {
			//á‚¢•û‚Ì•”‰®‚ª‰E‚©¶‚©‚Åê‡•ª‚¯
			//á‚¢‚Ù‚¤‚ª¶‚È‚ç
			if (rightBefore < leftAfter) {
				//•”‰®‚Ì‘Š‘ÎˆÊ’u‚Í¶‰º‚Éá‚¢•”‰®,‰Eã‚É”Nã‚Ì•”‰®
				int before = 0;
				int after = 0;

				//*****before‚Ì•”‰®‚Ì“üŒûŒŸõ
				//‚à‚µ’Ê˜H‚ª‚È‚¯‚ê‚Î:true
				if (CheckPassWay(upBefore, leftBefore, rightBefore, 0)) {
					//ã•Ó‚Ì‚Ç‚±‚©‚É“ü‚èŒû‚ğì‚é
					passWayBefore = gManager->GetRandEx(leftBefore + 1, rightBefore - 1);
					before = 1;
				}
				else if (CheckPassWay(rightBefore, upBefore, downBefore, 1)) {
					//‰E•Ó‚Ì‚Ç‚±‚©‚É“ü‚èŒû‚ğì‚é
					passWayBefore = gManager->GetRandEx(upBefore + 1, downBefore - 1);
					before = 2;
				}
				else {
					before = -1;
				}
				//*****

				//*****After‚Ì•”‰®‚Ì“ü‚èŒûŒŸõ
				if (CheckPassWay(leftAfter, upAfter, downAfter, 3)) {
					//¶•Ó‚Ì‚Ç‚±‚©‚É“ü‚èŒû‚ğì‚é
					passWayAfter = gManager->GetRandEx(upAfter + 1, downAfter - 1);
					after = 1;
				}
				else if (CheckPassWay(downAfter, leftAfter, rightAfter, 2)) {
					//‰º•Ó‚Ì‚Ç‚±‚©‚É“ü‚èŒû‚ğì‚é
					passWayBefore = gManager->GetRandEx(leftAfter + 1, rightAfter - 1);
					after = 2;
				}
				else {
					after = -1;
				}
				//*****
				//“ü‚èŒû‚ª‚Ç‚¿‚ç‚©ˆê•û‚Å‚àì‚ê‚È‚©‚Á‚½‚çŸ‚Ìƒ‹[ƒv‚Ö”²‚¯‚é
				if (before == -1 || after == -1)
				{
					count_Jump += 1;
					continue;
				}

				//“ü‚èŒû‚ª—¼•ûo—ˆ‚Ä‚¢‚½‚ç
				if (before == 1 && after == 1) {
					//’Ê˜H‚ğ‚Â‚È‚®
					//Œğ·“_‚©‚çbefore‚Ü‚Å
					SetAllChip(passWayBefore, passWayAfter, passWayBefore, upBefore);
					//Œğ·“_‚©‚çafter‚Ü‚Å
					SetAllChip(passWayBefore, passWayAfter, leftAfter, passWayAfter);
					count_Jump += 1;
					continue;
				}
				else if (before == 2 && after == 2) {
					//’Ê˜H‚ğ‚Â‚È‚®
					//before‚©‚çŒğ·“_‚Ü‚Å
					SetAllChip(rightBefore, passWayBefore, passWayAfter, passWayBefore);
					//after‚©‚çŒğ·“_‚Ü‚Å
					SetAllChip(passWayAfter, downAfter, passWayAfter, passWayBefore);
					count_Jump += 1;
					continue;
				}
				else {
					//’Ê˜H‚ªŒğ·‚µ‚È‚¢ê‡‚Í‚Â‚­‚ç‚È‚¢
					count_Jump += 1;
					continue;
				}
			}
			//á‚¢•û‚ª‰E‚È‚ç
			else {
				//•”‰®‚Ì‘Š‘ÎˆÊ’u‚Í‰E‰º‚Éá‚¢•”‰®,¶ã‚É”Nã‚Ì•”‰®
				int before = 0;
				int after = 0;

				//*****before‚Ì•”‰®‚Ì“üŒûŒŸõ
				//‚à‚µ’Ê˜H‚ª‚È‚¯‚ê‚Î:true
				if (CheckPassWay(upBefore, leftBefore, rightBefore, 0)) {
					//ã•Ó‚Ì‚Ç‚±‚©‚É“ü‚èŒû‚ğì‚é
					passWayBefore = gManager->GetRandEx(leftBefore + 1, rightBefore - 1);
					before = 1;
				}
				else if (CheckPassWay(leftBefore, upBefore, downBefore, 3)) {
					//¶•Ó‚Ì‚Ç‚±‚©‚É“ü‚èŒû‚ğì‚é
					passWayBefore = gManager->GetRandEx(upBefore + 1, downBefore - 1);
					before = 2;
				}
				else {
					before = -1;
				}
				//*****

				//*****After‚Ì•”‰®‚Ì“ü‚èŒûŒŸõ
				if (CheckPassWay(rightAfter, upAfter, downAfter, 1)) {
					//‰E•Ó‚Ì‚Ç‚±‚©‚É“ü‚èŒû‚ğì‚é
					passWayAfter = gManager->GetRandEx(upAfter + 1, downAfter - 1);
					after = 1;
				}
				else if (CheckPassWay(downAfter, leftAfter, rightAfter, 2)) {
					//‰º•Ó‚Ì‚Ç‚±‚©‚É“ü‚èŒû‚ğì‚é
					passWayBefore = gManager->GetRandEx(leftAfter + 1, rightAfter - 1);
					after = 2;
				}
				else {
					after = -1;
				}
				//*****
				//“ü‚èŒû‚ª‚Ç‚¿‚ç‚©ˆê•û‚Å‚àì‚ê‚È‚©‚Á‚½‚çŸ‚Ìƒ‹[ƒv‚Ö”²‚¯‚é
				if (before == -1 || after == -1)
				{
					count_Jump += 1;
					continue;
				}

				//“ü‚èŒû‚ª—¼•ûo—ˆ‚Ä‚¢‚½‚ç
				if (before == 1 && after == 1) {
					//’Ê˜H‚ğ‚Â‚È‚®
					//Œğ·“_‚©‚çbefore‚Ü‚Å
					SetAllChip(passWayBefore, passWayAfter, passWayBefore, upBefore);
					//after‚©‚çŒğ·“_‚Ü‚Å
					SetAllChip(rightAfter, passWayAfter, passWayBefore, passWayAfter);
					count_Jump += 1;
					continue;
				}
				else if (before == 2 && after == 2) {
					//’Ê˜H‚ğ‚Â‚È‚®
					//Œğ·“_‚©‚çbefore‚Ü‚Å
					SetAllChip(passWayBefore, passWayAfter, leftBefore, passWayBefore);
					//after‚©‚çŒğ·“_‚Ü‚Å
					SetAllChip(passWayAfter, downAfter, passWayBefore, passWayAfter);
					count_Jump += 1;
					continue;
				}
				else {
					//’Ê˜H‚ªŒğ·‚µ‚È‚¢ê‡‚Í‚Â‚­‚ç‚È‚¢
					count_Jump += 1;
					continue;
				}
			}
		}

	}
	//‚ ‚é•”‰®‚©‚çŒ©‚Äã‰º¶‰E‚É—×‚è‡‚¤•”‰®‚ª‚ ‚é‚©Šm”F‚·‚é
	//—×‚è‡‚¤•”‰®‚ª‚ ‚éê‡
	//‚à‚µ‚»‚Ì•”‰®‚ÆŒq‚ª‚Á‚Ä‚¢‚ê‚Îreturn‚·‚é
	//‚à‚µŒq‚ª‚Á‚Ä‚È‚¯‚ê‚Î‚»‚Ì•”‰®‚Æ©•ª‚Ì•”‰®‚É‚Â‚È‚°‚éƒ|ƒCƒ“ƒg‚ğì‚é
	//ƒ|ƒCƒ“ƒg‚©‚çˆê”Ô‹ß‚¢divideline‚ğŠ„‚èo‚·
	//divideline‚ÉŒü‚©‚Á‚ÄL‚Î‚·
	//’Ê˜H‚Å–„‚ß‚é

}

bool Map::CheckPassWay(int roomPos_set, int roomPos_moveStart, int roomPos_moveGoal, int dir)
{
	bool check = true;
	for (int i = roomPos_moveStart; i < roomPos_moveGoal; ++i) {
		//’Ê˜H‚ª‚ ‚ê‚Îfor•¶‚ğ”²‚¯‚é
		if (dir == 0) {
			//¶‚©‚ç‰E‚Ö’²¸
			if (GetChip(roomPos_set, i-1) == PASSWAY) {
				check = false;
				break;
			}
		}
		else if (dir == 1) {
			//ã‚©‚ç‰º‚Ö’²¸
			if (GetChip(i+1, roomPos_set) == PASSWAY) {
				check = false;
				break;
			}
		}
		else if (dir == 2) {
			//¶‚©‚ç‰E‚Ö’²¸
			if (GetChip(roomPos_set, i + 1) == PASSWAY) {
				check = false;
				break;
			}
		}
		else if (dir == 3) {
			//ã‚©‚ç‰º‚Ö’²¸
			if (GetChip(i - 1, roomPos_set) == PASSWAY) {
				check = false;
				break;
			}
		}
	}
	return check;
}
