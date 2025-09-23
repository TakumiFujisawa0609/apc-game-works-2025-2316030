#include "Inventory.h"

Inventory::Inventory(void)
{
	items_.clear();
	weapons_.clear();
}

Inventory::~Inventory(void)
{
}

void Inventory::AddItem(int itemId, int count)
{
	if (count <= 0)
	{
		return;
	}
	items_[itemId] += count;
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
