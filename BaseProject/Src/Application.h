#pragma once
#include <string>
#include <memory>

class Input;
class SceneController;
class Camera;

/// <summary>
/// アプリケーションの起動と終了を
/// コントロールするシングルトンクラス
/// </summary>
class Application
{
public:

	struct Fog
	{
		// フォグの始まり
		float fogStart_;

		// フォグの終わり
		float fogEnd_;
	};

	// スクリーンサイズ
	static constexpr int SCREEN_SIZE_X = 1024;
	static constexpr int SCREEN_SIZE_Y = 640;

	const int TARGET_FPS = 60;
	const int FRAME_TIME = 1000 / TARGET_FPS; // 1フレームあたりの時間（ミリ秒）

	const double DELTATIME_DURATION = 1000000000.0;

	// データパス関連
	//-------------------------------------------
	static const std::wstring PATH_IMAGE;
	static const std::wstring PATH_MODEL;
	static const std::wstring PATH_EFFECT;
	static const std::wstring PATH_SHADER;
	static const std::wstring PATH_SOUND;
	//-------------------------------------------

	~Application();//デストラクタ

	// 明示的にインスタンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static Application& GetInstance();

	/// <summary>
	/// アプリケーションの初期化
	/// </summary>
	/// <returns>true:初期化成功 / false:初期化失敗</returns>
	void Init(void);

	/// <summary>
	/// ゲームを起動する(メインループを実行)
	/// </summary>
	void Run();

	/// <summary>
	/// アプリケーションの終了処理
	/// </summary>
	void Terminate();

	// 初期化成功／失敗の判定
	bool IsInitFail(void) const;

	// 解放成功／失敗の判定
	bool IsReleaseFail(void) const;

	// ゲームを終了させる
	bool IsGemeEnd(void);

	// ゲーム終了を設定する
	void SetGemeEnd(bool frag);

	// デルタタイムの更新処理
	void UpdateDeltaTime(void);

	/// <summary>
	/// 前フレームからの経過時間(秒)を取得する
	/// </summary>
	/// <param name=""></param> </param>
	float GetDeltaTime(void);

	// デルタタイムをリセットする
	void ResetDeltaTime(void);

	// カメラの取得
	std::shared_ptr<Camera> GetCamera(void);

	// フォントの取得
	int GetFontHandle(void)const;

	// フォグの取得
	const Fog& GetFog(void) const;

	// シーンコントローラーの取得
	std::shared_ptr<SceneController> GetSceneController(void) const;

	// 入力の取得
	std::shared_ptr<Input> GetInput(void) const;

private:
	Application();//コンストラクタをprivateに
	//コピー＆代入の禁止
	Application(const Application& app) = delete;
	void operator=(const Application& app) = delete;

	// 静的インスタンス
	static Application* instance_;

	// キー入力
	std::shared_ptr<Input> input_;

	// シーン管理
	std::shared_ptr<SceneController> controller_;

	// カメラ
	std::shared_ptr<Camera> camera_;

	// 初期化失敗
	bool isInitFailed_;

	// 解放失敗
	bool isReleaseFailed_;

	// ゲーム終了
	bool isGemeEnd_;

	// FPS計測用変数
	int frameCount_;

	// FPS
	int fps_;

	// 前フレーム処理を開始した瞬間時刻
	int lastTime_;

	// 前フレームからの経過時間(秒)
	float deltaTime_;

	// fps計測用の変数
	int startTime_;

	// 3D関連の初期化
	void Init3D(void);

	// Effekseerの初期化
	void InitEffekseer(void);

	// フォントハンドル
	int fontHandle_;

	// フレームレートの計算処理
	void CalcFrameRate(void);

	// フレームレートの表示
	void DrawFrameRate(void);

	// ドローコールの表示
	void DrawDrawCall(void);

	Fog fog_;
};

