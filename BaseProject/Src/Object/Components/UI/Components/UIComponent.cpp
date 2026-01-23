#include "UIComponent.h"
#include "../UIElement.h"

UIComponent::UIComponent(std::shared_ptr<ActorBase> owner)
	:
	Component(owner)
{
}

UIComponent::~UIComponent(void)
{

}

void UIComponent::Update(float deltaTime)
{
	for (auto& e : elements_){
		e->Update(deltaTime);
	}
}

void UIComponent::Draw(void)
{
	for (auto& e : elements_){
		e->Draw();
	}
}
