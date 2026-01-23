#pragma once
#include "Scene.h"
class OverScene :
    public Scene
{
public:
	OverScene(SceneController& controller);
	~OverScene();

	virtual void Init(Input& input)override;
	virtual void Update(Input& input)override;
	virtual void Draw(void)override;
	virtual void DrawUI(void)override;

private:

	using UpdateFunc_t = void(OverScene::*)(Input& input);
	using DrawFunc_t = void(OverScene::*)();

	UpdateFunc_t update_;
	DrawFunc_t draw_;

	void FadeInUpadte(Input&);
	void NormalUpadte(Input&);
	void FadeOutUpadte(Input&);

	void NormalDraw();
	void FadeDraw();

};

