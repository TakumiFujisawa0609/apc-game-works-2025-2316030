#pragma once
class Input;
class SceneController;
/// <summary>
/// ゲームシーンの基底クラス(純粋仮想クラス)
/// </summary>
class Scene
{
protected:
	SceneController& controller_;///持ち主の参照
public:
	/// <summary>
	/// シーン初期化(の際にシーンコントローラも代入)
	/// </summary>
	/// <param name="controller"></param>
	Scene(SceneController& controller);
	virtual ~Scene() {}
	/// <summary>
	/// 毎フレーム状態を更新する
	/// </summary>
	/// <param name="input">入力クラス</param>
	virtual void Update(Input& input) = 0;

	/// <summary>
	/// 毎フレーム描画する
	/// </summary>
	virtual void Draw() = 0;
};

