#pragma once
#include "../BaseCopyImage.h"

class CopyImageBlendScreen : public BaseCopyImage
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="log">ログ出力</param>
	/// <param name="dxc">シェーダコンパイラ</param>
	void Initialize(OutputLog* log, DirectXShaderCompiler* dxc,
		Microsoft::WRL::ComPtr<ID3D12Device> device, IDxcBlob* vertexShaderBlob, IDxcBlob* pixelShaderBlob) override;
};

