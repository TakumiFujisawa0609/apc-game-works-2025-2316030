#pragma once
#include "../Charactor.h"

class Collider;
class Capsule;

class EnemyMoveComponent;
class EnemyChaseComponent;

class EnemyBase :
    public Charactor
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

    // 移動中に障害物として認識される物を設定する
    void SetObstacle(std::vector<std::shared_ptr<Transform>> obstacle);

protected:

    // モデル情報初期化
    virtual void InitModel(VECTOR pos, VECTOR scl, VECTOR quaRotLocal);

    // 視界内にプレイヤーが入っているか
    bool isPlayerInSight_;

    // 音を検知したかどうか
    bool isHearingSound_;

    EnemyMoveComponent* moveComponent_;     // 移動
    EnemyChaseComponent* chaseComponent_;   // 追跡

private:

    // 回転処理
    void Rotate(void);

    void Collision(void);               // 衝突判定
    void CollisionCapsule(void);        // カプセルとの衝突判定
    void CollisionGravity(void);        // 重力との衝突判定
    void CalcGravityPow(void);          // 重力加算処理

};

