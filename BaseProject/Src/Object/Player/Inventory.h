#pragma once
#include <map>
#include <vector>

class Inventory
{
public:
	Inventory(void);
	~Inventory(void);

	// アイテムをインベントリに追加
	void AddItem(int itemId, int count);

	// アイテムをインベントリから削除
	void RemoveItem(int itemId, int count);

	// アイテムを所有しているかを確認
	bool HasItem(int itemId) const;

	// アイテムの所持数を取得
	int GetItemCount(int itemId) const;

	// 所持しているアイテムIDのリストを返す
	std::vector<int> GetAllItemIds() const;


private:
	std::map<int, int> items_; // アイテムIDとその所持数
	std::map<int, int> weapons_; // アイテムIDとその最大所持数
	
};

