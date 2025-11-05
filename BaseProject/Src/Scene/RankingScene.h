#pragma once
#include "Scene.h"
class RankingScene :
    public Scene
{
public:
	RankingScene(SceneController& controller);
	~RankingScene();
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

	virtual void DrawUI(void)override;

private:

};

