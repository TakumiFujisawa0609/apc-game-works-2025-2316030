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


	auto& size = Application::GetInstance().GetWindowSize();
	DrawFormatString(size.width - 200, 32, GetColor(255, 5, 5), storage_[currentSelectedIndex_]->GetName().c_str());
}

int ItemSlot::GetSelectedUIHandle(void) const
{
	if (currentSelectedIndex_ != -1 && currentSelectedIndex_ < storage_.size())
	{
		return storage_[currentSelectedIndex_]->GetItemId();
	}
	return -1;
}
