#pragma once
#include "../ObjectBase/Charactor.h"

// 入力系
class TimeLimitComponent;
class PlayerInput;

//// インベントリ
//class Inventory;

class HandLight;

// 描画系
class UIComponent;

class Collider;
class Capsule;

class EnemyBase;

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

	// 制限時間コンポーネントの取得
	TimeLimitComponent* GetTimeLimitComp();

	// 入力コンポーネントの取得
	PlayerInput* GetInputComp();

	// アイテムの取得条件
	bool TakeItem(int itemId, int count);

	int GetWalkSH(void);

	int GetRunSH(void);

	void SetHandLight(std::weak_ptr<HandLight> handLight);

	void SetEnemyBase(std::weak_ptr<EnemyBase> enemy);

	void DrawUI(void);

	// 正気度の回復
	void SetSanityLevel(float value);

	float GetHealthRatio(void) const;

	float GetSanityRatio(void) const;

	void SetHitPoint(float value);

	// 攻撃判定があるものからダメージを受ける
	void TakeDamage(float damege);

	float GetHp(void);

	float GetSanity(void);

	void CollisionEnemy(void);

private:

	// 酸素コンポーネント
	TimeLimitComponent* tLimit_;

	// 入力コンポーネント
	PlayerInput* input_;

	std::weak_ptr<HandLight> light_;

	std::weak_ptr<EnemyBase> enemy_;

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

	// 正気度値
	float sanV_;

	void SetGoalRotate(float rotRad);

	virtual void Collision(void);
	virtual void CollisionCapsule(void);
	virtual void CollisionGravity(void);
	virtual void CalcGravityPow(void);
};