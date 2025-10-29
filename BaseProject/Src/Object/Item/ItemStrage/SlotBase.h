#pragma once
#include <string>
#include <vector>
#include <memory>
#include <type_traits>
#include <algorithm>
#include "../ItemBase.h"

class SlotBase :
	public std::enable_shared_from_this<SlotBase>
{
public:
    SlotBase(void);
    ~SlotBase(void);

	void Update(float deltaTime);
	void Draw(void);

	//// アイテムをスロットに追加する
 //   template<typename U>
	//bool AddItem(std::shared_ptr<U> item)
	//{
	//	static_assert(std::is_base_of_v<ItemBase, U>, "U must be derived from ItemBase");
	//	/*std::shared_ptr<ItemBase> compatibleItem = std::static_pointer_cast<ItemBase>(item);
	//	int newIndex = static_cast<int>(slots_.size());
	//	slots_.push_back(compatibleItem);
	//	if (std::shared_ptr<ItemBase> itemBase = std::static_pointer_cast<ItemBase>(compatibleItem))
	//	{
	//		itemBase->SetOwnerSlot(shared_from_this(), newIndex);
	//	}
	//	if (currentSelectedIndex_ == -1)
	//	{*/
	//	std::shared_ptr<ItemBase> compatibleItem = std::static_pointer_cast<ItemBase>(item);
	//	int newIndex = static_cast<int>(slots_.size());
	//	slots_.push_back(compatibleItem);

	//	// ItemBaseの処理（スロットオーナーの設定）
	//	if (std::shared_ptr<ItemBase> itemBase = compatibleItem) // compatibleItemは既にItemBase型なのでキャストは不要だが、ご提示コードの形式を維持
	//	{
	//		itemBase->SetOwnerSlot(shared_from_this(), newIndex);
	//	}
	//	if (currentSelectedIndex_ == -1)
	//	{
	//		currentSelectedIndex_ = 0; // 最初のアイテムが追加された場合、選択インデックスを更新
	//	}
	//	// 現在のインデックス
	//	int currentIndex = currentSelectedIndex_;

	//		
	//	return true; // スロットが満杯で追加に失敗
	//}

	template<typename U>
	bool AddItem(std::shared_ptr<U> item)
	{
		// UがItemBaseを継承していることをコンパイル時に確認
		static_assert(std::is_base_of_v<ItemBase, U>, "U must be derived from ItemBase");

		// ItemBase型にダウンキャストしてリストに格納
		std::shared_ptr<ItemBase> compatibleItem = std::static_pointer_cast<ItemBase>(item);

		// 格納順にインデックスを割り当てる
		int newIndex = static_cast<int>(slots_.size());
		slots_.push_back(compatibleItem);

		// ItemBase側にスロットのオーナーとインデックスを設定
		if (compatibleItem)
		{
			compatibleItem->SetOwnerSlot(shared_from_this(), newIndex);
		}

		// 最初のアイテムが追加された場合、それを選択状態にする
		if (currentSelectedIndex_ == -1)
		{
			currentSelectedIndex_ = newIndex;
			// 初期状態をINUSEにする（選択状態）
			compatibleItem->ChangeState(ItemBase::STATE::INUSE);
		}
		else {
			// 2つ目以降はININVENTORY状態にする
			compatibleItem->ChangeState(ItemBase::STATE::ININVENTORY);
		}

		return true;
	}

	// アイテムの切り替え
	bool SelectItem(int index);

	// 循環切り替えする
	void CycleByWheel(bool scrollUp);

	int GetCurrentSelectedIndex(void);

protected:
    
    std::wstring slotName_;
    // スロットが格納するアイテムリスト
	std::vector<std::shared_ptr<ItemBase>> slots_;
	int currentSelectedIndex_;

	// 循環切り替えを計算する
	void UpdateIndex(int direction);
};

