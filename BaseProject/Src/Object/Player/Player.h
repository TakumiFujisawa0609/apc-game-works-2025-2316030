#pragma once
#include "../ActorBase.h"

class OxygenComponent;
class PlayerInput;

class Player : public ActorBase
{
public:

	// マウス感度
	static constexpr float MOUSE_SENSITIVITY = 0.002f;

	Player(void);
	virtual ~Player(void);
	virtual void Init(void) override;
	virtual void Update(float deltaTime) override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void Draw(void) override;

	// 酸素コンポーネントの取得
	OxygenComponent* GetOxygenComp();
	// 入力コンポーネントの取得
	PlayerInput* GetInputComp();

private:
	OxygenComponent* oxygen_;	// 酸素コンポーネント
	PlayerInput* input_;			// 入力コンポーネント

	float moveSpeed_;	// 移動速度
	float yaw;			// 水平回転（ヨー）
	float pitch;		// 垂直回転（ピッチ）

};

