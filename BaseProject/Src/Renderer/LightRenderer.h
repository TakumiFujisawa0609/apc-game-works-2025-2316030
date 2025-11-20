#pragma once
#include <memory>
#include <DxLib.h>

class ModelMaterial;
class ModelRenderer;

class HandLight;
class LightRenderer
{
public:

    static constexpr float OUT_ANGLE = 0.5f;
    static constexpr float IN_ANGLE = 0.15f;
    static constexpr float RANGE = 1000.0f;
    static constexpr float ATTEN0 = 0.5f;
    static constexpr float ATTEN1 = 0.0005f;
    static constexpr float ATTEN2 = 0.0f;

    enum class TYPE
    {
        REGIDBODY,
        SKINING
    };

    LightRenderer(void);
    ~LightRenderer(void);

    void InitLightRenderer(const TYPE& type, int modelId);
    void UpdateRenderer(float deltaTime, bool isActive);
    void DrawRenderer(void);

    void SetHandLight(HandLight* light);
    int GetRendererDepthScreen(void);

private:

    std::unique_ptr<ModelMaterial> material_;
    std::unique_ptr<ModelRenderer> renderer_;

    HandLight* light_;

    float blinkIntensity_;

    float blinkTimer_;
    float nextBlinkDuration_;
    bool isBlinkActive_;
    const float BLINK_MIN_DURATION = 0.05f;
    const float BLINK_MAX_DURATION = 0.3f;
};

