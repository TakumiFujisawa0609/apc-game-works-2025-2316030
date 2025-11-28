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
	TitleScene(SceneController& controller);
	~TitleScene();

	void Init(Input& input)override;
	void Update(Input& input)override;
	void Draw(void)override;
	void DrawUI(void)override;

private:

	// タイトル画像
	int titleH_;
	
	// タイトルサウンド
	int soundH_;

	// 現在のインデックス
	int currentIndex_ = 0;

	//メニューの文字列と関数のテーブル
	using MenuFunc_t = std::function<void(Input&)>;

	// タイトルメニューの配列
	std::vector<std::wstring> menuList_;

	// メニューテーブル
	std::map<std::wstring, MenuFunc_t> menuFuncTable_;

	//メンバ関数ポインタ型の別名を定義
	using UpdateFunc_t = void (TitleScene::*)(Input&);
	using DrawFunc_t = void(TitleScene::*)();
	
	//Update用のメンバ関数ポインタ
	UpdateFunc_t update_;

	//Draw用のメンバ関数ポインタ
	DrawFunc_t draw_;

	// 更新関数
	//フェードイン
	void FadeInUpdate(Input&);

	//通常(入力受付)
	void NormalUpdate(Input&);
	
	//フェードアウト
	void FadeOutUpdate(Input&);

	// 描画関数
	//フェード時の描画
	void FadeDraw();

	//通常時の描画
	void NormalDraw();

	void DrawMenuList(void);

};

