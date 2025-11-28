#pragma once



class Config
{
public:

	// ウィンドウサイズ
	struct WindowSize{
		int width_,
			height_; 
	};

	// 明示的にインスタンスを生成
	static void CreateInstance();

	// 静的インスタンスの取得
	static Config& GetInstance();

	/// <summary>
	/// 設定の初期化
	/// </summary>
	/// <param name=""></param>
	void Init(void);

	// スクリーンのサイズの取得・設定
	const WindowSize& GetWindowSize(void) const;
	void SetWindowSize(int width = 1280, int height = 720);

	// フルスクリーンの状態確認・状態変更
	bool IsFullScreen(void) const;
	void SetFullScreen(bool isFull = false);

	// モード切替時の初期化
	void ScreenInitialize(void);

	// マウス感度の取得・設定
	float GetMouseSensitivity(void) const;
	void SetMouseSensitivity(float sensitivity = 0.0002f);

	// BGM音量の取得・設定
	int GetBGMVolume(void) const;
	void SetBGMVolume(int volume = 255);

	// SE音量の取得・設定
	int GetSEVolume(void) const;
	void SetSEVolume(int volume = 255);

	// ジョイパッドの振動の取得・設定
	bool isJoyPadVibrationFlag(void) const;
	void SetJoyPadVibration(bool flag = true);

	// リソースの削除
	void Destroy(void);

private:

	Config(void);
	~Config(void) = default;
	
	static Config* instance_;

	// グラフィック関係
	WindowSize window_;
	bool isFullS_;	// フルスクリーンであるかどうか

	// 解像度変更
	int fullScreenWidth_, fullScreenHeight_;

	// 入力関係

	// マウス感度
	float mouseSensitivity_;

	// パッド操作
	bool isVibration_;

	// サウンド関係
	int bgmVolume_;
	int seVolume_;
};

