#include <DxLib.h>
#include "../../../../Application.h"
#include "ItemSlot.h"


ItemSlot::ItemSlot(void)
{
	posX_ = static_cast<float>(Application::SCREEN_SIZE_X * 0.45);
	posY_ = static_cast<float>(Application::SCREEN_SIZE_Y * 0.67);

}

ItemSlot::~ItemSlot(void)
{
}

void ItemSlot::Update(float deltaTime)
{



}

void ItemSlot::Draw(void)
{
	// ˜g
	//DrawBox(posX_, posY_, posX_ + posX_ * 0.1, posY_ + posY_ * 0.1, GetColor(255, 255, 255), true);

}

int ItemSlot::GetSelectedUIHandle(void) const
{
	if (currentSelectedIndex_ != -1 && currentSelectedIndex_ < storage_.size())
	{
		return storage_[currentSelectedIndex_]->GetItemId();
	}
	return -1;
}
