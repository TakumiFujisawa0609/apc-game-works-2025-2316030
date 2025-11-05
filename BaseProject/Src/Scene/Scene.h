#pragma once
class Input;
class SceneController;
class ResourceManager;

/// <summary>
/// ゲームシーンの基底クラス(純粋仮想クラス)
/// </summary>
class Scene
{
protected:
	SceneController& controller_;///持ち主の参照
	// リソース管理
	ResourceManager& resMng_;

	int newRT_;			// scene画面の描画先

public:
	/// <summary>
	/// シーン初期化(の際にシーンコントローラも代入)
	/// </summary>
	/// <param name="controller"></param>
	Scene(SceneController& controller);
	virtual ~Scene() {}

	/// <summary>
	/// シーン開始時に一度だけ呼ばれる初期化関数
	/// </summary>
	/// <param name="input">入力クラス</param>
	virtual void Init(Input& input) = 0;


	/// <summary>
	/// 毎フレーム状態を更新する
	/// </summary>
	/// <param name="input">入力クラス</param>
	virtual void Update(Input& input) = 0;

	/// <summary>
	/// 毎フレーム描画する
	/// </summary>
	virtual void Draw(void) = 0;


	/// <summary>
	/// レンダーターゲットの取得
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	int GetRenderTarget(void);


	/// <summary>
	/// レンダーターゲットを設定する
	/// </summary>
	/// <param name="handle"></param>
	void SetRenderTarget(int handle);

	virtual void DrawUI(void) = 0;
};

