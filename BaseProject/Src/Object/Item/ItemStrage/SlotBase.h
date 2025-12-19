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

	// 定数

	// 新しいスロットの幅の値
	static constexpr float SLOT_WIDTH = 0.10f;

	// スロットの幅補正
	static constexpr float SLOT_PADDING = 0.0125f;

	// スロットの高さ
	static constexpr float SLOT_HEIGHT = 0.175f;

	// アイコンの横
	static constexpr float ICON_WIDTH = 0.0875f;

	// アイコンの縦
	static constexpr float ICON_HEIGHT = 0.15556f;

	// アイコンのオフセット横
	static constexpr float ICON_OFFSET_X = 0.00625f;

	// アイコンのオフセット縦
	static constexpr float ICON_OFFSET_Y = 0.00833f;

	// 枠線の太さ横
	static constexpr float THICKNESS_WIDTH = 0.003125f;

	// 枠線の太さ縦
	static constexpr float THICKNESS_HEIGHT = 0.00416f;

	// 
	static constexpr float START_Y = 0.23f;

	SlotBase(void);
	~SlotBase(void);

	void Update(float deltaTime);
	void Draw(void);

	template<typename U>
	bool AddItem(std::shared_ptr<U> item){
		// UがItemBaseを継承していることをコンパイル時に確認
		static_assert(std::is_base_of_v<ItemBase, U>, "U must be derived from ItemBase");

		// ItemBase型にダウンキャストしてリストに格納
		std::shared_ptr<ItemBase> compatibleItem = std::static_pointer_cast<ItemBase>(item);

		// 格納順にインデックスを割り当てる
		int newIndex = static_cast<int>(slots_.size());
		slots_.push_back(compatibleItem);

		// ItemBase側にスロットのオーナーとインデックスを設定
		if (compatibleItem){
			compatibleItem->SetOwnerSlot(shared_from_this(), newIndex);
		}

		// 最初のアイテムが追加された場合、それを選択状態にする
		if (currentSelectedIndex_ == -1){
			currentSelectedIndex_ = newIndex;
			// 初期状態をINUSEにする（選択状態）
			compatibleItem->ChangeState(ItemBase::STATE::ININVENTORY);
			compatibleItem->ChangeUse(ItemBase::USE::INUSE);
		}
		else {
			// 2つ目以降はININVENTORY状態にする
			compatibleItem->ChangeState(ItemBase::STATE::ININVENTORY);
			compatibleItem->ChangeUse(ItemBase::USE::NONE);
		}

		return true;
	}

	// 現在のアイテムを使用する
	void UseCurrentItem(void);

	// 現在のアイテムのポインタを返す
	std::shared_ptr<ItemBase> GetCurrentItem(void) const;

	// アイテムの切り替え
	bool SelectItem(int index);

	// 循環切り替えする
	void CycleByWheel(bool scrollUp);

	// 現在選択されているインデックスを返す
	int GetCurrentSelectedIndex(void);

	// スロットの取得
	std::vector<std::shared_ptr<ItemBase>> GetSlots(void) const;

protected:

	// スロットが格納するアイテムリスト
	std::vector<std::shared_ptr<ItemBase>> slots_;

	// 現在のインデックス
	int currentSelectedIndex_;

	// 循環切り替えを計算する
	void UpdateIndex(int direction);

	// アイテムの削除
	void RemoveItem(const std::shared_ptr<ItemBase>& item);

	// 現在のアイテムのタイプを返す
	std::wstring GetCurrentItemType(void) const;
};

