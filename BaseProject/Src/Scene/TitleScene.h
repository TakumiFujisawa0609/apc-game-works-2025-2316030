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

	virtual void Init(Input& input)override;
	virtual void Update(Input& input)override;
	virtual void Draw(void)override;
	virtual void DrawUI(void)override;

private:

	// 現在のインデックス
	int currentIndex_ = 0;		

	// メニューの文字列と関数のテーブル
	using MenuFunc_t = std::function<void(Input&)>;
	std::vector<std::wstring> menuList_;
	std::map<std::wstring, MenuFunc_t> menuFuncTable_;

	// メンバ関数ポインタ型の別名を定義
	using UpdateFunc_t = void (TitleScene::*)(Input&);
	using DrawFunc_t = void (TitleScene::*)();
	
	// Update用のメンバ関数ポインタ
	UpdateFunc_t update_;

	// Draw用のメンバ関数ポインタ
	DrawFunc_t draw_;					

	// フェードイン
	void FadeInUpdate(Input&);

	// 通常(入力受付)
	void NormalUpdate(Input&);

	// フェードアウト
	void FadeOutUpdate(Input&);

	// フェード時の描画
	void FadeDraw();

	// 通常時の描画
	void NormalDraw();

	// メニューリストの描画
	void DrawMenuList(void);

	// 点滅カウンター
	int blinkCounter_ = 0;

	// 前フレームのPOV方向
	int prevPOVDirection_;
};

