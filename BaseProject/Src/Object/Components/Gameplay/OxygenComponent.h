#pragma once
#include "../Charactor/CharactorComponent.h"
class TimeLimitComponent :
    public CharactorComponent
{
public:
	TimeLimitComponent(std::shared_ptr<Charactor> owner, float max = 100.0f, float depletionRate = 1.0f);
	~TimeLimitComponent();
	
	void Update(float deltaTime) override;
	
	// 現在の時間を取得
	float GetTimeValue(void) const;

	// 時間を回復
	void AddTimeValue(float amount);

	// 時間が切れたかどうか
	bool IsTimeDepleted(void) const;

	// 時間の減少
	void Consume(float deltaTime);

	// 現在の時間の割合
	float GetDepletionRate(void) const;

private:
	float value_;		// 現在の値
	float maxValue_;	// 最大値
	float depletionRate_; // 消費率（秒あたりの減少量）
	bool isDepleted_;	// 値切れフラグ

};

