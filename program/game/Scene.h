///*****Description*****
///�V�[���̊��N���X
///�S�ẴV�[���͂��̃N���X���p������
///*********************
#pragma once

class BaseScene {
public:
    virtual ~BaseScene() {}
    virtual void Update() = 0;
    virtual void Draw() = 0;

};