#pragma once
#include "../ActorBase.h"

// 入力系
class OxygenComponent;
class PlayerInput;

//// インベントリ
//class Inventory;

// 描画系
class UIComponent;

class Collider;
class Capsule;

class Player : public ActorBase
{
public:

	static constexpr float MOVE_WALK_SPEED = 5.0f;				// 歩きスピード
	static constexpr float MOVE_RUN_SPEED = 10.0f;				// 走りスピード
	static constexpr float MOUSE_SENSITIVITY = 0.002f;			// マウス感度

	Player(void);
	~Player(void);

	void Init(void) override;
	void Update(float deltaTime) override;
	void OnUpdate(float deltaTime) override;
	void Draw(void) override;

	// 酸素コンポーネントの取得
	OxygenComponent* GetOxygenComp();

	// 入力コンポーネントの取得
	PlayerInput* GetInputComp();

	// アイテムの取得条件
	bool TakeItem(int itemId, int count);

	void AddCollider(std::shared_ptr<Collider> collider);	// コライダ追加
	void ClearCollider(void);								// コライダ全削除

	const std::shared_ptr<Capsule> GetCapsule(void) const;	// 衝突用カプセルの取得


private:

	OxygenComponent* oxygen_;		// 酸素コンポーネント
	PlayerInput* input_;			// 入力コンポーネント

	//std::shared_ptr<Inventory> inventory_;		// インベントリ

	float moveSpeed_;	// 移動速度
	VECTOR moveDir_;		// 移動方向
	VECTOR movePow_;		// 移動量
	VECTOR movedPos_;		// 移動後座標

	// 回転
	Quaternion playerRotY_;
	Quaternion goalQuaRot_;
	float stepRotTime_;

	float yaw;			// 水平回転（ヨー）
	float pitch;		// 垂直回転（ピッチ）

	void SetGoalRotate(float rotRad);

	// アイテムをインベントリに追加
	void GiveItem(int itemId, int count);

	std::vector<std::shared_ptr<Collider>> colliders_;		// 衝突判定に用いられるコライダ
	std::shared_ptr<Capsule> capsule_;						// プレイヤー当たり判定カプセル

	VECTOR gravHitPosDown_;				// 下方向の接地判定座標
	VECTOR gravHitPosUp_;				// 上方向の接地判定座標

	// ジャンプ量
	VECTOR jumpPow_;

	// ジャンプ状態であるか
	bool isJump_;

	void Collision(void);
	void CollisionCapsule(void);
	void CollisionGravity(void);
	void CalcGravityPow(void);
};

