#pragma once
#include "../ActorBase.h"

class Collider;
class Capsule;

class EnemyBase :
    public ActorBase
{
public:

    // 回転完了するまでの時間
    static constexpr float TIME_ROT = 1.0f;

    EnemyBase(void);
    ~EnemyBase(void);

    void Init(void) override;
    void Update(float deltaTime) override;
    void OnUpdate(float deltaTime) override;
    void Draw(void) override;

    void AddCollider(std::shared_ptr<Collider> collider);	// コライダ追加
    void ClearCollider(void);								// コライダ全削除

    const std::shared_ptr<Capsule> GetCapsule(void) const;	// 衝突用カプセルの取得

protected:

private:

    float moveSpeed_;	    // 移動速度
    VECTOR moveDir_;		// 移動方向
    VECTOR movePow_;		// 移動量
    VECTOR movedPos_;		// 移動後座標

    // 回転
    Quaternion startRotY_;
    Quaternion goalQuaRot_;
    float stepRotTime_;

    std::vector<std::shared_ptr<Collider>> colliders_;		// 衝突判定に用いられるコライダ
    std::shared_ptr<Capsule> capsule_;						// プレイヤー当たり判定カプセル

    VECTOR gravHitPosDown_;				// 下方向の接地判定座標
    VECTOR gravHitPosUp_;				// 上方向の接地判定座標

    // ジャンプ量
    VECTOR jumpPow_;

    // ジャンプ状態であるか
    bool isJump_;

    // 回転処理
    void Rotate(void);

    void Collision(void);               // 衝突判定
    void CollisionCapsule(void);        // カプセルとの衝突判定
    void CollisionGravity(void);        // 重力との衝突判定
    void CalcGravityPow(void);          // 重力加算処理

};

