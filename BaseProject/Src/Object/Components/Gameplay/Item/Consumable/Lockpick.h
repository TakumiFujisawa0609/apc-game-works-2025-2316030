#pragma once
#include "ItemBase.h"
class Lockpick :
    public ItemBase
{
public:
	static constexpr VECTOR INIT_POS = { 0.0f,0.0f,0.0f };              // 初期座標
	static constexpr VECTOR INIT_SCL = { 0.05f,0.05f,0.05f };           // 初期拡大率
	static constexpr VECTOR INIT_QUAROTLOCAL = { 0.0f,-90.0f,0.0f };    // 初期ローカル回転

	// 調整用座標
	static constexpr VECTOR TARGET_POS = { 25.0f,-20.0f, 30.0f };


	Lockpick(std::shared_ptr<ActorBase> owner);
	~Lockpick(void) override;
	void Init(void) override;
	void Update(float deltaTime) override;
	void Draw(void) override;

	// 使用されたかどうか
	bool IsUse(bool use);

	// 開錠
	void UpdateUnlock(float deltaTime);

private:

	bool isUse_;
	float angles_;

	virtual void UpdateOnStage(float deltaTime) override;
	virtual void UpdateInVentory(float deltaTime) override;
	virtual void UpdateInUse(float deltaTime) override;
	virtual void UpdateUsedUp(float deltaTime) override;
	virtual void UpdateDisabled(float deltaTime) override;


};