#pragma once
#include <wrl.h>
#include "../../Base/Logging/Logging.h"
#include "../../Draw/DirectXShaderCompiler/DirectXShaderCompiler.h"

class BaseCopyImage
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="dxc">シェーダコンパイラ</param>
	virtual void Initialize(Logging* logging , DirectXShaderCompiler* dxc,
		ID3D12Device* device, IDxcBlob* vertexShaderBlob, IDxcBlob* pixelShaderBlob);

	/// <summary>
	/// コマンドリストにPSOの設定を行う
	/// </summary>
	/// <param name="commandList"></param>
	void CommandListSet(ID3D12GraphicsCommandList* commandList);


protected:

	// ロギング
	Logging* logging_ = nullptr;

	// DXC
	DirectXShaderCompiler* dxc_ = nullptr;

	// 頂点シェーダのバイナリ
	IDxcBlob* vertexShaderBlob_ = nullptr;

	// ピクセルシェーダのバイナリ
	IDxcBlob* pixelShaderBlob_ = nullptr;



	// シグネチャのバイナリ
	Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob_ = nullptr;

	// エラーのバイナリ
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob_ = nullptr;

	// ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;

	// パイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> grahpicsPipelineState_ = nullptr;
};

