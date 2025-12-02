#pragma once
#include "PermanentItemBase.h"
class Lockpick :
	public PermanentItemBase
{
public:

	// 初期座標
	static constexpr VECTOR INIT_POS = { -400.0f, 150.0f, 200.0f };

	// 初期拡大率
	static constexpr VECTOR INIT_SCL = { 15.0f,15.0f,15.0f };

	// 初期ローカル回転
	static constexpr VECTOR INIT_QUAROTLOCAL = { 0.0f,0.0f,15.0f };

	// 調整用座標
	static constexpr VECTOR TARGET_POS = { 15.0f,-13.0f, 30.0f };

	// 制限度数
	// 下限
	static constexpr float MIN_ANGLE = 180.0f;
	// 上限
	static constexpr float MAX_ANGLE = 270.0f;

	// 全体の範囲
	static constexpr float TOTAL_RANGE = MAX_ANGLE - MIN_ANGLE;

	// 解錠中の座標
	static constexpr VECTOR UNLOCK_POS = { 0.0f,0.0f,15.0f };

	Lockpick(Player& player);
	~Lockpick(void) override;

	void Init(void) override;
	void Update(float deltaTime) override;
	void Draw(void) override;

	// 使用されたかどうか
	bool IsUse(bool use);

	// 開錠
	void UpdateUnlock(float deltaTime);

	// 解錠の段階を設定する
	void SetLockLevel(int level);

	// 回転割合を取得
	float GetRotationRate(void);

	// デフォルト状態を返す
	bool GetDefault(void);

	// デフォルト状態の設定
	void SetIsDefault(bool flag);

	// 解錠状態の設定
	void SetIsUnlocking(bool flag);

	// 成功したかどうかの設定
	void SetIsSuccess(bool flag);

	// 成功状態を返す
	bool IsSuccess(void);

private:

	// 使用可能かどうか
	bool isUse_;

	// 角度
	float angle_;

	// 施錠レベル
	int lLevel_;

	// 回転割合
	float rotRate_;

	// デフォルト状態をセット
	bool isDefault_;

	// 解錠中であるか否か
	bool isUnlocking_;

	// 解錠に成功したか否か
	bool isSuccess_;

	virtual void OnUpdate(float deltaTime) override;
	virtual void UpdateOnStage(float deltaTime) override;
	virtual void UpdateInVentory(float deltaTime) override;
	virtual void UpdateInUse(float deltaTime) override;
	virtual void UpdateUsedUp(float deltaTime) override;

	// 回転割合の計算
	float CalculateRotRate(void);

	// 初期状態に戻す
	void SetDefault(void);
};