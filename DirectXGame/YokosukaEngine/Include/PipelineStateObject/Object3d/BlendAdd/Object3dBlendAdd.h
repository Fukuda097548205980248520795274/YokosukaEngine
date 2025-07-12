#pragma once
#include "../BaseObject3d.h"

class Object3dBlendAdd : public BaseObject3d
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="log">ログ出力</param>
	/// <param name="dxc">シェーダコンパイラ</param>
	void Initialize(Logging* logging, DirectXShaderCompiler* dxc,
		Microsoft::WRL::ComPtr<ID3D12Device> device, IDxcBlob* vertexShaderBlob, IDxcBlob* pixelShaderBlob) override;
};

