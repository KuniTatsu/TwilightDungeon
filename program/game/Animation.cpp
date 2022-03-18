#include "Animation.h"
#include"DxLib.h"
#include"GameManager.h"
#include"Camera.h"

extern GameManager* gManager;
Animation::Animation(std::string Gh, t2k::Vector3 Pos, int ActSpeed, int MaxIndex)
{
	//‰æ‘œ‚Ì•ªŠ„ƒ[ƒh
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
	actSpeed = 15;
	maxMotionIndex = 10;
	animationType = 1;
}

Animation::~Animation()
{
}

void Animation::Update()
{
	//•`‰æ‚·‚é‰æ‘œ‚ð‘I‘ð‚·‚é
	if (--actWait <= 0) {
		actIndex++;
		actWait = actSpeed;
		actIndex %= maxMotionIndex;
	}
	if (animationType == 0)drawGh = gh[actIndex];
	else drawGh = levelUpGh[actIndex];
	if (drawGh == gh[maxMotionIndex-1])isAlive = false;
}

void Animation::Draw()
{
	DrawRotaGraph(pos.x - gManager->camera->cameraPos.x, pos.y - gManager->camera->cameraPos.y, 1, 0, drawGh, true);
}
