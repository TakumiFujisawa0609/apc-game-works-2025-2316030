#pragma once
#include "../CharactorComponent.h"
class Player;
class EnemyComponent :
    public CharactorComponent
{
public:
    EnemyComponent(std::shared_ptr<Charactor> owner, Player& player);
    virtual ~EnemyComponent(void);

    virtual void Update(float deltaTime) override;

protected:
    Player& player_;

private:

};

