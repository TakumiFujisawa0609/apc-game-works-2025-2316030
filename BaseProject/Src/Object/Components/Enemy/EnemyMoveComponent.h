#pragma once
#include "../../Common/Component.h"
class EnemyMoveComponent :
    public Component
{
public:
    EnemyMoveComponent(std::shared_ptr<ActorBase> owner);
    virtual ~EnemyMoveComponent(void);

    virtual void Update(float deltaTime) override;


private:

};

