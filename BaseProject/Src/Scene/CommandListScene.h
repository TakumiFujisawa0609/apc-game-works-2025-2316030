#pragma once
#include "Scene.h"
class CommandListScene :
    public Scene
{
public:
    CommandListScene(SceneController& controller);

    void Init(Input& input)override;
    void Update(Input& input)override;
    void Draw(void)override;
    void DrawUI(void)override;
};

