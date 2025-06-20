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

	// PSO
	for (uint32_t i = 0; i < kBlendModekCountOfBlendMode; i++)
	{
		delete psoObject3d_[i];
		delete psoParticle_[i];
		delete psoLine3d_[i];
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
	rtvDescriptorHeap_ = CreateDescritprHeap(device_, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, kNumRtvDescriptors, false);

	// SRV用ディスクリプタヒープを生成する
	srvDescriptorHeap_ = CreateDescritprHeap(device_, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, kNumSrvDescriptors, true);

	// DSV用ディスクリプタヒープを生成する
	dsvDescriptorHeap_ = CreateDescritprHeap(device_, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, kNunDsvDescriptors, false);

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


	// Object3D用のPSOの生成と初期化
	//psoObject3d_[kBlendModeNone] = new Object3dBlendNone();
	//psoObject3d_[kBlendModeNone]->Initialize(log_, dxc_, device_);

	psoObject3d_[kBlendModeNormal] = new Object3dBlendNormal();
	psoObject3d_[kBlendModeNormal]->Initialize(log_, dxc_, device_);

	//psoObject3d_[kBlendModeAdd] = new Object3dBlendAdd();
	//psoObject3d_[kBlendModeAdd]->Initialize(log_, dxc_, device_);

	//psoObject3d_[kBlendModeSubtract] = new Object3dBlendSubtract();
	//psoObject3d_[kBlendModeSubtract]->Initialize(log_, dxc_, device_);

	//psoObject3d_[kBlendModeMultiply] = new Object3dBlendMultiply();
	//psoObject3d_[kBlendModeMultiply]->Initialize(log_, dxc_, device_);

	//psoObject3d_[kBlendModeScreen] = new Object3dBlendScreen();
	//psoObject3d_[kBlendModeScreen]->Initialize(log_, dxc_, device_);


	// Particle用のPSOの生成と初期化
	psoParticle_[kBlendModeNone] = new ParticleBlendNone();
	psoParticle_[kBlendModeNone]->Initialize(log_, dxc_, device_);

	psoParticle_[kBlendModeNormal] = new ParticleBlendNormal();
	psoParticle_[kBlendModeNormal]->Initialize(log_, dxc_, device_);

	psoParticle_[kBlendModeAdd] = new ParticleBlendAdd();
	psoParticle_[kBlendModeAdd]->Initialize(log_, dxc_, device_);

	psoParticle_[kBlendModeSubtract] = new ParticleBlendSubtract();
	psoParticle_[kBlendModeSubtract]->Initialize(log_, dxc_, device_);

	psoParticle_[kBlendModeMultiply] = new ParticleBlendMultiply();
	psoParticle_[kBlendModeMultiply]->Initialize(log_, dxc_, device_);

	psoParticle_[kBlendModeScreen] = new ParticleBlendScreen();
	psoParticle_[kBlendModeScreen]->Initialize(log_, dxc_, device_);

	
	// Line3d用のPSOの生成と初期化
	psoLine3d_[kBlendModeNone] = new Line3dBlendNone();
	psoLine3d_[kBlendModeNone]->Initialize(log_, dxc_, device_);

	psoLine3d_[kBlendModeNormal] = new Line3dBlendNormal();
	psoLine3d_[kBlendModeNormal]->Initialize(log_, dxc_, device_);

	psoLine3d_[kBlendModeAdd] = new Line3dBlendAdd();
	psoLine3d_[kBlendModeAdd]->Initialize(log_, dxc_, device_);

	psoLine3d_[kBlendModeSubtract] = new Line3dBlendSubtract();
	psoLine3d_[kBlendModeSubtract]->Initialize(log_, dxc_, device_);

	psoLine3d_[kBlendModeMultiply] = new Line3dBlendMultiply();
	psoLine3d_[kBlendModeMultiply]->Initialize(log_, dxc_, device_);

	psoLine3d_[kBlendModeScreen] = new Line3dBlendScreen();
	psoLine3d_[kBlendModeScreen]->Initialize(log_, dxc_, device_);


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


	/*-----------------------------
	    バッファリソースを作成する
	-----------------------------*/

	// インデックスリソース
	indexResourceSprite_ = CreateBufferResource(device_, sizeof(uint32_t) * 6);

	// 頂点リソース
	vertexBufferResourceSprite_ = CreateBufferResource(device_, sizeof(VertexData) * 4);


	for (uint32_t i = 0; i < kMaxNumResource; i++)
	{
		// モデル
		MaterialResourceModel_[i] = CreateBufferResource(device_, sizeof(Material));
		TransformationResourceModel_[i] = CreateBufferResource(device_, sizeof(TransformationMatrix));
		pointLightResourceModel_[i] = CreateBufferResource(device_, sizeof(PointLightFoirGPU));
		spotLightResourceModel_[i] = CreateBufferResource(device_, sizeof(SpotLightForGPU));
		cameraResourceModel_[i] = CreateBufferResource(device_, sizeof(CameraForGPU));

		// スプライト
		MaterialResourceSprite_[i] = CreateBufferResource(device_, sizeof(Material));
		TransformationResourceSprite_[i] = CreateBufferResource(device_, sizeof(TransformationMatrix));

		// 線
		vertexBufferResourceLine_[i] = CreateBufferResource(device_, sizeof(Vector4) * 2);
		MaterialResourceLine_[i] = CreateBufferResource(device_, sizeof(Material));
		TransformationResourceLine_[i] = CreateBufferResource(device_, sizeof(TransformationMatrix));
	}


	/*-------------
	    平行光源
	-------------*/

	// リソース
	instancingDirectionalLightResource_ = CreateBufferResource(device_, sizeof(DirectionalLightForGPU) * kMaxNumDirectionalLight);

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
	instancingDirectionalLightSrvHandleCPU_ = GetCPUDescriptorHandle(srvDescriptorHeap_, device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV), 1);
	instancingDirectionalLightSrvHandleGPU_ = GetGPUDescriptorHandle(srvDescriptorHeap_, device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV), 1);
	device_->CreateShaderResourceView(instancingDirectionalLightResource_.Get(), &instancingDirectionalLightSrvDesc, instancingDirectionalLightSrvHandleCPU_);


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
	instancingSrvHandleCPU_ = GetCPUDescriptorHandle(srvDescriptorHeap_, device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV), 2);
	instancingSrvHandleGPU_ = GetGPUDescriptorHandle(srvDescriptorHeap_, device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV), 2);
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
}

/// <summary>
/// 描画後処理
/// </summary>
void DirectXCommon::PostDraw()
{
	// ImGuiの内部コマンドを生成する
	ImGui::Render();

	// ImGuiの描画コマンドを積む
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList_.Get());

	// これから書き込むバックバッファのインデックスを取得
	UINT backBuffeIndex = swapChain_->GetCurrentBackBufferIndex();

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
	useNumResourceModel_ = 0;
	useNumResourceSprite_ = 0;
	useNumResourceLine_ = 0;

	// ライト
	useNumDirectionalLightCount_ = 0;
	useNumDirectionLightData_->num = useNumDirectionalLightCount_;
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

	// データに書き込む
	DirectionalLightForGPU* directionalLightData = nullptr;
	instancingDirectionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));

	// 色
	directionalLightData[useNumDirectionalLightCount_].color = directionalLight->color_ * directionalLight->intensity_;

	// 向き
	directionalLightData[useNumDirectionalLightCount_].direction = Normalize(directionalLight->direction_);


	// カウントする
	useNumDirectionalLightCount_++;
	useNumDirectionLightData_->num = useNumDirectionalLightCount_;
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
	const Camera3D* camera, uint32_t modelHandle, Vector4 color,
	const PointLight* pointLight, const SpotLight* spotLight)
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
	MaterialResourceModel_[useNumResourceModel_]->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	materialData->color = color;
	materialData->enableLighting = true;
	materialData->uvTransform = 
		MakeScaleMatrix(uvTransform->scale_) * MakeRotateZMatrix(uvTransform->rotation_.z) * MakeTranslateMatrix(uvTransform->translation_);
	materialData->shininess = 18.0f;


	/*------------------
		座標変換の行列
	------------------*/

	// データを書き込む
	TransformationMatrix* transformationData = nullptr;
	TransformationResourceModel_[useNumResourceModel_]->Map(0, nullptr, reinterpret_cast<void**>(&transformationData));
	transformationData->worldViewProjection = worldTransform->worldMatrix_ * camera->viewMatrix_ * camera->projectionMatrix_;
	transformationData->world = worldTransform->worldMatrix_;
	transformationData->worldInverseTranspose = MakeTransposeMatrix(MakeInverseMatrix(worldTransform->worldMatrix_));

	/*------------
		点光源
	------------*/

	// データを書き込む
	PointLightFoirGPU* pointLightData = nullptr;
	pointLightResourceModel_[useNumResourceModel_]->Map(0, nullptr, reinterpret_cast<void**>(&pointLightData));
	(*pointLightData).color = pointLight->color_;
	(*pointLightData).position = pointLight->position_;
	(*pointLightData).intensity = pointLight->intensity_;
	(*pointLightData).radius = pointLight->radius_;
	(*pointLightData).decay = pointLight->decay_;


	/*------------------
	    スポットライト
	------------------*/

	// データを書き込む
	SpotLightForGPU* spotLightData = nullptr;
	spotLightResourceModel_[useNumResourceModel_]->Map(0, nullptr, reinterpret_cast<void**>(&spotLightData));
	(*spotLightData).color = spotLight->color_;
	(*spotLightData).position = spotLight->position_;
	(*spotLightData).direction = Normalize(spotLight->direction_);
	(*spotLightData).intensity = spotLight->intensity_;
	(*spotLightData).distance = spotLight->distance_;
	(*spotLightData).decay = spotLight->decay_;
	(*spotLightData).cosAngle = spotLight->cosAngle_;
	(*spotLightData).fallofStart = spotLight->fallofStart_;


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
	commandList_->SetGraphicsRootConstantBufferView(0, MaterialResourceModel_[useNumResourceModel_]->GetGPUVirtualAddress());

	// 座標変換用のCBVを設定
	commandList_->SetGraphicsRootConstantBufferView(1, TransformationResourceModel_[useNumResourceModel_]->GetGPUVirtualAddress());

	// 平行光源用のインスタンシングの設定
	commandList_->SetGraphicsRootDescriptorTable(7, instancingDirectionalLightSrvHandleGPU_);
	commandList_->SetGraphicsRootConstantBufferView(3, useNumDirectionalLightResource_->GetGPUVirtualAddress());

	// 点光源用のCBVを設定
	commandList_->SetGraphicsRootConstantBufferView(5, pointLightResourceModel_[useNumResourceModel_]->GetGPUVirtualAddress());

	// スポットライトのCBVを設定
	commandList_->SetGraphicsRootConstantBufferView(6, spotLightResourceModel_[useNumResourceModel_]->GetGPUVirtualAddress());

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
/// モデルを描画する
/// </summary>
/// <param name="worldTransform"></param>
/// <param name="uvTransform"></param>
/// <param name="camera"></param>
/// <param name="modelHandle"></param>
/// <param name="color"></param>
void DirectXCommon::DrawModel(const WorldTransform* worldTransform, const UvTransform* uvTransform,
	const Camera3D* camera, uint32_t modelHandle, Vector4 color)
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
	MaterialResourceModel_[useNumResourceModel_]->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	materialData->color = color;
	materialData->enableLighting = false;
	materialData->uvTransform =
		MakeScaleMatrix(uvTransform->scale_) * MakeRotateZMatrix(uvTransform->rotation_.z) * MakeTranslateMatrix(uvTransform->translation_);
	materialData->shininess = 18.0f;


	/*------------------
		座標変換の行列
	------------------*/

	// データを書き込む
	TransformationMatrix* transformationData = nullptr;
	TransformationResourceModel_[useNumResourceModel_]->Map(0, nullptr, reinterpret_cast<void**>(&transformationData));
	transformationData->worldViewProjection = worldTransform->worldMatrix_ * camera->viewMatrix_ * camera->projectionMatrix_;
	transformationData->world = worldTransform->worldMatrix_;
	transformationData->worldInverseTranspose = MakeTransposeMatrix(MakeInverseMatrix(worldTransform->worldMatrix_));



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
	commandList_->SetGraphicsRootConstantBufferView(0, MaterialResourceModel_[useNumResourceModel_]->GetGPUVirtualAddress());

	// 座標変換用のCBVを設定
	commandList_->SetGraphicsRootConstantBufferView(1, TransformationResourceModel_[useNumResourceModel_]->GetGPUVirtualAddress());

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
	MaterialResourceLine_[useNumResourceLine_]->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	materialData->color = color;
	materialData->enableLighting = false;
	materialData->shininess = 0.0f;
	materialData->uvTransform = MakeIdenityMatirx();


	/*-------------
	    座標変換
	-------------*/

	// データを書き込む
	TransformationMatrix* transformationData = nullptr;
	TransformationResourceLine_[useNumResourceLine_]->Map(0, nullptr, reinterpret_cast<void**>(&transformationData));

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
	commandList_->SetGraphicsRootConstantBufferView(0, MaterialResourceLine_[useNumResourceLine_]->GetGPUVirtualAddress());

	// 座標変換用のCBVを設定
	commandList_->SetGraphicsRootConstantBufferView(1, TransformationResourceLine_[useNumResourceLine_]->GetGPUVirtualAddress());

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
	MaterialResourceSprite_[useNumResourceSprite_]->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	materialData->color = color;
	materialData->enableLighting = false;
	materialData->uvTransform = 
		MakeScaleMatrix(uvTransform->scale_) * MakeRotateZMatrix(uvTransform->rotation_.z) * MakeTranslateMatrix(uvTransform->translation_);


	/*------------------
		座標変換の行列
	------------------*/

	// データを書き込む
	TransformationMatrix* transformationData = nullptr;
	TransformationResourceSprite_[useNumResourceSprite_]->Map(0, nullptr, reinterpret_cast<void**>(&transformationData));
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
	commandList_->SetGraphicsRootConstantBufferView(0, MaterialResourceSprite_[useNumResourceSprite_]->GetGPUVirtualAddress());

	// 座標変換用のCBVを設定
	commandList_->SetGraphicsRootConstantBufferView(1, TransformationResourceSprite_[useNumResourceSprite_]->GetGPUVirtualAddress());

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