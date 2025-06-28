#pragma once
#include "../BasePrimitive.h"

class PrimitiveBlendMultiply : public BasePrimitive
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PrimitiveBlendMultiply();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="log">ログ出力</param>
	/// <param name="dxc">シェーダコンパイラ</param>
	void Initialize(OutputLog* log, DirectXShaderCompiler* dxc, Microsoft::WRL::ComPtr<ID3D12Device> device) override;
};

