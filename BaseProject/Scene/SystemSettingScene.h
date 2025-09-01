#pragma once
#include "Scene.h"
class SystemSettingScene :
    public Scene
{

public:
    SystemSettingScene(SceneController& controller);
    virtual void Update(Input& input)override;
    virtual void Draw()override;
};

