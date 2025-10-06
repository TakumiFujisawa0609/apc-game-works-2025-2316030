#include <DxLib.h>
#include "../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::instance_ = nullptr;

void ResourceManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ResourceManager();
	}
	instance_->Init();
}

ResourceManager& ResourceManager::GetInstance(void)
{
	return *instance_;
}

void ResourceManager::Init(void)
{

	// 推奨しませんが、どうしても使いたい方は
	using RES = Resource;
	using RES_T = RES::TYPE;
	static std::wstring PATH_IMG = Application::PATH_IMAGE;
	static std::wstring PATH_MDL = Application::PATH_MODEL;
	static std::wstring PATH_EFF = Application::PATH_EFFECT;
	static std::wstring PATH_SND = Application::PATH_SOUND;

	std::shared_ptr<Resource> res;

	// リソース登録

	// 潜水艦
	res = std::make_shared<RES>(RES_T::MODEL, PATH_MDL + L"submarine/psx_sub_free.mv1");
	resourcesMap_.emplace(SRC::SUBMARINE, res);

	// ステージ
	res = std::make_shared<RES>(RES_T::MODEL, PATH_MDL + L"Stage/MainPlanet.mv1");
	resourcesMap_.emplace(SRC::STAGE, res);

	// ボトルモデル
	res = std::make_shared<RES>(RES_T::MODEL, PATH_MDL + L"Item/bottle/bottle.mv1");
	resourcesMap_.emplace(SRC::BOTTLE_M, res);

	// ボトルUI
	res = std::make_shared<RES>(RES_T::IMG, PATH_IMG + L"Item/bottle/UI_bottle_Img.png");
	resourcesMap_.emplace(SRC::BOTTLE_I, res);

	// ナイフモデル
	res = std::make_shared<RES>(RES_T::MODEL, PATH_MDL + L"Item/Knife/Knife.mv1");
	resourcesMap_.emplace(SRC::KNIFE_M, res);

	// ナイフUI
	res = std::make_shared<RES>(RES_T::IMG, PATH_IMG + L"Img/Weapon/Melee/Knife/");
	resourcesMap_.emplace(SRC::KNIFE_I, res);

	// フラッシュライトモデル
	res = std::make_shared<RES>(RES_T::MODEL, PATH_MDL + L"Item/Flashlight/flashlight.mv1");
	resourcesMap_.emplace(SRC::FLASHLIGHT_M, res);

	// フラッシュライトUI
	res = std::make_shared<RES>(RES_T::IMG, PATH_IMG + L"Item/Flashlight/flashlight.png");
	resourcesMap_.emplace(SRC::FLASHLIGHT_I, res);

	// ラジオモデル
	res = std::make_shared<RES>(RES_T::MODEL, PATH_MDL + L"Item/Radio/Radio.mv1");
	resourcesMap_.emplace(SRC::RADIO_M, res);

	// ラジオUI
	res = std::make_shared<RES>(RES_T::IMG, PATH_IMG + L"Item/Radio/radio.png");
	resourcesMap_.emplace(SRC::RADIO_I, res);

	// ロックピックモデル
	res = std::make_shared<RES>(RES_T::MODEL, PATH_MDL + L"Item/bottle/bottle.mv1");
	resourcesMap_.emplace(SRC::LOCKPICK_M, res);

	// ロックピックUI
	res = std::make_shared<RES>(RES_T::IMG, PATH_MDL + L"Stage/alphaltFloor.mv1");
	resourcesMap_.emplace(SRC::LOCKPICK_I, res);

	// 針金モデル
	res = std::make_shared<RES>(RES_T::MODEL, PATH_MDL + L"Item/bottle/bottle.mv1");
	resourcesMap_.emplace(SRC::WIRE_M, res);

	// 針金モデル
	res = std::make_shared<RES>(RES_T::MODEL, PATH_MDL + L"Item/bottle/bottle.mv1");
	resourcesMap_.emplace(SRC::KEYHOLE_M, res);

}

void ResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second.Release();
	}

	loadedMap_.clear();
}

void ResourceManager::Destroy(void)
{
	Release();
	for (auto& res : resourcesMap_)
	{
		res.second->Release();
		//delete res.second;
	}
	resourcesMap_.clear();
	delete instance_;
}

const Resource& ResourceManager::Load(SRC src)
{
	Resource& res = _Load(src);
	if (res.type_ == Resource::TYPE::NONE)
	{
		return dummy_;
	}
	return res;
}

int ResourceManager::LoadModelDuplicate(SRC src)
{
	Resource& res = _Load(src);
	if (res.type_ == Resource::TYPE::NONE)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res.handleId_);
	res.duplicateModelIds_.push_back(duId);

	return duId;
}

ResourceManager::ResourceManager(void)
{
}

Resource& ResourceManager::_Load(SRC src)
{

	// ロード済みチェック
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return *resourcesMap_.find(src)->second;
	}

	// リソース登録チェック
	const auto& rPair = resourcesMap_.find(src);
	if (rPair == resourcesMap_.end())
	{
		// 登録されていない
		return dummy_;
	}

	// ロード処理
	rPair->second->Load();

	// 念のためコピーコンストラクタ
	loadedMap_.emplace(src, *rPair->second);

	return *rPair->second;

}
