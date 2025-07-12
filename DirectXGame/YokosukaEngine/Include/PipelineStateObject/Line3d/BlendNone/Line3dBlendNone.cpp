#include "Line3dBlendNone.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="dxc">コンパイルシェーダ</param>
void Line3dBlendNone::Initialize(Logging* logging, DirectXShaderCompiler* dxc,
	ID3D12Device* device, IDxcBlob* vertexShaderBlob, IDxcBlob* pixelShaderBlob)
{
	BaseLine3d::Initialize(logging, dxc, device, vertexShaderBlob, pixelShaderBlob);


	/*-----------------------------
		ルートシグネチャを生成する
	-----------------------------*/


	/*   ルートパラメータ   */

	D3D12_ROOT_PARAMETER rootParameters[2] = {};

	// CBV PixeShader b0
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[0].Descriptor.ShaderRegister = 0;

	// CBV VertexShader b0
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[1].Descriptor.ShaderRegister = 0;


	/*   ルートシグネチャ   */

	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);

	// シリアライズにしてバイナリにする
	HRESULT hr = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob_, &errorBlob_);
	if (FAILED(hr))
	{
		logging_->Log(reinterpret_cast<char*>(errorBlob_->GetBufferPointer()));
		assert(false);
	}

	// バイナリを元に生成する
	hr = device->CreateRootSignature(0, signatureBlob_->GetBufferPointer(), signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));
	assert(SUCCEEDED(hr));


	/*-------------------------------
		インプットレイアウトを設定する
	-------------------------------*/

	D3D12_INPUT_ELEMENT_DESC inputElementDescs[1] = {};

	// float4 POSITION 0
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);


	/*-----------------------------
		ブレンドステートを設定する
	-----------------------------*/

	D3D12_BLEND_DESC blendDesc{};

	// 全ての色要素を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;



	/*--------------------------------
		ラスタライザステートを設定する
	--------------------------------*/

	D3D12_RASTERIZER_DESC rasterizeDesc{};

	// 裏面（時計回り）を表示しない
	rasterizeDesc.CullMode = D3D12_CULL_MODE_BACK;

	// 三角形の中を塗りつぶす
	rasterizeDesc.FillMode = D3D12_FILL_MODE_SOLID;


	/*------------------------------------
		デプスステンシルステートを設定する
	------------------------------------*/

	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};

	// デプスの機能を有効化する
	depthStencilDesc.DepthEnable = true;

	// 書き込む
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;

	// 近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;


	/*------------------
		PSOを生成する
	------------------*/

	// 設定を入れる
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = rootSignature_.Get();
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;
	graphicsPipelineStateDesc.VS = { vertexShaderBlob_->GetBufferPointer() , vertexShaderBlob_->GetBufferSize() };
	graphicsPipelineStateDesc.PS = { pixelShaderBlob_->GetBufferPointer() , pixelShaderBlob_->GetBufferSize() };
	graphicsPipelineStateDesc.BlendState = blendDesc;
	graphicsPipelineStateDesc.RasterizerState = rasterizeDesc;
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;

	// 書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	// 書き込むDSVの情報
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 利用するトポロジ（形状）のタイプ
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;

	// どのように画面に色を打ち込むか
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// 生成する
	hr = device->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&grahpicsPipelineState_));
	assert(SUCCEEDED(hr));
}