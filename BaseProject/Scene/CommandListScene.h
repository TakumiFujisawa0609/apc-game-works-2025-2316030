#pragma once
#include "Scene.h"
class CommandListScene :
    public Scene
{
public:
    CommandListScene(SceneController& controller);
    virtual void Update(Input& input)override;
    virtual void Draw()override;
};

