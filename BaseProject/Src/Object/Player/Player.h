#pragma once
#include "../ActorBase.h"

// 入力系
class OxygenComponent;
class PlayerInput;

// 描画系
class UIComponent;

class Player : public ActorBase
{
public:

	// マウス感度
	static constexpr float MOUSE_SENSITIVITY = 0.002f;

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

private:
	OxygenComponent* oxygen_;	// 酸素コンポーネント
	PlayerInput* input_;			// 入力コンポーネント

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

	void Rotate(void);
	void SetGoalRotate(float rotRad);
};

