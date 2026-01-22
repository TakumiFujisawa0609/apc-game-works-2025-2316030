#pragma once
#include "Scene.h"
class CommandListScene :
    public Scene
{
public:
    CommandListScene(SceneController& controller);

    virtual void Init(Input& input)override;
    virtual void Update(Input& input)override;
    virtual void Draw(void)override;
    virtual void DrawUI(void)override;
private:

};

