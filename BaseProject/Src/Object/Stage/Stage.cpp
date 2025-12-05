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
#include "../Renderer/LightRenderer.h"
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
	InitAreaInfo();

	// 敵にエリアマップを設定
	if (fieldImpactMap_) {
		eBase_.SetFieldImpactMap(fieldImpactMap_);
	}
}

void Stage::Update(float deltaTime)
{
	// 当たり判定の設定

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
	// ライトシェーダ―の更新バッファ
	renderer_->UpdateRenderer(deltaTime, handLight_.lock().get()->IsActive());
}

void Stage::Draw(void)
{
	renderer_->DrawRenderer();

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

std::shared_ptr<FieldImpactMap> Stage::GetFieldImpactMap(void)
{
	return fieldImpactMap_;
}

void Stage::InitPatrolInfo(void)
{
	// ノード上の初期化
	nodes_.emplace_back(VECTOR{ -401.0f,120.0f,3853.0f }, 1.0f, PatrolNode::ACTTYPE::NONE);
	nodes_.emplace_back(VECTOR{ -458.0f,120.0f,4683.0f }, 1.0f, PatrolNode::ACTTYPE::NONE);
	nodes_.emplace_back(VECTOR{ -1737.0f,120.0f,4711.0f }, 1.0f, PatrolNode::ACTTYPE::NONE);
	nodes_.emplace_back(VECTOR{ -2030.0f,120.0f,4371.0f }, 1.0f, PatrolNode::ACTTYPE::NONE);
	nodes_.emplace_back(VECTOR{ -1969.0f,120.0f,3971.0f }, 1.0f, PatrolNode::ACTTYPE::NONE);
	nodes_.emplace_back(VECTOR{ -1960.0f,120.0f,3167.0f }, 1.0f, PatrolNode::ACTTYPE::NONE);
	nodes_.emplace_back(VECTOR{ -2000.0f,120.0f,2589.0f }, 1.0f, PatrolNode::ACTTYPE::NONE);
	nodes_.emplace_back(VECTOR{ -2800.0f,120.0f,2589.0f }, 1.0f, PatrolNode::ACTTYPE::NONE);
	nodes_.emplace_back(VECTOR{ -3570.0f,120.0f,2589.0f }, 1.0f, PatrolNode::ACTTYPE::NONE);
	nodes_.emplace_back(VECTOR{ -3900.0f,120.0f,2589.0f }, 1.0f, PatrolNode::ACTTYPE::NONE);
	nodes_.emplace_back(VECTOR{ -3900.0f,120.0f,2424.0f }, 1.0f, PatrolNode::ACTTYPE::NONE);
	nodes_.emplace_back(VECTOR{ -3570.0f,120.0f,2424.0f }, 1.0f, PatrolNode::ACTTYPE::NONE);
	nodes_.emplace_back(VECTOR{ -2784.0f,120.0f,2424.0f }, 1.0f, PatrolNode::ACTTYPE::NONE);
	nodes_.emplace_back(VECTOR{ -2813.0f,120.0f,1840.0f }, 1.0f, PatrolNode::ACTTYPE::NONE);
	nodes_.emplace_back(VECTOR{ -2830.0f,120.0f,1114.0f }, 1.0f, PatrolNode::ACTTYPE::NONE);
	nodes_.emplace_back(VECTOR{ -2243.0f,120.0f,1076.0f }, 1.0f, PatrolNode::ACTTYPE::NONE);
	nodes_.emplace_back(VECTOR{ -1794.0f,120.0f,1076.0f }, 1.0f, PatrolNode::ACTTYPE::NONE);
	nodes_.emplace_back(VECTOR{ -1788.0f,120.0f,567.0f }, 1.0f, PatrolNode::ACTTYPE::NONE);
	nodes_.emplace_back(VECTOR{ -1779.0f,120.0f,125.0f }, 1.0f, PatrolNode::ACTTYPE::NONE);
	nodes_.emplace_back(VECTOR{ -1770.0f,120.0f,-251.0f }, 1.0f, PatrolNode::ACTTYPE::NONE);

	// 巡回パスを作成
	paths_.emplace_back(std::make_shared<PatrolPath>(nodes_, PatrolPath::PATHTYPE::ROUNDTRIP));
	
	// 敵に巡回パスを設定
	// paths_の0番目のパスを設定
	if (!paths_.empty()){
		eBase_.SetPatrolPath(paths_[0]);
	}

	navManager_ = std::make_shared<NavGridManager>();
	navManager_->InitGrid(100, 100, 200);

	// 障害物判定の設定
	//navManager_->CheckObstacles(obstacles_);
}

void Stage::InitObstacles(void)
{
	obstacles_.push_back(transform_);
}

void Stage::InitAreaInfo(void)
{
	fieldImpactMap_ = std::make_shared<FieldImpactMap>();

	// 仮想的なエリアの定義 (Stage::InitPatrolInfo のノード座標からエリアを推測)
	// 例えば、ノード 1-4 が Area 1、ノード 5-10 が Area 2、... のように分類

	// Area 1: 左上の部屋または通路
	Area area1(1, VECTOR{ -1000.0f, 120.0f, 4300.0f }, 2000.0f, 1000.0f);
	fieldImpactMap_->AddArea(area1);

	// Area 2: 右の通路
	Area area2(2, VECTOR{ -2800.0f, 120.0f, 2500.0f }, 2000.0f, 1000.0f);
	fieldImpactMap_->AddArea(area2);

	// Area 3: 下の部屋
	Area area3(3, VECTOR{ -2500.0f, 120.0f, 1000.0f }, 2000.0f, 1500.0f);
	fieldImpactMap_->AddArea(area3);

	// エリア間の接続 (ポータル) を定義
	// Area 1 <-> Area 2 の接続 (例: ノード4とノード5の間)
	VECTOR conn1_2_A = { -1969.0f, 120.0f, 3971.0f }; // Area 1 側の接続点
	VECTOR conn1_2_B = { -1960.0f, 120.0f, 3167.0f }; // Area 2 側の接続点
	fieldImpactMap_->AddConnection(1, 2, conn1_2_A, conn1_2_B);

	// Area 2 <-> Area 3 の接続 (例: ノード13とノード14の間)
	VECTOR conn2_3_A = { -2813.0f, 120.0f, 1840.0f };
	VECTOR conn2_3_B = { -2830.0f, 120.0f, 1114.0f };
	fieldImpactMap_->AddConnection(2, 3, conn2_3_A, conn2_3_B);

	// ... 他のエリアや接続点の定義 ...
}

void Stage::InitRenderer(void)
{
	renderer_ = std::make_unique<LightRenderer>();
	std::shared_ptr<HandLight> handLightPtr = handLight_.lock();
	renderer_->SetHandLight(handLightPtr.get());
	renderer_->InitLightRenderer(LightRenderer::TYPE::REGIDBODY, transform_.modelId);
}
