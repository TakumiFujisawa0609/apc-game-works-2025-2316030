#include <DxLib.h>
#include "../../../Application.h"
#include "../../../Utility/AsoUtility.h"
#include "../../../Manager/ResourceManager.h"
#include "../../../Manager/Camera.h"
#include "KeyholePlate.h"

KeyholePlate::KeyholePlate(void)
{

}

KeyholePlate::~KeyholePlate(void)
{
}

void KeyholePlate::Init(void)
{
	// g—p‚·‚é‰æ‘œ‚Ì‰Šú‰»
	imgH_ = resMng_.Load(ResourceManager::SRC::E_PLATE).handleId_;

	// 2Dî•ñ‚Ì‰Šú‰»
	InitImg(0.0f, 0.0f, 0.25f, 0.0f);
}

void KeyholePlate::OnUpdate(float deltaTime)
{
}

void KeyholePlate::Draw(void)
{
	PlazzleElementBase::Draw();
}