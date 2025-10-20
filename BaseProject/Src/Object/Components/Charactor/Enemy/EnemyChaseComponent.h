#pragma once
#include <vector>

#include "EnemyComponent.h"

class Transform;
class EnemyChaseComponent :
    public EnemyComponent
{
public:
    EnemyChaseComponent(std::shared_ptr<Charactor> owner, Player& player);
    virtual ~EnemyChaseComponent(void);

    virtual void Update(float deltaTime) override;

    // 追跡処理
    void Chase(float deltaTime);

    // 障害物を設定する
    void SetObstacle(std::vector<std::shared_ptr<Transform>> obstacle);

    //  障害物を取得する
    std::vector<std::shared_ptr<Transform>> GetObstacle(void);



private:

    // 障害物
    std::vector<std::shared_ptr<Transform>> obstacle_;

    // カプセルキャスト
    void CapsuleCast(float deltaTime);


};

