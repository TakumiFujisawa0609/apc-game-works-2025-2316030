#pragma once

#include <functional>
#include <map>
#include "ActorBase.h"
class AnimationController;
class Collider;
class Capsule;


class Submarine :
    public ActorBase
{
public:

	// 状態
	enum class STATE
	{
		NONE,
		MOVE,
		STOP,
		DESTROY
	};

	Submarine(void);
	virtual ~Submarine(void);

	virtual void Init(void) override;
	virtual void Update(float deltaTime) override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void Draw(void) override;

private:

	// アニメーション
	std::unique_ptr<AnimationController> animCtrl_;

	// 移動スピード
	float speed_;

	// 移動咆哮
	VECTOR moveDir_;

	// 移動量
	VECTOR movePow_;

	// 移動後座標
	VECTOR movedPos_;

	// 回転
	Quaternion startRotY_;
	Quaternion goalRotY_;
	float stepRotTime_;

	// 浮上量
	float ascendPow_;

	// 浮上判定
	bool isAscend_;

	// 線形運動量
	float linearMomentum_;

	// 角運動量
	float angularMomentum_;

	// 抗力
	float dragPow_;

	// 推進力
	float thrustPow_;

	// 水中での運動量における運動量の保存
	float momentumConservation_;

	// 潜水艦の姿勢制御に関する運動量
	float postureControlPow_;

	// 衝突判定に用いられるコライダ
	std::vector<std::shared_ptr<Collider>> colliders_;
	std::shared_ptr<Capsule> capsule_;

	// 衝突チェック
	VECTOR gravHitPosDown_;
	VECTOR gravHitPosUp_;

	// 回転
	void SetGoalRotate(double rotRad);
	void Rotate(void);

	// 衝突判定
	void Collision(void);

	// 状態管理
	STATE state_;

	// 状態
	std::map<STATE, std::function<void(void)>> stateMap_;

	void UpdateNone(void);
	void UpdateMove(void);
	void UpdateStop(void);
	void UpdateDestroy(void);

};

