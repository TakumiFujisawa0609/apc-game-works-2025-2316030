#pragma once
#include "ItemBase.h"
class Lockpick :
    public ItemBase
{
public:

	// 調整用座標
	static constexpr VECTOR TARGET_POS = { 25.0f,-20.0f, 30.0f };


	Lockpick(std::shared_ptr<ActorBase> owner);
	~Lockpick(void) override;
	void Init(void) override;
	void Update(float deltaTime) override;
	void Draw(void) override;

	// 使用されたかどうか
	bool IsUse(bool use);

private:

	bool isUse_;
	float angles_;

	virtual void UpdateOnStage(float deltaTime) override;
	virtual void UpdateInVentory(float deltaTime) override;
	virtual void UpdateInUse(float deltaTime) override;
	virtual void UpdateUsedUp(float deltaTime) override;
	virtual void UpdateDisabled(float deltaTime) override;

	// 開錠
	void UpdateUnlock(float deltaTime);
};