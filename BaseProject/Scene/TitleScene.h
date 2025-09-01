#pragma once
#include "Scene.h"
class TitleScene :
    public Scene
{
private:
	int titleH_;
	int frame_ = 0;
	//メンバ関数ポインタ型の別名を定義
	using UpdateFunc_t = void (TitleScene::*)(Input&);
	using DrawFunc_t = void(TitleScene::*)();
	
	UpdateFunc_t update_;//Update用のメンバ関数ポインタ
	DrawFunc_t draw_;//Draw用のメンバ関数ポインタ

	//切り替えるべき関数を書いていく
	void FadeInUpdate(Input&);//フェードイン
	void NormalUpdate(Input&);//通常(入力受付)
	void FadeOutUpdate(Input&);//フェードアウト

	void FadeDraw();//フェード時の描画
	void NormalDraw();//通常時の描画

public:
	TitleScene(SceneController& controller);
	~TitleScene();
	/// <summary>
	/// 毎フレーム状態を更新する
	/// </summary>
	/// <param name="input">入力クラス</param>
	virtual void Update(Input& input)override;

	/// <summary>
	/// 毎フレーム描画する
	/// </summary>
	virtual void Draw()override;
};

