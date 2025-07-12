#include "PostEffect.h"


/// <summary>
/// 初期化
/// </summary>
/// <param name="dxc">シェーダコンパイラ</param>
void PostEffect::Initialize(Logging* logging, DirectXShaderCompiler* dxc,
	ID3D12Device* device, IDxcBlob* vertexShaderBlob, IDxcBlob* pixelShaderBlob)
{
	// nullptrチェック
	assert(logging);
	assert(dxc);
	assert(vertexShaderBlob);
	assert(pixelShaderBlob);

	logging_ = logging;
	dxc_ = dxc;
	vertexShaderBlob_ = vertexShaderBlob;
	pixelShaderBlob_ = pixelShaderBlob;
}

/// <summary>
/// コマンドリストにPSOの設定を行う
/// </summary>
/// <param name="commandList"></param>
void PostEffect::CommandListSet(ID3D12GraphicsCommandList* commandList)
{
	// ルートシグネチャの設定
	commandList->SetGraphicsRootSignature(rootSignature_.Get());

	// PSOの設定
	commandList->SetPipelineState(grahpicsPipelineState_.Get());
}