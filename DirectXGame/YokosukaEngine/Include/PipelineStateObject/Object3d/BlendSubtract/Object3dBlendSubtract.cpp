#include "Object3dBlendSubtract.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="dxc">コンパイルシェーダ</param>
void Object3dBlendSubtract::Initialize(Logging* logging, DirectXShaderCompiler* dxc,
	ID3D12Device* device, IDxcBlob* vertexShaderBlob, IDxcBlob* pixelShaderBlob)
{
	BaseObject3d::Initialize(logging, dxc, device, vertexShaderBlob, pixelShaderBlob);


	/*-----------------------------
		ルートシグネチャを生成する
	-----------------------------*/

	/*   ディスクリプタレンジ   */

	// テクスチャのディスクリプタテーブル
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;
	descriptorRange[0].NumDescriptors = 1;
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// 平行光源のインスタンシング
	D3D12_DESCRIPTOR_RANGE directionLightInstancing[1] = {};
	directionLightInstancing[0].BaseShaderRegister = 1;
	directionLightInstancing[0].NumDescriptors = 1;
	directionLightInstancing[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	directionLightInstancing[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// ポイントライトのインスタンシング
	D3D12_DESCRIPTOR_RANGE pointLightInstancing[1] = {};
	pointLightInstancing[0].BaseShaderRegister = 2;
	pointLightInstancing[0].NumDescriptors = 1;
	pointLightInstancing[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	pointLightInstancing[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// スポットライトのインスタンシング
	D3D12_DESCRIPTOR_RANGE spotLightInstancing[1] = {};
	spotLightInstancing[0].BaseShaderRegister = 3;
	spotLightInstancing[0].NumDescriptors = 1;
	spotLightInstancing[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	spotLightInstancing[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;


	/*   ルートパラメータ   */

	D3D12_ROOT_PARAMETER rootParameters[10] = {};

	// CBV PixeShader b0
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[0].Descriptor.ShaderRegister = 0;

	// CBV VertexShader b0
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[1].Descriptor.ShaderRegister = 0;

	// DescriptorTable PixelShader
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);

	// CBV PixelShader b1
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[3].Descriptor.ShaderRegister = 1;

	// CBV PixelShader b2
	rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[4].Descriptor.ShaderRegister = 2;

	// CBV PixelShader b3
	rootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[5].Descriptor.ShaderRegister = 3;

	// CBV PixelShader b4
	rootParameters[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[6].Descriptor.ShaderRegister = 4;

	// DescriptorTable PixelShader
	rootParameters[7].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[7].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[7].DescriptorTable.pDescriptorRanges = directionLightInstancing;
	rootParameters[7].DescriptorTable.NumDescriptorRanges = _countof(directionLightInstancing);

	// DescriptorTable PixelShader
	rootParameters[8].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[8].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[8].DescriptorTable.pDescriptorRanges = pointLightInstancing;
	rootParameters[8].DescriptorTable.NumDescriptorRanges = _countof(pointLightInstancing);

	// DescriptorTable PixelShader
	rootParameters[9].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[9].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[9].DescriptorTable.pDescriptorRanges = spotLightInstancing;
	rootParameters[9].DescriptorTable.NumDescriptorRanges = _countof(spotLightInstancing);


	/*   サンプラー   */

	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};

	// バイナリフィルタ
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;

	// 0 ~ 1 の範囲外をリピート
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;

	// 比較しない
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;

	// ありったけのミップマップを使う
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;

	// レジスタ番号0を使う
	staticSamplers[0].ShaderRegister = 0;

	// PixelShaderを使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;


	/*   ルートシグネチャ   */

	// ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

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

	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};

	// float4 POSITION 0
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	// float2 TEXCOORD 0
	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	// float3 NORMAL 0
	inputElementDescs[2].SemanticName = "NORMAL";
	inputElementDescs[2].SemanticIndex = 0;
	inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);


	/*-----------------------------
		ブレンドステートを設定する
	-----------------------------*/

	D3D12_BLEND_DESC blendDesc{};

	// 全ての色要素を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;



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
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// どのように画面に色を打ち込むか
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// 生成する
	hr = device->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&grahpicsPipelineState_));
	assert(SUCCEEDED(hr));
}