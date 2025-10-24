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

    //// áŠQ•¨î•ñ‚ğNavGridManager‚Éİ’è
    //void SetObstacle(std::vector<Transform> obstacle);

protected:

    Player& player_;

};

