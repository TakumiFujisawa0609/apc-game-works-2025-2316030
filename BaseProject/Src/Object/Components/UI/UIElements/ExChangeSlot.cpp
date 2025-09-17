#include "ExChangeSlot.h"

ExChangeSlot::ExChangeSlot(int front, 
    int back)
    :
    front_(front),
    back_(back),
    showFront_(true)
{
}

void ExChangeSlot::Update(float deltaTime)
{

}

void ExChangeSlot::Draw(void)
{

}

void ExChangeSlot::Toggle(void)
{
    showFront_ = false;
    // アイテムスロットが入れ替わったので通知
    NotifyToggle();
}

int ExChangeSlot::GetActiveItemId(void) const
{
    if (showFront_)
    {
        return front_;
    }
    else
    {
        return back_;
    }
}

void ExChangeSlot::SetToggleCallback(std::function<void(int itemId, bool isFrontNow)> callBack)
{
    onToggle_ = std::move(callBack);
}

void ExChangeSlot::NotifyToggle(void)
{
    if (onToggle_)
    {
        onToggle_(GetActiveItemId(), showFront_);
    }
}
