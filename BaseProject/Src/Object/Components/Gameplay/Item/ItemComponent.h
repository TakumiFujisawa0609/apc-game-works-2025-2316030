#pragma once
#include "../../../Common/Component.h"
class ItemComponent :
    public Component
{
public:

	enum class ItemType
	{
		NONE,
		CONSUMABLE,
		MELEE_WEAPON,
		RANGED_WEAPON,
	};

	ItemComponent(std::shared_ptr<ActorBase> owner);
	virtual ~ItemComponent(void) = default;
	virtual void Init(void) = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Draw(void) = 0;

	int GetItemId(void)const;
	int GetCount(void)const;
	ItemType GetItemType(void)const;

	// コンポーネントの識別名を取得
	const std::wstring& GetName(void) const;

protected:
	int itemId_;
	ItemType itemType_;
	int count_; // 所持数
	std::wstring mName_; // コンポーネントの識別名
};

