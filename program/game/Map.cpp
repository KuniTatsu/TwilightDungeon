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

void Map::SetStairs(int x, int y)
{




}

Map::Map(int Width, int Height)
{
	mapChip[0] = gManager->LoadGraphEx("graphics/PassWay_20.png");
	mapChip[1] = gManager->LoadGraphEx("graphics/Wall_20.png");
	mapChip[2] = gManager->LoadGraphEx("graphics/Stairs.png");

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
	//区画に情報を格納
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
	//部屋の情報を格納
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

	/*bool isVertical;*/
	//if (GetRand(1) == 1) {
	//	isVertical = true;
	//}
	//else {
	//	isVertical = false;
	//}

	while (1) {
		if (roomId > 0) {
			//部屋の最低幅より狭かったら分割しない
			if (dumpDivideArea[roomId - 1][2] - dumpDivideArea[roomId - 1][0] < 2 * roomMinWidth + 4) {
				break;
			}
			if (dumpDivideArea[roomId - 1][3] - dumpDivideArea[roomId - 1][1] < 2 * roomMinHeight + 4) {
				break;
			}
		}
		//最大部屋数に達したら分割をやめる
		if (roomId > roomMaxNum) {
			break;
		}

		////部屋の最低幅より狭かったら分割しない
		//if (upperWidth - lowerWidth < 2 * roomMinWidth + 4) {
		//	break;
		//}
		//if (upperHeight - lowerHeight < 2 * roomMinHeight + 4) {
		//	break;
		//}
		////最大部屋数に達したら分割をやめる
		//if (roomId > roomMaxNum) {
		//	break;
		//}

		//分割座標
		int dividePoint = 0;

		//分割線から部屋までは最低2離す

		//最初の分割だったら
		if (!doneFirstDivide) {
			//もし横が縦より大きかったら
			if ((upperWidth - lowerWidth) > (upperHeight - lowerHeight)) {
				//横のどこかで縦に切る
				dividePoint = gManager->GetRandEx((lowerWidth + 2 + roomMinWidth), (upperWidth - roomMinWidth - 2));

				//半分より左なら →左の部屋が小さく、右の部屋は広い
				if (dividePoint < (upperWidth / 2)) {
					//小さい方を登録する
					SetDivideArea(lowerWidth + 1, lowerHeight + 1, dividePoint - 2, upperHeight - 1, roomId);
					//大きい方を登録する
					SetLargeDivideArea(dividePoint + 1, lowerHeight - 1, upperWidth - 1, upperHeight - 1, roomId);
				}
				//半分より右なら →右の部屋が小さく、左の部屋は広い
				else {
					//小さい方を登録する
					SetDivideArea(dividePoint + 2, lowerHeight + 1, upperWidth - 1, upperHeight - 1, roomId);
					//大きい方を登録する
					SetLargeDivideArea(lowerWidth + 1, lowerHeight - 1, dividePoint - 1, upperHeight - 1, roomId);
				}
				//分割線を引く
				SetDivideLine(dividePoint, lowerHeight + 1, dividePoint, upperHeight - 1, VERTICAL);
			}
			//もし縦が横より大きかったら
			else {
				//縦のどこかで横にきる
				dividePoint = gManager->GetRandEx((lowerHeight + 2 + roomMinHeight), (upperHeight - roomMinHeight - 2));
				//半分より上なら →上の部屋が小さく、下の部屋は広い
				if (dividePoint < (upperHeight / 2)) {
					//小さい方を登録する
					SetDivideArea(lowerWidth + 1, lowerHeight + 1, upperWidth - 1, dividePoint - 2, roomId);
					//大きい方を登録する
					SetLargeDivideArea(lowerWidth + 1, dividePoint + 1, upperWidth - 1, upperHeight - 1, roomId);

				}
				//半分より下なら →上の部屋が大きく、下の部屋は小さい
				else {
					//小さい方を登録する
					SetDivideArea(lowerWidth + 1, dividePoint + 2, upperWidth - 1, upperHeight - 1, roomId);
					//大きい方を登録する
					SetLargeDivideArea(lowerWidth + 1, lowerHeight + 1, upperWidth - 1, dividePoint - 1, roomId);
				}

				//分割線を引く
				SetDivideLine(lowerWidth + 1, dividePoint, upperWidth - 1, dividePoint, HORIZONTAL);
			}
			//最初の分割を終了する　以降はSetLargeDivideAreaに登録された部屋を更に分割していく
			doneFirstDivide = true;
			++roomId;
			continue;
		}
		else {

			int left = dumpDivideArea[roomId - 1][0];
			int up = dumpDivideArea[roomId - 1][1];
			int right = dumpDivideArea[roomId - 1][2];
			int down = dumpDivideArea[roomId - 1][3];

			//もし横が縦より大きかったら
			if ((right - left) > (down - up)) {
				//横のどこかで縦に切る
				dividePoint = gManager->GetRandEx((left + 2 + roomMinWidth), (right - roomMinWidth - 2));

				//半分より左なら →左の部屋が小さく、右の部屋は広い
				if (dividePoint < ((right + left) / 2)) {
					//小さい方を登録する
					SetDivideArea(left + 1, up + 1, dividePoint - 2, down - 1, roomId);
					//大きい方を登録する
					SetLargeDivideArea(dividePoint + 1, up - 1, right - 1, down - 1, roomId);
				}
				//半分より右なら →右の部屋が小さく、左の部屋は広い
				else {
					//小さい方を登録する
					SetDivideArea(dividePoint + 2, up + 1, right - 1, down - 1, roomId);
					//大きい方を登録する
					SetLargeDivideArea(left + 1, up - 1, dividePoint - 1, down - 1, roomId);
				}
				//分割線を引く
				SetDivideLine(dividePoint, up + 1, dividePoint, down - 1, VERTICAL);
			}
			//もし縦が横より大きかったら
			else {
				//縦のどこかで横にきる
				dividePoint = gManager->GetRandEx((up + 2 + roomMinHeight), (down - roomMinHeight - 2));

				//半分より上なら →上の部屋が小さく、下の部屋は広い
				if (dividePoint < ((down + up) / 2)) {
					//小さい方を登録する
					SetDivideArea(left + 1, up + 1, right - 1, dividePoint - 2, roomId);
					//大きい方を登録する
					SetLargeDivideArea(left + 1, dividePoint + 1, right - 1, down - 1, roomId);

				}
				//半分より下なら →上の部屋が大きく、下の部屋は小さい
				else {
					//小さい方を登録する
					SetDivideArea(left + 1, dividePoint + 2, right - 1, down - 1, roomId);
					//大きい方を登録する
					SetLargeDivideArea(left + 1, up + 1, right - 1, dividePoint - 1, roomId);
				}

				//分割線を引く
				SetDivideLine(left + 1, dividePoint, right - 1, dividePoint, HORIZONTAL);
			}

			++roomId;
			continue;
		}

	}
#if 0

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
#endif
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

	int count_Jump = 0;
	while (count_Jump + 2 < divideRoom.size()) {
		//部屋の情報の取得
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

		//どこかの点を基準に部屋の相対位置を取得したい
		//もし若い方の部屋が上なら(数が小さい)
		if (upBefore < upAfter) {
			//若い方の部屋が右か左かで場合分け

			//若いほうが左なら
			if (rightBefore < leftAfter) {
				//部屋の相対位置は左上に若い部屋,右下に年上の部屋
				//通路が作られていない辺に通路を作りたい
				//一辺を全て眺めてWALL以外があったら通路が存在する

				int before = 0;
				int after = 0;

				//*****beforeの部屋の入口検索
				//もし通路がなければ:true
				if (CheckPassWay(rightBefore, upBefore, downBefore, 1)) {
					//右辺のどこかに入り口を作る
					passWayBefore = gManager->GetRandEx(upBefore + 1, downBefore - 1);
					before = 1;
				}
				else if (CheckPassWay(downBefore, leftBefore, rightBefore, 0)) {
					//下辺のどこかに入り口を作る
					passWayBefore = gManager->GetRandEx(leftBefore + 1, rightBefore - 1);
					before = 2;
				}
				else {
					before = -1;
				}
				//*****

				//*****Afterの部屋の入り口検索
				if (CheckPassWay(upAfter, leftAfter, rightAfter, 0)) {
					//上辺のどこかに入り口を作る
					passWayAfter = gManager->GetRandEx(leftAfter + 1, rightAfter - 1);
					after = 1;
				}
				else if (CheckPassWay(leftAfter, upAfter, downAfter, 1)) {
					//左辺のどこかに入り口を作る
					passWayBefore = gManager->GetRandEx(upAfter + 1, downAfter - 1);
					after = 2;
				}
				else {
					after = -1;
				}
				//*****
				//入り口がどちらか一方でも作れなかったら次のループへ抜ける
				if (before == -1 || after == -1)
				{
					count_Jump += 1;
					continue;
				}

				//入り口が両方出来ていたら
				if (before == 1 && after == 1) {
					//通路をつなぐ
					//beforeから交差点まで
					SetAllChip(rightBefore, passWayBefore, passWayAfter, passWayBefore);
					//交差点からafterまで
					SetAllChip(passWayAfter, passWayBefore, passWayAfter, upAfter);
					count_Jump += 1;
					continue;
				}
				else if (before == 2 && after == 2) {
					//通路をつなぐ
					//beforeから交差点まで
					SetAllChip(passWayBefore, downBefore, passWayBefore, passWayAfter);
					//交差点からafter
					SetAllChip(passWayBefore, passWayAfter, leftAfter, passWayAfter);
					count_Jump += 1;
					continue;
				}
				else {
					//通路が交差しない場合はつくらない
					count_Jump += 1;
					continue;
				}
			}
			//若い方が右なら
			else {
				//部屋の相対位置は右上に若い部屋,左下に年上の部屋
				int before = 0;
				int after = 0;

				//*****beforeの部屋の入口検索
				//もし通路がなければ:true
				if (CheckPassWay(leftBefore, upBefore, downBefore, 1)) {
					//左辺のどこかに入り口を作る
					passWayBefore = gManager->GetRandEx(upBefore + 1, downBefore - 1);
					before = 1;
				}
				else if (CheckPassWay(downBefore, leftBefore, rightBefore, 0)) {
					//下辺のどこかに入り口を作る
					passWayBefore = gManager->GetRandEx(leftBefore + 1, rightBefore - 1);
					before = 2;
				}
				else {
					before = -1;
				}
				//*****

				//*****Afterの部屋の入り口検索
				if (CheckPassWay(upAfter, leftAfter, rightAfter, 0)) {
					//上辺のどこかに入り口を作る
					passWayAfter = gManager->GetRandEx(leftAfter + 1, rightAfter - 1);
					after = 1;
				}
				else if (CheckPassWay(rightAfter, upAfter, downAfter, 1)) {
					//右辺のどこかに入り口を作る
					passWayBefore = gManager->GetRandEx(upAfter + 1, downAfter - 1);
					after = 2;
				}
				else {
					after = -1;
				}
				//*****
				//入り口がどちらか一方でも作れなかったら次のループへ抜ける
				if (before == -1 || after == -1)
				{
					count_Jump += 1;
					continue;
				}

				//入り口が両方出来ていたら
				if (before == 1 && after == 1) {
					//通路をつなぐ
					//交差点からbeforeまで
					SetAllChip(passWayAfter, passWayBefore, leftBefore, passWayBefore);
					//交差点からafterまで
					SetAllChip(passWayAfter, passWayBefore, passWayAfter, upAfter);
					count_Jump += 1;
					continue;
				}
				else if (before == 2 && after == 2) {
					//通路をつなぐ
					//beforeから交差点まで
					SetAllChip(passWayBefore, downBefore, passWayBefore, passWayAfter);
					//afterから交差点まで
					SetAllChip(leftAfter, passWayAfter, passWayBefore, passWayAfter);
					count_Jump += 1;
					continue;
				}
				else {
					//通路が交差しない場合はつくらない
					count_Jump += 1;
					continue;
				}
			}//上で左右の場合終わり
		}
		//若い部屋が下なら
		else {
			//若い方の部屋が右か左かで場合分け
			//若いほうが左なら
			if (rightBefore < leftAfter) {
				//部屋の相対位置は左下に若い部屋,右上に年上の部屋
				int before = 0;
				int after = 0;

				//*****beforeの部屋の入口検索
				//もし通路がなければ:true
				if (CheckPassWay(upBefore, leftBefore, rightBefore, 0)) {
					//上辺のどこかに入り口を作る
					passWayBefore = gManager->GetRandEx(leftBefore + 1, rightBefore - 1);
					before = 1;
				}
				else if (CheckPassWay(rightBefore, upBefore, downBefore, 1)) {
					//右辺のどこかに入り口を作る
					passWayBefore = gManager->GetRandEx(upBefore + 1, downBefore - 1);
					before = 2;
				}
				else {
					before = -1;
				}
				//*****

				//*****Afterの部屋の入り口検索
				if (CheckPassWay(leftAfter, upAfter, downAfter, 1)) {
					//上辺のどこかに入り口を作る
					passWayAfter = gManager->GetRandEx(upAfter + 1, downAfter - 1);
					after = 1;
				}
				else if (CheckPassWay(downAfter, leftAfter, rightAfter, 0)) {
					//右辺のどこかに入り口を作る
					passWayBefore = gManager->GetRandEx(leftAfter + 1, rightAfter - 1);
					after = 2;
				}
				else {
					after = -1;
				}
				//*****
				//入り口がどちらか一方でも作れなかったら次のループへ抜ける
				if (before == -1 || after == -1)
				{
					count_Jump += 1;
					continue;
				}

				//入り口が両方出来ていたら
				if (before == 1 && after == 1) {
					//通路をつなぐ
					//交差点からbeforeまで
					SetAllChip(passWayBefore, passWayAfter, passWayBefore, upBefore);
					//交差点からafterまで
					SetAllChip(passWayBefore, passWayAfter, leftAfter, passWayAfter);
					count_Jump += 1;
					continue;
				}
				else if (before == 2 && after == 2) {
					//通路をつなぐ
					//beforeから交差点まで
					SetAllChip(rightBefore, passWayBefore, passWayAfter, passWayBefore);
					//afterから交差点まで
					SetAllChip(passWayAfter, downAfter, passWayAfter, passWayBefore);
					count_Jump += 1;
					continue;
				}
				else {
					//通路が交差しない場合はつくらない
					count_Jump += 1;
					continue;
				}
			}
			//若い方が右なら
			else {
				//部屋の相対位置は右下に若い部屋,左上に年上の部屋
				int before = 0;
				int after = 0;

				//*****beforeの部屋の入口検索
				//もし通路がなければ:true
				if (CheckPassWay(upBefore, leftBefore, rightBefore, 0)) {
					//上辺のどこかに入り口を作る
					passWayBefore = gManager->GetRandEx(leftBefore + 1, rightBefore - 1);
					before = 1;
				}
				else if (CheckPassWay(leftBefore, upBefore, downBefore, 1)) {
					//左辺のどこかに入り口を作る
					passWayBefore = gManager->GetRandEx(upBefore + 1, downBefore - 1);
					before = 2;
				}
				else {
					before = -1;
				}
				//*****

				//*****Afterの部屋の入り口検索
				if (CheckPassWay(rightAfter, upAfter, downAfter, 1)) {
					//右辺のどこかに入り口を作る
					passWayAfter = gManager->GetRandEx(upAfter + 1, downAfter - 1);
					after = 1;
				}
				else if (CheckPassWay(downAfter, leftAfter, rightAfter, 0)) {
					//下辺のどこかに入り口を作る
					passWayBefore = gManager->GetRandEx(leftAfter + 1, rightAfter - 1);
					after = 2;
				}
				else {
					after = -1;
				}
				//*****
				//入り口がどちらか一方でも作れなかったら次のループへ抜ける
				if (before == -1 || after == -1)
				{
					count_Jump += 1;
					continue;
				}

				//入り口が両方出来ていたら
				if (before == 1 && after == 1) {
					//通路をつなぐ
					//交差点からbeforeまで
					SetAllChip(passWayBefore, passWayAfter, passWayBefore, upBefore);
					//afterから交差点まで
					SetAllChip(rightAfter, passWayAfter, passWayBefore, passWayAfter);
					count_Jump += 1;
					continue;
				}
				else if (before == 2 && after == 2) {
					//通路をつなぐ
					//交差点からbeforeまで
					SetAllChip(passWayBefore, passWayAfter, leftBefore, passWayBefore);
					//afterから交差点まで
					SetAllChip(passWayAfter, downAfter, passWayBefore, passWayAfter);
					count_Jump += 1;
					continue;
				}
				else {
					//通路が交差しない場合はつくらない
					count_Jump += 1;
					continue;
				}
			}
		}

	}
	//ある部屋から見て上下左右に隣り合う部屋があるか確認する
	//隣り合う部屋がある場合
	//もしその部屋と繋がっていればreturnする
	//もし繋がってなければその部屋と自分の部屋につなげるポイントを作る
	//ポイントから一番近いdividelineを割り出す
	//dividelineに向かって伸ばす
	//通路で埋める

}

bool Map::CheckPassWay(int roomPos_set, int roomPos_moveStart, int roomPos_moveGoal, int dir)
{
	bool check = true;
	for (int i = roomPos_moveStart; i < roomPos_moveGoal; ++i) {
		//通路があればfor文を抜ける
		if (dir == 0) {
			//左から右へ調査
			if (GetChip(roomPos_set, i) == PASSWAY) {
				check = false;
				break;
			}
		}
		else if (dir == 1) {
			//上から下へ調査
			if (GetChip(i, roomPos_set) == PASSWAY) {
				check = false;
				break;
			}
		}
	}
	return check;
}
