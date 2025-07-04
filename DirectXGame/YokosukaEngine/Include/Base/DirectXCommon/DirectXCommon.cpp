#include "DirectXCommon.h"
#include "../WinApp/WinApp.h"

/// <summary>
/// デストラクタ
/// </summary>
DirectXCommon::~DirectXCommon()
{
	// ImGuiの終了処理
	ImGui_ImplWin32_Shutdown();
	ImGui_ImplDX12_Shutdown();
	ImGui::DestroyContext();

	delete copyImage_;

	// PSO
	for (uint32_t i = 0; i < kBlendModekCountOfBlendMode; i++)
	{
		delete psoObject3d_[i];
		delete psoParticle_[i];
		delete psoLine3d_[i];
		delete psoPrimitive_[i];
	}

	// DXC
	delete dxc_;

	// イベント
	CloseHandle(fenceEvent_);

	// モデルデータストア
	delete modelDataStore_;

	// テクスチャストア
	delete textureStore_;
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="log">ログ</param>
void DirectXCommon::Initialize(OutputLog* log, WinApp* windowApplication)
{
	// 乱数生成器の初期化
	std::random_device seedGenerater;
	std::mt19937 randomEngine(seedGenerater());

	//nullptrチェック
	assert(log);
	assert(windowApplication);

	// 引数を受け取る
	log_ = log;
	windowApplication_ = windowApplication;

	// テクスチャストアを初期化する
	textureStore_ = new TextureStore();
	textureStore_->Initialize(this);

	// モデルデータストアを初期化する
	modelDataStore_ = new ModelDataStore();
	modelDataStore_->Initialize(textureStore_);

	// デバッグレイヤーを有効化する
	ActiveDebugLayer();

	// DXGIファクトリーを生成する
	GenerateDXGIFactory();

	// 使用するアダプタを取得する
	SelectUseAdapter();

	// 採用したアダプタでデバイスを生成する
	GenerateDevice();

	// コマンドキューを生成する
	GenerateCommandQueue();

	// コマンドアロケータを生成する
	GenerateCommandAllocator();

	// コマンドリストを生成する
	GenerateCommandList();

	// エラーと警告で停止させる
	StopOnErrorsAndWarnings();

	// RTV用ディスクリプタヒープを生成する
	rtvDescriptorHeap_ = CreateDescritprHeap(device_, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, kMaxNumRtvDescriptors, false);

	// SRV用ディスクリプタヒープを生成する
	srvDescriptorHeap_ = CreateDescritprHeap(device_, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, kMaxNumSrvDescriptors, true);

	// DSV用ディスクリプタヒープを生成する
	dsvDescriptorHeap_ = CreateDescritprHeap(device_, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, kMaxNunDsvDescriptors, false);

	// スワップチェーンを生成する
	GenerateSwapChain();

	// スワップチェーンからリソースを引っ張ってくる
	PullSwapChainResource();

	// RTVを作り、リソースを格納する
	GenerateRTV();

	// 深度情報のリソースを生成する
	depthStencilResource_ = CreateDepthStencilTextureResource(device_, windowApplication_->GetWindowWidth(), windowApplication_->GetWindowHeight());

	// DSVを作り、リソースを格納する
	GenerateDSV();

	// フェンスを生成する
	GenerateFence();

	// イベントを生成する
	GenerateEvent();

	// DXCの生成と初期化
	dxc_ = new DirectXShaderCompiler();
	dxc_->Initialize(log_);


	// Object3Dのシェーダをコンパイルする
	object3dVertexShaderBlob_ = dxc_->CompileShader(L"YokosukaEngine/Shader/Object3d.VS.hlsl", L"vs_6_0");
	assert(object3dVertexShaderBlob_ != nullptr);
	object3dPixelShaderBlob_ = dxc_->CompileShader(L"YokosukaEngine/Shader/Object3d.PS.hlsl", L"ps_6_0");
	assert(object3dPixelShaderBlob_ != nullptr);

	// Object3D用のPSOの生成と初期化
	psoObject3d_[kBlendModeNone] = new Object3dBlendNone();
	psoObject3d_[kBlendModeNone]->Initialize(log_, dxc_, device_, object3dVertexShaderBlob_.Get(), object3dPixelShaderBlob_.Get());

	psoObject3d_[kBlendModeNormal] = new Object3dBlendNormal();
	psoObject3d_[kBlendModeNormal]->Initialize(log_, dxc_, device_ ,object3dVertexShaderBlob_.Get(), object3dPixelShaderBlob_.Get());

	psoObject3d_[kBlendModeAdd] = new Object3dBlendAdd();
	psoObject3d_[kBlendModeAdd]->Initialize(log_, dxc_, device_, object3dVertexShaderBlob_.Get(), object3dPixelShaderBlob_.Get());

	psoObject3d_[kBlendModeSubtract] = new Object3dBlendSubtract();
	psoObject3d_[kBlendModeSubtract]->Initialize(log_, dxc_, device_, object3dVertexShaderBlob_.Get(), object3dPixelShaderBlob_.Get());

	psoObject3d_[kBlendModeMultiply] = new Object3dBlendMultiply();
	psoObject3d_[kBlendModeMultiply]->Initialize(log_, dxc_, device_, object3dVertexShaderBlob_.Get(), object3dPixelShaderBlob_.Get());

	psoObject3d_[kBlendModeScreen] = new Object3dBlendScreen();
	psoObject3d_[kBlendModeScreen]->Initialize(log_, dxc_, device_, object3dVertexShaderBlob_.Get(), object3dPixelShaderBlob_.Get());


	// Particleのシェーダをコンパイルする
	particleVertexShaderBlob_ = dxc_->CompileShader(L"YokosukaEngine/Shader/Particle.VS.hlsl", L"vs_6_0");
	assert(particleVertexShaderBlob_ != nullptr);
	particlePixelShaderBlob_ = dxc_->CompileShader(L"YokosukaEngine/Shader/Particle.PS.hlsl", L"ps_6_0");
	assert(particlePixelShaderBlob_ != nullptr);

	// Particle用のPSOの生成と初期化
	psoParticle_[kBlendModeNone] = new ParticleBlendNone();
	psoParticle_[kBlendModeNone]->Initialize(log_, dxc_, device_, particleVertexShaderBlob_.Get() , particlePixelShaderBlob_.Get());

	psoParticle_[kBlendModeNormal] = new ParticleBlendNormal();
	psoParticle_[kBlendModeNormal]->Initialize(log_, dxc_, device_, particleVertexShaderBlob_.Get(), particlePixelShaderBlob_.Get());

	psoParticle_[kBlendModeAdd] = new ParticleBlendAdd();
	psoParticle_[kBlendModeAdd]->Initialize(log_, dxc_, device_, particleVertexShaderBlob_.Get(), particlePixelShaderBlob_.Get());

	psoParticle_[kBlendModeSubtract] = new ParticleBlendSubtract();
	psoParticle_[kBlendModeSubtract]->Initialize(log_, dxc_, device_, particleVertexShaderBlob_.Get(), particlePixelShaderBlob_.Get());

	psoParticle_[kBlendModeMultiply] = new ParticleBlendMultiply();
	psoParticle_[kBlendModeMultiply]->Initialize(log_, dxc_, device_, particleVertexShaderBlob_.Get(), particlePixelShaderBlob_.Get());

	psoParticle_[kBlendModeScreen] = new ParticleBlendScreen();
	psoParticle_[kBlendModeScreen]->Initialize(log_, dxc_, device_, particleVertexShaderBlob_.Get(), particlePixelShaderBlob_.Get());



	// Line3dのシェーダをコンパイルする
	line3dVertexShaderBlob_ = dxc_->CompileShader(L"YokosukaEngine/Shader/Line3d.VS.hlsl", L"vs_6_0");
	assert(line3dVertexShaderBlob_ != nullptr);
	line3dPixelShaderBlob_ = dxc_->CompileShader(L"YokosukaEngine/Shader/Line3d.PS.hlsl", L"ps_6_0");
	assert(line3dPixelShaderBlob_ != nullptr);
	
	// Line3d用のPSOの生成と初期化
	psoLine3d_[kBlendModeNone] = new Line3dBlendNone();
	psoLine3d_[kBlendModeNone]->Initialize(log_, dxc_, device_,line3dVertexShaderBlob_.Get(),line3dPixelShaderBlob_.Get());

	psoLine3d_[kBlendModeNormal] = new Line3dBlendNormal();
	psoLine3d_[kBlendModeNormal]->Initialize(log_, dxc_, device_, line3dVertexShaderBlob_.Get(), line3dPixelShaderBlob_.Get());

	psoLine3d_[kBlendModeAdd] = new Line3dBlendAdd();
	psoLine3d_[kBlendModeAdd]->Initialize(log_, dxc_, device_, line3dVertexShaderBlob_.Get(), line3dPixelShaderBlob_.Get());

	psoLine3d_[kBlendModeSubtract] = new Line3dBlendSubtract();
	psoLine3d_[kBlendModeSubtract]->Initialize(log_, dxc_, device_, line3dVertexShaderBlob_.Get(), line3dPixelShaderBlob_.Get());

	psoLine3d_[kBlendModeMultiply] = new Line3dBlendMultiply();
	psoLine3d_[kBlendModeMultiply]->Initialize(log_, dxc_, device_, line3dVertexShaderBlob_.Get(), line3dPixelShaderBlob_.Get());

	psoLine3d_[kBlendModeScreen] = new Line3dBlendScreen();
	psoLine3d_[kBlendModeScreen]->Initialize(log_, dxc_, device_, line3dVertexShaderBlob_.Get(), line3dPixelShaderBlob_.Get());


	// Primitiveのシェーダをコンパイルする
	primitiveVertexShaderBlob_ = dxc_->CompileShader(L"YokosukaEngine/Shader/Primitive.VS.hlsl", L"vs_6_0");
	assert(primitiveVertexShaderBlob_ != nullptr);
	primitivePixelShaderBlob_ = dxc_->CompileShader(L"YokosukaEngine/Shader/Primitive.PS.hlsl", L"ps_6_0");
	assert(primitivePixelShaderBlob_ != nullptr);

	// Primitive用のPSOの生成と初期化
	psoPrimitive_[kBlendModeNone] = new PrimitiveBlendNone();
	psoPrimitive_[kBlendModeNone]->Initialize(log_, dxc_, device_, primitiveVertexShaderBlob_.Get(), primitivePixelShaderBlob_.Get());

	psoPrimitive_[kBlendModeNormal] = new PrimitiveBlendNormal();
	psoPrimitive_[kBlendModeNormal]->Initialize(log_, dxc_, device_, primitiveVertexShaderBlob_.Get(), primitivePixelShaderBlob_.Get());

	psoPrimitive_[kBlendModeAdd] = new PrimitiveBlendAdd();
	psoPrimitive_[kBlendModeAdd]->Initialize(log_, dxc_, device_, primitiveVertexShaderBlob_.Get(), primitivePixelShaderBlob_.Get());

	psoPrimitive_[kBlendModeSubtract] = new PrimitiveBlendSubtract();
	psoPrimitive_[kBlendModeSubtract]->Initialize(log_, dxc_, device_, primitiveVertexShaderBlob_.Get(), primitivePixelShaderBlob_.Get());

	psoPrimitive_[kBlendModeMultiply] = new PrimitiveBlendMultiply();
	psoPrimitive_[kBlendModeMultiply]->Initialize(log_, dxc_, device_, primitiveVertexShaderBlob_.Get(), primitivePixelShaderBlob_.Get());

	psoPrimitive_[kBlendModeScreen] = new PrimitiveBlendScreen();
	psoPrimitive_[kBlendModeScreen]->Initialize(log_, dxc_, device_, primitiveVertexShaderBlob_.Get(), primitivePixelShaderBlob_.Get());


	// CopyImageのシェーダをコンパイルする
	copyImageVertexShaderBlob_ = dxc_->CompileShader(L"YokosukaEngine/Shader/CopyImage.VS.hlsl", L"vs_6_0");
	assert(primitiveVertexShaderBlob_ != nullptr);
	copyImagePixelShaderBlob_ = dxc_->CompileShader(L"YokosukaEngine/Shader/CopyImage.PS.hlsl", L"ps_6_0");
	assert(primitivePixelShaderBlob_ != nullptr);
	copyImage_ = new CopyImagePipeline();
	copyImage_->Initialize(log_, dxc_, device_, copyImageVertexShaderBlob_.Get(), copyImagePixelShaderBlob_.Get());



	// ビューポート
	viewport_.Width = static_cast<float>(windowApplication_->GetWindowWidth());
	viewport_.Height = static_cast<float>(windowApplication_->GetWindowHeight());
	viewport_.TopLeftX = 0;
	viewport_.TopLeftY = 0;
	viewport_.MinDepth = 0.0f;
	viewport_.MaxDepth = 1.0f;

	// シザーレクト
	scissorRect_.left = 0;
	scissorRect_.right = static_cast<LONG>(windowApplication_->GetWindowWidth());
	scissorRect_.top = 0;
	scissorRect_.bottom = static_cast<LONG>(windowApplication_->GetWindowHeight());

	// ImGuiを初期化する
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(windowApplication_->GetHwnd());
	ImGui_ImplDX12_Init(device_.Get(), swapChainDesc_.BufferCount, rtvDesc_.Format,
		srvDescriptorHeap_.Get(), srvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart(), srvDescriptorHeap_->GetGPUDescriptorHandleForHeapStart());


	/*----------------------------
	    オフスクリーンレンダリング
	----------------------------*/

	// レンダーテクスチャを作成する
	const Vector4 kRenderTargetClearColor = { 1.0f, 0.0f, 0.0f, 1.0f };
	renderTextureResource_ = CreateRenderTextureResource(device_, (uint32_t)windowApplication_->GetWindowWidth(), (uint32_t)windowApplication_->GetWindowHeight(),
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, kRenderTargetClearColor);
	renderTextureRtvCPUHnalde_ = GetCPUDescriptorHandle(rtvDescriptorHeap_, device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV), 2);
	device_->CreateRenderTargetView(renderTextureResource_.Get(), &rtvDesc_, renderTextureRtvCPUHnalde_);


	// レンダーテクスチャを読み込むSRVを作成する
	D3D12_SHADER_RESOURCE_VIEW_DESC renderTextureSrvDesc{};
	renderTextureSrvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	renderTextureSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	renderTextureSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	renderTextureSrvDesc.Texture2D.MipLevels = 1;

	renderTextureSrvCPUHandle_ = 
		GetCPUDescriptorHandle(srvDescriptorHeap_, device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV), GetNumSrvCPUDescriptors());
	CountNumSrvCPUDescriptors();

	renderTextureSrvGPUHandle_ =
		GetGPUDescriptorHandle(srvDescriptorHeap_, device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV), GetNumSrvGPUDescriptors());
	CountNumSrvGPUDescriptors();

	device_->CreateShaderResourceView(renderTextureResource_.Get(), &renderTextureSrvDesc, renderTextureSrvCPUHandle_);


	/*-----------------------------
	    バッファリソースを作成する
	-----------------------------*/

	// インデックスリソース
	indexResourcePlane_ = CreateBufferResource(device_, sizeof(uint32_t) * 6);
	indexResourceSphere_ = CreateBufferResource(device_, (sizeof(uint32_t) * (kSphereMaxSubdivisions * kSphereMaxSubdivisions)) * 6);
	indexResourceRing_ = CreateBufferResource(device_, sizeof(uint32_t) * (kRingMaxSubdivisions * 6));
	indexResourceCylinder_ = CreateBufferResource(device_, sizeof(uint32_t) * (kCylinderMaxSubdivisions * 6));
	indexResourceSprite_ = CreateBufferResource(device_, sizeof(uint32_t) * 6);

	// 頂点リソース
	vertexBufferResourcePlane_ = CreateBufferResource(device_, sizeof(VertexData) * 4);
	vertexBufferResourceSphere_ = CreateBufferResource(device_ ,sizeof(VertexData) * (kSphereMaxSubdivisions * kSphereMaxSubdivisions) * 4);
	vertexBufferResourceRing_ = CreateBufferResource(device_, sizeof(VertexData) * (kRingMaxSubdivisions * 4));
	vertexBufferResourceCylinder_ = CreateBufferResource(device_, sizeof(VertexData) * (kCylinderMaxSubdivisions * 4));
	vertexBufferResourceSprite_ = CreateBufferResource(device_, sizeof(VertexData) * 4);


	for (uint32_t i = 0; i < kMaxNumResource; i++)
	{
		// 平面
		materialResourcePlane_[i] = CreateBufferResource(device_, sizeof(Material));
		transformationResourcePlane_[i] = CreateBufferResource(device_, sizeof(TransformationMatrix));
		cameraResourcePlane_[i] = CreateBufferResource(device_, sizeof(CameraForGPU));

		// 球
		materialResourceSphere_[i] = CreateBufferResource(device_, sizeof(Material));
		transformationResourceSphere_[i] = CreateBufferResource(device_, sizeof(TransformationMatrix));
		cameraResourceSphere_[i] = CreateBufferResource(device_, sizeof(CameraForGPU));

		// リング
		materialResourceRing_[i] = CreateBufferResource(device_, sizeof(Material));
		transformationResourceRing_[i] = CreateBufferResource(device_, sizeof(TransformationMatrix));
		cameraResourceRing_[i] = CreateBufferResource(device_, sizeof(CameraForGPU));

		// 円柱
		materialResourceCylinder_[i] = CreateBufferResource(device_, sizeof(Material));
		transformationResourceCylinder_[i] = CreateBufferResource(device_, sizeof(TransformationMatrix));
		cameraResourceCylinder_[i] = CreateBufferResource(device_, sizeof(CameraForGPU));

		// モデル
		materialResourceModel_[i] = CreateBufferResource(device_, sizeof(Material));
		transformationResourceModel_[i] = CreateBufferResource(device_, sizeof(TransformationMatrix));
		cameraResourceModel_[i] = CreateBufferResource(device_, sizeof(CameraForGPU));

		// スプライト
		materialResourceSprite_[i] = CreateBufferResource(device_, sizeof(Material));
		transformationResourceSprite_[i] = CreateBufferResource(device_, sizeof(TransformationMatrix));

		// 線
		vertexBufferResourceLine_[i] = CreateBufferResource(device_, sizeof(Vector4) * 2);
		materialResourceLine_[i] = CreateBufferResource(device_, sizeof(Material));
		transformationResourceLine_[i] = CreateBufferResource(device_, sizeof(TransformationMatrix));
	}


	/*-------------
	    平行光源
	-------------*/

	// リソース
	instancingDirectionalLightResource_ = CreateBufferResource(device_, sizeof(DirectionalLightForGPU) * kMaxNumDirectionalLight);
	instancingDirectionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));

	useNumDirectionalLightResource_ = CreateBufferResource(device_, sizeof(UseNumDirectionalLight));
	useNumDirectionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&useNumDirectionLightData_));
	useNumDirectionLightData_->num = 0;

	// ビュー
	D3D12_SHADER_RESOURCE_VIEW_DESC instancingDirectionalLightSrvDesc{};
	instancingDirectionalLightSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	instancingDirectionalLightSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	instancingDirectionalLightSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	instancingDirectionalLightSrvDesc.Buffer.FirstElement = 0;
	instancingDirectionalLightSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	instancingDirectionalLightSrvDesc.Buffer.NumElements = kMaxNumDirectionalLight;
	instancingDirectionalLightSrvDesc.Buffer.StructureByteStride = sizeof(DirectionalLightForGPU);

	// ポインタのハンドル（住所）を取得する
	instancingDirectionalLightSrvHandleCPU_ = 
		GetCPUDescriptorHandle(srvDescriptorHeap_, device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV), GetNumSrvCPUDescriptors());
	CountNumSrvCPUDescriptors();

	instancingDirectionalLightSrvHandleGPU_ = 
		GetGPUDescriptorHandle(srvDescriptorHeap_, device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV), GetNumSrvGPUDescriptors());
	CountNumSrvGPUDescriptors();

	device_->CreateShaderResourceView(instancingDirectionalLightResource_.Get(), &instancingDirectionalLightSrvDesc, instancingDirectionalLightSrvHandleCPU_);


	/*------------------
	    ポイントライト
	------------------*/

	// リソース
	instancingPointLightResource_ = CreateBufferResource(device_, sizeof(PointLightForGPU) * kMaxNumPointLight);
	instancingPointLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&pointLightData_));

	useNumPointLightResource_ = CreateBufferResource(device_, sizeof(UseNumPointLight));
	useNumPointLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&useNumPointLightData_));
	useNumPointLightData_->num = 0;

	// ビュー
	D3D12_SHADER_RESOURCE_VIEW_DESC instancingPointLightSrvDesc{};
	instancingPointLightSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	instancingPointLightSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	instancingPointLightSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	instancingPointLightSrvDesc.Buffer.FirstElement = 0;
	instancingPointLightSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	instancingPointLightSrvDesc.Buffer.NumElements = kMaxNumPointLight;
	instancingPointLightSrvDesc.Buffer.StructureByteStride = sizeof(PointLightForGPU);

	// ポインタのハンドル（住所）を取得する
	instancingPointLightSrvHandleCPU_ = 
		GetCPUDescriptorHandle(srvDescriptorHeap_, device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV), GetNumSrvCPUDescriptors());
	CountNumSrvCPUDescriptors();

	instancingPointLightSrvHandleGPU_ = 
		GetGPUDescriptorHandle(srvDescriptorHeap_, device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV), GetNumSrvGPUDescriptors());
	CountNumSrvGPUDescriptors();

	device_->CreateShaderResourceView(instancingPointLightResource_.Get(), &instancingPointLightSrvDesc, instancingPointLightSrvHandleCPU_);


	/*------------------
	    スポットライト
	------------------*/

	// リソース
	instancingSpotLightResource_ = CreateBufferResource(device_, sizeof(SpotLightForGPU) * kMaxNumSpotLight);
	instancingSpotLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&spotLightData_));

	useNumSpotLightResource_ = CreateBufferResource(device_, sizeof(UseNumSpotLight));
	useNumSpotLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&useNumSpotLightData_));
	useNumSpotLightData_->num = 0;

	// ビュー
	D3D12_SHADER_RESOURCE_VIEW_DESC instancingSpotLightSrvDesc{};
	instancingSpotLightSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	instancingSpotLightSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	instancingSpotLightSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	instancingSpotLightSrvDesc.Buffer.FirstElement = 0;
	instancingSpotLightSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	instancingSpotLightSrvDesc.Buffer.NumElements = kMaxNumSpotLight;
	instancingSpotLightSrvDesc.Buffer.StructureByteStride = sizeof(SpotLightForGPU);

	// ポインタのハンドル（住所）を取得する
	instancingSpotLightSrvHandleCPU_ = 
		GetCPUDescriptorHandle(srvDescriptorHeap_, device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV), GetNumSrvCPUDescriptors());
	CountNumSrvCPUDescriptors();

	instancingSpotLightSrvHandleGPU_ = 
		GetGPUDescriptorHandle(srvDescriptorHeap_, device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV), GetNumSrvGPUDescriptors());
	CountNumSrvGPUDescriptors();

	device_->CreateShaderResourceView(instancingSpotLightResource_.Get(), &instancingSpotLightSrvDesc, instancingSpotLightSrvHandleCPU_);


	/*----------------
	    パーティクル
	----------------*/

	// パーティクル
	instancingResourcesParticle_ = CreateBufferResource(device_, sizeof(ParticleForGPU) * kNumMaxInstance);
	materialResourceParticle_ = CreateBufferResource(device_, sizeof(Material));

	// パーティクルのビュー
	D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc{};
	instancingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	instancingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	instancingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	instancingSrvDesc.Buffer.FirstElement = 0;
	instancingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	instancingSrvDesc.Buffer.NumElements = kNumMaxInstance;
	instancingSrvDesc.Buffer.StructureByteStride = sizeof(ParticleForGPU);

	// ポインタのハンドル（住所）を取得する
	instancingSrvHandleCPU_ = 
		GetCPUDescriptorHandle(srvDescriptorHeap_, device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV), GetNumSrvCPUDescriptors());
	CountNumSrvCPUDescriptors();

	instancingSrvHandleGPU_ = 
		GetGPUDescriptorHandle(srvDescriptorHeap_, device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV), GetNumSrvGPUDescriptors());
	CountNumSrvGPUDescriptors();


	device_->CreateShaderResourceView(instancingResourcesParticle_.Get(), &instancingSrvDesc, instancingSrvHandleCPU_);

	// エミッター
	emitter_.count = 3;
	emitter_.frequency = 0.5f;
	emitter_.frequencyTime = 0.0f;
	emitter_.transform.scale = { 1.0f , 1.0f , 1.0f };
	emitter_.transform.rotation = { 0.0f , 0.0f , 0.0f };
	emitter_.transform.translation = { 0.0f , 0.0f , 0.0f };

	// 場所
	accelerationFeild_.acceleration = { 15.0f , 0.0f , 0.0f };
	accelerationFeild_.area.min = { -1.0f , -1.0f , -1.0f };
	accelerationFeild_.area.max = { 1.0f , 1.0f , 1.0f };
}

/// <summary>
/// 描画前処理
/// </summary>
void DirectXCommon::PreDraw()
{
	// ImGuiに新しいフレームであることを通知する
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();



	// 描画先のRTVとDSVを設定する
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	commandList_->OMSetRenderTargets(1, &renderTextureRtvCPUHnalde_, false, &dsvHandle);

	// 指定した色で画面全体をクリアする
	float clearColor[] = { 1.0f , 0.0f , 0.0f , 1.0f };
	commandList_->ClearRenderTargetView(renderTextureRtvCPUHnalde_, clearColor, 0, nullptr);

	// 指定した深度で画面全体をクリアする
	commandList_->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// 描画用のディスクリプタヒープの設定
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeaps[] = { srvDescriptorHeap_ };
	commandList_->SetDescriptorHeaps(1, descriptorHeaps->GetAddressOf());

	// ビューポート設定
	commandList_->RSSetViewports(1, &viewport_);

	// シザーレクト設定
	commandList_->RSSetScissorRects(1, &scissorRect_);



	// 使用したブレンドモードを初期化する
	useObject3dBlendMode_ = kBlendModeNormal;
	useParticleBlendMode_ = kBlendModeAdd;
	useLine3dBlendMode_ = kBlendModeNormal;
	usePrimitiveBlendMode_ = kBlendModeNormal;
}

/// <summary>
/// 描画後処理
/// </summary>
void DirectXCommon::PostDraw()
{
	// ImGuiの内部コマンドを生成する
	ImGui::Render();


	// これから書き込むバックバッファのインデックスを取得
	UINT backBuffeIndex = swapChain_->GetCurrentBackBufferIndex();

	// Present -> RenderTarget
	ChangeResourceState(commandList_, swapChainResources_[backBuffeIndex], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	// 描画先のRTVとDSVを設定する
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	commandList_->OMSetRenderTargets(1, &rtvHandles_[backBuffeIndex], false, &dsvHandle);

	// 指定した色で画面全体をクリアする
	float clearColor[] = { 0.1f , 0.1f , 0.1f , 1.0f };
	commandList_->ClearRenderTargetView(rtvHandles_[backBuffeIndex], clearColor, 0, nullptr);

	// 描画用のディスクリプタヒープの設定
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeaps[] = { srvDescriptorHeap_ };
	commandList_->SetDescriptorHeaps(1, descriptorHeaps->GetAddressOf());

	// ビューポート設定
	commandList_->RSSetViewports(1, &viewport_);

	// シザーレクト設定
	commandList_->RSSetScissorRects(1, &scissorRect_);



	// RenderTarget -> PixelShader
	ChangeResourceState(commandList_, renderTextureResource_, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);



	// PixelShader -> RenderTarget
	ChangeResourceState(commandList_, renderTextureResource_, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);



	// ImGuiの描画コマンドを積む
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList_.Get());

	// RenderTarget -> Present
	ChangeResourceState(commandList_, swapChainResources_[backBuffeIndex], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

	// コマンドリストの内容を確定させる
	HRESULT hr = commandList_->Close();
	assert(SUCCEEDED(hr));

	// GPUにコマンドリストの実行を行わせる
	Microsoft::WRL::ComPtr<ID3D12CommandList> commandLists[] = { commandList_ };
	commandQueue_->ExecuteCommandLists(1, commandLists->GetAddressOf());

	// GPUとOSに画面の交換を行うように通知する
	swapChain_->Present(1, 0);

	// GPUの実行完了を待つ
	WaitForGPU();

	// 次のフレーム用のコマンドリストを準備
	hr = commandAllocator_->Reset();
	assert(SUCCEEDED(hr));
	hr = commandList_->Reset(commandAllocator_.Get(), nullptr);
	assert(SUCCEEDED(hr));

	// カウントしたリソースを初期化する
	useNumResourcePlane_ = 0;
	useNumResourceSphere_ = 0;
	useNumResourceRing_ = 0;
	useNumResourceCylinder_ = 0;
	useNumResourceModel_ = 0;
	useNumResourceSprite_ = 0;
	useNumResourceLine_ = 0;

	// ライト
	useNumDirectionalLightCount_ = 0;
	useNumDirectionLightData_->num = useNumDirectionalLightCount_;
	useNumPointLightCount_ = 0;
	useNumPointLightData_->num = useNumPointLightCount_;
	useNumSpotLightCount_ = 0;
	useNumSpotLightData_->num = useNumSpotLightCount_;
}

/// <summary>
/// 平行光源を設置する
/// </summary>
/// <param name="directionalLight"></param>
void DirectXCommon::SetDirectionalLight(const DirectionalLight* directionalLight)
{
	// 最大数を越えないようにする
	if (useNumDirectionalLightCount_ >= kMaxNumDirectionalLight)
		return;

	// 色
	directionalLightData_[useNumDirectionalLightCount_].color = directionalLight->color_ * directionalLight->intensity_;

	// 向き
	directionalLightData_[useNumDirectionalLightCount_].direction = Normalize(directionalLight->direction_);


	// カウントする
	useNumDirectionalLightCount_++;
	useNumDirectionLightData_->num = useNumDirectionalLightCount_;
}

/// <summary>
/// ポイントライトを設置する
/// </summary>
/// <param name="pointLight"></param>
void DirectXCommon::SetPointLight(const PointLight* pointLight)
{
	// 最大数を越えないようにする
	if (useNumPointLightCount_ >= kMaxNumPointLight)
		return;

	// 色
	pointLightData_[useNumPointLightCount_].color = pointLight->color_ * pointLight->intensity_;

	// 位置
	pointLightData_[useNumPointLightCount_].position = pointLight->position_;

	// ライトの届く半径
	pointLightData_[useNumPointLightCount_].radius = pointLight->radius_;

	// 減衰率
	pointLightData_[useNumPointLightCount_].decay = pointLight->decay_;

	// カウントする
	useNumPointLightCount_++;
	useNumPointLightData_->num = useNumPointLightCount_;
}

/// <summary>
/// スポットライトを設置する
/// </summary>
/// <param name="spotLight"></param>
void DirectXCommon::SetSpotLight(const SpotLight* spotLight)
{
	// 最大数を越えないようにする
	if (useNumSpotLightCount_ >= kMaxNumSpotLight)
		return;

	// 色
	spotLightData_[useNumSpotLightCount_].color = spotLight->color_ * spotLight->intensity_;

	// 位置
	spotLightData_[useNumSpotLightCount_].position = spotLight->position_;

	// 向き
	spotLightData_[useNumSpotLightCount_].direction = Normalize(spotLight->direction_);

	// スポットライトの余弦
	spotLightData_[useNumSpotLightCount_].cosAngle = spotLight->cosAngle_;

	// 減衰率
	spotLightData_[useNumSpotLightCount_].decay = spotLight->decay_;

	// フォールオフ開始値
	spotLightData_[useNumSpotLightCount_].fallofStart = spotLight->fallofStart_;

	// ライトの光が届く距離
	spotLightData_[useNumSpotLightCount_].distance = spotLight->distance_;


	// カウントする
	useNumSpotLightCount_++;
	useNumSpotLightData_->num = useNumSpotLightCount_;
}

/// <summary>
/// 平面を描画する
/// </summary>
/// <param name="worldTransform"></param>
/// <param name="uvTransform"></param>
/// <param name="camera"></param>
/// <param name="textureHandle"></param>
/// <param name="color"></param>
/// <param name="isLighting"></param>
void DirectXCommon::DrawPlane(const WorldTransform* worldTransform, const UvTransform* uvTransform,
	const Camera3D* camera, uint32_t textureHandle, Vector4 color, bool isLighting)
{
	// 使用できるリソース数を越えないようにする
	if (useNumResourcePlane_ >= kMaxNumResource)
	{
		return;
	}


	/*-----------------
		インデックス
	-----------------*/

	// ビューを作成する
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};
	indexBufferView.BufferLocation = indexResourcePlane_->GetGPUVirtualAddress();
	indexBufferView.SizeInBytes = sizeof(uint32_t) * 6;
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;

	// データを書き込む
	uint32_t* indexData = nullptr;
	indexResourcePlane_->Map(0, nullptr, reinterpret_cast<void**>(&indexData));
	indexData[0] = 0; indexData[1] = 1; indexData[2] = 2;
	indexData[3] = 0; indexData[4] = 3; indexData[5] = 1;
	


	/*----------
		頂点
	----------*/

	// ビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	vertexBufferView.BufferLocation = vertexBufferResourcePlane_->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 4;
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	// データを書き込む
	VertexData* vertexData = nullptr;
	vertexBufferResourcePlane_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	vertexData[0].position = { -1.0f , 0.0f , 1.0f , 1.0f };
	vertexData[0].normal = { 0.0f , 1.0f , 0.0f };
	vertexData[0].texcoord = { 0.0f , 0.0f };

	vertexData[1].position = { 1.0f , 0.0f , -1.0f , 1.0f };
	vertexData[1].normal = { 0.0f , 1.0f , 0.0f };
	vertexData[1].texcoord = { 1.0f , 1.0f };

	vertexData[2].position = { -1.0f , 0.0f , -1.0f , 1.0f };
	vertexData[2].normal = { 0.0f , 1.0f , 0.0f };
	vertexData[2].texcoord = { 0.0f , 1.0f };

	vertexData[3].position = { 1.0f , 0.0f , 1.0f , 1.0f };
	vertexData[3].normal = { 0.0f , 1.0f , 0.0f };
	vertexData[3].texcoord = { 1.0f , 0.0f };


	/*---------------
		マテリアル
	---------------*/

	// データを書き込む
	Material* materialData = nullptr;
	materialResourcePlane_[useNumResourcePlane_]->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	materialData->color = color;
	materialData->enableLighting = isLighting;
	materialData->uvTransform =
		MakeScaleMatrix(uvTransform->scale_) * MakeRotateZMatrix(uvTransform->rotation_.z) * MakeTranslateMatrix(uvTransform->translation_);
	materialData->shininess = 18.0f;


	/*------------------
		座標変換の行列
	------------------*/

	// データを書き込む
	TransformationMatrix* transformationData = nullptr;
	transformationResourcePlane_[useNumResourcePlane_]->Map(0, nullptr, reinterpret_cast<void**>(&transformationData));
	transformationData->worldViewProjection = worldTransform->worldMatrix_ * camera->viewMatrix_ * camera->projectionMatrix_;
	transformationData->world = worldTransform->worldMatrix_;
	transformationData->worldInverseTranspose = MakeTransposeMatrix(MakeInverseMatrix(worldTransform->worldMatrix_));


	/*-----------
		カメラ
	-----------*/

	// データを書き込む
	CameraForGPU* cameraData = nullptr;
	cameraResourcePlane_[useNumResourcePlane_]->Map(0, nullptr, reinterpret_cast<void**>(&cameraData));
	cameraData->worldPosition = camera->translation_;



	/*------------------
		コマンドを積む
	------------------*/

	// ルートシグネチャやPSOの設定
	psoPrimitive_[useObject3dBlendMode_]->CommandListSet(commandList_);

	// IBVを設定する
	commandList_->IASetIndexBuffer(&indexBufferView);

	// VBVを設定する
	commandList_->IASetVertexBuffers(0, 1, &vertexBufferView);

	// 形状を設定
	commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// マテリアル用のCBVを設定
	commandList_->SetGraphicsRootConstantBufferView(0, materialResourcePlane_[useNumResourcePlane_]->GetGPUVirtualAddress());

	// 座標変換用のCBVを設定
	commandList_->SetGraphicsRootConstantBufferView(1, transformationResourcePlane_[useNumResourcePlane_]->GetGPUVirtualAddress());

	// 平行光源用のインスタンシングの設定
	commandList_->SetGraphicsRootDescriptorTable(7, instancingDirectionalLightSrvHandleGPU_);
	commandList_->SetGraphicsRootConstantBufferView(3, useNumDirectionalLightResource_->GetGPUVirtualAddress());

	// ポイントライト用のCBVを設定
	commandList_->SetGraphicsRootDescriptorTable(8, instancingPointLightSrvHandleGPU_);
	commandList_->SetGraphicsRootConstantBufferView(5, useNumPointLightResource_->GetGPUVirtualAddress());

	// スポットライトのCBVを設定
	commandList_->SetGraphicsRootDescriptorTable(9, instancingSpotLightSrvHandleGPU_);
	commandList_->SetGraphicsRootConstantBufferView(6, useNumSpotLightResource_->GetGPUVirtualAddress());

	// カメラ用のCBVを設定
	commandList_->SetGraphicsRootConstantBufferView(4, cameraResourcePlane_[useNumResourcePlane_]->GetGPUVirtualAddress());

	// テクスチャ
	textureStore_->SelectTexture(commandList_, textureHandle);

	// 描画する
	commandList_->DrawIndexedInstanced(6, 1, 0, 0, 0);


	// カウントする
	useNumResourcePlane_++;
}

/// <summary>
/// 球を描画する
/// </summary>
/// <param name="worldTransform"></param>
/// <param name="uvTransform"></param>
/// <param name="camera"></param>
/// <param name="color"></param>
/// <param name="isLighting"></param>
void DirectXCommon::DrawSphere(const WorldTransform* worldTransform, const UvTransform* uvTransform,
	const Camera3D* camera, uint32_t textureHandle,uint32_t segment, uint32_t ring, Vector4 color, bool isLighting)
{
	// 使用できるリソース数を越えないようにする
	if (useNumResourceSphere_ >= kMaxNumResource)
	{
		return;
	}

	// 分割数の範囲を制限する
	segment = std::clamp(int(segment), 3, 32);
	ring = std::clamp(int(ring), 3, 32);

	/*-----------------
		インデックス
	-----------------*/

	// ビューを作成する
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};
	indexBufferView.BufferLocation = indexResourceSphere_->GetGPUVirtualAddress();
	indexBufferView.SizeInBytes = sizeof(uint32_t) * (segment * ring) * 6;
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;

	// データを書き込む
	uint32_t* indexData = nullptr;
	indexResourceSphere_->Map(0, nullptr, reinterpret_cast<void**>(&indexData));

	for (uint32_t latIndex = 0; latIndex < ring; ++latIndex)
	{
		for (uint32_t lonIndex = 0; lonIndex < segment; ++lonIndex)
		{
			// データの要素数
			uint32_t dataIndex = (latIndex * segment + lonIndex) * 6;

			// インデックス
			uint32_t index = (latIndex * segment + lonIndex) * 4;

			indexData[dataIndex] = index;
			indexData[dataIndex + 1] = index + 1;
			indexData[dataIndex + 2] = index + 2;
			indexData[dataIndex + 3] = index + 2;
			indexData[dataIndex + 4] = index + 1;
			indexData[dataIndex + 5] = index + 3;
		}
	}


	/*----------
	    頂点
	----------*/

	// ビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	vertexBufferView.BufferLocation = vertexBufferResourceSphere_->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = sizeof(VertexData) * (segment * ring) * 4;
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	// データを書き込む
	VertexData* vertexData = nullptr;
	vertexBufferResourceSphere_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	// 経度分割1つ分の角度
	const float kLonEvery = std::numbers::pi_v<float> *2.0f / float(segment);

	// 緯度分割1つ分の角度
	const float kLatEvery = std::numbers::pi_v<float> / float(ring);

	// 緯度方向に分割
	for (uint32_t latIndex = 0; latIndex < ring; ++latIndex)
	{
		// 緯度
		float lat = -std::numbers::pi_v<float> / 2.0f + kLatEvery * latIndex;

		// 経度方向に分割
		for (uint32_t lonIndex = 0; lonIndex < segment; ++lonIndex)
		{
			// 経度
			float lon = kLonEvery * lonIndex;

			// データの要素番号
			uint32_t dataIndex = (latIndex * segment + lonIndex) * 4;

			vertexData[dataIndex].position.x = std::cos(lat) * std::cos(lon);
			vertexData[dataIndex].position.y = std::sin(lat);
			vertexData[dataIndex].position.z = std::cos(lat) * std::sin(lon);
			vertexData[dataIndex].position.w = 1.0f;
			vertexData[dataIndex].normal.x = std::cos(lat) * std::cos(lon);
			vertexData[dataIndex].normal.y = std::sin(lat);
			vertexData[dataIndex].normal.z = std::cos(lat) * std::sin(lon);
			vertexData[dataIndex].texcoord.x = float(lonIndex) / float(segment);
			vertexData[dataIndex].texcoord.y = 1.0f - (float(latIndex) / float(ring));

			vertexData[dataIndex + 1].position.x = std::cos(lat + kLatEvery) * std::cos(lon);
			vertexData[dataIndex + 1].position.y = std::sin(lat + kLatEvery);
			vertexData[dataIndex + 1].position.z = std::cos(lat + kLatEvery) * std::sin(lon);
			vertexData[dataIndex + 1].position.w = 1.0f;
			vertexData[dataIndex + 1].normal.x = std::cos(lat + kLatEvery) * std::cos(lon);
			vertexData[dataIndex + 1].normal.y = std::sin(lat + kLatEvery);
			vertexData[dataIndex + 1].normal.z = std::cos(lat + kLatEvery) * std::sin(lon);
			vertexData[dataIndex + 1].texcoord.x = float(lonIndex) / float(segment);
			vertexData[dataIndex + 1].texcoord.y = 1.0f - (float(latIndex + 1) / float(ring));

			vertexData[dataIndex + 2].position.x = std::cos(lat) * std::cos(lon + kLonEvery);
			vertexData[dataIndex + 2].position.y = std::sin(lat);
			vertexData[dataIndex + 2].position.z = std::cos(lat) * std::sin(lon + kLonEvery);
			vertexData[dataIndex + 2].position.w = 1.0f;
			vertexData[dataIndex + 2].normal.x = std::cos(lat) * std::cos(lon + kLonEvery);
			vertexData[dataIndex + 2].normal.y = std::sin(lat);
			vertexData[dataIndex + 2].normal.z = std::cos(lat) * std::sin(lon + kLonEvery);
			vertexData[dataIndex + 2].texcoord.x = float(lonIndex + 1) / float(segment);
			vertexData[dataIndex + 2].texcoord.y = 1.0f - (float(latIndex) / float(ring));

			vertexData[dataIndex + 3].position.x = std::cos(lat + kLatEvery) * std::cos(lon + kLonEvery);
			vertexData[dataIndex + 3].position.y = std::sin(lat + kLatEvery);
			vertexData[dataIndex + 3].position.z = std::cos(lat + kLatEvery) * std::sin(lon + kLonEvery);
			vertexData[dataIndex + 3].position.w = 1.0f;
			vertexData[dataIndex + 3].normal.x = std::cos(lat + kLatEvery) * std::cos(lon + kLonEvery);
			vertexData[dataIndex + 3].normal.y = std::sin(lat + kLatEvery);
			vertexData[dataIndex + 3].normal.z = std::cos(lat + kLatEvery) * std::sin(lon + kLonEvery);
			vertexData[dataIndex + 3].texcoord.x = float(lonIndex + 1) / float(segment);
			vertexData[dataIndex + 3].texcoord.y = 1.0f - (float(latIndex + 1) / float(ring));
		}
	}


	/*---------------
		マテリアル
	---------------*/

	// データを書き込む
	Material* materialData = nullptr;
	materialResourceSphere_[useNumResourceSphere_]->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	materialData->color = color;
	materialData->enableLighting = isLighting;
	materialData->uvTransform =
		MakeScaleMatrix(uvTransform->scale_) * MakeRotateZMatrix(uvTransform->rotation_.z) * MakeTranslateMatrix(uvTransform->translation_);
	materialData->shininess = 18.0f;


	/*------------------
		座標変換の行列
	------------------*/

	// データを書き込む
	TransformationMatrix* transformationData = nullptr;
	transformationResourceSphere_[useNumResourceSphere_]->Map(0, nullptr, reinterpret_cast<void**>(&transformationData));
	transformationData->worldViewProjection = worldTransform->worldMatrix_ * camera->viewMatrix_ * camera->projectionMatrix_;
	transformationData->world = worldTransform->worldMatrix_;
	transformationData->worldInverseTranspose = MakeTransposeMatrix(MakeInverseMatrix(worldTransform->worldMatrix_));


	/*-----------
		カメラ
	-----------*/

	// データを書き込む
	CameraForGPU* cameraData = nullptr;
	cameraResourceSphere_[useNumResourceSphere_]->Map(0, nullptr, reinterpret_cast<void**>(&cameraData));
	cameraData->worldPosition = camera->translation_;



	/*------------------
		コマンドを積む
	------------------*/

	// ルートシグネチャやPSOの設定
	psoPrimitive_[useObject3dBlendMode_]->CommandListSet(commandList_);

	// IBVを設定する
	commandList_->IASetIndexBuffer(&indexBufferView);

	// VBVを設定する
	commandList_->IASetVertexBuffers(0, 1, &vertexBufferView);

	// 形状を設定
	commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// マテリアル用のCBVを設定
	commandList_->SetGraphicsRootConstantBufferView(0, materialResourceSphere_[useNumResourceSphere_]->GetGPUVirtualAddress());

	// 座標変換用のCBVを設定
	commandList_->SetGraphicsRootConstantBufferView(1, transformationResourceSphere_[useNumResourceSphere_]->GetGPUVirtualAddress());

	// 平行光源用のインスタンシングの設定
	commandList_->SetGraphicsRootDescriptorTable(7, instancingDirectionalLightSrvHandleGPU_);
	commandList_->SetGraphicsRootConstantBufferView(3, useNumDirectionalLightResource_->GetGPUVirtualAddress());

	// ポイントライト用のCBVを設定
	commandList_->SetGraphicsRootDescriptorTable(8, instancingPointLightSrvHandleGPU_);
	commandList_->SetGraphicsRootConstantBufferView(5, useNumPointLightResource_->GetGPUVirtualAddress());

	// スポットライトのCBVを設定
	commandList_->SetGraphicsRootDescriptorTable(9, instancingSpotLightSrvHandleGPU_);
	commandList_->SetGraphicsRootConstantBufferView(6, useNumSpotLightResource_->GetGPUVirtualAddress());

	// カメラ用のCBVを設定
	commandList_->SetGraphicsRootConstantBufferView(4, cameraResourceSphere_[useNumResourceSphere_]->GetGPUVirtualAddress());

	// テクスチャ
	textureStore_->SelectTexture(commandList_, textureHandle);

	// 描画する
	commandList_->DrawIndexedInstanced(segment* ring * 6, 1, 0, 0, 0);


	// カウントする
	useNumResourceSphere_++;
}

/// <summary>
/// リングを描画する
/// </summary>
/// <param name="worldTransform"></param>
/// <param name="uvTransform"></param>
/// <param name="camera"></param>
/// <param name="textureHandle"></param>
/// <param name="subdivisions"></param>
/// <param name="outRadius"></param>
/// <param name="inRadius"></param>
/// <param name="color"></param>
/// <param name="isLighting"></param>
void DirectXCommon::DrawRing(const WorldTransform* worldTransform, const UvTransform* uvTransform,
	const Camera3D* camera, uint32_t textureHandle, uint32_t subdivisions,float outRadius, float inRadius, Vector4 color, bool isLighting)
{
	// 使用できるリソース数を越えないようにする
	if (useNumResourceRing_ >= kMaxNumResource)
	{
		return;
	}

	// 分割数の範囲を制限する
	subdivisions = std::clamp(int(subdivisions), 3, 32);

	/*-----------------
		インデックス
	-----------------*/

	// ビューを作成する
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};
	indexBufferView.BufferLocation = indexResourceRing_->GetGPUVirtualAddress();
	indexBufferView.SizeInBytes = sizeof(uint32_t) * (subdivisions * 6);
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;

	// データを書き込む
	uint32_t* indexData = nullptr;
	indexResourceRing_->Map(0, nullptr, reinterpret_cast<void**>(&indexData));

	for (uint32_t index = 0; index < subdivisions; ++index)
	{
		uint32_t startIndex = index * 6;
		uint32_t vertexIndex = index * 4;

		indexData[startIndex] = vertexIndex + 0;
		indexData[startIndex + 1] = vertexIndex + 1;
		indexData[startIndex + 2] = vertexIndex + 2;
		indexData[startIndex + 3] = vertexIndex + 1;
		indexData[startIndex + 4] = vertexIndex + 3;
		indexData[startIndex + 5] = vertexIndex + 2;
	}


	/*----------
		頂点
	----------*/

	// ビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	vertexBufferView.BufferLocation = vertexBufferResourceRing_->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = sizeof(VertexData) * (subdivisions * 4);
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	// データを書き込む
	VertexData* vertexData = nullptr;
	vertexBufferResourceRing_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	const float radianPerDivid = 2.0f * std::numbers::pi_v<float> / static_cast<float>(subdivisions);

	for (uint32_t index = 0; index < subdivisions; ++index)
	{
		uint32_t startIndex = index * 4;

		float sin = std::sin(index * radianPerDivid);
		float cos = std::cos(index * radianPerDivid);

		float sinNext = std::sin((index + 1) * radianPerDivid);
		float cosNext = std::cos((index + 1) * radianPerDivid);

		float u = static_cast<float>(index) / static_cast<float>(subdivisions);
		float uNext = static_cast<float>(index + 1) / static_cast<float>(subdivisions);

		vertexData[startIndex].position = { -sin * outRadius , 0.0f , cos * outRadius  , 1.0f };
		vertexData[startIndex].texcoord = Vector2(u, 0.0f);
		vertexData[startIndex].normal = Vector3(0.0f, 0.0f, -1.0f);

		vertexData[startIndex + 1].position = { -sinNext * outRadius , 0.0f , cosNext * outRadius  , 1.0f };
		vertexData[startIndex + 1].texcoord = Vector2(uNext, 0.0f);
		vertexData[startIndex + 1].normal = Vector3(0.0f, 0.0f, -1.0f);

		vertexData[startIndex + 2].position = { -sin * inRadius , 0.0f , cos * inRadius , 1.0f };
		vertexData[startIndex + 2].texcoord = Vector2(u, 1.0f);
		vertexData[startIndex + 2].normal = Vector3(0.0f, 0.0f, -1.0f);

		vertexData[startIndex + 3].position = { -sinNext * inRadius , 0.0f , cosNext * inRadius  , 1.0f };
		vertexData[startIndex + 3].texcoord = Vector2(uNext, 1.0f);
		vertexData[startIndex + 3].normal = Vector3(0.0f, 0.0f, -1.0f);
	}


	/*---------------
		マテリアル
	---------------*/

	// データを書き込む
	Material* materialData = nullptr;
	materialResourceRing_[useNumResourceRing_]->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	materialData->color = color;
	materialData->enableLighting = isLighting;
	materialData->uvTransform =
		MakeScaleMatrix(uvTransform->scale_) * MakeRotateZMatrix(uvTransform->rotation_.z) * MakeTranslateMatrix(uvTransform->translation_);
	materialData->shininess = 18.0f;


	/*------------------
		座標変換の行列
	------------------*/

	// データを書き込む
	TransformationMatrix* transformationData = nullptr;
	transformationResourceRing_[useNumResourceRing_]->Map(0, nullptr, reinterpret_cast<void**>(&transformationData));
	transformationData->worldViewProjection = worldTransform->worldMatrix_ * camera->viewMatrix_ * camera->projectionMatrix_;
	transformationData->world = worldTransform->worldMatrix_;
	transformationData->worldInverseTranspose = MakeTransposeMatrix(MakeInverseMatrix(worldTransform->worldMatrix_));


	/*-----------
		カメラ
	-----------*/

	// データを書き込む
	CameraForGPU* cameraData = nullptr;
	cameraResourceRing_[useNumResourceRing_]->Map(0, nullptr, reinterpret_cast<void**>(&cameraData));
	cameraData->worldPosition = camera->translation_;



	/*------------------
		コマンドを積む
	------------------*/

	// ルートシグネチャやPSOの設定
	psoPrimitive_[useObject3dBlendMode_]->CommandListSet(commandList_);

	// IBVを設定する
	commandList_->IASetIndexBuffer(&indexBufferView);

	// VBVを設定する
	commandList_->IASetVertexBuffers(0, 1, &vertexBufferView);

	// 形状を設定
	commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// マテリアル用のCBVを設定
	commandList_->SetGraphicsRootConstantBufferView(0, materialResourceRing_[useNumResourceRing_]->GetGPUVirtualAddress());

	// 座標変換用のCBVを設定
	commandList_->SetGraphicsRootConstantBufferView(1, transformationResourceRing_[useNumResourceRing_]->GetGPUVirtualAddress());

	// 平行光源用のインスタンシングの設定
	commandList_->SetGraphicsRootDescriptorTable(7, instancingDirectionalLightSrvHandleGPU_);
	commandList_->SetGraphicsRootConstantBufferView(3, useNumDirectionalLightResource_->GetGPUVirtualAddress());

	// ポイントライト用のCBVを設定
	commandList_->SetGraphicsRootDescriptorTable(8, instancingPointLightSrvHandleGPU_);
	commandList_->SetGraphicsRootConstantBufferView(5, useNumPointLightResource_->GetGPUVirtualAddress());

	// スポットライトのCBVを設定
	commandList_->SetGraphicsRootDescriptorTable(9, instancingSpotLightSrvHandleGPU_);
	commandList_->SetGraphicsRootConstantBufferView(6, useNumSpotLightResource_->GetGPUVirtualAddress());

	// カメラ用のCBVを設定
	commandList_->SetGraphicsRootConstantBufferView(4, cameraResourceRing_[useNumResourceRing_]->GetGPUVirtualAddress());

	// テクスチャ
	textureStore_->SelectTexture(commandList_, textureHandle);

	// 描画する
	commandList_->DrawIndexedInstanced(subdivisions * 6, 1, 0, 0, 0);


	// カウントする
	useNumResourceRing_++;
}

/// <summary>
/// 円柱を描画する
/// </summary>
/// <param name="worldTransform"></param>
/// <param name="uvTransform"></param>
/// <param name="camera"></param>
/// <param name="textureHandle"></param>
/// <param name="subdivisions"></param>
/// <param name="height"></param>
/// <param name="radius"></param>
/// <param name="color"></param>
/// <param name="isLighting"></param>
void DirectXCommon::DrawCylinder(const WorldTransform* worldTransform, const UvTransform* uvTransform,
	const Camera3D* camera, uint32_t textureHandle, uint32_t subdivisions, float height, float radius, Vector4 color, bool isLighting)
{
	// 使用できるリソース数を越えないようにする
	if (useNumResourceCylinder_ >= kMaxNumResource)
	{
		return;
	}

	// 分割数の範囲を制限する
	subdivisions = std::clamp(int(subdivisions), 3, 32);

	/*-----------------
		インデックス
	-----------------*/

	// ビューを作成する
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};
	indexBufferView.BufferLocation = indexResourceCylinder_->GetGPUVirtualAddress();
	indexBufferView.SizeInBytes = sizeof(uint32_t) * (subdivisions * 6);
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;

	// データを書き込む
	uint32_t* indexData = nullptr;
	indexResourceCylinder_->Map(0, nullptr, reinterpret_cast<void**>(&indexData));

	for (uint32_t index = 0; index < subdivisions; ++index)
	{
		uint32_t startIndex = index * 6;
		uint32_t vertexIndex = index * 4;

		indexData[startIndex] = vertexIndex + 0;
		indexData[startIndex + 1] = vertexIndex + 1;
		indexData[startIndex + 2] = vertexIndex + 2;
		indexData[startIndex + 3] = vertexIndex + 1;
		indexData[startIndex + 4] = vertexIndex + 3;
		indexData[startIndex + 5] = vertexIndex + 2;
	}


	/*----------
		頂点
	----------*/

	// ビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	vertexBufferView.BufferLocation = vertexBufferResourceCylinder_->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = sizeof(VertexData) * (subdivisions * 4);
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	// データを書き込む
	VertexData* vertexData = nullptr;
	vertexBufferResourceCylinder_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	const float radianPerDivid = 2.0f * std::numbers::pi_v<float> / static_cast<float>(subdivisions);

	for (uint32_t index = 0; index < subdivisions; ++index)
	{
		uint32_t startIndex = index * 4;

		float sin = std::sin(index * radianPerDivid);
		float cos = std::cos(index * radianPerDivid);

		float sinNext = std::sin((index + 1) * radianPerDivid);
		float cosNext = std::cos((index + 1) * radianPerDivid);

		float u = static_cast<float>(index) / static_cast<float>(subdivisions);
		float uNext = static_cast<float>(index + 1) / static_cast<float>(subdivisions);

		vertexData[startIndex].position = { -sin * radius , height , cos * radius  , 1.0f };
		vertexData[startIndex].texcoord = Vector2(u, 0.0f);
		vertexData[startIndex].normal = Vector3(-sin, 0.0f, cos);

		vertexData[startIndex + 1].position = { -sinNext * radius , height , cosNext * radius  , 1.0f };
		vertexData[startIndex + 1].texcoord = Vector2(uNext, 0.0f);
		vertexData[startIndex + 1].normal = Vector3(-sinNext, 0.0f, cosNext);

		vertexData[startIndex + 2].position = { -sin * radius , 0.0f , cos * radius , 1.0f };
		vertexData[startIndex + 2].texcoord = Vector2(u, 1.0f);
		vertexData[startIndex + 2].normal = Vector3(-sin, 0.0f, cos);

		vertexData[startIndex + 3].position = { -sinNext * radius , 0.0f , cosNext * radius  , 1.0f };
		vertexData[startIndex + 3].texcoord = Vector2(uNext, 1.0f);
		vertexData[startIndex + 3].normal = Vector3(-sinNext, 0.0f, cosNext);
	}


	/*---------------
		マテリアル
	---------------*/

	// データを書き込む
	Material* materialData = nullptr;
	materialResourceCylinder_[useNumResourceCylinder_]->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	materialData->color = color;
	materialData->enableLighting = isLighting;
	materialData->uvTransform =
		MakeScaleMatrix(uvTransform->scale_) * MakeRotateZMatrix(uvTransform->rotation_.z) * MakeTranslateMatrix(uvTransform->translation_);
	materialData->shininess = 18.0f;


	/*------------------
		座標変換の行列
	------------------*/

	// データを書き込む
	TransformationMatrix* transformationData = nullptr;
	transformationResourceCylinder_[useNumResourceCylinder_]->Map(0, nullptr, reinterpret_cast<void**>(&transformationData));
	transformationData->worldViewProjection = worldTransform->worldMatrix_ * camera->viewMatrix_ * camera->projectionMatrix_;
	transformationData->world = worldTransform->worldMatrix_;
	transformationData->worldInverseTranspose = MakeTransposeMatrix(MakeInverseMatrix(worldTransform->worldMatrix_));


	/*-----------
		カメラ
	-----------*/

	// データを書き込む
	CameraForGPU* cameraData = nullptr;
	cameraResourceCylinder_[useNumResourceCylinder_]->Map(0, nullptr, reinterpret_cast<void**>(&cameraData));
	cameraData->worldPosition = camera->translation_;



	/*------------------
		コマンドを積む
	------------------*/

	// ルートシグネチャやPSOの設定
	psoPrimitive_[useObject3dBlendMode_]->CommandListSet(commandList_);

	// IBVを設定する
	commandList_->IASetIndexBuffer(&indexBufferView);

	// VBVを設定する
	commandList_->IASetVertexBuffers(0, 1, &vertexBufferView);

	// 形状を設定
	commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// マテリアル用のCBVを設定
	commandList_->SetGraphicsRootConstantBufferView(0, materialResourceCylinder_[useNumResourceCylinder_]->GetGPUVirtualAddress());

	// 座標変換用のCBVを設定
	commandList_->SetGraphicsRootConstantBufferView(1, transformationResourceCylinder_[useNumResourceCylinder_]->GetGPUVirtualAddress());

	// 平行光源用のインスタンシングの設定
	commandList_->SetGraphicsRootDescriptorTable(7, instancingDirectionalLightSrvHandleGPU_);
	commandList_->SetGraphicsRootConstantBufferView(3, useNumDirectionalLightResource_->GetGPUVirtualAddress());

	// ポイントライト用のCBVを設定
	commandList_->SetGraphicsRootDescriptorTable(8, instancingPointLightSrvHandleGPU_);
	commandList_->SetGraphicsRootConstantBufferView(5, useNumPointLightResource_->GetGPUVirtualAddress());

	// スポットライトのCBVを設定
	commandList_->SetGraphicsRootDescriptorTable(9, instancingSpotLightSrvHandleGPU_);
	commandList_->SetGraphicsRootConstantBufferView(6, useNumSpotLightResource_->GetGPUVirtualAddress());

	// カメラ用のCBVを設定
	commandList_->SetGraphicsRootConstantBufferView(4, cameraResourceCylinder_[useNumResourceCylinder_]->GetGPUVirtualAddress());

	// テクスチャ
	textureStore_->SelectTexture(commandList_, textureHandle);

	// 描画する
	commandList_->DrawIndexedInstanced(subdivisions * 6, 1, 0, 0, 0);


	// カウントする
	useNumResourceCylinder_++;
}

/// <summary>
/// モデルを描画する
/// </summary>
/// <param name="worldTransform">ワールドトランスフォーム</param>
/// <param name="uvTransform">UVトランスフォーム</param>
/// <param name="camera">カメラ</param>
/// <param name="modelHandle">モデルハンドル</param>
/// <param name="color">色</param>
void DirectXCommon::DrawModel(const WorldTransform* worldTransform, const UvTransform* uvTransform,
	const Camera3D* camera, uint32_t modelHandle, Vector4 color, bool isLighting)
{
	// 使用できるリソース数を越えないようにする
	if (useNumResourceModel_ >= kMaxNumResource)
	{
		return;
	}

	/*----------
		頂点
	----------*/

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	vertexBufferView.BufferLocation = modelDataStore_->GetVertexResource(modelHandle)->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * modelDataStore_->GetModelData(modelHandle).vertices.size());
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	// 頂点データを書き込む
	VertexData* vertexData = nullptr;
	modelDataStore_->GetVertexResource(modelHandle)->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	std::memcpy(vertexData, modelDataStore_->GetModelData(modelHandle).vertices.data(),
		sizeof(VertexData) * modelDataStore_->GetModelData(modelHandle).vertices.size());


	/*---------------
		マテリアル
	---------------*/

	// データを書き込む
	Material* materialData = nullptr;
	materialResourceModel_[useNumResourceModel_]->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	materialData->color = color;
	materialData->enableLighting = isLighting;
	materialData->uvTransform = 
		MakeScaleMatrix(uvTransform->scale_) * MakeRotateZMatrix(uvTransform->rotation_.z) * MakeTranslateMatrix(uvTransform->translation_);
	materialData->shininess = 18.0f;


	/*------------------
		座標変換の行列
	------------------*/

	// データを書き込む
	TransformationMatrix* transformationData = nullptr;
	transformationResourceModel_[useNumResourceModel_]->Map(0, nullptr, reinterpret_cast<void**>(&transformationData));
	transformationData->worldViewProjection = worldTransform->worldMatrix_ * camera->viewMatrix_ * camera->projectionMatrix_;
	transformationData->world = worldTransform->worldMatrix_;
	transformationData->worldInverseTranspose = MakeTransposeMatrix(MakeInverseMatrix(worldTransform->worldMatrix_));


	/*-----------
	    カメラ
	-----------*/

	// データを書き込む
	CameraForGPU* cameraData = nullptr;
	cameraResourceModel_[useNumResourceModel_]->Map(0, nullptr, reinterpret_cast<void**>(&cameraData));
	cameraData->worldPosition = camera->translation_;



	/*------------------
		コマンドを積む
	------------------*/

	// ルートシグネチャやPSOの設定
	psoObject3d_[useObject3dBlendMode_]->CommandListSet(commandList_);

	// VBVを設定する
	commandList_->IASetVertexBuffers(0, 1, &vertexBufferView);

	// 形状を設定
	commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// マテリアル用のCBVを設定
	commandList_->SetGraphicsRootConstantBufferView(0, materialResourceModel_[useNumResourceModel_]->GetGPUVirtualAddress());

	// 座標変換用のCBVを設定
	commandList_->SetGraphicsRootConstantBufferView(1, transformationResourceModel_[useNumResourceModel_]->GetGPUVirtualAddress());

	// 平行光源用のインスタンシングの設定
	commandList_->SetGraphicsRootDescriptorTable(7, instancingDirectionalLightSrvHandleGPU_);
	commandList_->SetGraphicsRootConstantBufferView(3, useNumDirectionalLightResource_->GetGPUVirtualAddress());

	// ポイントライト用のCBVを設定
	commandList_->SetGraphicsRootDescriptorTable(8, instancingPointLightSrvHandleGPU_);
	commandList_->SetGraphicsRootConstantBufferView(5, useNumPointLightResource_->GetGPUVirtualAddress());

	// スポットライトのCBVを設定
	commandList_->SetGraphicsRootDescriptorTable(9, instancingSpotLightSrvHandleGPU_);
	commandList_->SetGraphicsRootConstantBufferView(6, useNumSpotLightResource_->GetGPUVirtualAddress());

	// カメラ用のCBVを設定
	commandList_->SetGraphicsRootConstantBufferView(4, cameraResourceModel_[useNumResourceModel_]->GetGPUVirtualAddress());

	// テクスチャ
	textureStore_->SelectTexture(commandList_, modelDataStore_->GetTextureHandle(modelHandle));

	// 描画する
	commandList_->DrawInstanced(UINT(modelDataStore_->GetModelData(modelHandle).vertices.size()), 1, 0, 0);


	// カウントする
	useNumResourceModel_++;
}

/// <summary>
/// パーティクルを描画する
/// </summary>
/// <param name="camera"></param>
/// <param name="modelHandle"></param>
void DirectXCommon::DrawParticle(const Camera3D* camera, uint32_t modelHandle, Vector4 color)
{
	// 乱数生成器の初期化
	std::random_device seedGenerater;
	std::mt19937 randomEngine(seedGenerater());

	ImGui::Begin("Particle");
	ImGui::DragFloat3("translation", &emitter_.transform.translation.x, 0.1f);
	ImGui::End();

	/*----------
		頂点
	----------*/

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	vertexBufferView.BufferLocation = modelDataStore_->GetVertexResource(modelHandle)->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * modelDataStore_->GetModelData(modelHandle).vertices.size());
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	// 頂点データを書き込む
	VertexData* vertexData = nullptr;
	modelDataStore_->GetVertexResource(modelHandle)->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	std::memcpy(vertexData, modelDataStore_->GetModelData(modelHandle).vertices.data(),
		sizeof(VertexData) * modelDataStore_->GetModelData(modelHandle).vertices.size());


	/*---------------
		マテリアル
	---------------*/

	// データを書き込む
	Material* materialData = nullptr;
	materialResourceParticle_->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	materialData->color = color;
	materialData->enableLighting = false;
	materialData->uvTransform = MakeIdenityMatirx();


	/*-------------------
	    インスタンシング
	-------------------*/

	// データを書き込む
	ParticleForGPU* instancingData = nullptr;
	instancingResourcesParticle_->Map(0, nullptr, reinterpret_cast<void**>(&instancingData));

	// 描画できたパーティクルの数
	uint32_t numInstance = 0;

	// 発生する時間を進める
	emitter_.frequencyTime += kDeltaTime;

	// 到達したら発生させる
	if (emitter_.frequencyTime >= emitter_.frequency)
	{
		particles_.splice(particles_.end(), Emit(emitter_, randomEngine));

		// 時間を初期化する
		emitter_.frequencyTime -= emitter_.frequency;
	}

	// ビルボード行列
	Matrix4x4 billboardMatrix = MakeAffineMatrix(camera->scale_, camera->rotation_, camera->translation_);
	billboardMatrix.m[3][0] = 0.0f;
	billboardMatrix.m[3][1] = 0.0f;
	billboardMatrix.m[3][2] = 0.0f;

	for (std::list<Particle>::iterator particleIterator = particles_.begin(); particleIterator != particles_.end();)
	{
		// 寿命を越えたら、リストから消す
		if ((*particleIterator).currentTime >= (*particleIterator).lifeTime)
		{
			particleIterator = particles_.erase(particleIterator);
			continue;
		}

		// 最大数を越えないようにする
		if (numInstance < kNumMaxInstance)
		{

			// 徐々に透明にさせる
			float alpha = 1.0f - ((*particleIterator).currentTime / (*particleIterator).lifeTime);

			// 時間をカウントする
			(*particleIterator).currentTime += kDeltaTime;

			// フィールドの範囲内で、加速度が上昇する
			if (IsCollision(accelerationFeild_.area, (*particleIterator).transform.translation))
			{
				(*particleIterator).velocity += accelerationFeild_.acceleration * kDeltaTime;
			}

			// 移動させる
			(*particleIterator).transform.translation += (*particleIterator).velocity * kDeltaTime;

			// ワールド行列
			Matrix4x4 worldMatrix = 
				MakeScaleMatrix((*particleIterator).transform.scale) * billboardMatrix * MakeTranslateMatrix((*particleIterator).transform.translation);


			// データを書き込む
			instancingData[numInstance].worldViewProjection = worldMatrix * camera->viewMatrix_ * camera->projectionMatrix_;
			instancingData[numInstance].world = worldMatrix;
			instancingData[numInstance].color = (*particleIterator).color;
			instancingData[numInstance].color.w = alpha;

			// カウントする
			numInstance++;
		}

		// イテレータを進める
		particleIterator++;
	}



	/*------------------
		コマンドを積む
	------------------*/

	// ルートシグネチャやPSOの設定
	psoParticle_[useParticleBlendMode_]->CommandListSet(commandList_);

	// VBVを設定する
	commandList_->IASetVertexBuffers(0, 1, &vertexBufferView);

	// 形状を設定
	commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// マテリアル用のCBVを設定
	commandList_->SetGraphicsRootConstantBufferView(0, materialResourceParticle_->GetGPUVirtualAddress());

	// インスタンシング用のTableを設定
	commandList_->SetGraphicsRootDescriptorTable(1, instancingSrvHandleGPU_);

	// テクスチャ
	textureStore_->SelectTexture(commandList_, modelDataStore_->GetTextureHandle(modelHandle));

	// 描画する
	commandList_->DrawInstanced(UINT(modelDataStore_->GetModelData(modelHandle).vertices.size()), numInstance, 0, 0);
}

/// <summary>
/// 線を描画する
/// </summary>
/// <param name="start">始点</param>
/// <param name="end">終点</param>
/// <param name="camera">カメラ</param>
/// <param name="color">色</param>
void DirectXCommon::DrawLine(const Vector3& start , const Vector3& end, const Camera3D* camera, Vector4 color)
{
	// 使用できるリソース数を越えないようにする
	if (useNumResourceLine_ >= kMaxNumResource)
	{
		return;
	}

	/*----------
		頂点
	----------*/

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	vertexBufferView.BufferLocation = vertexBufferResourceLine_[useNumResourceLine_]->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = sizeof(Vector4) * 2;
	vertexBufferView.StrideInBytes = sizeof(Vector4);

	// 頂点データを書き込む
	Vector4* vertexData = nullptr;
	vertexBufferResourceLine_[useNumResourceLine_]->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	vertexData[0] = { start.x,start.y,start.z,1.0f };
	vertexData[1] = { end.x,end.y,end.z,1.0f };


	/*---------------
	    マテリアル
	---------------*/

	// データを書き込む
	Material* materialData = nullptr;
	materialResourceLine_[useNumResourceLine_]->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	materialData->color = color;
	materialData->enableLighting = false;
	materialData->shininess = 0.0f;
	materialData->uvTransform = MakeIdenityMatirx();


	/*-------------
	    座標変換
	-------------*/

	// データを書き込む
	TransformationMatrix* transformationData = nullptr;
	transformationResourceLine_[useNumResourceLine_]->Map(0, nullptr, reinterpret_cast<void**>(&transformationData));

	// ワールド行列
	Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f , 1.0f , 1.0f }, { 0.0f , 0.0f , 0.0f }, { 0.0f , 0.0f , 0.0f });

	transformationData->worldViewProjection = worldMatrix * camera->viewMatrix_ * camera->projectionMatrix_;
	transformationData->world = MakeIdenityMatirx();
	transformationData->worldInverseTranspose = MakeIdenityMatirx();


	/*------------------
	    コマンドを積む
	------------------*/

	psoLine3d_[useLine3dBlendMode_]->CommandListSet(commandList_);

	// VBVを設定する
	commandList_->IASetVertexBuffers(0, 1, &vertexBufferView);

	// 形状を設定
	commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);

	// マテリアル用のCBVを設定
	commandList_->SetGraphicsRootConstantBufferView(0, materialResourceLine_[useNumResourceLine_]->GetGPUVirtualAddress());

	// 座標変換用のCBVを設定
	commandList_->SetGraphicsRootConstantBufferView(1, transformationResourceLine_[useNumResourceLine_]->GetGPUVirtualAddress());

	// 描画する
	commandList_->DrawInstanced(2, 1, 0, 0);


	useNumResourceLine_++;
}

/// <summary>
/// スプライトを描画する
/// </summary>
/// <param name="worldTransform">ワールドトランスフォーム</param>
/// <param name="uvTransform">UVトランスフォーム</param>
/// <param name="camera">カメラ</param>
/// <param name="textureHandle">テクスチャハンドル</param>
/// <param name="color">色</param>
void DirectXCommon::DrawSprite(const WorldTransform* worldTransform, const UvTransform* uvTransform,
	const Camera2D* camera, uint32_t textureHandle, Vector4 color)
{
	// 使用できるリソース数を越えないようにする
	if (useNumResourceSprite_ >= kMaxNumResource)
	{
		return;
	}

	/*-----------------
		インデックス
	-----------------*/

	// ビューを作成する
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};
	indexBufferView.BufferLocation = indexResourceSprite_->GetGPUVirtualAddress();
	indexBufferView.SizeInBytes = sizeof(uint32_t) * 6;
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;

	// データを書き込む
	uint32_t* indexData = nullptr;
	indexResourceSprite_->Map(0, nullptr, reinterpret_cast<void**>(&indexData));
	indexData[0] = 0; indexData[1] = 1; indexData[2] = 2;
	indexData[3] = 1; indexData[4] = 3; indexData[5] = 2;


	/*---------
		頂点
	---------*/

	// ビューを作成する
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	vertexBufferView.BufferLocation = vertexBufferResourceSprite_->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 4;
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	// データを書き込む
	VertexData* vertexData = nullptr;
	vertexBufferResourceSprite_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));


	// 左下
	vertexData[0].position = { -1.0f , -1.0f , 0.0f , 1.0f };
	vertexData[0].texcoord = { 0.0f , 1.0f };
	vertexData[0].normal = { 0.0f , 0.0f , -1.0f };

	// 左上
	vertexData[1].position = { -1.0f , 1.0f , 0.0f , 1.0f };
	vertexData[1].texcoord = { 0.0f , 0.0f };
	vertexData[1].normal = { 0.0f , 0.0f , -1.0f };

	// 右下
	vertexData[2].position = { 1.0f , -1.0f , 0.0f , 1.0f };
	vertexData[2].texcoord = { 1.0f , 1.0f };
	vertexData[2].normal = { 0.0f , 0.0f , -1.0f };

	// 右上
	vertexData[3].position = { 1.0f , 1.0f , 0.0f , 1.0f };
	vertexData[3].texcoord = { 1.0f , 0.0f };
	vertexData[3].normal = { 0.0f , 0.0f , -1.0f };


	/*---------------
		マテリアル
	---------------*/

	// データを書き込む
	Material* materialData = nullptr;
	materialResourceSprite_[useNumResourceSprite_]->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	materialData->color = color;
	materialData->enableLighting = false;
	materialData->uvTransform = 
		MakeScaleMatrix(uvTransform->scale_) * MakeRotateZMatrix(uvTransform->rotation_.z) * MakeTranslateMatrix(uvTransform->translation_);


	/*------------------
		座標変換の行列
	------------------*/

	// データを書き込む
	TransformationMatrix* transformationData = nullptr;
	transformationResourceSprite_[useNumResourceSprite_]->Map(0, nullptr, reinterpret_cast<void**>(&transformationData));
	transformationData->worldViewProjection = worldTransform->worldMatrix_ * camera->viewMatrix_ * camera->projectionMatrix_;
	transformationData->world = worldTransform->worldMatrix_;



	/*------------------
		コマンドを積む
	------------------*/

	// ルートシグネチャやPSOの設定
	psoObject3d_[useObject3dBlendMode_]->CommandListSet(commandList_);

	// IBVを設定する
	commandList_->IASetIndexBuffer(&indexBufferView);

	// VBVを設定する
	commandList_->IASetVertexBuffers(0, 1, &vertexBufferView);

	// 形状を設定
	commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// マテリアル用のCBVを設定
	commandList_->SetGraphicsRootConstantBufferView(0, materialResourceSprite_[useNumResourceSprite_]->GetGPUVirtualAddress());

	// 座標変換用のCBVを設定
	commandList_->SetGraphicsRootConstantBufferView(1, transformationResourceSprite_[useNumResourceSprite_]->GetGPUVirtualAddress());

	// テクスチャ
	textureStore_->SelectTexture(commandList_, textureHandle);

	// 描画する
	commandList_->DrawIndexedInstanced(6, 1, 0, 0, 0);


	// カウントする
	useNumResourceSprite_++;
}

/// <summary>
/// ディスクリプタヒープを生成する
/// </summary>
/// <param name="device"></param>
/// <param name="heapType"></param>
/// <param name="numDescriptors"></param>
/// <param name="ShaderVisible"></param>
/// <returns></returns>
Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> DirectXCommon::CreateDescritprHeap(Microsoft::WRL::ComPtr<ID3D12Device> device,
	D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool ShaderVisible)
{
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap = nullptr;


	/*-----------------------------
	    ディスクリプタヒープの設定
	-----------------------------*/
	
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc{};

	// ディスクリプタヒープの種類
	descriptorHeapDesc.Type = heapType;

	// ディスクリプタの数
	descriptorHeapDesc.NumDescriptors = numDescriptors;

	// フラグ
	descriptorHeapDesc.Flags = ShaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	// 生成する
	HRESULT hr = device->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&descriptorHeap));
	assert(SUCCEEDED(hr));


	// ディスクリプタヒープを生成した旨のログ
	if (heapType == D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
		log_->Log("Create RTV DescriptorHeap \n");

	else if (heapType == D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		log_->Log("Create SRV DescriptorHeap \n");

	else if (heapType == D3D12_DESCRIPTOR_HEAP_TYPE_DSV)
		log_->Log("Create DSV DescriptorHeap \n");


	return descriptorHeap;
}


/// <summary>
/// 指定したディスクリプタヒープに格納するためのポインタを取得する（CPU）
/// </summary>
/// <param name="descriptorHeap">ディスクリプタヒープ</param>
/// <param name="descriptorSize">ディスクリプタのサイズ</param>
/// <param name="index">配列番号</param>
/// <returns></returns>
D3D12_CPU_DESCRIPTOR_HANDLE DirectXCommon::GetCPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap,
	uint32_t descriptorSize, uint32_t index)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize * index);
	return handleCPU;
}

/// <summary>
/// 指定したディスクリプタヒープに格納するためのポインタを取得する（GPU）
/// </summary>
/// <param name="descriptorHeap">ディスクリプタヒープ</param>
/// <param name="descriptorSize">ディスクリプタのサイズ</param>
/// <param name="index">配列番号</param>
/// <returns></returns>
D3D12_GPU_DESCRIPTOR_HANDLE DirectXCommon::GetGPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap,
	uint32_t descriptorSize, uint32_t index)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize * index);
	return handleGPU;
}

/// <summary>
/// RenderTextureを作成する
/// </summary>
/// <param name="device"></param>
/// <param name="width"></param>
/// <param name="height"></param>
/// <param name="format"></param>
/// <param name="clearColor"></param>
/// <returns></returns>
Microsoft::WRL::ComPtr<ID3D12Resource> DirectXCommon::CreateRenderTextureResource(Microsoft::WRL::ComPtr<ID3D12Device> device,
	uint32_t width, uint32_t height, DXGI_FORMAT format, const Vector4& clearColor)
{
	/*-----------------
	    リソースの設定
	-----------------*/

	D3D12_RESOURCE_DESC resourceDesc = {};

	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D; // 2Dテクスチャの場合
	resourceDesc.Alignment = 0; // 通常は0 (D3D12が最適なアライメントを選択)
	resourceDesc.Width = width;
	resourceDesc.Height = height;
	resourceDesc.DepthOrArraySize = 1; // 2Dテクスチャの配列サイズ（単一なら1）
	resourceDesc.MipLevels = 1; // レンダーターゲットの場合、通常は1ミップレベル
	resourceDesc.Format = format;
	resourceDesc.SampleDesc.Count = 1; // マルチサンプリングしない場合、通常1
	resourceDesc.SampleDesc.Quality = 0; // 通常0
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN; // 通常はUNKNOWN
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;


	/*-----------------
	    ヒープの設定
	-----------------*/

	D3D12_HEAP_PROPERTIES heapProperties = {};

	// VRAMに配置する
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;


	/*-------------
	     最適値
	-------------*/

	D3D12_CLEAR_VALUE clearValue = {};
	clearValue.Format = format;
	clearValue.Color[0] = clearColor.x;
	clearValue.Color[1] = clearColor.y;
	clearValue.Color[2] = clearColor.z;
	clearValue.Color[3] = clearColor.w;


	// リソースを生成する
	Microsoft::WRL::ComPtr<ID3D12Resource> resource = nullptr;
	HRESULT hr = device->CreateCommittedResource(
		&heapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_RENDER_TARGET, &clearValue, IID_PPV_ARGS(&resource));

	assert(SUCCEEDED(hr));

	return resource;
}

/// <summary>
/// リソースステートを変更する（バリアを張る）
/// </summary>
/// <param name="commandList">コマンドリスト</param>
/// <param name="resource">リソース</param>
/// <param name="beforeState">遷移前（現在）のリソースステート</param>
/// <param name="afterState">遷移後のリソースステート</param>
void DirectXCommon::ChangeResourceState(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList,
	Microsoft::WRL::ComPtr<ID3D12Resource> resource,D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState)
{
	/*--------------------
	    バリアを設定する
	--------------------*/

	D3D12_RESOURCE_BARRIER barrier{};

	// トランジションバリア
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;

	// None
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

	// バリアを張る対象のリソース
	barrier.Transition.pResource = resource.Get();

	// 遷移前（現在）のバリア
	barrier.Transition.StateBefore = beforeState;

	// 遷移後のバリア
	barrier.Transition.StateAfter = afterState;

	// トランジションバリアを張る
	commandList->ResourceBarrier(1, &barrier);
}


/// <summary>
/// デバッグレイヤーを有効化する
/// </summary>
void DirectXCommon::ActiveDebugLayer()
{
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController_))))
	{
		// デバッグレイヤーを有効化する
		debugController_->EnableDebugLayer();

		// GPU側でもチェックを行うようにする
		debugController_->SetEnableGPUBasedValidation(TRUE);
	}
}

/// <summary>
/// DXGIFactoryを生成する
/// </summary>
void DirectXCommon::GenerateDXGIFactory()
{
	HRESULT hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_));
	assert(SUCCEEDED(hr));

	// 生成完了ログ
	log_->Log("Create DXGIFactory \n");
}

/// <summary>
/// 使用するアダプタを取得する
/// </summary>
void DirectXCommon::SelectUseAdapter()
{
	// 良い順にアダプタを頼む
	for (UINT i = 0;
		dxgiFactory_->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&useAdapter_)) != DXGI_ERROR_NOT_FOUND;
		++i)
	{
		// アダプタ情報を取得する
		DXGI_ADAPTER_DESC3 adapterDesc{};
		HRESULT hr = useAdapter_->GetDesc3(&adapterDesc);
		assert(SUCCEEDED(hr));

		// ソフトウェアアダプタでなければ採用
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE))
		{
			// 採用したアダプタの情報をログに出力
			log_->Log(ConvertString(std::format(L"Use Adapter : {} \n", adapterDesc.Description)));
			break;
		}

		// ソフトウェアアダプタの場合はなかったことにする
		useAdapter_ = nullptr;
	}

	assert(useAdapter_ != nullptr);
}

/// <summary>
/// デバイスを生成する
/// </summary>
void DirectXCommon::GenerateDevice()
{
	// 機能レベル
	D3D_FEATURE_LEVEL featureLevels[] =
	{ D3D_FEATURE_LEVEL_12_2 , D3D_FEATURE_LEVEL_12_1 , D3D_FEATURE_LEVEL_12_0 };

	// ログ出力用の文字列
	const char* featureLevelStrings[] = { "12.2" , "12.1", "12.0" };

	// 高い順に生成できるか試していく
	for (size_t i = 0; i < _countof(featureLevels); ++i)
	{
		// 採用したアダプタでデバイスを生成する
		HRESULT hr = D3D12CreateDevice(useAdapter_.Get(), featureLevels[i], IID_PPV_ARGS(&device_));

		// 指定した機能レベルでデバイスが生成できたかどうか
		if (SUCCEEDED(hr))
		{
			// 生成できた旨を伝えるログ
			log_->Log(std::format("FeatureLevel : {} \n", featureLevelStrings[i]));
			break;
		}
	}

	assert(device_ != nullptr);

	// 初期化完了
	log_->Log("Complate create D3D12Device!!! \n");
}

/// <summary>
/// エラーと警告で停止させる
/// </summary>
void DirectXCommon::StopOnErrorsAndWarnings()
{
	Microsoft::WRL::ComPtr<ID3D12InfoQueue> infoQueue = nullptr;
	if (SUCCEEDED(device_->QueryInterface(IID_PPV_ARGS(&infoQueue))))
	{
		/*---------------------------
		    エラーと警告で停止させる
		---------------------------*/

		// やばいエラー時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);

		// エラー時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);

		// 警告時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);


		/*----------------------
		    エラーと警告の抑制
		----------------------*/

		// 抑制するメッセージID
		D3D12_MESSAGE_ID denyIds[] =
		{ D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE };

		// 抑制するレベル
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };

		D3D12_INFO_QUEUE_FILTER filter{};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;

		// 指定したメッセージの表示を抑制する
		infoQueue->PushStorageFilter(&filter);
	}
}

/// <summary>
/// コマンドキューを生成する
/// </summary>
void DirectXCommon::GenerateCommandQueue()
{
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};

	HRESULT hr = device_->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue_));
	assert(SUCCEEDED(hr));

	// コマンドキューを生成した旨のログ
	log_->Log("Create CommandQueue \n");
}

/// <summary>
/// コマンドアロケータを生成する
/// </summary>
void DirectXCommon::GenerateCommandAllocator()
{
	HRESULT hr = device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,IID_PPV_ARGS(&commandAllocator_));
	assert(SUCCEEDED(hr));

	// コマンドアロケータを生成した旨のログ
	log_->Log("Create CommandAllocator \n");
}

/// <summary>
/// コマンドリストを生成する
/// </summary>
void DirectXCommon::GenerateCommandList()
{
	HRESULT hr = device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator_.Get(), nullptr, IID_PPV_ARGS(&commandList_));
	assert(SUCCEEDED(hr));

	// コマンドリストを生成した旨のログ
	log_->Log("Create CommandList \n");
}


/// <summary>
/// スワップチェーンを生成する
/// </summary>
void DirectXCommon::GenerateSwapChain()
{
	/*----------------------------
	    スワップチェーンを設定する
	----------------------------*/

	// 画面の幅
	swapChainDesc_.Width = windowApplication_->GetWindowWidth();
	swapChainDesc_.Height = windowApplication_->GetWindowHeight();

	// 色の形式
	swapChainDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// マルチサンプルしない
	swapChainDesc_.SampleDesc.Count = 1;

	// 描画のターゲットとして利用する
	swapChainDesc_.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// ダブルバッファ
	swapChainDesc_.BufferCount = 2;

	// モニタに移したら、中身を破棄する
	swapChainDesc_.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	// 生成
	HRESULT hr = dxgiFactory_->CreateSwapChainForHwnd(commandQueue_.Get(), windowApplication_->GetHwnd(), &swapChainDesc_,
		nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(swapChain_.GetAddressOf()));
	assert(SUCCEEDED(hr));

	// スワップチェーンを生成した旨のログ
	log_->Log("Create SwapChain \n");
}

/// <summary>
/// スワップチェーンからリソースを引っ張ってくる
/// </summary>
void DirectXCommon::PullSwapChainResource()
{
	HRESULT hr = swapChain_->GetBuffer(0, IID_PPV_ARGS(&swapChainResources_[0]));
	assert(SUCCEEDED(hr));

	hr = swapChain_->GetBuffer(1, IID_PPV_ARGS(&swapChainResources_[1]));
	assert(SUCCEEDED(hr));
}

/// <summary>
/// RTVを生成する
/// </summary>
void DirectXCommon::GenerateRTV()
{
	/*---------------
	    RTVの設定
	---------------*/

	// 出力結果をSRGBに変換して書き込む
	rtvDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	// 2Dテクスチャとして書き込む
	rtvDesc_.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;


	/*--------------------------
	    ディスクリプタに格納する
	--------------------------*/

	rtvHandles_[0] = GetCPUDescriptorHandle(rtvDescriptorHeap_, device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV), 0);
	device_->CreateRenderTargetView(swapChainResources_[0].Get(), &rtvDesc_, rtvHandles_[0]);

	rtvHandles_[1] = GetCPUDescriptorHandle(rtvDescriptorHeap_, device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV), 1);
	device_->CreateRenderTargetView(swapChainResources_[1].Get(), &rtvDesc_, rtvHandles_[1]);
}

/// <summary>
/// DSVを生成する
/// </summary>
void DirectXCommon::GenerateDSV()
{
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};

	// フォーマット
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 二次元
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

	// DSVヒープの先頭にDSVを作る
	device_->CreateDepthStencilView(depthStencilResource_.Get(), &dsvDesc, dsvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart());
}

/// <summary>
/// フェンスを生成する
/// </summary>
void DirectXCommon::GenerateFence()
{
	HRESULT hr = device_->CreateFence(fenceValue_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
	assert(SUCCEEDED(hr));

	log_->Log("Create Fence \n");
}

/// <summary>
/// イベントを生成する
/// </summary>
void DirectXCommon::GenerateEvent()
{
	fenceEvent_ = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent_ != nullptr);

	log_->Log("Create Event \n");
}

/// <summary>
/// GPUの実行完了を待つ
/// </summary>
void DirectXCommon::WaitForGPU()
{
	// フェンスの値を更新
	fenceValue_++;

	// GPUがここまでたどり着いたときに、フェンスの値を指定した値に代入するようにシグナルを送る
	commandQueue_->Signal(fence_.Get(), fenceValue_);

	// フェンスの値が指定したシグナル値にたどり着いているか確認する
	if (fence_->GetCompletedValue() < fenceValue_)
	{
		// 指定したシグナル値にたどり着いていないので、たどり着くまで待つようにイベントを設定する
		fence_->SetEventOnCompletion(fenceValue_, fenceEvent_);

		// イベントを待つ
		WaitForSingleObject(fenceEvent_, INFINITE);
	}
}