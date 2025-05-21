#pragma once
#include "../BasePipelineStateObject.h"

class PipelineStateObjectNormalModel : public BasePipelineStateObject
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PipelineStateObjectNormalModel();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="log">ログ出力</param>
	/// <param name="dxc">シェーダコンパイラ</param>
	void Initialize(OutputLog* log, DirectXShaderCompiler* dxc, Microsoft::WRL::ComPtr<ID3D12Device> device) override;
};

