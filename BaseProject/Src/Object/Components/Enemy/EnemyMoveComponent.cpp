#include "../../ActorBase.h"
#include "EnemyMoveComponent.h"

EnemyMoveComponent::EnemyMoveComponent(std::shared_ptr<ActorBase> owner)
	:
	Component(owner)
{
}

EnemyMoveComponent::~EnemyMoveComponent(void)
{
}

void EnemyMoveComponent::Update(float deltaTime)
{

	// 回転(モデルが移動している方向を向かせる)


	// 当たり判定


	// 重力計算


	// 移動処理


	// モデル情報の更新


}
