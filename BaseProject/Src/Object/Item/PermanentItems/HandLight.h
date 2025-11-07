#pragma once

class ModelMaterial;
class ModelRenderer;

#include "PermanentItemBase.h"
class HandLight :
    public PermanentItemBase
{
public:

    enum class TYPE
    {
        REGIDBODY,
        SKINING
    };

    static constexpr VECTOR INIT_POS = { -500.0f, 150.0f, 150.0f };              // 初期座標
    static constexpr VECTOR INIT_SCL = { 0.05f,0.05f,0.05f };           // 初期拡大率
    static constexpr VECTOR INIT_QUAROTLOCAL = { 0.0f,-90.0f,0.0f };    // 初期ローカル回転

    static constexpr float MAX_VALUE = 10.0f;

    // 調整用座標
    static constexpr VECTOR TARGET_POS = { 25.0f,-20.0f, 50.0f };

    HandLight(Player& player);
    ~HandLight(void);

    void Init(void) override;
    void Update(float deltaTime) override;
    void Draw(void) override;

    void InitLightRenderer(const TYPE& type, int modelId);
    void UpdateRenderer(float deltaTime);
    void DrawRenderer(void);
    void DrawUI(void);

private:

    virtual void OnUpdate(float deltaTime) override;

    virtual void UpdateOnStage(float deltaTime) override;
    virtual void UpdateInVentory(float deltaTime) override;


    virtual void UpdateInUse(float deltaTime) override;
    virtual void UpdateUsedUp(float deltaTime) override;

    // モデル描画用
    std::unique_ptr<ModelMaterial> material_;
    std::unique_ptr<ModelRenderer> renderer_;

    float value_;     // 残量
    float blinkIntensity_;// 点滅強度

    bool canToggle_;
    const float TOGGLE_COOLDOWN = 0.2f; // 0.2秒間のクールダウン
    float toggleTimer_;                 // クリック受け付け

    float blinkTimer_;
    float nextBlinkDuration_;
    bool isBlinkActive_;
    const float BLINK_MIN_DURATION = 0.05f;
    const float BLINK_MAX_DURATION = 0.3f;
};

