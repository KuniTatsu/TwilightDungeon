#include "Animation.h"
#include"DxLib.h"
#include"GameManager.h"
#include"Camera.h"

extern GameManager* gManager;
Animation::Animation(std::string Gh, t2k::Vector3 Pos, int ActSpeed, int MaxIndex)
{
	//�摜�̕������[�h
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
	//�`�悷��摜��I������
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
	//�I�����ꂽ�摜��`�悷��
	/*float x = pos.x - gManager->camera->cameraPos.x;
	float y = pos.y - gManager->camera->cameraPos.y;*/

	//DrawRotaGraph(pos.x - gManager->camera->cameraPos.x, pos.y - gManager->camera->cameraPos.x, 1, 0, drawGh, true);
	DrawRotaGraph(pos.x - gManager->camera->cameraPos.x, pos.y - gManager->camera->cameraPos.y, 1, 0, drawGh, true);
	//DrawRotaGraph(pos.x, pos.y, 1, 0, drawGh, true);
	//DrawRotaGraph(500, 500, 1, 0, drawGh, true);
}
