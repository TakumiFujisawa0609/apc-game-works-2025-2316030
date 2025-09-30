#include "SlotBase.h"


SlotBase::SlotBase(void)
	:
	currentSelectedIndex_(-1)
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
	if (index >= 0 && index < storage_.size())
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
	//if (storage_.empty())
	//{
	//	currentSelectedIndex_ = -1;
	//	return;
	//}

	//int size = static_cast<int>(storage_.size());
	//int newIndex = currentSelectedIndex_ + direction;
	//if (newIndex < 0)
	//{
	//	newIndex = size - 1; // 最後のアイテムに戻る
	//}
	//else if (newIndex >= size)
	//{
	//	newIndex = 0; // 最初のアイテムに戻る
	//}
	//currentSelectedIndex_ = newIndex;

	if (storage_.empty())
	{
		currentSelectedIndex_ = -1;
		return;
	}

	// 【追加】以前選択されていたアイテムの状態を変更
	if (currentSelectedIndex_ != -1)
	{
		if (auto prevItemBase = std::dynamic_pointer_cast<ItemBase>(storage_[currentSelectedIndex_]))
		{
			// 以前のアイテムを ININVENTORY 状態に戻す
			prevItemBase->ChangeState(ItemBase::STATE::ININVENTORY);
		}
	}

	int size = static_cast<int>(storage_.size());
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
		if (auto newItemBase = std::dynamic_pointer_cast<ItemBase>(storage_[currentSelectedIndex_]))
		{
			// 新しいアイテムを INUSE 状態にする
			newItemBase->ChangeState(ItemBase::STATE::INUSE);
		}
	}
}
