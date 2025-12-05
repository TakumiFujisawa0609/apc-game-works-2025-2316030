#pragma once
#include "../ObjectBase/ActorBase.h"
#include "../Enemy/AStar/FieldImpactMap.h"

class Player;
class EnemyBase;
class HandLight;
class PatrolNode;
class PatrolPath;
class NavGridManager;
class LightRenderer;

class Stage :
    public ActorBase
{
public:
	Stage(Player& player, EnemyBase& enemyBase);
	~Stage(void);
	void Init(void) override;
	void Update(float deltaTime) override;
	void OnUpdate(float deltaTime) override;
	void Draw(void) override;

	// 使用するハンドライトを設定する
	void SetCurrentHandLight(std::shared_ptr<HandLight> light);

	// 巡回用のパスを取得する
	const std::shared_ptr<PatrolPath>& GetPatrolPath(const size_t& index) const;

	// A*経路探索の取得
	std::shared_ptr<NavGridManager> GetNavGridMananger(void);

	// エリアマップの取得
	std::shared_ptr<FieldImpactMap> GetFieldImpactMap(void);

	// モデル描画用
	void InitRenderer(void);

private:

	// モデル描画用
	std::unique_ptr<LightRenderer> renderer_;

	// プレイヤーの参照
	Player& player_;

	// 敵の参照
	EnemyBase& eBase_;
	
	// ここでは循環参照を防ぐためにweak_ptrを使ってハンドライトを定義する
	std::weak_ptr<HandLight> handLight_;

	// 巡回用のノード
	std::vector<PatrolNode> nodes_;

	// 巡回用のパス
	std::vector<std::shared_ptr<PatrolPath>> paths_;

	// 障害物として認識されるもの
	std::vector<Transform> obstacles_;

	// 巡回情報の初期化
	void InitPatrolInfo(void);

	// 障害物の初期化
	void InitObstacles(void);

	// エリア情報の初期化
	void InitAreaInfo(void);

	// A*経路探索
	std::shared_ptr<NavGridManager> navManager_;

	// エリアマップの取得
	std::shared_ptr<FieldImpactMap> fieldImpactMap_;
};

