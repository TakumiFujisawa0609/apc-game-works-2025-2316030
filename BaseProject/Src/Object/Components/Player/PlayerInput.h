#pragma once
#include "../../Common/Component.h"
class PlayerInput :
    public Component
{
public:
	PlayerInput(std::shared_ptr<ActorBase> owner);
	virtual ~PlayerInput(void);
	virtual void Update(float deltaTime) override;

	// 前後移動の取得
	float GetMoveForward(void) const;

	// 左右移動の取得
	float GetMoveRight(void) const;
	
	// マウス水平移動の取得
	float GetMouseDeltaX(void) const;
	
	// マウス垂直移動の取得
	float GetMouseDeltaY(void) const;

private:
	float moveForward_;	// 前後移動
	float moveRight_;	// 左右移動
	float mouseDeltaX_; // マウス水平移動
	float mouseDeltaY_; // マウス垂直移動
};

