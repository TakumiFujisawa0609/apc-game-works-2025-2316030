#pragma once
#include "Scene.h"
class RankingScene :
    public Scene
{
public:
	RankingScene(SceneController& controller);
	~RankingScene();
	
	virtual void Init(Input& input)override;
	virtual void Update(Input& input)override;
	virtual void Draw(void)override;
	virtual void DrawUI(void)override;

private:

};

