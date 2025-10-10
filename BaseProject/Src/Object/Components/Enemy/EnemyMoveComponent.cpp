#include "../../ActorBase.h"
#include "../../Common/Transform.h"
#include "EnemyMoveComponent.h"

EnemyMoveComponent::EnemyMoveComponent(std::shared_ptr<Charactor> owner)
	:
	CharactorComponent(owner)
{
}

EnemyMoveComponent::~EnemyMoveComponent(void)
{
}

void EnemyMoveComponent::Update(float deltaTime)
{

	// 敵ベースを取得
	std::shared_ptr<ActorBase> owner = this->GetOwner().lock();

	if (!owner)
	{
		return;
	}

	Transform transform = owner->GetTransform();

	// 回転(モデルが移動している方向を向かせる)


	// 当たり判定


	// 重力計算


	// 移動処理


}
