#pragma once

class BaseScene {
public:
    virtual ~BaseScene() {}
    virtual void Update() = 0;
    virtual void Draw() = 0;

    virtual void Save() {};
    virtual void Load() {};
};