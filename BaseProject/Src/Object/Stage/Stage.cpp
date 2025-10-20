#include <stdexcept>
#include <DxLib.h>
#include "../../Utility/AsoUtility.h"
#include "../../Manager/ResourceManager.h"
#include "../Common/Collider.h"
#include "../Common/Transform.h"
#include "../Player/Player.h"
#include "../Enemy/EnemyBase.h"
#include "../Enemy/Patrol/PatrolNode.h"
#include "../Enemy/Patrol/PatrolPath.h"
#include "Stage.h"


Stage::Stage(Player& player, EnemyBase& enemyBase)
	:
	player_(player),
	eBase_(enemyBase)
{
}

Stage::~Stage(void)
{
}

void Stage::Init(void)
{
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::STAGE));
	transform_.scl = AsoUtility::VECTOR_ONE;
	transform_.pos = { 0.0f,0.0f,0.0f };
	transform_.quaRot = Quaternion();

	transform_.MakeCollider(Collider::TYPE::STAGE);
	
	transform_.Update();


	//InitPatrolInfo();
}

void Stage::Update(float deltaTime)
{
	player_.ClearCollider();
	player_.AddCollider(transform_.collider);


	eBase_.ClearCollider();
	eBase_.AddCollider(transform_.collider);
}

void Stage::OnUpdate(float deltaTime)
{
}

void Stage::Draw(void)
{
	MV1DrawModel(transform_.modelId);

	//for (int i = 1; i < nodes_.size(); i++)
	//{
	//	nodes_[i].DebugDraw();
	//}
}

const std::shared_ptr<PatrolPath>& Stage::GetPatrolPath(size_t index) const
{
	return paths_.at(index);
}

void Stage::InitPatrolInfo(void)
{
	// ÉmÅ[Éhè„ÇÃèâä˙âª
	nodes_.emplace_back(VECTOR{ 100.0f,0.0f,50.0f }, 2.0f, PatrolNode::ACTTYPE::NONE);
	nodes_.emplace_back(VECTOR{ 500.0f,0.0f,50.0f }, 2.0f, PatrolNode::ACTTYPE::NONE);
	nodes_.emplace_back(VECTOR{ 500.0f,0.0f,300.0f }, 2.0f, PatrolNode::ACTTYPE::NONE);
	nodes_.emplace_back(VECTOR{ 500.0f,0.0f,600.0f }, 2.0f, PatrolNode::ACTTYPE::NONE);

	paths_[1] = std::make_shared<PatrolPath>(nodes_, PatrolPath::PATHTYPE::LOOP);

}
