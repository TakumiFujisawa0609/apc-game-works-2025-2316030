#pragma once
#include "../ItemBase.h"
class PermanentItemBase :
	public ItemBase
{
public:

	PermanentItemBase(Player& player);
	~PermanentItemBase(void);
	void Init(void) override = 0;
	void Update(float deltaTime) override = 0;
	void Draw(void) override = 0;

	virtual bool IsActive(void);

protected:

	bool isActive_;

	// ゲーム内空間でのアイテムの更新
	virtual void UpdateState(float deltaTime);
	virtual void UpdateOnStage(float deltaTime);
	virtual void UpdateInVentory(float deltaTime);

	// インベントリでのアイテムの更新
	virtual void UpdateUsed(float deltaTime);
	virtual void UpdateInUse(float deltaTime) = 0;
	virtual void UpdateUsedUp(float deltaTime) = 0;


private:
};

