#pragma once
#include "../Charactor.h"

// 入力系
class OxygenComponent;
class PlayerInput;

//// インベントリ
//class Inventory;

// 描画系
class UIComponent;

class Collider;
class Capsule;

class Player 
	: 
	public Charactor
{
public:

	static constexpr float MOVE_WALK_SPEED = 5.0f;				// 歩きスピード
	static constexpr float MOVE_RUN_SPEED = 10.0f;				// 走りスピード
	static constexpr float MOUSE_SENSITIVITY = 0.002f;			// マウス感度

	Player(void);
	~Player(void);

	void Init(void) override;
	void InitComponents(void) override;
	void Update(float deltaTime) override;
	void OnUpdate(float deltaTime) override;
	void Draw(void) override;

	// 酸素コンポーネントの取得
	OxygenComponent* GetOxygenComp();

	// 入力コンポーネントの取得
	PlayerInput* GetInputComp();

	// アイテムの取得条件
	bool TakeItem(int itemId, int count);

	int GetWalkSH(void);

	int GetRunSH(void);

private:

	OxygenComponent* oxygen_;		// 酸素コンポーネント
	PlayerInput* input_;			// 入力コンポーネント

	float yaw;			// 水平回転（ヨー）
	float pitch;		// 垂直回転（ピッチ）

	int walkSH_;
	bool isWalkSH_;
	int runSH_;
	bool isRunSH_;

	void SetGoalRotate(float rotRad);

	// アイテムをインベントリに追加
	void GiveItem(int itemId, int count);

	virtual void Collision(void);
	virtual void CollisionCapsule(void);
	virtual void CollisionGravity(void);
	virtual void CalcGravityPow(void);
};

