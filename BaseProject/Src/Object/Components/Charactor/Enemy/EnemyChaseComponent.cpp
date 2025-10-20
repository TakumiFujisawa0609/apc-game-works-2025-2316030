#include "../../../Player/Player.h"
#include "EnemyChaseComponent.h"

EnemyChaseComponent::EnemyChaseComponent(std::shared_ptr<Charactor> owner, Player& player)
    :
    EnemyComponent(owner, player)
{
}

EnemyChaseComponent::~EnemyChaseComponent(void)
{
}

void EnemyChaseComponent::Update(float deltaTime)
{

    CapsuleCast(deltaTime);

}

void EnemyChaseComponent::Chase(float deltaTime)
{
    // 敵とプレイヤーの位置を取得
    std::shared_ptr<Charactor> owner = this->GetCharactor().lock();
    if (!owner)return;

    VECTOR playerPos = player_.GetTransform().pos;

    // 経路探索や障害物回避の計算

    // プレイヤーへの移動ベクトルを計算
    VECTOR moveVector = VSub(playerPos, owner->GetTransform().pos);

    // 移動処理

    // モデルの回転処理

    // 追跡中の視線・当たり判定

    CapsuleCast(deltaTime);
}

void EnemyChaseComponent::SetObstacle(std::vector<std::shared_ptr<Transform>> obstacle)
{
    obstacle_ = obstacle;
}

std::vector<std::shared_ptr<Transform>> EnemyChaseComponent::GetObstacle(void)
{
    return obstacle_;
}

void EnemyChaseComponent::CapsuleCast(float deltaTime)
{

}
