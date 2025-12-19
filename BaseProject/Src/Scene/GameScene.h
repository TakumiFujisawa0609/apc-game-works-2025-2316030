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

	// 
	static constexpr float DRAW_X = 0.36875f;

	// 
	static constexpr float DRAW_Y = 0.04166f;

	//
	static constexpr float CLEAR_DRAW_Y = 0.0625f;

	// 
	static constexpr float CURRENT_TASK_DRAW_Y = 0.0625f;



	GameScene(SceneController& controller);
	~GameScene();


	void Init(Input& input)override;
	void Update(Input& input) override;
	void Draw(void) override;
	void DrawUI(void)override;

	enum class TASK
	{
		NONE,
		FIND_LIGHT,
		FIND_WIRE,
		FIND_DOOR,
		CLEARED
	};

private:

	// ゲームシーン画像
	int imgH_;

	using UpdateFunc_t = void (GameScene::*)(Input& input);
	using DrawFunc_t = void (GameScene::*)();

	// 更新の関数ポインタ
	UpdateFunc_t update_;

	// 描画の関数ポインタ
	DrawFunc_t draw_;

	// 更新
	// フェードイン
	void FadeInUpdate(Input& input);

	// 通常
	void NormalUpdate(Input& input);
	
	// フェードアウト
	void FadeOutUpdate(Input& input);

	// 描画
	// 通常描画
	void NormalDraw();

	// フェード描画
	void FadeDraw();

	struct Vector3 {
		float x, y, z;
	};

	struct Location {
		std::string name;//メッシュ名
		Vector3 pos = {};//場所
		Vector3 angle = {};//回転
	};

	// 位置情報データ
	std::vector<Location> locationData_;

	// 配置データを読み込む
	void LoadLocationData();

	// モデル
	std::vector<int> models_;

	// プレイヤー 
	std::shared_ptr<Player> player_;
	
	// 敵ベース
	std::shared_ptr<EnemyBase> eBase_;
	
	// ステージ
	std::shared_ptr<Stage> stage_;

	// アイテムのランダム生成位置
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

	// 当たり判定用
	std::shared_ptr<Dummy> dummy_;

	// アイテム全体を処理
	// アイテム全体の更新
	void UpdateItemPool(float deltaTime);
	
	// アイテム全体の描画
	void DrawItemPool(void);

	// アイテム全体のUI描画
	void DrawUIItemPool(void);

	// ホイール入力
	void HandleMouseWheel(Input& input);

	// ゲームの進行度
	enum class PROGRESS
	{
		TUTORIAL,
		MAINGAME
	};

	// チュートリアルの更新
	void UpdateTutorial(float deltaTime, Input& input);
	
	// メインゲームシーンの更新
	void UpdateMainGame(float deltaTime, Input& input);

	// チュートリアルの描画
	void DrawTutorial(void);

	// メインゲームシーンの描画
	void DrawMainGame(void);

	// ゲーム進行度
	PROGRESS progress_;

	// ゲーム進行度の変更
	void ChangeProgress(PROGRESS progress);

	// 一度だけ呼ばれるチュートリアル変更処理
	void ChangeTutorial(void);

	// 一度だけ呼ばれるメインゲーム変更処理
	void ChangeMainGame(void);

	// アイテムの取得(アイテムスロットの中に格納される)
	void ObtainItem(void);

	// アイテムスロットに格納予定のアイテムのポインタを返す
	std::shared_ptr<ItemBase> isObtainItems(void);

	// アイテムを取得できる状態を返す
	bool IsHitItems(void);

	// アイテムの取得判定
	bool isHitItem_;

	// アイテム削除
	void CleanUpItemPool(void);

	// ドアとの当たり判定
	bool IsHitDoor(void) const;

	// タスク
	TASK task_;

	// タスク更新
	void UpdateTaskState(TASK task);

	// 現在のタスクメッセージ
	std::wstring currentTaskMessage_ = L"";

	// 完了時に一瞬表示するメッセージ
	std::wstring clearMessage_ = L"";

	// 完了メッセージの表示時間
	float clearMessageTimer_ = 0.0f;

	// 完了メッセージの表示秒数 (3秒)
	constexpr static float CLEAR_MSG_DURATION = 3.0f;

	// アイテムを取得した際のタスク更新
	void UpdateItemTasks(void);
};

