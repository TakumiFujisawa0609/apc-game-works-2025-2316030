#pragma once

#include "ActorBase.h"
#include "../Components/Charactor/CharactorComponent.h"

//class CharactorComponent;

class Charactor :
	public ActorBase
{
public:

	// 回転完了するまでの時間
	static constexpr float TIME_ROT = 1.0f;		

	Charactor(void);
	virtual ~Charactor();

	virtual void Init(void) = 0;
	
	// コンポーネント初期化
	virtual void InitComponents(void) = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Draw(void) = 0;

	// コライダ追加
	virtual void AddCollider(std::shared_ptr<Collider> collider);
	
	// コライダ全削除
	virtual void ClearCollider(void);								

	// 衝突判定用カプセルの初期化
	void InitializeCapsule(VECTOR top, VECTOR down, float radius);

	// 衝突用カプセルの取得
	virtual const std::shared_ptr<Capsule> GetCapsule(void) const;

	std::shared_ptr<Charactor> shared_from_this()
	{
		return std::static_pointer_cast<Charactor>(ActorBase::shared_from_this());
	}

	std::shared_ptr<const Charactor> shared_from_this() const
	{
		return std::static_pointer_cast<const Charactor>(ActorBase::shared_from_this());
	}

	// コンポーネントの追加
	template<typename T, typename... Args>
	T* AddCharactorComponent(Args&&... args)
	{
		std::shared_ptr<Charactor> ownerPtr = shared_from_this();
		auto comp = std::make_unique<T>(ownerPtr, std::forward<Args>(args)...);
		T* ptr = comp.get();
		charaComponents_.emplace_back(std::move(comp));
		return ptr;
	}

protected:

	// 移動
	// 移動速度
	float moveSpeed_;

	// 移動方向
	VECTOR moveDir_;
	
	// 移動量
	VECTOR movePow_;

	// 移動後座標
	VECTOR movedPos_;

	// 回転
	// 回転の始点
	Quaternion startRotY_;

	// 回転の終点
	Quaternion goalQuaRot_;

	// 回転時間
	float stepRotTime_;

	// 衝突判定に用いられるコライダ
	std::vector<std::shared_ptr<Collider>> colliders_;

	// プレイヤー当たり判定カプセル
	std::shared_ptr<Capsule> capsule_;

	// 下方向の接地判定座標
	VECTOR gravHitPosDown_;

	// 上方向の接地判定座標
	VECTOR gravHitPosUp_;

	// y軸方向の変化移動量
	VECTOR velocityY_;

	// y方向に移動中か
	bool isVelocityY_;

	// 派生クラスで独自処理
	virtual void OnUpdate(float deltaTime) = 0;

	// 回転処理
	virtual void Rotate(void);

	// 衝突判定
	virtual void Collision(void);

	// カプセルとの衝突判定
	virtual void CollisionCapsule(void);

	// 重力との衝突判定
	virtual void CollisionGravity(void);

	// 重力加算処理
	virtual void CalcGravityPow(void);


private:

	// キャラクターコンポーネント群
	std::vector<std::unique_ptr<CharactorComponent>> charaComponents_;
};

