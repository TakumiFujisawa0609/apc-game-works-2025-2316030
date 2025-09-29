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
    public UIElement
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
		std::shared_ptr<ItemComponent> compatibleItem(static_pointer_cast<ItemComponent>(item));
		storage_.push_back(compatibleItem);
		if (currentSelectedIndex_ == -1)
		{
			currentSelectedIndex_ = 0; // 最初のアイテムが追加された場合、選択インデックスを更新
		}
		return true; // スロットが満杯で追加に失敗
	}

	// アイテムの切り替え
	bool SelectItem(int index);

	// 循環切り替えする
	void CycleByWheel(bool scrollUp);

protected:
    
    std::wstring slotName_;
    // スロットが格納するアイテムリスト
	std::vector<std::shared_ptr<ItemComponent>> storage_;
	int currentSelectedIndex_;


	// 循環切り替えを計算する
	void UpdateIndex(int direction);
};

