#include "../ComsumableItems/Battery.h"
#include "../PermanentItems/HandLight.h"
#include "../PermanentItems/Lockpick.h"
#include "../ComsumableItems/Battery.h"
#include "../ComsumableItems/Hemostatic.h"
#include "../ComsumableItems/Tranquilizer.h"
#include "../Wire.h"
#include "SlotBase.h"


SlotBase::SlotBase(void)
	:
	currentSelectedIndex_(-1)
{
}

SlotBase::~SlotBase(void)
{
}

void SlotBase::Update(float deltaTime)
{
}

void SlotBase::Draw(void)
{
	std::wstring itemName = GetCurrentItemType();

	DrawFormatString(10, 50, GetColor(255, 255, 255), L"Current: %ls", itemName.c_str());
}

void SlotBase::UseCurrentItem(void)
{
	auto currentItem = GetCurrentItem();

	if (auto battery = std::dynamic_pointer_cast<Battery>(currentItem))
	{
		battery->Use();
	}
	if (auto hemostatic = std::dynamic_pointer_cast<Hemostatic>(currentItem))
	{
		hemostatic->Use();
	}
	if (auto tranquilizer = std::dynamic_pointer_cast<Tranquilizer>(currentItem))
	{
		tranquilizer->Use();
	}

	if (!currentItem->IsDisabledItem())
	{
		RemoveItem(currentItem);
	}
}

std::shared_ptr<ItemBase> SlotBase::GetCurrentItem(void) const
{
	if (slots_.empty())
	{
		return nullptr;
	}

	// 現在のインデックスが有効な範囲内かをチェック
	if (currentSelectedIndex_ >= 0 && currentSelectedIndex_ < static_cast<int>(slots_.size()))
	{
		return slots_[currentSelectedIndex_];
	}
	return nullptr;
}

bool SlotBase::SelectItem(int index)
{
	if (index >= 0 && index < slots_.size())
	{
		currentSelectedIndex_ = index;
		return true;
	}
    return false;
}

void SlotBase::CycleByWheel(bool scrollUp)
{
	int direction = scrollUp ? -1 : 1;
	UpdateIndex(direction);
	
}

int SlotBase::GetCurrentSelectedIndex(void)
{
	return currentSelectedIndex_;
}

void SlotBase::UpdateIndex(int direction)
{
	if (slots_.empty())
	{
		currentSelectedIndex_ = -1;
		return;
	}

	if (currentSelectedIndex_ != -1)
	{
		if (auto prevItemBase = std::dynamic_pointer_cast<ItemBase>(slots_[currentSelectedIndex_]))
		{
			// 以前のアイテムを ININVENTORY 状態に戻す
			prevItemBase->ChangeState(ItemBase::STATE::ININVENTORY);
			prevItemBase->ChangeUse(ItemBase::USE::NONE);
		}
	}

	int size = static_cast<int>(slots_.size());
	int newIndex = currentSelectedIndex_ + direction;
	if (newIndex < 0)
	{
		newIndex = size - 1; // 最後のアイテムに戻る
	}
	else if (newIndex >= size)
	{
		newIndex = 0; // 最初のアイテムに戻る
	}
	currentSelectedIndex_ = newIndex;

	
	if (currentSelectedIndex_ != -1)
	{
		if (auto newItemBase = std::dynamic_pointer_cast<ItemBase>(slots_[currentSelectedIndex_]))
		{
			// 新しいアイテムを ININVENTORY 状態にする
			newItemBase->ChangeState(ItemBase::STATE::ININVENTORY);
			newItemBase->ChangeUse(ItemBase::USE::INUSE);
		}
	}
}

void SlotBase::RemoveItem(const std::shared_ptr<ItemBase>& item)
{
}

std::wstring SlotBase::GetCurrentItemType(void) const
{
	auto currentItem = GetCurrentItem();
	if (!currentItem)
	{
		return L"None";
	}

	// 恒久アイテム
	if (std::dynamic_pointer_cast<HandLight>(currentItem))
	{
		return L"HandLight";
	}
	if (std::dynamic_pointer_cast<Lockpick>(currentItem))
	{
		return L"Lockpick";
	}
	// 消費アイテム
	if (std::dynamic_pointer_cast<Battery>(currentItem))
	{
		return L"Battery";
	}
	if (std::dynamic_pointer_cast<Hemostatic>(currentItem))
	{
		return L"Hemostatic";
	}
	if (std::dynamic_pointer_cast<Tranquilizer>(currentItem))
	{
		return L"Tranquilizer";
	}
	// その他
	if (std::dynamic_pointer_cast<Wire>(currentItem))
	{
		return L"Wire";
	}

	return L"UnknownItem";
}
