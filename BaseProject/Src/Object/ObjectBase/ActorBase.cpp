#include "../Manager/ResourceManager.h"
#include "../Object/Common/Component.h"
#include "ActorBase.h"

ActorBase::ActorBase(void)
	:
	resMng_(ResourceManager::GetInstance())
	,type_(TYPE::NONE)
{
}

ActorBase::~ActorBase(void)
{
	transform_.Release();
}

void ActorBase::Update(float deltaTime)
{
	for (auto& comp : components_)
	{
		comp->Update(deltaTime);
	}
	OnUpdate(deltaTime);
}

const Transform& ActorBase::GetTransform(void) const
{
	return transform_;
}

ActorBase::TYPE ActorBase::GetModelType(void)
{
	return type_;
}

void ActorBase::OnUpdate(float deltaTime)
{
    // Šî’êƒNƒ‰ƒX‚Å‚Í‰½‚à‘‚©‚È‚¢
}

