#pragma once
#include <vector>
#include "../CharactorComponent.h"

class Transform;
class Player;
class EnemyComponent :
    public CharactorComponent
{
public:
    EnemyComponent(std::shared_ptr<Charactor> owner, Player& player);
    virtual ~EnemyComponent(void);

    virtual void Update(float deltaTime) override;

    //// 障害物情報をNavGridManagerに設定
    //void SetObstacle(std::vector<Transform> obstacle);

protected:

    // プレイヤー参照
    Player& player_;

};

