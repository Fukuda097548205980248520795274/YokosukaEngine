#pragma once
#include "../PostEffect.h"

class Hide : public PostEffect
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

