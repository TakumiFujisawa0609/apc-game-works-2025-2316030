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

	static constexpr float MOVE_WALK_SPEED = 5.0f;				// 歩きスピード
	static constexpr float MOVE_RUN_SPEED = 10.0f;				// 走りスピード
	static constexpr float MOUSE_SENSITIVITY = 0.002f;			// マウス感度

	static constexpr float MAX_SAN_VALUE = 50.0f;
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

	void SetSanityLevel(float value);

	float GetHealthRatio(void) const;

	float GetSanityRatio(void) const;

	void SetHitPoint(float value);

	void TakeDamage(float damege);


	float GetHp(void);

	float GetSanity(void);

	void CollisionEnemy(void);

private:

	TimeLimitComponent* tLimit_;		// 酸素コンポーネント
	PlayerInput* input_;			// 入力コンポーネント

	std::weak_ptr<HandLight> light_;

	std::weak_ptr<EnemyBase> enemy_;

	float yaw;			// 水平回転（ヨー）
	float pitch;		// 垂直回転（ピッチ）

	int walkSH_;		// 歩きの音
	bool isWalkSH_;		// 歩きの音を再生するかどうか
	int runSH_;			// 走るの音
	bool isRunSH_;		// 走るの音を再生するかどうか

	float hp_;
	float sanV_;		// san値

	void SetGoalRotate(float rotRad);

	virtual void Collision(void);
	virtual void CollisionCapsule(void);
	virtual void CollisionGravity(void);
	virtual void CalcGravityPow(void);


};

