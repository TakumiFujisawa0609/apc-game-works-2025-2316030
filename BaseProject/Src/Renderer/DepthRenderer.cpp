#include "ModelMaterial.h"
#include "ModelRenderer.h"
#include "DepthRenderer.h"

DepthRenderer::DepthRenderer(void)
{
}

DepthRenderer::~DepthRenderer(void)
{
}

void DepthRenderer::InitLightRenderer(const TYPE& type, int modelId)
{
	// モデル描画用
	material_ = std::make_unique<ModelMaterial>(L"NomalMeshDepthMapVS.cso", 0, L"DepthMapPS.cso", 0);
	if (type == TYPE::SKINING) {
		material_ = std::make_unique<ModelMaterial>(L"SkinMeshDepthMapVS.cso", 0, L"DepthMapPS.cso", 0);
	}

	// 頂点シェーダ―へのバッファ渡し
	material_->AddConstBufVS({ 0.0f,0.0f,0.0f,1.0f });
	material_->AddConstBufVS({ 0.3f,0.3f,0.3f,1.0f });
	
	// ピクセルシェーダ―へのバッファ渡し
	material_->AddConstBufPS({ 0.3f,0.3f,0.3f,1.0f });
	material_->AddConstBufPS({ 0.0f,0.0f,0.0f,0.0f });

	renderer_ = std::make_unique<ModelRenderer>(modelId, *material_);

}

void DepthRenderer::UpdateRenderer(float deltaTime)
{
	// 頂点シェーダ―のバッファの更新
	material_->SetConstBufVS(0, { 0.0f,0.0f,0.0f,0.0f });

	material_->SetConstBufVS(1, { 0.0f,0.0f,0.0f,0.0f });

	// ピクセルシェーダーのバッファ更新
	material_->SetConstBufPS(0, { 0.0f,0.0f,0.0f,0.0f });

	material_->SetConstBufPS(1, { 0.0f,0.0f,0.0f,0.0f });
}

void DepthRenderer::DrawRenderer(void)
{
	renderer_->Draw();
}

int DepthRenderer::GetRendererDepthScreen(void)
{
	return renderer_->GetDepthScreen();
}
