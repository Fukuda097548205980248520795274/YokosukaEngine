#include "BaseCopyImage.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="dxc">シェーダコンパイラ</param>
void BaseCopyImage::Initialize(OutputLog* log, DirectXShaderCompiler* dxc,
	Microsoft::WRL::ComPtr<ID3D12Device> device, IDxcBlob* vertexShaderBlob, IDxcBlob* pixelShaderBlob)
{
	// nullptrチェック
	assert(log);
	assert(dxc);
	assert(vertexShaderBlob);
	assert(pixelShaderBlob);

	log_ = log;
	dxc_ = dxc;
	vertexShaderBlob_ = vertexShaderBlob;
	pixelShaderBlob_ = pixelShaderBlob;
}

/// <summary>
/// コマンドリストにPSOの設定を行う
/// </summary>
/// <param name="commandList"></param>
void BaseCopyImage::CommandListSet(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList)
{
	// ルートシグネチャの設定
	commandList->SetGraphicsRootSignature(rootSignature_.Get());

	// PSOの設定
	commandList->SetPipelineState(grahpicsPipelineState_.Get());
}