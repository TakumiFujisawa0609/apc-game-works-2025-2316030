#include <DxLib.h>
#include "ResourceManager.h"
#include "SoundManager.h"

SoundManager* SoundManager::instance_ = nullptr;

void SoundManager::CreateInstance()
{
	if (instance_ == nullptr) {
		instance_ = new SoundManager();
		instance_->Init();
	}
}

SoundManager& SoundManager::GetInstance()
{
	return *instance_;
}

void SoundManager::Init(void)
{
	// ‚·‚×‚Ä‚Ì‰¹º‚ğ“Ç‚İ‚Ş
	Load();
}

void SoundManager::Load(void)
{
	//resManager_.Load(ResourceManager::SRC::WALK_SE);
}

void SoundManager::Play(SOUNDID soundID)
{

}

void SoundManager::Destory(void)
{
	delete instance_;
}

SoundManager::SoundManager(void)
	:
	resManager_(ResourceManager::GetInstance())
{
}
