#pragma once

#include "../ObjectBase/ActorBase.h"


class Player;
class SlotBase;

class ItemBase :
    public ActorBase
{
public:

	// アイテムの所持状態
	enum class STATE{
		NONE,		// なし
		ONSTAGE,	// 拾うことができる状態
		ININVENTORY// UIに表示され、使用装備が可能
	};

	// インベントリ状態で判別される状態
	enum class USE	{
		NONE,		// なし
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

	// インベントリ内での状態の変更
	virtual void ChangeUse(USE use);

	// 使用可能状態かを取得
	virtual bool IsDisabledItem(void) const;

	// 状態の取得
	virtual STATE GetState(void) const;

	// インベントリ内での状態の取得
	virtual USE GetUse(void) const;

	// UI描画
	virtual void DrawUI(void);

	// ランダム生成時の座標位置の設定
	virtual void SetPos(VECTOR pos);

protected:

	// 画像ハンドル
	int imgH_;

	// ステージにあるかどうか
	bool isOnStage_;

	// 装備中かどうか
	bool isEquipment_;

	// 効果があるかどうか
	bool isEfficacy_;

	// 使用できるかどうか
	bool isDisabled_;

	// プレイヤー
	Player& player_;

	// 所持状態
	STATE state_;

	// インベントリ内状態
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
	// スロット
	std::weak_ptr<SlotBase> ownerSlot_;

	// スロット数
	int slotIndex_;

};

