#pragma once
#include <wrl.h>
#include "../../Base/OutputLog/OutputLog.h"
#include "../../Draw/DirectXShaderCompiler/DirectXShaderCompiler.h"

class BasePrimitive
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="log">ログ出力</param>
	/// <param name="dxc">シェーダコンパイラ</param>
	virtual void Initialize(OutputLog* log, DirectXShaderCompiler* dxc, Microsoft::WRL::ComPtr<ID3D12Device> device);

	/// <summary>
	/// コマンドリストにPSOの設定を行う
	/// </summary>
	/// <param name="commandList"></param>
	void CommandListSet(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList);


protected:

	// ログ出力
	OutputLog* log_ = nullptr;

	// DXC
	DirectXShaderCompiler* dxc_ = nullptr;

	// シグネチャのバイナリ
	Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob_ = nullptr;

	// エラーのバイナリ
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob_ = nullptr;

	// ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_ = nullptr;

	// 頂点シェーダのバイナリ
	Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob_ = nullptr;

	// ピクセルシェーダのバイナリ
	Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob_ = nullptr;

	// パイプラインステート
	Microsoft::WRL::ComPtr<ID3D12PipelineState> grahpicsPipelineState_ = nullptr;
};

