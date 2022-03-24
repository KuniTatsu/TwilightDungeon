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
	animationType = 0;
	//camera = gManager->GetCamera();
}

Animation::Animation(std::string Gh, t2k::Vector3 Pos)
{
	LoadDivGraph(Gh.c_str(), 10, 10, 1, 30, 30, levelUpGh);
	pos = Pos;
	actSpeed = 10;
	maxMotionIndex = 10;
	animationType = 1;
	
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
		t2k::debugTrace("\nアニメーション更新%d回目\n", actIndex);
	}
	else return;

	if (animationType == 0)drawGh = gh[actIndex];
	else drawGh = levelUpGh[actIndex];

	if (animationType == 0) {
		if (drawGh == gh[maxMotionIndex - 1])isAlive = false;
	}
	else {
		if (drawGh == levelUpGh[maxMotionIndex - 1])isAlive = false;
	}
}

void Animation::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	DrawRotaGraph(pos.x - gManager->camera->cameraPos.x, pos.y - gManager->camera->cameraPos.y, 1, 0, drawGh, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
