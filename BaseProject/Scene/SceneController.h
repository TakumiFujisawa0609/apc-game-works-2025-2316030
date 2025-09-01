#pragma once

#include<memory>
#include<list>
class Scene;
class Input;
/// <summary>
/// シーンの切り替え関数を提供し
/// 毎フレーム手持ちのシーンの更新を行う
/// </summary>
class SceneController
{
private:
	std::list<std::shared_ptr<Scene>> scenes_;//次はここをlist化します
public:
	/// <summary>
	/// 実行すべきシーンを切り替えます
	/// </summary>
	/// <param name="scene">次から実行したいシーン</param>
	void ChangeScene(std::shared_ptr<Scene> scene);
	/// <summary>
	/// 手持ちのシーンを更新する
	/// </summary>
	/// <param name="input"></param>
	void Update(Input& input);
	/// <summary>
	/// 手持ちのシーンを描画する
	/// </summary>
	void Draw();

	/// <summary>
	/// シーンを新しく「積む」
	/// </summary>
	/// <param name="scene"></param>
	void PushScene(std::shared_ptr<Scene> scene);

	/// <summary>
	/// 最後に追加したシーンを削除する
	/// </summary>
	void PopScene();

	/// <summary>
	/// 強制的に特定のシーンに飛ぶ。積んでてもリセットされる
	/// </summary>
	/// <param name="scene">ジャンプ先シーン</param>
	void JumpScene(std::shared_ptr<Scene> scene);
};

