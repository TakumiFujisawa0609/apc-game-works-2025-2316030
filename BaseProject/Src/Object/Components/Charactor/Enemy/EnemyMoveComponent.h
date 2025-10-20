#pragma once

class PatrolPath;

#include "EnemyComponent.h"
class EnemyMoveComponent :
    public EnemyComponent
{
public:
    EnemyMoveComponent(std::shared_ptr<Charactor> owner, Player& player);
    virtual ~EnemyMoveComponent(void);

    virtual void Update(float deltaTime) override;

    
    /// <summary>
    /// 巡回処理
    /// </summary>
    /// <param name="">巡回のパス</param>
    /// <param name="currentindex">現在の巡回のノードの番号</param>
    /// <param name="deltaTime">処理時間</param>
    void Patrol(std::shared_ptr<PatrolPath> path, int& currentIndex, float deltaTime);

private:

};

