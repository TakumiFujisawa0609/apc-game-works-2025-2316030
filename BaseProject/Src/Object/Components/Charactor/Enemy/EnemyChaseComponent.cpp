#include "../../../../Utility/AsoUtility.h"
#include "../../../Player/Player.h"
#include "EnemyChaseComponent.h"
#include "../../../Enemy/EnemyBase.h"

EnemyChaseComponent::EnemyChaseComponent(std::shared_ptr<Charactor> owner, Player& player)
    :
    EnemyComponent(owner, player),
    currentPathIndex_(0),
    pathRecalcTimer_(0.0f)
{
}

EnemyChaseComponent::~EnemyChaseComponent(void)
{
}

void EnemyChaseComponent::Update(float deltaTime)
{

    CapsuleCast(deltaTime);

}

void EnemyChaseComponent::Chase(float deltaTime,
    Transform& transform,
    VECTOR& moveDir,
    VECTOR& movePow,
    float moveSpeed,
    Quaternion& outRotation)
{
    // A*の再計算タイミング
    pathRecalcTimer_ -= deltaTime;
    if (pathRecalcTimer_ <= 0.0f)
    {
        // プレイヤーの位置をゴールとして新しい経路を計算
        currentPath_ = FindPath(transform.pos, player_.GetTransform().pos);
        currentPathIndex_ = 0;
        pathRecalcTimer_ = 0.5f; // 0.5秒ごとに再計算
    }

    if (currentPath_.empty() || currentPathIndex_ >= currentPath_.size())
    {
        moveDir = AsoUtility::VECTOR_ZERO; // 経路がない/終了
        return;
    }

    // ----------------------------------------------------
    // 移動ロジック
    // ----------------------------------------------------
    VECTOR targetPos = currentPath_[currentPathIndex_];

    // Y軸を無視した移動計算
    VECTOR currentPos = transform.pos;
    currentPos.y = 0.0f;
    targetPos.y = 0.0f;

    VECTOR moveVector = VSub(targetPos, currentPos);
    float distance = VSize(moveVector);

    // ノードに到達したら次のノードへ
    if (distance < 5.0f) // 許容誤差を少し大きめに
    {
        currentPathIndex_++;
        // 最後のノード（プレイヤー位置）に到達したら停止
        if (currentPathIndex_ >= currentPath_.size())
        {
            moveDir = AsoUtility::VECTOR_ZERO;
            return;
        }
        // 次のノードを目標に再設定
        targetPos = currentPath_[currentPathIndex_];

        // 再度 moveVector を計算するロジックが必要だが、簡略化のためスキップ
        // 複雑になるため、ここでは次のフレームで再度計算されると仮定
    }

    // 移動方向を設定
    VECTOR moveDirection = VNorm(VSub(targetPos, currentPos));
    moveDir = moveDirection;

    movePow = VScale(moveDir, 4.0f);

    // 4. 回転処理
    // 移動方向を向く回転角度を計算
    float rotationY = atan2f(moveDirection.x, moveDirection.z);
    Quaternion targetRotation = Quaternion::Euler({ 0.0f, rotationY, 0.0f });

    // 滑らかに補間
    outRotation = Quaternion::Slerp(transform.quaRot, targetRotation, 5.0f * deltaTime);

    CapsuleCast(deltaTime);
}

void EnemyChaseComponent::SetObstacle(std::vector<Transform> obstacle)
{
    obstacle_ = obstacle;
}

void EnemyChaseComponent::SetCurrentPath(std::vector<VECTOR> currentPath)
{
}

void EnemyChaseComponent::CapsuleCast(float deltaTime)
{

}

VECTOR EnemyChaseComponent::CalculateAvoidanceForce(const Transform& transform, float lookAheadDistance, float deltaTime)
{
    // 現在の進行方向を取得（クォータニオンから前方ベクトルを計算）
    //VECTOR forward = transform.quaRot.ToForwardVector();
    VECTOR forward = transform.quaRot.GetForward();
    forward.y = 0.0f; // 水平方向のみ考慮
    forward = VNorm(forward);

    // 敵の予測される将来の位置 (レイの終点)
    VECTOR ahead = VAdd(transform.pos, VScale(forward, lookAheadDistance));

    // 敵の正面方向の障害物をチェックするためのレイキャスト
    VECTOR avoidanceForce = AsoUtility::VECTOR_ZERO;
    float maxAvoidanceDist = 0.0f;

    // シンプルな円形/球形の障害物回避の例:
    for (const auto& obstacle : obstacle_)
    {
        //if (!obstacle) continue;

        VECTOR obsPos = obstacle.pos;
        obsPos.y = transform.pos.y; // Y軸を合わせる

        VECTOR diff = VSub(ahead, obsPos);
        float distance = VSize(diff);

        // 障害物の半径 + 敵の回避余裕 (ここでは仮に 1.5f を障害物のサイズとして利用)
        const float collisionRadius = 1.5f + 1.0f; // 例: 障害物のサイズ + 敵の回避半径

        if (distance < collisionRadius)
        {
            // 衝突が近づいている: 障害物から離れる方向の力を計算
            VECTOR awayFromObstacle = VSub(transform.pos, obsPos);
            awayFromObstacle.y = 0.0f;

            // 距離が近いほど力が強くなるように正規化し、反比例させる
            // 距離が0に近づくと力が無限大になるのを避けるため、一定値でクリップ
            // 修正後のコード:
            float strength = (collisionRadius - distance) / collisionRadius;
            if (strength < 0.0f) {
                strength = 0.0f;
            }

            VECTOR desiredVelocity = VScale(VNorm(awayFromObstacle), strength * 10.0f); // 10.0fは回避の強さ

            // 複数の障害物がある場合は、最も回避の必要な力を合成
            avoidanceForce = VAdd(avoidanceForce, desiredVelocity);

            // レイキャストを使用する場合は、衝突点と法線ベクトルから正確な回避方向を計算します。
        }
    }

    return avoidanceForce; // 回避するための「力」ベクトルを返す
}
