#pragma once
#include <cassert>
#include "../ObjectBase/ActorBase.h"


class Player;
class SlotBase;

class ItemBase :
    public ActorBase
{
public:

	enum class STATE
	{
		NONE,		// なし
		ONSTAGE,	// 拾うことができる状態
		ININVENTORY// UIに表示され、使用装備が可能
	};

	enum class USE	// インベントリ状態で判別される状態
	{
		NONE,
		INUSE,		// 現在使用中
		USEDUP,		// 使用済みでインベントリから削除される状態
	};

	ItemBase(Player& player);
	~ItemBase(void) override = default;
	void Init(void) override = 0;
	void Update(float deltaTime) override = 0;
	void Draw(void) override = 0;

	// 画像IDを取得
	virtual int GetImgId(void);

	// プレイヤーの持ち手座標を取得する
	virtual void SetTargetPos(const Transform* target);

	// スロット番号を設定する
	void SetOwnerSlot(std::shared_ptr<SlotBase> slot, int index);

	// アイテムが現在選択されているか確認する
	virtual bool IsCurrentSelected(void) const;

	// 状態の変更
	virtual void ChangeState(STATE state);

	virtual void ChangeUse(USE use);

	virtual bool IsDisabledItem(void) const;

	virtual STATE GetState(void) const;

	virtual USE GetUse(void) const;

	virtual void DrawUI(void);

	virtual void SetPos(VECTOR pos);

protected:
	int imgH_;			// 画像ハンドル
	bool isOnStage_;	// ステージにあるかどうか
	bool isEquipment_;	// 装備中かどうか
	bool isEfficacy_;	// 効果があるかどうか
	bool isDisabled_;	// 使用できるかどうか

	bool isInUse_;
	bool isUsed_;

	Player& player_;

	// 状態
	STATE state_;

	USE use_;

	// 追従対象の座標
	const Transform* targetTransform_;

	// 派生クラスで独自処理
	virtual void OnUpdate(float deltaTime) = 0;

	// それぞれの状態でのアイテムの更新
	virtual void UpdateState(float deltaTime);
	virtual void UpdateOnStage(float deltaTime);
	virtual void UpdateInVentory(float deltaTime);

	virtual void UpdateUsed(float deltaTime);
	virtual void UpdateInUse(float deltaTime) = 0;
	virtual void UpdateUsedUp(float deltaTime) = 0;

	// モデル情報初期化
	virtual void InitModel(VECTOR pos, VECTOR scl, VECTOR quaRotLocal);

	// プレイヤーに追従
	virtual void FollowTarget(float deltaTime,VECTOR targetPos);
	
	// アイテムスロット
	std::weak_ptr<SlotBase> ownerSlot_;		// スロット
	int slotIndex_;							// スロット数

};

