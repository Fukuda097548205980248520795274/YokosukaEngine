#pragma once
#include "../BasePrimitive.h"

class PrimitiveBlendScreen : public BasePrimitive
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="log">ログ出力</param>
	/// <param name="dxc">シェーダコンパイラ</param>
	void Initialize(Logging* logging, DirectXShaderCompiler* dxc,
		ID3D12Device* device, IDxcBlob* vertexShaderBlob, IDxcBlob* pixelShaderBlob) override;
};

