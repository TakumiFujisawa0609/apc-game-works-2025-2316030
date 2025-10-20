#include "../Object/ObjectBase/ActorBase.h"
#include "../../../Common/Transform.h"
#include "../../../Player/Player.h"
#include "../../../Enemy/Patrol/PatrolPath.h"
#include "../../../Enemy/Patrol/PatrolNode.h"
#include "EnemyMoveComponent.h"

EnemyMoveComponent::EnemyMoveComponent(std::shared_ptr<Charactor> owner, Player& player)
	:
	EnemyComponent(owner, player)
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

void EnemyMoveComponent::Patrol(std::shared_ptr<PatrolPath> path, int& currentIndex, float deltaTime)
{
	std::shared_ptr<Charactor> owner = this->GetCharactor().lock();
	if (!owner || !path)return;

	// 現在の目標ノードを取得
	const PatrolNode& targetNode = path->GetNodeIndex(currentIndex);
	VECTOR targetPos = targetNode.GetPos();

	// 現在位置から目標位置への移動を計算
	VECTOR currentPos = owner->GetTransform().pos;
	VECTOR moveVector = VSub(targetPos, currentPos);

	// 目標位置に到達したかをチェック
	if (VSize(moveVector) < 1.0f)
	{
		// 目標に到達したら次のノードを決定
		const PatrolNode& nextNode = path->GetNextNode(currentIndex);

		// ノード固有のアクションがれば実行

	}
	else
	{
		// 移動処理


		// モデルの回転処理

	}

}
