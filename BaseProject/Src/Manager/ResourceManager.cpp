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

	// ステージ
	//res = std::make_shared<RES>(RES_T::MODEL, PATH_MDL + L"Stage/stage2.mv1");
	//res = std::make_shared<RES>(RES_T::MODEL, PATH_MDL + L"Stage/eStage.mv1");
	res = std::make_shared<RES>(RES_T::MODEL, PATH_MDL + L"Stage/Horror_Game_Starter_Kit.mv1");
	res = std::make_shared<RES>(RES_T::MODEL, PATH_MDL + L"Stage/stage.mv1");
	resourcesMap_.emplace(SRC::STAGE, res);

	// フラッシュライトモデル
	res = std::make_shared<RES>(RES_T::MODEL, PATH_MDL + L"Item/Flashlight/flashlight.mv1");
	resourcesMap_.emplace(SRC::FLASHLIGHT, res);

	// ロックピックモデル
	res = std::make_shared<RES>(RES_T::MODEL, PATH_MDL + L"Item/Lockpick/folding.mv1");
	resourcesMap_.emplace(SRC::LOCKPICK, res);

	// 針金モデル
	res = std::make_shared<RES>(RES_T::MODEL, PATH_MDL + L"Item/Wire/hook.mv1");
	resourcesMap_.emplace(SRC::WIRE, res);

	// バッテリーモデル
	res = std::make_shared<RES>(RES_T::MODEL, PATH_MDL + L"Item/Battery/.mv1");
	resourcesMap_.emplace(SRC::BATTERY, res);

	// 精神安定剤モデル
	res = std::make_shared<RES>(RES_T::MODEL, PATH_MDL + L"Item/Tranquilizer/.mv1");
	resourcesMap_.emplace(SRC::TRANQUILIZER, res);

	// 止血モデル
	res = std::make_shared<RES>(RES_T::MODEL, PATH_MDL + L"Item/Hemostatic/.mv1");
	resourcesMap_.emplace(SRC::HEMOSTATIC, res);

	// 鍵穴プレート
	res = std::make_shared<RES>(RES_T::IMG, PATH_IMG + L"Keyhole/plate.png");
	resourcesMap_.emplace(SRC::E_PLATE, res);

	// 鍵穴
	res = std::make_shared<RES>(RES_T::IMG, PATH_IMG + L"Keyhole/keyhole.png");
	resourcesMap_.emplace(SRC::KEYHOLE, res);

	// タイトル
	res = std::make_shared<RES>(RES_T::IMG, PATH_IMG + L"Scene/title.png");
	resourcesMap_.emplace(SRC::TITLE, res);

	// ゲームオーバー
	res = std::make_shared<RES>(RES_T::IMG, PATH_IMG + L"Scene/gameOver.png");
	resourcesMap_.emplace(SRC::GAMEOVER, res);

	// ゲームクリア
	res = std::make_shared<RES>(RES_T::IMG, PATH_IMG + L"Scene/gameClear.png");
	resourcesMap_.emplace(SRC::GAMECLEAR, res);

	// 決定SE
	res = std::make_shared<RES>(RES_T::SOUND, PATH_SND + L"decide.mp3");
	resourcesMap_.emplace(SRC::DECIDE_SE, res);

	// 走り
	res = std::make_shared<RES>(RES_T::SOUND, PATH_SND + L"run.mp3");
	resourcesMap_.emplace(SRC::RUN_SE, res);

	// 歩き
	res = std::make_shared<RES>(RES_T::SOUND, PATH_SND + L"walk.mp3");
	resourcesMap_.emplace(SRC::WALK_SE, res);

	// 敵
	res = std::make_shared<RES>(RES_T::MODEL, PATH_MDL + L"Enemy/Y-bot/Y-bot.mv1");
	resourcesMap_.emplace(SRC::ENEMY, res);

	// 影
	res = std::make_shared<RES>(RES_T::IMG, PATH_IMG + L"Shadow/Shadow.png");
	resourcesMap_.emplace(SRC::SHADOW, res);
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

const Resource& ResourceManager::Load(const SRC& src)
{
	Resource& res = _Load(src);
	if (res.type_ == Resource::TYPE::NONE)
	{
		return dummy_;
	}
	return res;
}

int ResourceManager::LoadModelDuplicate(const SRC& src)
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
