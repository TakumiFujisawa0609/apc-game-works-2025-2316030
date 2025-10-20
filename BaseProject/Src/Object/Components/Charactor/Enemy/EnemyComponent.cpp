#include "EnemyComponent.h"

EnemyComponent::EnemyComponent(std::shared_ptr<Charactor> owner, Player& player)
	:
	CharactorComponent(owner),
	player_(player)
{
}

EnemyComponent::~EnemyComponent(void)
{
}

void EnemyComponent::Update(float deltaTime)
{
}
