#pragma once

#include "ActorBase.h"
#include "Components/Charactor/CharactorComponent.h"

//class CharactorComponent;

class Charactor :
	public ActorBase,
	public std::enable_shared_from_this<Charactor>
{
public:
	static constexpr float TIME_ROT = 1.0f;		// 回転完了するまでの時間

	Charactor(void);
	virtual ~Charactor();

	virtual void Init(void) = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Draw(void) = 0;

	virtual void AddCollider(std::shared_ptr<Collider> collider);	// コライダ追加
	virtual void ClearCollider(void);								// コライダ全削除

	virtual const std::shared_ptr<Capsule> GetCapsule(void) const;	// 衝突用カプセルの取得

	// コンポーネントの追加
	template<typename T, typename... Args>
	T* AddCharactorComponent(Args&&... args)
	{
		auto chara_owner = std::enable_shared_from_this<Charactor>::shared_from_this();

		auto comp = std::make_unique<T>(chara_owner, std::forward<Args>(args)...);
		T* ptr = comp.get();
		charaComponents_.emplace_back(std::move(comp));
		return ptr;
	}

protected:

	float moveSpeed_;	    // 移動速度
	VECTOR moveDir_;		// 移動方向
	VECTOR movePow_;		// 移動量
	VECTOR movedPos_;		// 移動後座標

	// 回転
	Quaternion startRotY_;			// 回転の始点
	Quaternion goalQuaRot_;			// 回転の終点
	float stepRotTime_;				// 回転時間

	std::vector<std::shared_ptr<Collider>> colliders_;		// 衝突判定に用いられるコライダ
	std::shared_ptr<Capsule> capsule_;						// プレイヤー当たり判定カプセル

	VECTOR gravHitPosDown_;				// 下方向の接地判定座標
	VECTOR gravHitPosUp_;				// 上方向の接地判定座標

	// y軸方向の変化移動量
	VECTOR velocityY_;

	// y方向に移動中か
	bool isVelocityY_;

	// 派生クラスで独自処理
	virtual void OnUpdate(float deltaTime) = 0;

	// 回転処理
	virtual void Rotate(void);

	virtual void Collision(void);               // 衝突判定
	virtual void CollisionCapsule(void);        // カプセルとの衝突判定
	virtual void CollisionGravity(void);        // 重力との衝突判定
	virtual void CalcGravityPow(void);          // 重力加算処理


private:

	// コンポーネント群
	std::vector<std::unique_ptr<CharactorComponent>> charaComponents_;
};

