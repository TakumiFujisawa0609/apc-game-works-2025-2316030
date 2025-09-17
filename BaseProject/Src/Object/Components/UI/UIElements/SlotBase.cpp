#include "SlotBase.h"

SlotBase::SlotBase(void)
{
    itemId_ = -1;
}

void SlotBase::SetItem(int id)
{
    itemId_ = id;
}

int SlotBase::GetSlotItemId(void) const
{
    return itemId_;
}

void SlotBase::Update(float deltaTime)
{
}

void SlotBase::Draw(void)
{
}
