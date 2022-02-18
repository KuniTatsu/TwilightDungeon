#include "Animation.h"
#include"DxLib.h"
#include"GameManager.h"
#include"Camera.h"

extern GameManager* gManager;
Animation::Animation(std::string Gh, t2k::Vector3 Pos, int ActSpeed, int MaxIndex)
{
	//画像の分割ロード
	LoadDivGraph(Gh.c_str(), 5, 5, 1, 30, 30, gh);
	pos = Pos;
	actSpeed = ActSpeed;
	maxMotionIndex = MaxIndex;
	//camera = gManager->GetCamera();
}

Animation::~Animation()
{
}

void Animation::Update()
{
	//描画する画像を選択する
	if (--actWait <= 0) {
		actIndex++;
		actWait = actSpeed;
		actIndex %= maxMotionIndex;
	}
	drawGh = gh[actIndex];
	if (drawGh == gh[4])isAlive = false;
}

void Animation::Draw()
{
	//選択された画像を描画する
	/*float x = pos.x - gManager->camera->cameraPos.x;
	float y = pos.y - gManager->camera->cameraPos.y;*/

	//DrawRotaGraph(pos.x - gManager->camera->cameraPos.x, pos.y - gManager->camera->cameraPos.x, 1, 0, drawGh, true);
	DrawRotaGraph(pos.x - gManager->camera->cameraPos.x, pos.y - gManager->camera->cameraPos.y, 1, 0, drawGh, true);
	//DrawRotaGraph(pos.x, pos.y, 1, 0, drawGh, true);
	//DrawRotaGraph(500, 500, 1, 0, drawGh, true);
}
