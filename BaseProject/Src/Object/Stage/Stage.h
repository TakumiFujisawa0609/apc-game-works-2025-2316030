#pragma once
#include "../ObjectBase/ActorBase.h"

class Player;
class EnemyBase;

class PatrolNode;
class PatrolPath;

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

	const std::shared_ptr<PatrolPath>& GetPatrolPath(size_t index) const;

private:
	Player& player_;
	EnemyBase& eBase_;

	std::vector<PatrolNode> nodes_;		// 巡回用のノード
	std::vector<std::shared_ptr<PatrolPath>> paths_;		// 巡回用のパス

	// 巡回情報の初期化
	void InitPatrolInfo(void);

};

