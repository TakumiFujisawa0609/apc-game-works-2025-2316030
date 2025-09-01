#pragma once

struct Size {
	int width;//幅
	int height;//高さ
};

/// <summary>
/// アプリケーションの起動と終了を
/// コントロールするシングルトンクラス
/// </summary>
class Application
{
private:
	Application();//コンストラクタをprivateに
	//コピー＆代入の禁止
	Application(const Application& app) = delete;
	void operator=(const Application& app) = delete;

	Size windowSize_ = { 640,480 };

public:
	~Application();//デストラクタ
	static Application& GetInstance();

	const Size& GetWindowSize()const;

	/// <summary>
	/// アプリケーションの初期化
	/// </summary>
	/// <returns>true:初期化成功 / false:初期化失敗</returns>
	bool Init(int w = 640, int h = 480);

	/// <summary>
	/// ゲームを起動する(メインループを実行)
	/// </summary>
	void Run();

	/// <summary>
	/// アプリケーションの終了処理
	/// </summary>
	void Terminate();
};

