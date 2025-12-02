#pragma once
#include "Scene.h"
class ClearScene :
    public Scene
{

public:
	ClearScene(SceneController& controller);
	~ClearScene();

	void Init(Input& input)override;
	void Update(Input& input)override;
	void Draw(void)override;
	void DrawUI(void)override;

private:

	int imgH_ = -1;
	int soundH_ = -1;
	using UpdateFunc_t = void(ClearScene::*)(Input& input);
	using DrawFunc_t = void(ClearScene::*)();

	UpdateFunc_t update_;
	DrawFunc_t draw_;

	void FadeInUpadte(Input& input);
	void NormalUpadte(Input& input);
	void FadeOutUpadte(Input& input);

	void NormalDraw();
	void FadeDraw();
};

