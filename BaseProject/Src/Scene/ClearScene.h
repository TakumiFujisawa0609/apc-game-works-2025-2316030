#pragma once
#include "Scene.h"
class ClearScene :
    public Scene
{

public:
	ClearScene(SceneController& controller);
	~ClearScene();

	virtual void Init(Input& input)override;
	virtual void Update(Input& input)override;
	virtual void Draw(void)override;
	virtual void DrawUI(void)override;

private:

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

