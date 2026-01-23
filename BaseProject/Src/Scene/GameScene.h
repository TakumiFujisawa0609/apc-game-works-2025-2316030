#pragma once
#include "Scene.h"
#include <DxLib.h>
#include<vector>
#include<string>
#include<memory>

class Player;
class EnemyBase;
class Stage;
class Lockpick;
class HandLight;
class Wire;
class ItemBase;
class SlotBase;
class PlayerStatusUI;
class Dummy;

class GameScene :
	public Scene
{
public:
	GameScene(SceneController& controller);
	~GameScene();

	virtual void Init(Input& input)override;
	virtual void Update(Input& input) override;
	virtual void Draw(void) override;
	virtual void DrawUI(void)override;

	// ゲームの目的
	enum class TASK
	{
		FIND_LIGHT,
		FIND_WIRE,
		FIND_DOOR,
		CLEARED
	};

private:
	
	using UpdateFunc_t = void (GameScene::*)(Input& input);
	using DrawFunc_t = void (GameScene::*)();

	UpdateFunc_t update_;
	DrawFunc_t draw_;

	// フェードイン
	void FadeInUpdate(Input& input);

	// 通常
	void NormalUpdate(Input& input);

	// フェードアウト
	void FadeOutUpdate(Input& input);

	// 通常描画
	void NormalDraw();

	// フェード描画
	void FadeDraw();

	struct Vector3 {
		float x, y, z;
	};
	struct Location {
		std::string name;		//メッシュ名
		Vector3 pos = {};		//場所
		Vector3 angle = {};		//回転
	};
	std::vector<Location> locationData_;

	// 配置データを読み込む
	void LoadLocationData();
	std::vector<int> models_;

	std::shared_ptr<Player> player_;
	std::shared_ptr<EnemyBase> eBase_;
	std::shared_ptr<Stage> stage_;
	std::vector<VECTOR> spownPos_;

	// アイテムスロット
	std::shared_ptr<SlotBase> itemSlot_;

	// ロックピック
	std::shared_ptr<Lockpick> lockpick_;

	// ハンドライト
	std::shared_ptr<HandLight> light_;

	// ワイヤー
	std::shared_ptr<Wire> wire_;

	// プレイヤーの状態
	std::shared_ptr<PlayerStatusUI> status_;

	// メインアイテムプールで管理
	std::vector<std::shared_ptr<ItemBase>> itemPool_;

	// ランダム生成のアイテム
	std::vector<std::shared_ptr<ItemBase>> randomItems_;

	// 当たり判定用のダミー
	std::shared_ptr<Dummy> dummy_;

	// アイテム全体を処理
	void UpdateItemPool(float deltaTime);
	void DrawItemPool(void);
	void DrawUIItemPool(void);

	// ホイール入力
	void HandleMouseWheel(Input& input);

	// メインカメラがfps視点であるがどうか
	bool isFps_;

	// ゲーム進行状態
	enum class STATE
	{
		TUTORIAL,
		MAINGAME
	};

	void UpdateTutorial(float deltaTime, Input& input);
	void UpdateMainGame(float deltaTime, Input& input);

	void DrawTutorial(void);
	void DrawMainGame(void);

	// ゲーム進行状態
	STATE state_;

	void ChangeState(STATE state);
	void ChangeTutorial(void);
	void ChangeMainGame(void);

	// アイテムの取得
	void ObtainItem(void);

	// 取得されるアイテムのポインタを返す
	std::shared_ptr<ItemBase> isObtainItems(void);

	// アイテムとの当たり判定処理
	bool IsHitItems(void);

	// アイテムとの当たり判定のフラグ
	bool isHitItem_;

	// 消費されたアイテムの削除
	void CleanUpItemPool(void);

	// ドアとの当たり判定
	bool IsHitDoor(void) const;

	// 目的
	TASK task_;

	void UpdateTaskState(TASK task);
	std::wstring currentTaskMessage_ = L"";

	// 完了時に一瞬表示するメッセージ
	std::wstring clearMessage_ = L"";

	// 完了メッセージの表示時間
	float clearMessageTimer_ = 0.0f;

	// 完了メッセージの表示秒数 (3秒)
	constexpr static float CLEAR_MSG_DURATION = 3.0f;

	// アイテムの目的の更新
	void UpdateItemTasks(void);
};

