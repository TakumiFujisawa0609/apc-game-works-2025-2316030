#pragma once
#include "../ObjectBase/Charactor.h"

// 入力系
class TimeLimitComponent;
class PlayerInput;
class HandLight;
class UIComponent;
class Collider;
class Capsule;

class Player
	:
	public Charactor
{
public:

	// 歩きスピード
	static constexpr float MOVE_WALK_SPEED = 5.0f;

	// 走りスピード
	static constexpr float MOVE_RUN_SPEED = 10.0f;

	// マウス感度
	static constexpr float MOUSE_SENSITIVITY = 0.002f;

	// 最大正気度
	static constexpr float MAX_SAN_VALUE = 50.0f;

	// 最大HP
	static constexpr float MAX_HP = 100.0f;

	Player(void);
	~Player(void);

	void Init(void) override;
	void InitComponents(void) override;
	void Update(float deltaTime) override;
	void OnUpdate(float deltaTime) override;
	void Draw(void) override;
	void DrawUI(void);

	// 制限時間コンポーネントの取得
	TimeLimitComponent* GetTimeLimitComp();

	// 入力コンポーネントの取得
	PlayerInput* GetInputComp();

	// アイテムの取得条件
	bool TakeItem(int itemId, int count);

	// 歩きSEの取得
	int GetWalkSH(void);

	// 走りSEの取得
	int GetRunSH(void);

	// ハンドライトの設定
	void SetHandLight(std::weak_ptr<HandLight> handLight);

	// 正気度レベルの設定
	void SetSanityLevel(float value);

	// HP割合の取得
	float GetHealthRatio(void) const;

	// 正気度割合の取得
	float GetSanityRatio(void) const;

	// HP回復用設定
	void SetHitPoint(float value);

	// 与えられたダメージ
	void TakeDamage(float damege);

	// 現在のHPの取得
	float GetHp(void);

	// 現在の正気度の取得
	float GetSanity(void);

private:

	// 酸素コンポーネント
	TimeLimitComponent* tLimit_;

	// 入力コンポーネント
	PlayerInput* input_;

	// 循環参照を防ぐためにweak_ptrで定義
	std::weak_ptr<HandLight> light_;

	// 水平回転（ヨー）
	float yaw;	

	// 垂直回転（ピッチ）
	float pitch;

	// 歩きの音
	int walkSH_;

	// 歩きの音を再生するかどうか
	bool isWalkSH_;

	// 走るの音
	int runSH_;

	// 走るの音を再生するかどうか
	bool isRunSH_;

	// 体力
	float hp_;

	// 正気度
	float sanV_;

	// 回転処理
	void SetGoalRotate(float rotRad);

	virtual void Collision(void);
	virtual void CollisionCapsule(void);
	virtual void CollisionGravity(void);
	virtual void CalcGravityPow(void);
};

