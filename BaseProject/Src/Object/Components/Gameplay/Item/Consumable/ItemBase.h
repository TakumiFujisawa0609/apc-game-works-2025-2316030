#pragma once
#include "../../../../Common/Transform.h"

#include "../ItemComponent.h"

class ResourceManager;

class ItemBase :
    public ItemComponent
{
public:

	enum class STATE
	{
		NONE,		// なし
		ONSTAGE,	// 拾うことができる状態
		ININVENTORY,// UIに表示され、使用装備が可能
		INUSE,		// 現在使用中
		USEDUP,		// 使用済みでインベントリから削除される状態
		DISABLED	// アイテムが一時的に使えない状態
	};

	ItemBase(std::shared_ptr<ActorBase> owner);
	~ItemBase(void) override = default;
	void Init(void) override = 0;
	void Update(float deltaTime) override = 0;
	void Draw(void) override = 0;
	void Release(void) override = 0;

	virtual int GetImgId(void);

	// プレイヤーの持ち手座標を取得する
	void SetTargetPos(const Transform* target);

protected:
	int useCount_;		// 使用回数
	int maxUseCount_;	// 最大使用回数
	int imgId_;			// UI用画像
	bool isOnStage_;	// ステージにあるかどうか
	bool isEquipment_;	// 装備中かどうか
	bool isEfficacy_;	// 効果があるかどうか
	bool isDisabled_;	// 使用できるかどうか

	// シングルトン参照
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
	virtual void UpdateUsedUp(float deltaTime) = 0;
	virtual void UpdateDisabled(float deltaTime) = 0;

	// 状態の変更
	virtual void ChangeState(STATE state);
};

