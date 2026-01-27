#pragma once
#include "../ObjectBase/ActorBase.h"

class Player;
class EnemyBase;
class HandLight;
class PatrolNode;
class PatrolPath;
class NavGridManager;
class ModelMaterial;
class ModelRenderer;

class Stage :
    public ActorBase
{
public:
	Stage(Player& player, EnemyBase& enemyBase);
	virtual ~Stage(void);
	virtual void Init(void) override;
	virtual void Update(float deltaTime) override;
	virtual void OnUpdate(float deltaTime) override;
	virtual void Draw(void) override;

	void SetCurrentHandLight(std::shared_ptr<HandLight> light);

	const std::shared_ptr<PatrolPath>& GetPatrolPath(const size_t& index) const;

	std::shared_ptr<NavGridManager> GetNavGridMananger(void);

	// モデル描画用
	void InitRenderer(void);

private:

	// モデル描画用
	std::unique_ptr<ModelMaterial> material_;
	std::unique_ptr<ModelRenderer> renderer_;

	Player& player_;
	EnemyBase& eBase_;
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

	std::shared_ptr<NavGridManager> navManager_;
};