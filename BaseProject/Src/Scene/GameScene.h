#pragma once
#include "Scene.h"
#include<vector>
#include<string>
#include<memory>
class Player;
class Stage;

class Inventory;

class Lockpick;
class HandLight;
class Knife;
class Radio;

class Wire;

class ItemSlot;

class PlayerStatusUI;

class GameScene :
    public Scene
{
public:
	/// <summary>
	/// シーン初期化(の際にシーンコントローラも代入)
	/// </summary>
	/// <param name="controller"></param>
	GameScene(SceneController& controller);
	~GameScene();

	/// <summary>
	/// シーン開始時に一度だけ呼ばれる初期化関数
	/// </summary>
	/// <param name="input">入力クラス</param>
	virtual void Init(Input& input)override;
	/// <summary>
	/// 毎フレーム状態を更新する
	/// </summary>
	/// <param name="input">入力クラス</param>
	void Update(Input& input) override;

	/// <summary>
	/// 毎フレーム描画する
	/// </summary>
	virtual void Draw(void) override;
private:
	int imgH_;
	int frame_ = 0;

	using UpdateFunc_t = void (GameScene::*)(Input& input);
	using DrawFunc_t = void (GameScene::*)();

	UpdateFunc_t update_;
	DrawFunc_t draw_;

	void FadeInUpdate(Input& input);//フェードイン
	void NormalUpdate(Input& input);//通常
	void FadeOutUpdate(Input& input);//フェードアウト

	void NormalDraw();//通常描画
	void FadeDraw();//フェード描画

	struct Vector3 {
		float x, y, z;
	};
	struct Location {
		std::string name;//メッシュ名
		Vector3 pos = {};//場所
		Vector3 angle = {};//回転
	};
	std::vector<Location> locationData_;
	void LoadLocationData();//配置データを読み込む

	std::vector<int> models_;

	std::shared_ptr<Player> player_;
	std::shared_ptr<Stage> stage_;

	// インベントリ
	std::shared_ptr<Inventory> inventory_;

	// アイテムスロット
	std::shared_ptr<ItemSlot> itemSlot_;

	// ロックピック
	std::shared_ptr<Lockpick> lockpick_;

	// ハンドライト
	std::shared_ptr<HandLight> light_;

	// ナイフ
	std::shared_ptr<Knife> knife_;

	// ラジオ
	std::shared_ptr<Radio> radio_;

	// ワイヤー
	std::shared_ptr<Wire> wire_;

	// プレイヤーの状態
	std::shared_ptr<PlayerStatusUI> status_;

	// ホイール入力
	void HandleMouseWheel(Input& input);

	// メインカメラがfps視点であるがどうか
	bool isFps_;
};

