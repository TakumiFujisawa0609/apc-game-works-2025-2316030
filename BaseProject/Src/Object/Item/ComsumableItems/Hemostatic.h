#pragma once
#include "ConsumableItemBase.h"
class Hemostatic :
    public ConsumableItemBase
{
public:

    static constexpr VECTOR INIT_POS = { -500.0f, 100.0f, 100.0f };     // 初期座標
    static constexpr VECTOR INIT_SCL = { 0.05f,0.05f,0.05f };           // 初期拡大率
    static constexpr VECTOR INIT_QUAROTLOCAL = { 0.0f,-90.0f,0.0f };    // 初期ローカル回転

    static constexpr float MAX_VALUE = 10.0f;                           // 最大回復量
    static constexpr VECTOR TARGET_POS = { 25.0f,-20.0f, 50.0f };       // 調整用座標

    Hemostatic(Player& player);
    ~Hemostatic(void);

    void Init(void) override;
    void Update(float deltaTime) override;
    void Draw(void) override;
    void Use(void)override;

private:

    virtual void OnUpdate(float deltaTime) override;

    virtual void UpdateOnStage(float deltaTime) override;
    virtual void UpdateInVentory(float deltaTime) override;

    virtual void UpdateInUse(float deltaTime) override;
    virtual void UpdateUsedUp(float deltaTime) override;
};

