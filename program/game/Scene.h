///*****Description*****
///シーンの基底クラス
///全てのシーンはこのクラスを継承する
///*********************
#pragma once

class BaseScene {
public:
    virtual ~BaseScene() {}
    virtual void Update() = 0;
    virtual void Draw() = 0;

};