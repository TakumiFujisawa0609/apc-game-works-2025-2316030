#pragma once
#include "../Charactor/CharactorComponent.h"
class EnemyMoveComponent :
    public CharactorComponent
{
public:
    EnemyMoveComponent(std::shared_ptr<Charactor> owner);
    virtual ~EnemyMoveComponent(void);

    virtual void Update(float deltaTime) override;


private:

};

