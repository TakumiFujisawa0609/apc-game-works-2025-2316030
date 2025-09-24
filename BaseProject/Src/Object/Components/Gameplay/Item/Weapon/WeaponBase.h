#pragma once
#include "../../../../Common/Transform.h"
#include <vector>

class ResourceManager;
class Collider;
class Capsule;

#include "../ItemComponent.h"
class WeaponBase :
    public ItemComponent
{
public:

	enum STATE
	{
		NONE,
		ONSTAGE,	// 拾うことができる状態
		ININVENTORY,// UIに表示され、使用装備が可能
		INUSE,		// 現在使用中
	};

	WeaponBase(std::shared_ptr<ActorBase> owner);
	~WeaponBase(void) override = default;
	void Init(void) override = 0;
	void Update(float deltaTime) override = 0;
	void Draw(void) override = 0;
	virtual void Attack(void) = 0;	// 攻撃

	// プレイヤーの持ち手座標を取得する
	void SetTargetPos(const Transform* target);

	// 衝突判定に用いられるコライダ制御
	void AddCollider(std::shared_ptr<Collider> collider);
	void ClearCollider(void);

	// 衝突カプセルの取得
	const std::shared_ptr<Capsule> GetCapsule(void);


protected:
	int imgId_;			// UI用画像
	bool isOnStage_;	// ステージにあるかどうか
	bool isEquipment_;	// 装備中かどうか
	bool isEfficacy_;	// 効果があるかどうか
	bool isDisabled_;	// 使用できるかどうか

	// リソース
	ResourceManager& resMng_;

	// モデル情報
	Transform transform_;

	// 状態
	STATE state_;

	// 追従対象の座標
	const Transform* targetTransform_;

	// それぞれの状態でのアイテムの更新
	virtual void UpdateState(float deltaTime);
	virtual void UpdateOnStage(float deltaTime) = 0;
	virtual void UpdateInVentory(float deltaTime) = 0;
	virtual void UpdateInUse(float deltaTime) = 0;

	// 状態の変更
	virtual void ChangeState(STATE state);

	// 衝突判定に用いられるコライダ
	std::vector<std::shared_ptr<Collider>> colliders_;
	std::shared_ptr<Capsule> capsule_;

private:



};

