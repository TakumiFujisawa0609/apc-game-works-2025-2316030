#pragma once
#include <memory>
#include <DxLib.h>

class ModelMaterial;
class ModelRenderer;

class HandLight;
class LightRenderer
{
public:
    enum class TYPE
    {
        REGIDBODY,
        SKINING
    };

    void InitLightRenderer(const TYPE& type, int modelId);
    void UpdateRenderer(float deltaTime);
    void DrawRenderer(void);

    void SetHandLight(std::shared_ptr<HandLight> light);
    int GetRendererDepthScreen(void);
private:

    std::unique_ptr<ModelMaterial> material_;
    std::unique_ptr<ModelRenderer> renderer_;

    std::weak_ptr<HandLight> light_;

    float blinkIntensity_;
};

