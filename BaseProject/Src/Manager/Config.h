#pragma once

struct WindowSize
{
	int width_,
		height_;
};

class Config
{
public:

	Config(void);
	~Config(void);

	void Init(void);
	void Update(void);


	// スクリーンのサイズを取得する
	const WindowSize& GetWindowSize(void) const;

private:

	// グラフィック関係
	WindowSize window_;
	bool isFullS_;	// フルスクリーンであるかどうか

	// 解像度変更

	// 入力関係

	// マウス感度

};

