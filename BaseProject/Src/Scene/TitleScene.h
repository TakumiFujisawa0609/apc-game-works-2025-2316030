#pragma once
#include<string>
#include<vector>
#include<map>
#include<functional>
#include "Scene.h"
class TitleScene :
    public Scene
{
public:

	static constexpr float STICK_THRESHOLD = 0.5f;		// スティック反応閾値
	static constexpr int POV_UP = 0;					// POV上方向
	static constexpr int POV_RIGHT = 9000;				// POV右方向
	static constexpr int POV_DOWN = 18000;				// POV下方向
	static constexpr int POV_LEFT = 27000;				// POV左方向
	static constexpr float STICK_SCALE = 1000.0f;		// スティック入力スケーリング係数

	TitleScene(SceneController& controller);
	~TitleScene();
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
	/// 毎フレームメイン画面を描画する
	/// </summary>
	virtual void Draw(void)override;


	/// <summary>
	/// 毎フレームUI画面を描画する
	/// </summary>
	/// <param name=""></param>
	virtual void DrawUI(void)override;

private:

	int titleH_;				// タイトル画像
	int frame_ = 0;				// フレーム
	int soundH_;				// 音

	int currentIndex_ = 0;		// 現在のインデックス

	//メニューの文字列と関数のテーブル
	using MenuFunc_t = std::function<void(Input&)>;
	std::vector<std::wstring> menuList_;
	std::map<std::wstring, MenuFunc_t> menuFuncTable_;

	//メンバ関数ポインタ型の別名を定義
	using UpdateFunc_t = void (TitleScene::*)(Input&);
	using DrawFunc_t = void(TitleScene::*)();
	
	UpdateFunc_t update_;				//Update用のメンバ関数ポインタ
	DrawFunc_t draw_;					//Draw用のメンバ関数ポインタ

	void FadeInUpdate(Input&);			//フェードイン
	void NormalUpdate(Input&);			//通常(入力受付)
	void FadeOutUpdate(Input&);			//フェードアウト

	void FadeDraw();					//フェード時の描画
	void NormalDraw();					//通常時の描画

	void DrawMenuList(void);			// メニューリストの描画

	int blinkCounter_ = 0;				// 点滅カウンター
	int prevPOVDirection_;				// 前フレームのPOV方向
};

