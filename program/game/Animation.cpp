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
	//camera = gManager->GetCamera();
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
	drawGh = gh[actIndex];
	if (drawGh == gh[4])isAlive = false;
}

void Animation::Draw()
{
	DrawRotaGraph(pos.x - gManager->camera->cameraPos.x, pos.y - gManager->camera->cameraPos.y, 1, 0, drawGh, true);
}
