#include "EnemyChaseComponent.h"

EnemyChaseComponent::EnemyChaseComponent(std::shared_ptr<Charactor> owner)
    :
    CharactorComponent(owner)
{
}

EnemyChaseComponent::~EnemyChaseComponent(void)
{
}

void EnemyChaseComponent::Update(float deltaTime)
{

    CapsuleCast(deltaTime);

}

void EnemyChaseComponent::SetObstacle(std::vector<std::shared_ptr<Transform>> obstacle)
{
    obstacle_ = obstacle;
}

std::vector<std::shared_ptr<Transform>> EnemyChaseComponent::GetObstacle(void)
{
    return obstacle_;
}

void EnemyChaseComponent::CapsuleCast(float deltaTime)
{

}
