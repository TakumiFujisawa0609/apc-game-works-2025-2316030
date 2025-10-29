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
			// 新しいアイテムを INUSE 状態にする
			newItemBase->ChangeState(ItemBase::STATE::INUSE);
		}
	}
}
