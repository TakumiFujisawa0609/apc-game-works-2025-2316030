#pragma once

class Config;
#include "Scene.h"
class SystemSettingScene :
    public Scene
{

public:
    SystemSettingScene(SceneController& controller);
    virtual void Init(Input& input)override;
    virtual void Update(Input& input)override;
    virtual void Draw(void)override;
    virtual void DrawUI(void)override;

private:


};

