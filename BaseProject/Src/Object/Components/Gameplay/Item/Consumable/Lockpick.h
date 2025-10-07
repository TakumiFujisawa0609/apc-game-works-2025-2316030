#pragma once
#include "ItemBase.h"
class Lockpick :
    public ItemBase
{
public:
	static constexpr VECTOR INIT_POS = { 0.0f,0.0f,0.0f };              // 初期座標
	static constexpr VECTOR INIT_SCL = { 0.05f,0.05f,0.05f };           // 初期拡大率
	static constexpr VECTOR INIT_QUAROTLOCAL = { 0.0f,-90.0f,0.0f };    // 初期ローカル回転

	// 調整用座標
	static constexpr VECTOR TARGET_POS = { 25.0f,-20.0f, 30.0f };

	static constexpr float MIN_ANGLE = 180.0f;
	static constexpr float MAX_ANGLE = 270.0f;

	// 全体の範囲
	static constexpr float TOTAL_RANGE = MAX_ANGLE - MIN_ANGLE;

	Lockpick(std::shared_ptr<ActorBase> owner);
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

private:

	bool isUse_;				// 使用可能かどうか
	float angle_;				// 角度
	int lLevel_;				// 施錠レベル
	float rotRate_;				// 回転割合

	virtual void UpdateOnStage(float deltaTime) override;
	virtual void UpdateInVentory(float deltaTime) override;
	virtual void UpdateInUse(float deltaTime) override;
	virtual void UpdateUsedUp(float deltaTime) override;
	virtual void UpdateDisabled(float deltaTime) override;

	// 回転割合の計算
	float CalculateRotRate(void);
};