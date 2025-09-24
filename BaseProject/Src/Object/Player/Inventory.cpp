#include "Inventory.h"

Inventory::Inventory(int size)
{
	items_.clear();
	weapons_.clear();
	slots_.resize(size, nullptr);
}

Inventory::~Inventory(void)
{
}

void Inventory::AddItem(std::shared_ptr<ItemComponent> item)
{
	// スタック可能なアイテムを探す
	for (auto& slot : slots_)
	{
		if (slot && slot->GetItemId() == item->GetItemId())
		{
			// アイテムが既に存在する場合、所持数を増やす
			items_[item->GetItemId()]++;
			return;
		}
	}

	// 新しい空のスロットを探して追加する
	for (size_t i=0;i<slots_.size() ; ++i)
	{
		if (slots_[i] == nullptr) // 空のスロットを見つけた場合
		{
			slots_[i] = item; // アイテムをスロットに追加
			items_[item->GetItemId(), item->GetCount()] = 1; // 所持数を1に設定
			return;
		}
		return; // インベントリが満杯の場合は何もしない
	}
}

void Inventory::RemoveItem(int itemId, int count)
{
	if (count <= 0)
	{
		return;
	}

	// アイテムが存在するか確認
	auto it = items_.find(itemId);
	if (it != items_.end())
	{
		return;
	}

	// 削除する数量が現在の所持数以上の場合
	it->second -= count;
	if (it->second <= 0)
	{
		items_.erase(it);
	}
	else
	{
		it->second -= count;
	}

}

bool Inventory::HasItem(int itemId) const
{
	return items_.count(itemId) > 0;
}

int Inventory::GetItemCount(int itemId) const
{
	auto it = items_.find(itemId);
	if (it != items_.end())
	{
		return it->second;
	}
    return 0;
}

std::vector<int> Inventory::GetAllItemIds() const
{
	std::vector<int> itemIds;
	for (const auto& pair : items_)
	{
		itemIds.push_back(pair.first);
	}
	return itemIds;
}
