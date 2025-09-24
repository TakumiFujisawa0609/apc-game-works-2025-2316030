#include "../../../../../Application.h"
#include "../../../../../Common/Quaternion.h"
#include "../../../../../Manager/ResourceManager.h"
#include "../../../../../Manager/Camera.h"
#include "../../../../../Utility/AsoUtility.h"
#include "../../../../Common/Collider.h"
#include "../../../../Common/Capsule.h"
#include "Knife.h"

Knife::Knife(std::shared_ptr<ActorBase> owner)
	:
	WeaponBase(owner)
{
	itemType_ = ItemType::MELEE_WEAPON;
}

void Knife::Init(void)
{
	// モデル情報
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::BOTTLE_M));
	transform_.scl = { 10.0f,10.0f,10.0f };
	transform_.pos = { 0.0f,0.0f,0.0f };
	transform_.quaRot = Quaternion();
	transform_.Update();

	// UI画像
	imgId_ = resMng_.LoadModelDuplicate(ResourceManager::SRC::BOTTLE_I);

	// 状態の初期化
	isOnStage_ = true;
	isEquipment_ = false;
	isEfficacy_ = false;
	isDisabled_ = false;
	ChangeState(STATE::ININVENTORY);

	// カプセル情報
	capsule_ = std::make_shared<Capsule>(transform_);
	/*capsule_->SetLocalPosTop();
	capsule_->SetLocalPosDown();
	capsule_->SetRadius();*/


}

void Knife::Update(float deltaTime)
{
	// モデル情報の動機

	WeaponBase::UpdateState(deltaTime);

	// モデルの更新
	transform_.Update();
}

void Knife::Draw(void)
{

	MV1DrawModel(transform_.modelId);
}

void Knife::Attack(void)
{


}

void Knife::UpdateOnStage(float deltaTime)
{
	// ステージに配置されている場合
}

void Knife::UpdateInVentory(float deltaTime)
{
	// インベントリ内に格納されている状態

	// 装備しているかどうか
	if (isEquipment_)
	{
		ChangeState(STATE::INUSE);
	}
}

void Knife::UpdateInUse(float deltaTime)
{


	// 攻撃が当たったときの場合のみ処理を通るようにする
	Attack();
}
