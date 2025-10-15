#pragma once
#include <string>
#include <vector>
#include <memory>
#include <type_traits>
#include <algorithm>
#include "../UIElement.h"
#include "../../Gameplay/Item/ItemComponent.h"
#include "../../Gameplay/Item/Consumable/ItemBase.h"

class SlotBase :
    public UIElement,
	public std::enable_shared_from_this<SlotBase>
{
public:
    SlotBase(void);
    virtual ~SlotBase(void) = default;

	void Update(float deltaTime) override = 0;
	void Draw(void) override = 0;

	// アイテムをスロットに追加する
    template<typename U>
	bool AddItem(std::shared_ptr<U> item)
	{
		static_assert(std::is_base_of_v<ItemComponent, U>, "U must be derived from ItemComponent");
		std::shared_ptr<ItemComponent> compatibleItem = std::static_pointer_cast<ItemComponent>(item);
		int newIndex = static_cast<int>(storage_.size());
		storage_.push_back(compatibleItem);
		if (std::shared_ptr<ItemBase> itemBase = std::static_pointer_cast<ItemBase>(compatibleItem))
		{
			itemBase->SetOwnerSlot(shared_from_this(), newIndex);
		}
		if (currentSelectedIndex_ == -1)
		{
			currentSelectedIndex_ = 0; // 最初のアイテムが追加された場合、選択インデックスを更新
		}
		// 現在のインデックス
		int currentIndex = currentSelectedIndex_;

			
		return true; // スロットが満杯で追加に失敗
	}

	// アイテムの切り替え
	bool SelectItem(int index);

	// 循環切り替えする
	void CycleByWheel(bool scrollUp);

	int GetCurrentSelectedIndex(void);

protected:
    
    std::wstring slotName_;
    // スロットが格納するアイテムリスト
	std::vector<std::shared_ptr<ItemComponent>> storage_;
	int currentSelectedIndex_;

	// 循環切り替えを計算する
	void UpdateIndex(int direction);
};

