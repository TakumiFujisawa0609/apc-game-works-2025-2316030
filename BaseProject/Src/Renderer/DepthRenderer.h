#pragma once

#include <memory>
#include <DxLib.h>

class ModelMaterial;
class ModelRenderer;
class DepthRenderer
{
public:

	enum class TYPE
	{
		REGIDBODY,
		SKINING
	};

	DepthRenderer(void);
	~DepthRenderer(void);

	void InitLightRenderer(const TYPE& type, int modelId);
	void UpdateRenderer(float deltaTime);
	void DrawRenderer(void);

	int GetRendererDepthScreen(void);

private:

	std::unique_ptr<ModelMaterial> material_;
	std::unique_ptr<ModelRenderer> renderer_;

};

