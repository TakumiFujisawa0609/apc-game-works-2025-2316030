#include <stdexcept>
#include <DxLib.h>
#include "../../Application.h"
#include "../../Utility/AsoUtility.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Camera.h"
#include "../Common/Collider.h"
#include "../Common/Transform.h"
#include "../Player/Player.h"
#include "../Enemy/EnemyBase.h"
#include "../Object/Item/PermanentItems/HandLight.h"
#include "../Enemy/Patrol/PatrolNode.h"
#include "../Enemy/Patrol/PatrolPath.h"
#include "../Enemy/AStar/NavGridManager.h"
#include "../Renderer/ModelMaterial.h"
#include "../Renderer/ModelRenderer.h"
#include "Stage.h"

Stage::Stage(Player& player, EnemyBase& enemyBase)
	:
	player_(player),
	eBase_(enemyBase),
	obstacles_{}
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
	transform_.quaRotLocal = Quaternion::Euler({ AsoUtility::Deg2RadF(0.0f),
		AsoUtility::Deg2RadF(0.0f),AsoUtility::Deg2RadF(0.0f) });;

	transform_.MakeCollider(Collider::TYPE::STAGE);

	transform_.Update();

	InitObstacles();
	InitPatrolInfo();

}

void Stage::Update(float deltaTime)
{
	player_.ClearCollider();
	player_.AddCollider(transform_.collider);


	eBase_.ClearCollider();
	eBase_.AddCollider(transform_.collider);

	// 障害物設定
	eBase_.SetObstacle(obstacles_);
	//eBase_.SetNavGridManagedr(navManager_);


	OnUpdate(deltaTime);
}

void Stage::OnUpdate(float deltaTime)
{
	//handLight_.UpdateRenderer(deltaTime);
	handLight_.lock()->UpdateRenderer(deltaTime);
}

void Stage::Draw(void)
{

	//MV1DrawModel(transform_.modelId);
	
	handLight_.lock()->DrawRenderer();

#ifdef _DEBUG

	//if (!paths_.empty())
	//{
	//	const std::shared_ptr<PatrolPath>& path = paths_[0];
	//	const auto& nodes = path->GetNodes();

	//	VECTOR prevPos = { 0.0f, 0.0f, 0.0f };
	//	bool firstNode = true;

	//	for (size_t i = 0; i < nodes.size(); ++i)
	//	{
	//		const PatrolNode& node = nodes[i];
	//		VECTOR currentPos = node.GetPos();

	//		// 1. ノード自体のデバッグ描画 (球体など)
	//		node.DebugDraw();

	//		// 2. ノード間のパスの描画 (線)
	//		if (!firstNode)
	//		{
	//			// ノード間を線で結ぶ
	//			DrawLine3D(prevPos, currentPos, GetColor(255, 0, 0)); // 赤線
	//		}

	//		// 3. 次の描画のために現在のノード位置を保存
	//		prevPos = currentPos;
	//		firstNode = false;
	//	}

	//	// パスタイプがループの場合、最後のノードと最初のノードを結ぶ
	//	if (paths_[0]->GetPathType() == PatrolPath::PATHTYPE::LOOP && nodes.size() > 1)
	//	{
	//		VECTOR firstPos = nodes[0].GetPos();
	//		DrawLine3D(prevPos, firstPos, GetColor(255, 0, 0)); // 赤線
	//	}
	//}

	//Transform hlt = handLight_.GetTransform();
	//VECTOR forward = hlt.quaRot.GetForward();
	//VECTOR dir = VNorm(forward);

	//auto& windowSize_ = Application::GetInstance().GetWindowSize();
	//DrawFormatString(0, 32, GetColor(255, 255, 255), L"hPos = (%.2f,%.2f,%.2f)", hlt.pos.x, hlt.pos.y, hlt.pos.z);
	//DrawFormatString(0, 48, GetColor(255, 255, 255), L"spPos = (%.2f,%.2f,%.2f)", GetLightPosition().x, GetLightPosition().y, GetLightPosition().z);

	//DrawFormatString(0, 64, GetColor(255, 255, 255), L"hdir = (%.2f,%.2f,%.2f)", dir.x, dir.y, dir.z);
	//DrawFormatString(0, 80, GetColor(255, 255, 255), L"spdir = (%.2f,%.2f,%.2f)", GetLightDirection().x, GetLightDirection().y, GetLightDirection().z);


#endif // _DEBUGe



}

void Stage::SetCurrentHandLight(std::shared_ptr<HandLight> light)
{
	handLight_ = light;
}

const std::shared_ptr<PatrolPath>& Stage::GetPatrolPath(const size_t& index) const
{
	return paths_.at(index);
}

std::shared_ptr<NavGridManager> Stage::GetNavGridMananger(void)
{
	return navManager_;
}

void Stage::InitPatrolInfo(void)
{
	// ノード上の初期化
	nodes_.emplace_back(VECTOR{ 100.0f,180.0f,50.0f }, 1.0f, PatrolNode::ACTTYPE::NONE);
	nodes_.emplace_back(VECTOR{ 500.0f,180.0f,50.0f }, 1.0f, PatrolNode::ACTTYPE::NONE);
	nodes_.emplace_back(VECTOR{ 900.0f,180.0f,50.0f }, 1.0f, PatrolNode::ACTTYPE::NONE);
	nodes_.emplace_back(VECTOR{ 900.0f,180.0f,-500.0f }, 1.0f, PatrolNode::ACTTYPE::NONE);
	nodes_.emplace_back(VECTOR{ 900.0f,180.0f,-900.0f }, 1.0f, PatrolNode::ACTTYPE::NONE);
	nodes_.emplace_back(VECTOR{ 1500.0f,180.0f,-900.0f }, 1.0f, PatrolNode::ACTTYPE::NONE);
	nodes_.emplace_back(VECTOR{ 1700.0f,180.0f,-1500.0f }, 1.0f, PatrolNode::ACTTYPE::NONE);

	// 巡回パスを作成
	paths_.emplace_back(std::make_shared<PatrolPath>(nodes_, PatrolPath::PATHTYPE::ROUNDTRIP));
	
	// 敵に巡回パスを設定
	// paths_の0番目のパスを設定
	if (!paths_.empty())
	{
		eBase_.SetPatrolPath(paths_[0]);
	}

	navManager_ = std::make_shared<NavGridManager>();
	navManager_->InitGrid(100, 100, 200);
	//navManager_->CheckObstacles(obstacles_);
}

void Stage::InitObstacles(void)
{
	obstacles_.push_back(transform_);
}

void Stage::InitRenderer(void)
{
	//Transform hlt = handLight_.GetTransform();
	Transform hlt = handLight_.lock()->GetTransform();
	VECTOR forward = hlt.quaRot.GetForward();
	VECTOR dir = VNorm(forward);

	//handLight_.InitLightRenderer(HandLight::TYPE::REGIDBODY, transform_.modelId);
	handLight_.lock()->InitLightRenderer(HandLight::TYPE::REGIDBODY, transform_.modelId);
}
