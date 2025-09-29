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
	//if (currentSelectedIndex_ != -1)
	//{
	//	storage_[currentSelectedIndex_]->Use();
	//}
}


void SlotBase::UpdateIndex(int direction)
{
	if (storage_.empty())
	{
		currentSelectedIndex_ = -1;
		return;
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
}
