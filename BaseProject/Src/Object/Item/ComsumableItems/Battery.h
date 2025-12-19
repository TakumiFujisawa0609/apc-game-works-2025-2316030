#pragma once

class HandLight;
#include "ConsumableItemBase.h"
class Battery :
    public ConsumableItemBase
{
public:

    // 初期座標
    static constexpr VECTOR INIT_POS = { -500.0f, 150.0f, 200.0f };

    // 初期拡大率
    static constexpr VECTOR INIT_SCL = { 0.09f,0.09f,0.09f };

    // 初期ローカル回転
    static constexpr VECTOR INIT_QUAROTLOCAL = { 0.0f,-90.0f,0.0f };
    
    // 回復量
    static constexpr float MAX_VALUE = 10.0f;

    // 調整用座標
    static constexpr VECTOR TARGET_POS = { 25.0f,-20.0f, 50.0f };

    // 回復量
    static constexpr float BATTERY_RECOVERY = 100;

    Battery(Player& player);
    ~Battery(void);

    void Init(void) override;
    void Update(float deltaTime) override;
    void Draw(void) override;
    void Use(void)override;

    // ハンドライトの設定
    void SetHandLight(std::shared_ptr<HandLight> light);

private:

    // 循環参照を防ぐためにweak_ptrで定義
    std::weak_ptr<HandLight> hLight_;

    virtual void OnUpdate(float deltaTime) override;
    virtual void UpdateInVentory(float deltaTime) override;
    virtual void UpdateInUse(float deltaTime) override;
    virtual void UpdateUsedUp(float deltaTime) override;

};

