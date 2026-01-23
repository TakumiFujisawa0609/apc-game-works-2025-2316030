#include "VertexMaterial.h"

VertexMaterial::VertexMaterial(std::wstring shaderFileName, int constBufFloat4Size)
    :
    shader_(-1),
    constBufFloat4Size_(-1),
    constBuf_(-1),
    texAddress_(TEXADDRESS::NONE),
    constBufs_{},
    textures_{}
{
}

VertexMaterial::~VertexMaterial(void)
{
}

void VertexMaterial::AddConstBuf(const FLOAT4& contBuf)
{
}

void VertexMaterial::SetConstBuf(int idx, const FLOAT4& contBuf)
{
}

void VertexMaterial::AddTextureBuf(int texDiffuse)
{
}

void VertexMaterial::SetTextureBuf(int idx, int texDiffuse)
{
}

int VertexMaterial::GetShader(void) const
{
    return 0;
}

int VertexMaterial::GetConstBuf(void) const
{
    return 0;
}

const std::vector<FLOAT4>& VertexMaterial::GetConstBufs(void) const
{
    return constBufs_;
}

const std::vector<int>& VertexMaterial::GetTextures(void) const
{
    return textures_;
}

VertexMaterial::TEXADDRESS VertexMaterial::GetTextureAddress(void) const
{
    return texAddress_;
}

void VertexMaterial::SetTextureAddress(TEXADDRESS texA)
{
}
