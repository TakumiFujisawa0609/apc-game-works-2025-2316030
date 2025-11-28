#pragma once
#include "Scene.h"
class RankingScene :
    public Scene
{
public:
	RankingScene(SceneController& controller);
	~RankingScene();

	void Init(Input& input)override;
	void Update(Input& input)override;
	void Draw(void)override;
	void DrawUI(void)override;

private:

};

