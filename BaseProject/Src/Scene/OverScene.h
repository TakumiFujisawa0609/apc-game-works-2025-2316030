#pragma once
#include "Scene.h"
class OverScene :
    public Scene
{
public:
	OverScene(SceneController& controller);
	~OverScene();
	
	void Init(Input& input)override;
	void Update(Input& input)override;
	void Draw(void)override;
	void DrawUI(void)override;

private:
	
	// ゲームオーバー画像
	int imgH_ = 0;

	// ゲームオーバーサウンド
	int soundH_;

	using UpdateFunc_t = void(OverScene::*)(Input& input);
	using DrawFunc_t = void(OverScene::*)();

	// 更新関数ポインタ
	UpdateFunc_t update_;

	// 描画関数ポインタ
	DrawFunc_t draw_;

	// 更新
	// フェードイン
	void FadeInUpadte(Input&);
	
	// 通常更新
	void NormalUpadte(Input&);
	
	// フェードアウト
	void FadeOutUpadte(Input&);

	// 描画
	// 通常描画
	void NormalDraw();

	// フェード描画
	void FadeDraw();

};

