#pragma once
#include "../BaseObject3d.h"

class Object3dBlendAdd : public BaseObject3d
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Object3dBlendAdd();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="log">ログ出力</param>
	/// <param name="dxc">シェーダコンパイラ</param>
	void Initialize(OutputLog* log, DirectXShaderCompiler* dxc,
		Microsoft::WRL::ComPtr<ID3D12Device> device, IDxcBlob* vertexShaderBlob, IDxcBlob* pixelShaderBlob) override;
};

