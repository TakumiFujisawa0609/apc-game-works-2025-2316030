#pragma once
#include "Scene.h"
class ClearScene :
    public Scene
{

public:
	ClearScene(SceneController& controller);
	~ClearScene();
	/// <summary>
	/// シーン開始時に一度だけ呼ばれる初期化関数
	/// </summary>
	/// <param name="input">入力クラス</param>
	virtual void Init(Input& input)override;

	/// <summary>
	/// 毎フレーム状態を更新する
	/// </summary>
	/// <param name="input">入力クラス</param>
	virtual void Update(Input& input)override;

	/// <summary>
	/// 毎フレーム描画する
	/// </summary>
	virtual void Draw(void)override;
private:
	int imgH_ = 0;
	int frame_ = 0;
	int soundH_;
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

