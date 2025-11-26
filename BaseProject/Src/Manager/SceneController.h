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
	
<<<<<<< HEAD
	int mainScreen_;			// 最初の描画先
	int depthScreen_;			// 深度マップ
=======
	int mainScreen_=-1;			// 最初の描画先

	int depthScreen_ = -1;
	int blur1Screen_ = -1;	// ブラースクリーン１
	int blur2Screen_ = -1;	// ブラースクリーン２

>>>>>>> ecefbc80def25fa1d0a042bdaabdac08dc61600b

public:
	/// <summary>
	/// 実行すべきシーンを切り替えます
	/// </summary>
	/// <param name="scene">次から実行したいシーン</param>
	void ChangeScene(std::shared_ptr<Scene> scene, Input& input);
	/// <summary>
	/// 手持ちのシーンを更新する
	/// </summary>
	/// <param name="input"></param>
	void Update(Input& input);
	/// <summary>
	/// 手持ちのシーンを描画する
	/// </summary>
	void Draw(void);

	/// <summary>
	/// UI専用の描画
	/// </summary>
	void DrawUI(void);

	/// <summary>
	/// シーンを新しく「積む」
	/// </summary>
	/// <param name="scene"></param>
	void PushScene(std::shared_ptr<Scene> scene, Input& input);

	/// <summary>
	/// 最後に追加したシーンを削除する
	/// </summary>
	void PopScene(Input& input);

	/// <summary>
	/// 強制的に特定のシーンに飛ぶ。積んでてもリセットされる
	/// </summary>
	/// <param name="scene">ジャンプ先シーン</param>
	void JumpScene(std::shared_ptr<Scene> scene,Input& input);

	void DrawPushScene(void);

	// メイン用スクリーンの取得
	int GetMainScreen(void) const;

	// 深度バッファ用スクリーンを取得する
	int GetDepthScreen(void) const;

	// ブラー１用のスクリーンの取得
	int GetBlur1Screen(void) const;

	// ブラー２用スクリーンの取得
	int GetBlur2Screen(void) const;
};

