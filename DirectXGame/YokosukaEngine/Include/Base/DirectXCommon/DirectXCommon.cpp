
#include "DirectXCommon.h"
#include "../WinApp/WinApp.h"

#pragma comment(lib , "d3d12.lib")
#pragma comment(lib , "dxgi.lib")

/// <summary>
/// デストラクタ
/// </summary>
DirectXCommon::~DirectXCommon()
{
	// ImGuiの終了処理
	ImGui_ImplWin32_Shutdown();
	ImGui_ImplDX12_Shutdown();
	ImGui::DestroyContext();

	for (uint32_t i = 0; i < kEfectCount; i++)
	{
		delete psoPostEffect_[i];
	}

	// PSO
	for (uint32_t i = 0; i < kBlendModekCountOfBlendMode; i++)
	{
		delete psoObject3d_[i];
		delete psoParticle_[i];
		delete psoLine3d_[i];
		delete psoPrimitive_[i];
		delete psoCopyImage_[i];
	}

	// イベント
	CloseHandle(fenceEvent_);
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="log">ログ</param>
void DirectXCommon::Initialize(Logging* logging, WinApp* winApp)
{
	// 乱数生成器の初期化
	std::random_device seedGenerater;
	std::mt19937 randomEngine(seedGenerater());

	//nullptrチェック
	assert(logging);
	assert(winApp);

	// 引数を受け取る
	logging_ = logging;
	winApp_ = winApp;



	// テクスチャストアを初期化する
	textureStore_ = std::make_unique<TextureStore>();
	textureStore_->Initialize(this);

	// モデルデータストアを初期化する
	modelDataStore_ = std::make_unique<ModelDataStore>();
	modelDataStore_->Initialize(textureStore_.get());

	// DXCの生成と初期化
	dxc_ = std::make_unique<DirectXShaderCompiler>();
	dxc_->Initialize(logging_);


#ifdef _DEBUG

	// デバッグレイヤーを有効化する
	ActiveDebugLayer();

#endif

	// DirectXGPUの生成と初期化
	directXGPU_ = std::make_unique<DirectXGPU>();
	directXGPU_->Initialize(logging_);

	// DirectXCommandの生成と初期化
	directXCommand_ = std::make_unique<DirectXCommand>();
	directXCommand_->Initialize(logging_, directXGPU_.get());

#ifdef _DEBUG

	// エラーと警告で停止させる
	StopOnErrorsAndWarnings();

#endif

	// RTV用ディスクリプタヒープを生成する
	rtvDescriptorHeap_ = CreateDescritprHeap(directXGPU_->GetDevice(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV, kMaxNumRtvDescriptors, false);

	// SRV用ディスクリプタヒープを生成する
	srvDescriptorHeap_ = CreateDescritprHeap(directXGPU_->GetDevice(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, kMaxNumSrvDescriptors, true);

	// DSV用ディスクリプタヒープを生成する
	dsvDescriptorHeap_ = CreateDescritprHeap(directXGPU_->GetDevice(), D3D12_DESCRIPTOR_HEAP_TYPE_DSV, kMaxNunDsvDescriptors, false);

	// スワップチェーンを生成する
	GenerateSwapChain();

	// スワップチェーンからリソースを引っ張ってくる
	PullSwapChainResource();

	// RTVを作り、リソースを格納する
	GenerateRTV();

	// 深度情報のリソースを生成する
	depthStencilResource_ = CreateDepthStencilTextureResource(directXGPU_->GetDevice(), winApp_->GetWindowWidth(), winApp_->GetWindowHeight());

	// DSVを作り、リソースを格納する
	GenerateDSV();

	// フェンスを生成する
	GenerateFence();

	// イベントを生成する
	GenerateEvent();


	// Object3dを生成する
	CreateObject3d();

	// Particleを生成する
	CreateParticle();

	// Line3dを生成する
	CreateLine3d();

	// Primitiveを生成する
	CreatePrimitive();

	// CopyImageを生成する
	CreateCopyImage();

	// PostEffectを生成する
	CreatePostEffect();



	// ビューポート
	viewport_.Width = static_cast<float>(winApp_->GetWindowWidth());
	viewport_.Height = static_cast<float>(winApp_->GetWindowHeight());
	viewport_.TopLeftX = 0;
	viewport_.TopLeftY = 0;
	viewport_.MinDepth = 0.0f;
	viewport_.MaxDepth = 1.0f;

	// シザーレクト
	scissorRect_.left = 0;
	scissorRect_.right = static_cast<LONG>(winApp_->GetWindowWidth());
	scissorRect_.top = 0;
	scissorRect_.bottom = static_cast<LONG>(winApp_->GetWindowHeight());

	// ImGuiを初期化する
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(winApp_->GetHwnd());
	ImGui_ImplDX12_Init(directXGPU_->GetDevice(), swapChainDesc_.BufferCount, rtvDesc_.Format,
		srvDescriptorHeap_.Get(), srvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart(), srvDescriptorHeap_->GetGPUDescriptorHandleForHeapStart());


	/*----------------------------
		オフスクリーンレンダリング
	----------------------------*/

	for (uint32_t i = 0; i < kMaxNumOffscreen; i++)
	{
		// レンダーテクスチャを作成する
		const Vector4 kRenderTargetClearColor = { 0.1f , 0.1f , 0.1f , 1.0f };
		offscreen_[i].renderTextureResource =
			CreateRenderTextureResource(directXGPU_->GetDevice(), (uint32_t)winApp_->GetWindowWidth(), (uint32_t)winApp_->GetWindowHeight(),
				DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, kRenderTargetClearColor);

		offscreen_[i].renderTextureRtvCPUHnalde = GetRTVCPUDescriptorHandle(rtvDescriptorHeap_, directXGPU_->GetDevice());
		directXGPU_->GetDevice()->CreateRenderTargetView(offscreen_[i].renderTextureResource.Get(), &rtvDesc_, offscreen_[i].renderTextureRtvCPUHnalde);
	}

	for (uint32_t i = 0; i < kMaxNumOffscreen; i++)
	{
		// レンダーテクスチャを読み込むSRVを作成する
		D3D12_SHADER_RESOURCE_VIEW_DESC renderTextureSrvDesc{};
		renderTextureSrvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		renderTextureSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		renderTextureSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		renderTextureSrvDesc.Texture2D.MipLevels = 1;

		offscreen_[i].renderTextureSrvCPUHandle = GetSRVCPUDescriptorHandle(srvDescriptorHeap_, directXGPU_->GetDevice());
		offscreen_[i].renderTextureSrvGPUHandle = GetSRVGPUDescriptorHandle(srvDescriptorHeap_, directXGPU_->GetDevice());

		directXGPU_->GetDevice()->CreateShaderResourceView(offscreen_[i].renderTextureResource.Get(),
			&renderTextureSrvDesc, offscreen_[i].renderTextureSrvCPUHandle);
	}


	/*-----------------------------
		バッファリソースを作成する
	-----------------------------*/

	// インデックスリソース
	indexResourceSprite_ = CreateBufferResource(directXGPU_->GetDevice(), sizeof(uint32_t) * 6);

	// 頂点リソース
	vertexBufferResourceSprite_ = CreateBufferResource(directXGPU_->GetDevice(), sizeof(VertexData) * 4);


	for (uint32_t i = 0; i < kMaxNumResource; i++)
	{
		// 平面
		planeResources_[i] = std::make_unique<PlaneResources>();
		planeResources_[i]->Initialize(directXGPU_->GetDevice());

		// 球
		sphereResources_[i] = std::make_unique<SphereResources>();
		sphereResources_[i]->Initialize(directXGPU_->GetDevice());

		// リング
		ringResources_[i] = std::make_unique<RingResources>();
		ringResources_[i]->Initialize(directXGPU_->GetDevice());

		// 円柱
		cylinderResources_[i] = std::make_unique<CylinderResources>();
		cylinderResources_[i]->Initialize(directXGPU_->GetDevice());

		// モデル
		materialResourceModel_[i] = CreateBufferResource(directXGPU_->GetDevice(), sizeof(Material));
		transformationResourceModel_[i] = CreateBufferResource(directXGPU_->GetDevice(), sizeof(TransformationMatrix));
		cameraResourceModel_[i] = CreateBufferResource(directXGPU_->GetDevice(), sizeof(CameraForGPU));

		// スプライト
		materialResourceSprite_[i] = CreateBufferResource(directXGPU_->GetDevice(), sizeof(Material));
		transformationResourceSprite_[i] = CreateBufferResource(directXGPU_->GetDevice(), sizeof(TransformationMatrix));

		// 線
		lineResources_[i] = std::make_unique<LineResources>();
		lineResources_[i]->Initliaze(directXGPU_->GetDevice());
	}


	/*-------------
		平行光源
	-------------*/

	// インスタンシングリソース
	instancingDirectionalLight_ = CreateStructuredBufferResource(directXGPU_->GetDevice(),
		srvDescriptorHeap_.Get(), kMaxNumDirectionalLight, sizeof(DirectionalLightForGPU));
	instancingDirectionalLight_.resource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));

	// カウントリソース
	useNumDirectionalLightResource_ = CreateBufferResource(directXGPU_->GetDevice(), sizeof(UseNumDirectionalLight));
	useNumDirectionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&useNumDirectionLightData_));
	useNumDirectionLightData_->num = 0;


	/*------------------
		ポイントライト
	------------------*/

	// インスタンシングリソース
	instancingPointLight_ = CreateStructuredBufferResource(directXGPU_->GetDevice(),
		srvDescriptorHeap_.Get(), kMaxNumPointLight, sizeof(PointLightForGPU));
	instancingPointLight_.resource->Map(0, nullptr, reinterpret_cast<void**>(&pointLightData_));

	// カウントリソース
	useNumPointLightResource_ = CreateBufferResource(directXGPU_->GetDevice(), sizeof(UseNumPointLight));
	useNumPointLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&useNumPointLightData_));
	useNumPointLightData_->num = 0;


	/*------------------
		スポットライト
	------------------*/

	// インスタンシングリソース
	instancingSpotLight_ = CreateStructuredBufferResource(directXGPU_->GetDevice(),
		srvDescriptorHeap_.Get(), kMaxNumSpotLight, sizeof(SpotLightForGPU));
	instancingSpotLight_.resource->Map(0, nullptr, reinterpret_cast<void**>(&spotLightData_));

	// カウントリソース
	useNumSpotLightResource_ = CreateBufferResource(directXGPU_->GetDevice(), sizeof(UseNumSpotLight));
	useNumSpotLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&useNumSpotLightData_));
	useNumSpotLightData_->num = 0;


	/*----------------
		パーティクル
	----------------*/

	// パーティクル
	instancing_ = CreateStructuredBufferResource(directXGPU_->GetDevice(), srvDescriptorHeap_.Get(), kNumMaxInstance, sizeof(ParticleForGPU));
	materialResourceParticle_ = CreateBufferResource(directXGPU_->GetDevice(), sizeof(Material));

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

	// 使用したブレンドモードを初期化する
	useObject3dBlendMode_ = kBlendModeNormal;
	useParticleBlendMode_ = kBlendModeAdd;
	useLine3dBlendMode_ = kBlendModeNormal;
	usePrimitiveBlendMode_ = kBlendModeNormal;
	useCopyImageBlendMode_ = kBlendModeNormal;

	// カウントしたリソースを初期化する
	useNumResourcePlane_ = 0;
	useNumResourceSphere_ = 0;
	useNumResourceRing_ = 0;
	useNumResourceCylinder_ = 0;
	useNumMaterialResourceModel_ = 0;
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

	// オフスクリーン
	useNumOffscreen_ = 0;


	// オフスクリーンをセットする
	SetOffscreen();
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
	ChangeResourceState(directXCommand_->GetCommandList(), swapChainResources_[backBuffeIndex], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	// 描画先のRTVとDSVを設定する
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	directXCommand_->GetCommandList()->OMSetRenderTargets(1, &rtvHandles_[backBuffeIndex], false, &dsvHandle);

	// 指定した色で画面全体をクリアする
	float clearColor[] = { 0.1f , 0.1f , 0.1f , 1.0f };
	directXCommand_->GetCommandList()->ClearRenderTargetView(rtvHandles_[backBuffeIndex], clearColor, 0, nullptr);

	// 描画用のディスクリプタヒープの設定
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeaps[] = { srvDescriptorHeap_ };
	directXCommand_->GetCommandList()->SetDescriptorHeaps(1, descriptorHeaps->GetAddressOf());

	// ビューポート設定
	directXCommand_->GetCommandList()->RSSetViewports(1, &viewport_);

	// シザーレクト設定
	directXCommand_->GetCommandList()->RSSetScissorRects(1, &scissorRect_);



	// 既にオフスクリーンを使用していたらスワップチェーンのコピーする
	DrawCopyImage();



	// ImGuiの描画コマンドを積む
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), directXCommand_->GetCommandList());

	// RenderTarget -> Present
	ChangeResourceState(directXCommand_->GetCommandList(), swapChainResources_[backBuffeIndex], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

	// コマンドリストの内容を確定させる
	HRESULT hr = directXCommand_->GetCommandList()->Close();
	assert(SUCCEEDED(hr));

	// GPUにコマンドリストの実行を行わせる
	Microsoft::WRL::ComPtr<ID3D12CommandList> commandLists[] = { directXCommand_->GetCommandList() };
	directXCommand_->GetCommandQueue()->ExecuteCommandLists(1, commandLists->GetAddressOf());

	// GPUとOSに画面の交換を行うように通知する
	swapChain_->Present(1, 0);

	// GPUの実行完了を待つ
	WaitForGPU();

	// 次のフレーム用のコマンドリストを準備
	hr = directXCommand_->GetCommandAllocator()->Reset();
	assert(SUCCEEDED(hr));
	hr = directXCommand_->GetCommandList()->Reset(directXCommand_->GetCommandAllocator(), nullptr);
	assert(SUCCEEDED(hr));
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
/// オフスクリーンをセットする
/// </summary>
void DirectXCommon::SetOffscreen()
{
	// 使用できるオフスクリーン数を越えないようにする
	if (useNumOffscreen_ >= kMaxNumOffscreen)
		return;


	// 描画先のRTVとDSVを設定する
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	directXCommand_->GetCommandList()->OMSetRenderTargets(1, &offscreen_[useNumOffscreen_].renderTextureRtvCPUHnalde, false, &dsvHandle);

	// 指定した色で画面全体をクリアする
	float clearColor[] = { 0.1f , 0.1f , 0.1f , 1.0f };
	directXCommand_->GetCommandList()->ClearRenderTargetView(offscreen_[useNumOffscreen_].renderTextureRtvCPUHnalde, clearColor, 0, nullptr);

	// 指定した深度で画面全体をクリアする
	directXCommand_->GetCommandList()->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// 描画用のディスクリプタヒープの設定
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeaps[] = { srvDescriptorHeap_ };
	directXCommand_->GetCommandList()->SetDescriptorHeaps(1, descriptorHeaps->GetAddressOf());

	// ビューポート設定
	directXCommand_->GetCommandList()->RSSetViewports(1, &viewport_);

	// シザーレクト設定
	directXCommand_->GetCommandList()->RSSetScissorRects(1, &scissorRect_);


	// カウントする
	useNumOffscreen_++;
}

/// <summary>
/// オフスクリーンをセットする
/// </summary>
uint32_t DirectXCommon::SetOffscreenEffect(Effect effect)
{
	// 使用できるオフスクリーン数を越えないようにする
	if (useNumOffscreen_ >= kMaxNumOffscreen)
		return 0;


	// 描画先のRTVとDSVを設定する
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	directXCommand_->GetCommandList()->OMSetRenderTargets(1, &offscreen_[useNumOffscreen_].renderTextureRtvCPUHnalde, false, &dsvHandle);

	// 指定した色で画面全体をクリアする
	float clearColor[] = { 0.1f , 0.1f , 0.1f , 1.0f };
	directXCommand_->GetCommandList()->ClearRenderTargetView(offscreen_[useNumOffscreen_].renderTextureRtvCPUHnalde, clearColor, 0, nullptr);

	// 指定した深度で画面全体をクリアする
	directXCommand_->GetCommandList()->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// 描画用のディスクリプタヒープの設定
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeaps[] = { srvDescriptorHeap_ };
	directXCommand_->GetCommandList()->SetDescriptorHeaps(1, descriptorHeaps->GetAddressOf());

	// ビューポート設定
	directXCommand_->GetCommandList()->RSSetViewports(1, &viewport_);

	// シザーレクト設定
	directXCommand_->GetCommandList()->RSSetScissorRects(1, &scissorRect_);


	// エフェクトで切り替える
	switch (effect)
	{
	case kCopyImage:
	default:
		// 通常コピー

		DrawCopyImage();

		break;

	case kGrayScale:
		// グレースケール

		DrawGrayScale();

		break;

	case kSepia:
		// セピア

		DrawSepia();

		break;

	case kVignetteing:
		// ヴィネッティング

		DrawVignetting();

		break;

	case kSmoothing:
		// 平滑化

		DrawSmoothing();

		break;

	case kGaussianFilter:
		// ガウシアンフィルター

		DrawGaussianFilter();

		break;

	case kLuminanceBaseOutline:
		// アウトライン

		DrawOutline();

		break;

	case kBrightnessExtraction:
		// 高輝度抽出

		DrawBrightnessExtraction();

		break;

	case kHide:
		// 隠す

		DrawHide();

		break;

	case kRasterScroll:
		// ラスタースクロール

		DrawRasterScroll();

		break;
	}


	// カウントする
	useNumOffscreen_++;

	return useNumOffscreen_ - 1;
}

/// <summary>
/// RTVに描画したテクスチャをコピーする
/// </summary>
void DirectXCommon::CopyRtvImage(uint32_t rtvNum)
{
	// 既にオフスクリーンを使用していたら上書きする
	if (useNumOffscreen_ <= 0)
		return;

	// 使われていない、描画されていないオフスクリーンは上書きできない
	if (rtvNum >= useNumOffscreen_ - 1)
		return;

	// RenderTarget -> PixelShader
	ChangeResourceState(directXCommand_->GetCommandList(), offscreen_[rtvNum].renderTextureResource, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	// PSOの設定
	psoCopyImage_[useCopyImageBlendMode_]->CommandListSet(directXCommand_->GetCommandList());

	// RenderTextureのRTVを張り付ける
	directXCommand_->GetCommandList()->SetGraphicsRootDescriptorTable(0, offscreen_[rtvNum].renderTextureSrvGPUHandle);

	// 頂点3つ描画
	directXCommand_->GetCommandList()->DrawInstanced(3, 1, 0, 0);

	// PixelShader -> RenderTarget
	ChangeResourceState(directXCommand_->GetCommandList(),
		offscreen_[rtvNum].renderTextureResource, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
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

	// データを書き込む
	uint32_t* indexData = nullptr;
	planeResources_[useNumResourcePlane_]->indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData));
	indexData[0] = 0; indexData[1] = 1; indexData[2] = 2;
	indexData[3] = 0; indexData[4] = 3; indexData[5] = 1;



	/*----------
		頂点
	----------*/

	// データを書き込む
	VertexData* vertexData = nullptr;
	planeResources_[useNumResourcePlane_]->vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

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
	planeResources_[useNumResourcePlane_]->materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
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
	planeResources_[useNumResourcePlane_]->transformationResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationData));
	transformationData->worldViewProjection = worldTransform->worldMatrix_ * camera->viewMatrix_ * camera->projectionMatrix_;
	transformationData->world = worldTransform->worldMatrix_;
	transformationData->worldInverseTranspose = MakeTransposeMatrix(MakeInverseMatrix(worldTransform->worldMatrix_));


	/*-----------
		カメラ
	-----------*/

	// データを書き込む
	CameraForGPU* cameraData = nullptr;
	planeResources_[useNumResourcePlane_]->cameraResource_->Map(0, nullptr, reinterpret_cast<void**>(&cameraData));
	cameraData->worldPosition = camera->translation_;



	/*------------------
		コマンドを積む
	------------------*/

	// ルートシグネチャやPSOの設定
	psoPrimitive_[useObject3dBlendMode_]->CommandListSet(directXCommand_->GetCommandList());

	// IBVを設定する
	directXCommand_->GetCommandList()->IASetIndexBuffer(&planeResources_[useNumResourcePlane_]->indexBufferView_);

	// VBVを設定する
	directXCommand_->GetCommandList()->IASetVertexBuffers(0, 1, &planeResources_[useNumResourcePlane_]->vertexBufferView_);

	// 形状を設定
	directXCommand_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// マテリアル用のCBVを設定
	directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(0, planeResources_[useNumResourcePlane_]->materialResource_->GetGPUVirtualAddress());

	// 座標変換用のCBVを設定
	directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(1, planeResources_[useNumResourcePlane_]->transformationResource_->GetGPUVirtualAddress());

	// 平行光源用のインスタンシングの設定
	directXCommand_->GetCommandList()->SetGraphicsRootDescriptorTable(7, instancingDirectionalLight_.gpuHandle);
	directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(3, useNumDirectionalLightResource_->GetGPUVirtualAddress());

	// ポイントライト用のCBVを設定
	directXCommand_->GetCommandList()->SetGraphicsRootDescriptorTable(8, instancingPointLight_.gpuHandle);
	directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(5, useNumPointLightResource_->GetGPUVirtualAddress());

	// スポットライトのCBVを設定
	directXCommand_->GetCommandList()->SetGraphicsRootDescriptorTable(9, instancingSpotLight_.gpuHandle);
	directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(6, useNumSpotLightResource_->GetGPUVirtualAddress());

	// カメラ用のCBVを設定
	directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(4, planeResources_[useNumResourcePlane_]->cameraResource_->GetGPUVirtualAddress());

	// テクスチャ
	textureStore_->SelectTexture(directXCommand_->GetCommandList(), textureHandle);

	// 描画する
	directXCommand_->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);


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
	const Camera3D* camera, uint32_t textureHandle, uint32_t segment, uint32_t ring, Vector4 color, bool isLighting)
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

	// データを書き込む
	uint32_t* indexData = nullptr;
	sphereResources_[useNumResourceSphere_]->indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData));

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

	// データを書き込む
	VertexData* vertexData = nullptr;
	sphereResources_[useNumResourceSphere_]->vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

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
	sphereResources_[useNumResourceSphere_]->materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
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
	sphereResources_[useNumResourceSphere_]->transformationResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationData));
	transformationData->worldViewProjection = worldTransform->worldMatrix_ * camera->viewMatrix_ * camera->projectionMatrix_;
	transformationData->world = worldTransform->worldMatrix_;
	transformationData->worldInverseTranspose = MakeTransposeMatrix(MakeInverseMatrix(worldTransform->worldMatrix_));


	/*-----------
		カメラ
	-----------*/

	// データを書き込む
	CameraForGPU* cameraData = nullptr;
	sphereResources_[useNumResourceSphere_]->cameraResource_->Map(0, nullptr, reinterpret_cast<void**>(&cameraData));
	cameraData->worldPosition = camera->translation_;



	/*------------------
		コマンドを積む
	------------------*/

	// ルートシグネチャやPSOの設定
	psoPrimitive_[useObject3dBlendMode_]->CommandListSet(directXCommand_->GetCommandList());

	// IBVを設定する
	directXCommand_->GetCommandList()->IASetIndexBuffer(&sphereResources_[useNumResourceSphere_]->indexBufferView_);

	// VBVを設定する
	directXCommand_->GetCommandList()->IASetVertexBuffers(0, 1, &sphereResources_[useNumResourceSphere_]->vertexBufferView_);

	// 形状を設定
	directXCommand_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// マテリアル用のCBVを設定
	directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(0, sphereResources_[useNumResourceSphere_]->materialResource_->GetGPUVirtualAddress());

	// 座標変換用のCBVを設定
	directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(1, sphereResources_[useNumResourceSphere_]->transformationResource_->GetGPUVirtualAddress());

	// 平行光源用のインスタンシングの設定
	directXCommand_->GetCommandList()->SetGraphicsRootDescriptorTable(7, instancingDirectionalLight_.gpuHandle);
	directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(3, useNumDirectionalLightResource_->GetGPUVirtualAddress());

	// ポイントライト用のCBVを設定
	directXCommand_->GetCommandList()->SetGraphicsRootDescriptorTable(8, instancingPointLight_.gpuHandle);
	directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(5, useNumPointLightResource_->GetGPUVirtualAddress());

	// スポットライトのCBVを設定
	directXCommand_->GetCommandList()->SetGraphicsRootDescriptorTable(9, instancingSpotLight_.gpuHandle);
	directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(6, useNumSpotLightResource_->GetGPUVirtualAddress());

	// カメラ用のCBVを設定
	directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(4, sphereResources_[useNumResourceSphere_]->cameraResource_->GetGPUVirtualAddress());

	// テクスチャ
	textureStore_->SelectTexture(directXCommand_->GetCommandList(), textureHandle);

	// 描画する
	directXCommand_->GetCommandList()->DrawIndexedInstanced(segment * ring * 6, 1, 0, 0, 0);


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
	const Camera3D* camera, uint32_t textureHandle, uint32_t subdivisions, float outRadius, float inRadius, Vector4 color, bool isLighting)
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

	// データを書き込む
	uint32_t* indexData = nullptr;
	ringResources_[useNumResourceRing_]->indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData));

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

	// データを書き込む
	VertexData* vertexData = nullptr;
	ringResources_[useNumResourceRing_]->vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

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
	ringResources_[useNumResourceRing_]->materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
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
	ringResources_[useNumResourceRing_]->transformationResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationData));
	transformationData->worldViewProjection = worldTransform->worldMatrix_ * camera->viewMatrix_ * camera->projectionMatrix_;
	transformationData->world = worldTransform->worldMatrix_;
	transformationData->worldInverseTranspose = MakeTransposeMatrix(MakeInverseMatrix(worldTransform->worldMatrix_));


	/*-----------
		カメラ
	-----------*/

	// データを書き込む
	CameraForGPU* cameraData = nullptr;
	ringResources_[useNumResourceRing_]->cameraResource_->Map(0, nullptr, reinterpret_cast<void**>(&cameraData));
	cameraData->worldPosition = camera->translation_;



	/*------------------
		コマンドを積む
	------------------*/

	// ルートシグネチャやPSOの設定
	psoPrimitive_[useObject3dBlendMode_]->CommandListSet(directXCommand_->GetCommandList());

	// IBVを設定する
	directXCommand_->GetCommandList()->IASetIndexBuffer(&ringResources_[useNumResourceRing_]->indexBufferView_);

	// VBVを設定する
	directXCommand_->GetCommandList()->IASetVertexBuffers(0, 1, &ringResources_[useNumResourceRing_]->vertexBufferView_);

	// 形状を設定
	directXCommand_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// マテリアル用のCBVを設定
	directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(0, ringResources_[useNumResourceRing_]->materialResource_->GetGPUVirtualAddress());

	// 座標変換用のCBVを設定
	directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(1, ringResources_[useNumResourceRing_]->transformationResource_->GetGPUVirtualAddress());

	// 平行光源用のインスタンシングの設定
	directXCommand_->GetCommandList()->SetGraphicsRootDescriptorTable(7, instancingDirectionalLight_.gpuHandle);
	directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(3, useNumDirectionalLightResource_->GetGPUVirtualAddress());

	// ポイントライト用のCBVを設定
	directXCommand_->GetCommandList()->SetGraphicsRootDescriptorTable(8, instancingPointLight_.gpuHandle);
	directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(5, useNumPointLightResource_->GetGPUVirtualAddress());

	// スポットライトのCBVを設定
	directXCommand_->GetCommandList()->SetGraphicsRootDescriptorTable(9, instancingSpotLight_.gpuHandle);
	directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(6, useNumSpotLightResource_->GetGPUVirtualAddress());

	// カメラ用のCBVを設定
	directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(4, ringResources_[useNumResourceRing_]->cameraResource_->GetGPUVirtualAddress());

	// テクスチャ
	textureStore_->SelectTexture(directXCommand_->GetCommandList(), textureHandle);

	// 描画する
	directXCommand_->GetCommandList()->DrawIndexedInstanced(subdivisions * 6, 1, 0, 0, 0);


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

	// データを書き込む
	uint32_t* indexData = nullptr;
	cylinderResources_[useNumResourceCylinder_]->indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData));

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

	// データを書き込む
	VertexData* vertexData = nullptr;
	cylinderResources_[useNumResourceCylinder_]->vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

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
	cylinderResources_[useNumResourceCylinder_]->materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
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
	cylinderResources_[useNumResourceCylinder_]->transformationResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationData));
	transformationData->worldViewProjection = worldTransform->worldMatrix_ * camera->viewMatrix_ * camera->projectionMatrix_;
	transformationData->world = worldTransform->worldMatrix_;
	transformationData->worldInverseTranspose = MakeTransposeMatrix(MakeInverseMatrix(worldTransform->worldMatrix_));


	/*-----------
		カメラ
	-----------*/

	// データを書き込む
	CameraForGPU* cameraData = nullptr;
	cylinderResources_[useNumResourceCylinder_]->cameraResource_->Map(0, nullptr, reinterpret_cast<void**>(&cameraData));
	cameraData->worldPosition = camera->translation_;



	/*------------------
		コマンドを積む
	------------------*/

	// ルートシグネチャやPSOの設定
	psoPrimitive_[useObject3dBlendMode_]->CommandListSet(directXCommand_->GetCommandList());

	// IBVを設定する
	directXCommand_->GetCommandList()->IASetIndexBuffer(&cylinderResources_[useNumResourceCylinder_]->indexBufferView_);

	// VBVを設定する
	directXCommand_->GetCommandList()->IASetVertexBuffers(0, 1, &cylinderResources_[useNumResourceCylinder_]->vertexBufferView_);

	// 形状を設定
	directXCommand_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// マテリアル用のCBVを設定
	directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(0, cylinderResources_[useNumResourceCylinder_]->materialResource_->GetGPUVirtualAddress());

	// 座標変換用のCBVを設定
	directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(1, cylinderResources_[useNumResourceCylinder_]->transformationResource_->GetGPUVirtualAddress());

	// 平行光源用のインスタンシングの設定
	directXCommand_->GetCommandList()->SetGraphicsRootDescriptorTable(7, instancingDirectionalLight_.gpuHandle);
	directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(3, useNumDirectionalLightResource_->GetGPUVirtualAddress());

	// ポイントライト用のCBVを設定
	directXCommand_->GetCommandList()->SetGraphicsRootDescriptorTable(8, instancingPointLight_.gpuHandle);
	directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(5, useNumPointLightResource_->GetGPUVirtualAddress());

	// スポットライトのCBVを設定
	directXCommand_->GetCommandList()->SetGraphicsRootDescriptorTable(9, instancingSpotLight_.gpuHandle);
	directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(6, useNumSpotLightResource_->GetGPUVirtualAddress());

	// カメラ用のCBVを設定
	directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(4, cylinderResources_[useNumResourceCylinder_]->cameraResource_->GetGPUVirtualAddress());

	// テクスチャ
	textureStore_->SelectTexture(directXCommand_->GetCommandList(), textureHandle);

	// 描画する
	directXCommand_->GetCommandList()->DrawIndexedInstanced(subdivisions * 6, 1, 0, 0, 0);


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
void DirectXCommon::DrawModel(const WorldTransform* worldTransform, const std::vector<std::unique_ptr<UvTransform>>& uvTransforms,
	const Camera3D* camera, uint32_t modelHandle, Vector4 color, bool isLighting)
{
	// 使用できるリソース数を越えないようにする
	if (useNumResourceModel_ >= kMaxNumResource)
	{
		return;
	}

	// UVトランスフォームのインデックス最大数
	const int32_t kMaxUvTransformIndex = static_cast<uint32_t>(uvTransforms.size() - 1);

	//  UVトランスフォームのインデックス
	int32_t uvTransformIndex = 0;


	// モデルの情報を取得する
	ModelInfo* modelInfo = modelDataStore_->GetModelInfo(modelHandle);


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

	int i = 0;

	for (ModelData& modelData : modelInfo->modelData)
	{
		// データを書き込む
		Material* materialData = nullptr;
		materialResourceModel_[useNumMaterialResourceModel_]->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

		// マテリアルデータを設定する
		materialData->color = color;
		materialData->enableLighting = isLighting;
		materialData->shininess = 18.0f;

		// UVトランスフォームが設定されていなかったら単位行列で行う
		if (kMaxUvTransformIndex < 0)
		{
			materialData->uvTransform = MakeIdenityMatirx();
		} else
		{
			// 設定されているとき
			materialData->uvTransform = MakeScaleMatrix(uvTransforms[uvTransformIndex]->scale_) *
				MakeRotateZMatrix(uvTransforms[uvTransformIndex]->rotation_.z) * MakeTranslateMatrix(uvTransforms[uvTransformIndex]->translation_);
		}


		// ルートシグネチャやPSOの設定
		psoObject3d_[useObject3dBlendMode_]->CommandListSet(directXCommand_->GetCommandList());

		// VBVを設定する
		directXCommand_->GetCommandList()->IASetVertexBuffers(0, 1, &modelInfo->vertexBufferView[i]);

		// 形状を設定
		directXCommand_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// マテリアル用のCBVを設定
		directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResourceModel_[useNumMaterialResourceModel_]->GetGPUVirtualAddress());

		// 座標変換用のCBVを設定
		directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationResourceModel_[useNumResourceModel_]->GetGPUVirtualAddress());

		// 平行光源用のインスタンシングの設定
		directXCommand_->GetCommandList()->SetGraphicsRootDescriptorTable(7, instancingDirectionalLight_.gpuHandle);
		directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(3, useNumDirectionalLightResource_->GetGPUVirtualAddress());

		// ポイントライト用のCBVを設定
		directXCommand_->GetCommandList()->SetGraphicsRootDescriptorTable(8, instancingPointLight_.gpuHandle);
		directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(5, useNumPointLightResource_->GetGPUVirtualAddress());

		// スポットライトのCBVを設定
		directXCommand_->GetCommandList()->SetGraphicsRootDescriptorTable(9, instancingSpotLight_.gpuHandle);
		directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(6, useNumSpotLightResource_->GetGPUVirtualAddress());

		// カメラ用のCBVを設定
		directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(4, cameraResourceModel_[useNumResourceModel_]->GetGPUVirtualAddress());

		// テクスチャ
		textureStore_->SelectTexture(directXCommand_->GetCommandList(), modelInfo->textureHandle[i]);

		// 描画する
		directXCommand_->GetCommandList()->DrawInstanced(UINT(modelInfo->modelData[i].vertices.size()), 1, 0, 0);


		// カウントを進める
		i++;

		// UVのインデックス最大数までカウントする
		uvTransformIndex++;
		uvTransformIndex = std::min(uvTransformIndex, kMaxUvTransformIndex);

		// 使用したマテリアルリソースをカウントする
		useNumMaterialResourceModel_++;
	}


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
/// <param name="isLighting"></param>
void DirectXCommon::DrawModel(const WorldTransform* worldTransform, const UvTransform* uvTransform,
	const Camera3D* camera, uint32_t modelHandle, Vector4 color, bool isLighting)
{
	// 使用できるリソース数を越えないようにする
	if (useNumResourceModel_ >= kMaxNumResource)
	{
		return;
	}


	// モデルの情報を取得する
	ModelInfo* modelInfo = modelDataStore_->GetModelInfo(modelHandle);


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

	int i = 0;

	for (ModelData& modelData : modelInfo->modelData)
	{
		// データを書き込む
		Material* materialData = nullptr;
		materialResourceModel_[useNumMaterialResourceModel_]->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

		// マテリアルデータを設定する
		materialData->color = color;
		materialData->enableLighting = isLighting;
		materialData->shininess = 18.0f;
		materialData->uvTransform = MakeScaleMatrix(uvTransform->scale_) *
			MakeRotateZMatrix(uvTransform->rotation_.z) * MakeTranslateMatrix(uvTransform->translation_);


		// ルートシグネチャやPSOの設定
		psoObject3d_[useObject3dBlendMode_]->CommandListSet(directXCommand_->GetCommandList());

		// VBVを設定する
		directXCommand_->GetCommandList()->IASetVertexBuffers(0, 1, &modelInfo->vertexBufferView[i]);

		// 形状を設定
		directXCommand_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// マテリアル用のCBVを設定
		directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResourceModel_[useNumMaterialResourceModel_]->GetGPUVirtualAddress());

		// 座標変換用のCBVを設定
		directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationResourceModel_[useNumResourceModel_]->GetGPUVirtualAddress());

		// 平行光源用のインスタンシングの設定
		directXCommand_->GetCommandList()->SetGraphicsRootDescriptorTable(7, instancingDirectionalLight_.gpuHandle);
		directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(3, useNumDirectionalLightResource_->GetGPUVirtualAddress());

		// ポイントライト用のCBVを設定
		directXCommand_->GetCommandList()->SetGraphicsRootDescriptorTable(8, instancingPointLight_.gpuHandle);
		directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(5, useNumPointLightResource_->GetGPUVirtualAddress());

		// スポットライトのCBVを設定
		directXCommand_->GetCommandList()->SetGraphicsRootDescriptorTable(9, instancingSpotLight_.gpuHandle);
		directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(6, useNumSpotLightResource_->GetGPUVirtualAddress());

		// カメラ用のCBVを設定
		directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(4, cameraResourceModel_[useNumResourceModel_]->GetGPUVirtualAddress());

		// テクスチャ
		textureStore_->SelectTexture(directXCommand_->GetCommandList(), modelInfo->textureHandle[i]);

		// 描画する
		directXCommand_->GetCommandList()->DrawInstanced(UINT(modelInfo->modelData[i].vertices.size()), 1, 0, 0);


		// カウントを進める
		i++;

		// 使用したマテリアルリソースをカウントする
		useNumMaterialResourceModel_++;
	}


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
/// <param name="isLighting"></param>
void DirectXCommon::DrawModel(const WorldTransform* worldTransform, const Camera3D* camera, uint32_t modelHandle, Vector4 color, bool isLighting)
{
	// 使用できるリソース数を越えないようにする
	if (useNumResourceModel_ >= kMaxNumResource)
	{
		return;
	}


	// モデルの情報を取得する
	ModelInfo* modelInfo = modelDataStore_->GetModelInfo(modelHandle);


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

	int i = 0;

	for (ModelData& modelData : modelInfo->modelData)
	{
		// データを書き込む
		Material* materialData = nullptr;
		materialResourceModel_[useNumMaterialResourceModel_]->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

		// マテリアルデータを設定する
		materialData->color = color;
		materialData->enableLighting = isLighting;
		materialData->shininess = 18.0f;
		materialData->uvTransform = MakeIdenityMatirx();


		// ルートシグネチャやPSOの設定
		psoObject3d_[useObject3dBlendMode_]->CommandListSet(directXCommand_->GetCommandList());

		// VBVを設定する
		directXCommand_->GetCommandList()->IASetVertexBuffers(0, 1, &modelInfo->vertexBufferView[i]);

		// 形状を設定
		directXCommand_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// マテリアル用のCBVを設定
		directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResourceModel_[useNumMaterialResourceModel_]->GetGPUVirtualAddress());

		// 座標変換用のCBVを設定
		directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationResourceModel_[useNumResourceModel_]->GetGPUVirtualAddress());

		// 平行光源用のインスタンシングの設定
		directXCommand_->GetCommandList()->SetGraphicsRootDescriptorTable(7, instancingDirectionalLight_.gpuHandle);
		directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(3, useNumDirectionalLightResource_->GetGPUVirtualAddress());

		// ポイントライト用のCBVを設定
		directXCommand_->GetCommandList()->SetGraphicsRootDescriptorTable(8, instancingPointLight_.gpuHandle);
		directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(5, useNumPointLightResource_->GetGPUVirtualAddress());

		// スポットライトのCBVを設定
		directXCommand_->GetCommandList()->SetGraphicsRootDescriptorTable(9, instancingSpotLight_.gpuHandle);
		directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(6, useNumSpotLightResource_->GetGPUVirtualAddress());

		// カメラ用のCBVを設定
		directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(4, cameraResourceModel_[useNumResourceModel_]->GetGPUVirtualAddress());

		// テクスチャ
		textureStore_->SelectTexture(directXCommand_->GetCommandList(), modelInfo->textureHandle[i]);

		// 描画する
		directXCommand_->GetCommandList()->DrawInstanced(UINT(modelInfo->modelData[i].vertices.size()), 1, 0, 0);


		// カウントを進める
		i++;

		// 使用したマテリアルリソースをカウントする
		useNumMaterialResourceModel_++;
	}


	// カウントする
	useNumResourceModel_++;
}

/// <summary>
/// レンダーテクスチャを貼りつけた平面を描画する
/// </summary>
/// <param name="worldTransform"></param>
/// <param name="uvTransform"></param>
/// <param name="camera"></param>
/// <param name="textureHandle"></param>
/// <param name="color"></param>
/// <param name="isLighting"></param>
void DirectXCommon::DrawRenderTexturePlane(const WorldTransform* worldTransform, const UvTransform* uvTransform,
	const Camera3D* camera, uint32_t screenHandle, Vector4 color, bool isLighting)
{
	// 使用できるリソース数を越えないようにする
	if (useNumResourcePlane_ >= kMaxNumResource)
	{
		return;
	}


	/*-----------------
		インデックス
	-----------------*/

	// データを書き込む
	uint32_t* indexData = nullptr;
	planeResources_[useNumResourcePlane_]->indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData));
	indexData[0] = 0; indexData[1] = 1; indexData[2] = 2;
	indexData[3] = 0; indexData[4] = 3; indexData[5] = 1;



	/*----------
		頂点
	----------*/

	// データを書き込む
	VertexData* vertexData = nullptr;
	planeResources_[useNumResourcePlane_]->vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

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
	planeResources_[useNumResourcePlane_]->materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
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
	planeResources_[useNumResourcePlane_]->transformationResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationData));
	transformationData->worldViewProjection = worldTransform->worldMatrix_ * camera->viewMatrix_ * camera->projectionMatrix_;
	transformationData->world = worldTransform->worldMatrix_;
	transformationData->worldInverseTranspose = MakeTransposeMatrix(MakeInverseMatrix(worldTransform->worldMatrix_));


	/*-----------
		カメラ
	-----------*/

	// データを書き込む
	CameraForGPU* cameraData = nullptr;
	planeResources_[useNumResourcePlane_]->cameraResource_->Map(0, nullptr, reinterpret_cast<void**>(&cameraData));
	cameraData->worldPosition = camera->translation_;



	/*------------------
		コマンドを積む
	------------------*/

	// RenderTarget -> PixelShader
	ChangeResourceState(directXCommand_->GetCommandList(), offscreen_[screenHandle].renderTextureResource, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	// ルートシグネチャやPSOの設定
	psoPrimitive_[useObject3dBlendMode_]->CommandListSet(directXCommand_->GetCommandList());

	// IBVを設定する
	directXCommand_->GetCommandList()->IASetIndexBuffer(&planeResources_[useNumResourcePlane_]->indexBufferView_);

	// VBVを設定する
	directXCommand_->GetCommandList()->IASetVertexBuffers(0, 1, &planeResources_[useNumResourcePlane_]->vertexBufferView_);

	// 形状を設定
	directXCommand_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// マテリアル用のCBVを設定
	directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(0, planeResources_[useNumResourcePlane_]->materialResource_->GetGPUVirtualAddress());

	// 座標変換用のCBVを設定
	directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(1, planeResources_[useNumResourcePlane_]->transformationResource_->GetGPUVirtualAddress());

	// 平行光源用のインスタンシングの設定
	directXCommand_->GetCommandList()->SetGraphicsRootDescriptorTable(7, instancingDirectionalLight_.gpuHandle);
	directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(3, useNumDirectionalLightResource_->GetGPUVirtualAddress());

	// ポイントライト用のCBVを設定
	directXCommand_->GetCommandList()->SetGraphicsRootDescriptorTable(8, instancingPointLight_.gpuHandle);
	directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(5, useNumPointLightResource_->GetGPUVirtualAddress());

	// スポットライトのCBVを設定
	directXCommand_->GetCommandList()->SetGraphicsRootDescriptorTable(9, instancingSpotLight_.gpuHandle);
	directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(6, useNumSpotLightResource_->GetGPUVirtualAddress());

	// カメラ用のCBVを設定
	directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(4, planeResources_[useNumResourcePlane_]->cameraResource_->GetGPUVirtualAddress());

	// RenderTextureのRTVを張り付ける
	directXCommand_->GetCommandList()->SetGraphicsRootDescriptorTable(2, offscreen_[screenHandle].renderTextureSrvGPUHandle);

	// 描画する
	directXCommand_->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);

	// PixelShader -> RenderTarget
	ChangeResourceState(directXCommand_->GetCommandList(),
		offscreen_[screenHandle].renderTextureResource, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);


	// カウントする
	useNumResourcePlane_++;
}

/// <summary>
/// 線を描画する
/// </summary>
/// <param name="start">始点</param>
/// <param name="end">終点</param>
/// <param name="camera">カメラ</param>
/// <param name="color">色</param>
void DirectXCommon::DrawLine(const Vector3& start, const Vector3& end, const Camera3D* camera, Vector4 color)
{
	// 使用できるリソース数を越えないようにする
	if (useNumResourceLine_ >= kMaxNumResource)
	{
		return;
	}

	/*----------
		頂点
	----------*/

	// 頂点データを書き込む
	Vector4* vertexData = nullptr;
	lineResources_[useNumResourceLine_]->vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	vertexData[0] = { start.x,start.y,start.z,1.0f };
	vertexData[1] = { end.x,end.y,end.z,1.0f };


	/*---------------
		マテリアル
	---------------*/

	// データを書き込む
	Vector4* materialData = nullptr;
	lineResources_[useNumResourceLine_]->materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	*materialData = color;


	/*-------------
		座標変換
	-------------*/

	// データを書き込む
	Matrix4x4* transformationData = nullptr;
	lineResources_[useNumResourceLine_]->transformationResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationData));

	// ワールド行列
	Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f , 1.0f , 1.0f }, { 0.0f , 0.0f , 0.0f }, { 0.0f , 0.0f , 0.0f });

	*transformationData = worldMatrix * camera->viewMatrix_ * camera->projectionMatrix_;


	/*------------------
		コマンドを積む
	------------------*/

	psoLine3d_[useLine3dBlendMode_]->CommandListSet(directXCommand_->GetCommandList());

	// VBVを設定する
	directXCommand_->GetCommandList()->IASetVertexBuffers(0, 1, &lineResources_[useNumResourceLine_]->vertexBufferView_);

	// 形状を設定
	directXCommand_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);

	// マテリアル用のCBVを設定
	directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(0, lineResources_[useNumResourceLine_]->materialResource_->GetGPUVirtualAddress());

	// 座標変換用のCBVを設定
	directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(1, lineResources_[useNumResourceLine_]->transformationResource_->GetGPUVirtualAddress());

	// 描画する
	directXCommand_->GetCommandList()->DrawInstanced(2, 1, 0, 0);


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
void DirectXCommon::DrawSprite(const Vector2 v1, const Vector2 v2, const Vector2 v3, const Vector2 v4,
	const UvTransform* uvTransform, const Camera2D* camera, uint32_t textureHandle, Vector4 color)
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
	vertexData[0].position = { v3.x , v3.y , 0.0f , 1.0f };
	vertexData[0].texcoord = { 0.0f , 1.0f };
	vertexData[0].normal = { 0.0f , 0.0f , -1.0f };

	// 左上
	vertexData[1].position = { v1.x , v1.y , 0.0f , 1.0f };
	vertexData[1].texcoord = { 0.0f , 0.0f };
	vertexData[1].normal = { 0.0f , 0.0f , -1.0f };

	// 右下
	vertexData[2].position = { v4.x , v4.y , 0.0f , 1.0f };
	vertexData[2].texcoord = { 1.0f , 1.0f };
	vertexData[2].normal = { 0.0f , 0.0f , -1.0f };

	// 右上
	vertexData[3].position = { v2.x , v2.y , 0.0f , 1.0f };
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

	// ワールド行列
	Matrix4x4 worldMatrix = MakeAffineMatrix(Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f));

	// データを書き込む
	TransformationMatrix* transformationData = nullptr;
	transformationResourceSprite_[useNumResourceSprite_]->Map(0, nullptr, reinterpret_cast<void**>(&transformationData));
	transformationData->worldViewProjection = worldMatrix * camera->viewMatrix_ * camera->projectionMatrix_;
	transformationData->world = worldMatrix;



	/*------------------
		コマンドを積む
	------------------*/

	// ルートシグネチャやPSOの設定
	psoObject3d_[useObject3dBlendMode_]->CommandListSet(directXCommand_->GetCommandList());

	// IBVを設定する
	directXCommand_->GetCommandList()->IASetIndexBuffer(&indexBufferView);

	// VBVを設定する
	directXCommand_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);

	// 形状を設定
	directXCommand_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// マテリアル用のCBVを設定
	directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResourceSprite_[useNumResourceSprite_]->GetGPUVirtualAddress());

	// 座標変換用のCBVを設定
	directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationResourceSprite_[useNumResourceSprite_]->GetGPUVirtualAddress());

	// テクスチャ
	textureStore_->SelectTexture(directXCommand_->GetCommandList(), textureHandle);

	// 描画する
	directXCommand_->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);


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
Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> DirectXCommon::CreateDescritprHeap(ID3D12Device* device,
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
		logging_->Log("Create RTV DescriptorHeap \n");

	else if (heapType == D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		logging_->Log("Create SRV DescriptorHeap \n");

	else if (heapType == D3D12_DESCRIPTOR_HEAP_TYPE_DSV)
		logging_->Log("Create DSV DescriptorHeap \n");


	return descriptorHeap;
}


/// <summary>
/// 指定したディスクリプタヒープに格納するためのポインタを取得する（CPU）
/// </summary>
/// <param name="descriptorHeap">ディスクリプタヒープ</param>
/// <param name="descriptorSize">ディスクリプタのサイズ</param>
/// <param name="index">配列番号</param>
/// <returns></returns>
D3D12_CPU_DESCRIPTOR_HANDLE DirectXCommon::GetRTVCPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap, ID3D12Device* device)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV) * numRtvCPUDescriptors);
	numRtvCPUDescriptors++;
	return handleCPU;
}

/// <summary>
/// 指定したディスクリプタヒープに格納するためのポインタを取得する（CPU）
/// </summary>
/// <param name="descriptorHeap">ディスクリプタヒープ</param>
/// <param name="descriptorSize">ディスクリプタのサイズ</param>
/// <param name="index">配列番号</param>
/// <returns></returns>
D3D12_CPU_DESCRIPTOR_HANDLE DirectXCommon::GetSRVCPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap, ID3D12Device* device)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * numSrvCPUDescriptors);
	numSrvCPUDescriptors++;
	return handleCPU;
}

/// <summary>
/// 指定したディスクリプタヒープに格納するためのポインタを取得する（GPU）
/// </summary>
/// <param name="descriptorHeap">ディスクリプタヒープ</param>
/// <param name="descriptorSize">ディスクリプタのサイズ</param>
/// <param name="index">配列番号</param>
/// <returns></returns>
D3D12_GPU_DESCRIPTOR_HANDLE DirectXCommon::GetSRVGPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap, ID3D12Device* device)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * numSrvGPUDescriptors);
	numSrvGPUDescriptors++;
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
Microsoft::WRL::ComPtr<ID3D12Resource> DirectXCommon::CreateRenderTextureResource(ID3D12Device* device,
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
void DirectXCommon::ChangeResourceState(ID3D12GraphicsCommandList* commandList,
	Microsoft::WRL::ComPtr<ID3D12Resource> resource, D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState)
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


#ifdef _DEBUG

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
/// エラーと警告で停止させる
/// </summary>
void DirectXCommon::StopOnErrorsAndWarnings()
{
	Microsoft::WRL::ComPtr<ID3D12InfoQueue> infoQueue = nullptr;
	if (SUCCEEDED(directXGPU_->GetDevice()->QueryInterface(IID_PPV_ARGS(&infoQueue))))
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

#endif


/// <summary>
/// スワップチェーンを生成する
/// </summary>
void DirectXCommon::GenerateSwapChain()
{
	/*----------------------------
		スワップチェーンを設定する
	----------------------------*/

	// 画面の幅
	swapChainDesc_.Width = winApp_->GetWindowWidth();
	swapChainDesc_.Height = winApp_->GetWindowHeight();

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
	HRESULT hr = directXGPU_->GetDXGIfactory()->CreateSwapChainForHwnd(directXCommand_->GetCommandQueue(), winApp_->GetHwnd(), &swapChainDesc_,
		nullptr, nullptr, reinterpret_cast<IDXGISwapChain1**>(swapChain_.GetAddressOf()));
	assert(SUCCEEDED(hr));

	// スワップチェーンを生成した旨のログ
	logging_->Log("Create SwapChain \n");
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

	rtvHandles_[0] = GetRTVCPUDescriptorHandle(rtvDescriptorHeap_, directXGPU_->GetDevice());
	directXGPU_->GetDevice()->CreateRenderTargetView(swapChainResources_[0].Get(), &rtvDesc_, rtvHandles_[0]);

	rtvHandles_[1] = GetRTVCPUDescriptorHandle(rtvDescriptorHeap_, directXGPU_->GetDevice());
	directXGPU_->GetDevice()->CreateRenderTargetView(swapChainResources_[1].Get(), &rtvDesc_, rtvHandles_[1]);
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
	directXGPU_->GetDevice()->CreateDepthStencilView(depthStencilResource_.Get(), &dsvDesc, dsvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart());
}

/// <summary>
/// フェンスを生成する
/// </summary>
void DirectXCommon::GenerateFence()
{
	HRESULT hr = directXGPU_->GetDevice()->CreateFence(fenceValue_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
	assert(SUCCEEDED(hr));

	logging_->Log("Create Fence \n");
}

/// <summary>
/// イベントを生成する
/// </summary>
void DirectXCommon::GenerateEvent()
{
	fenceEvent_ = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent_ != nullptr);

	logging_->Log("Create Event \n");
}

/// <summary>
/// GPUの実行完了を待つ
/// </summary>
void DirectXCommon::WaitForGPU()
{
	// フェンスの値を更新
	fenceValue_++;

	// GPUがここまでたどり着いたときに、フェンスの値を指定した値に代入するようにシグナルを送る
	directXCommand_->GetCommandQueue()->Signal(fence_.Get(), fenceValue_);

	// フェンスの値が指定したシグナル値にたどり着いているか確認する
	if (fence_->GetCompletedValue() < fenceValue_)
	{
		// 指定したシグナル値にたどり着いていないので、たどり着くまで待つようにイベントを設定する
		fence_->SetEventOnCompletion(fenceValue_, fenceEvent_);

		// イベントを待つ
		WaitForSingleObject(fenceEvent_, INFINITE);
	}
}

/// <summary>
/// 構造化バッファのリソースを作成する
/// </summary>
/// <param name="useNum"></param>
/// <returns></returns>
DirectXCommon::StructuredBufferResource DirectXCommon::CreateStructuredBufferResource(ID3D12Device* device, ID3D12DescriptorHeap* srvDescriptorHeap, uint32_t maxNum, int32_t strideBytes)
{
	// 構造化バッファのリソース
	StructuredBufferResource structuredBufferResource;

	// リソースを作成する
	structuredBufferResource.resource = CreateBufferResource(device, strideBytes * maxNum);

	// パーティクルのビュー
	D3D12_SHADER_RESOURCE_VIEW_DESC SrvDesc{};
	SrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	SrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	SrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	SrvDesc.Buffer.FirstElement = 0;
	SrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	SrvDesc.Buffer.NumElements = maxNum;
	SrvDesc.Buffer.StructureByteStride = strideBytes;

	// ポインタのハンドル（住所）を取得する
	structuredBufferResource.cpuHandle = GetSRVCPUDescriptorHandle(srvDescriptorHeap, device);
	structuredBufferResource.gpuHandle = GetSRVGPUDescriptorHandle(srvDescriptorHeap, device);

	device->CreateShaderResourceView(structuredBufferResource.resource.Get(), &SrvDesc, structuredBufferResource.cpuHandle);

	return structuredBufferResource;
}

/// <summary>
/// Object3Dを生成する
/// </summary>
void DirectXCommon::CreateObject3d()
{
	// Object3Dのシェーダをコンパイルする
	object3dVertexShaderBlob_ = dxc_->CompileShader(L"YokosukaEngine/Shader/Object3d/Object3d.VS.hlsl", L"vs_6_0");
	assert(object3dVertexShaderBlob_ != nullptr);
	object3dPixelShaderBlob_ = dxc_->CompileShader(L"YokosukaEngine/Shader/Object3d/Object3d.PS.hlsl", L"ps_6_0");
	assert(object3dPixelShaderBlob_ != nullptr);

	// Object3D用のPSOの生成と初期化
	psoObject3d_[kBlendModeNone] = new Object3dBlendNone();
	psoObject3d_[kBlendModeNone]->Initialize(logging_, dxc_.get(), directXGPU_->GetDevice(), object3dVertexShaderBlob_.Get(), object3dPixelShaderBlob_.Get());

	psoObject3d_[kBlendModeNormal] = new Object3dBlendNormal();
	psoObject3d_[kBlendModeNormal]->Initialize(logging_, dxc_.get(), directXGPU_->GetDevice(), object3dVertexShaderBlob_.Get(), object3dPixelShaderBlob_.Get());

	psoObject3d_[kBlendModeAdd] = new Object3dBlendAdd();
	psoObject3d_[kBlendModeAdd]->Initialize(logging_, dxc_.get(), directXGPU_->GetDevice(), object3dVertexShaderBlob_.Get(), object3dPixelShaderBlob_.Get());

	psoObject3d_[kBlendModeSubtract] = new Object3dBlendSubtract();
	psoObject3d_[kBlendModeSubtract]->Initialize(logging_, dxc_.get(), directXGPU_->GetDevice(), object3dVertexShaderBlob_.Get(), object3dPixelShaderBlob_.Get());

	psoObject3d_[kBlendModeMultiply] = new Object3dBlendMultiply();
	psoObject3d_[kBlendModeMultiply]->Initialize(logging_, dxc_.get(), directXGPU_->GetDevice(), object3dVertexShaderBlob_.Get(), object3dPixelShaderBlob_.Get());

	psoObject3d_[kBlendModeScreen] = new Object3dBlendScreen();
	psoObject3d_[kBlendModeScreen]->Initialize(logging_, dxc_.get(), directXGPU_->GetDevice(), object3dVertexShaderBlob_.Get(), object3dPixelShaderBlob_.Get());
}

/// <summary>
/// Particleを生成する
/// </summary>
void DirectXCommon::CreateParticle()
{
	// Particleのシェーダをコンパイルする
	particleVertexShaderBlob_ = dxc_->CompileShader(L"YokosukaEngine/Shader/Particle/Particle.VS.hlsl", L"vs_6_0");
	assert(particleVertexShaderBlob_ != nullptr);
	particlePixelShaderBlob_ = dxc_->CompileShader(L"YokosukaEngine/Shader/Particle/Particle.PS.hlsl", L"ps_6_0");
	assert(particlePixelShaderBlob_ != nullptr);

	// Particle用のPSOの生成と初期化
	psoParticle_[kBlendModeNone] = new ParticleBlendNone();
	psoParticle_[kBlendModeNone]->Initialize(logging_, dxc_.get(), directXGPU_->GetDevice(), particleVertexShaderBlob_.Get(), particlePixelShaderBlob_.Get());

	psoParticle_[kBlendModeNormal] = new ParticleBlendNormal();
	psoParticle_[kBlendModeNormal]->Initialize(logging_, dxc_.get(), directXGPU_->GetDevice(), particleVertexShaderBlob_.Get(), particlePixelShaderBlob_.Get());

	psoParticle_[kBlendModeAdd] = new ParticleBlendAdd();
	psoParticle_[kBlendModeAdd]->Initialize(logging_, dxc_.get(), directXGPU_->GetDevice(), particleVertexShaderBlob_.Get(), particlePixelShaderBlob_.Get());

	psoParticle_[kBlendModeSubtract] = new ParticleBlendSubtract();
	psoParticle_[kBlendModeSubtract]->Initialize(logging_, dxc_.get(), directXGPU_->GetDevice(), particleVertexShaderBlob_.Get(), particlePixelShaderBlob_.Get());

	psoParticle_[kBlendModeMultiply] = new ParticleBlendMultiply();
	psoParticle_[kBlendModeMultiply]->Initialize(logging_, dxc_.get(), directXGPU_->GetDevice(), particleVertexShaderBlob_.Get(), particlePixelShaderBlob_.Get());

	psoParticle_[kBlendModeScreen] = new ParticleBlendScreen();
	psoParticle_[kBlendModeScreen]->Initialize(logging_, dxc_.get(), directXGPU_->GetDevice(), particleVertexShaderBlob_.Get(), particlePixelShaderBlob_.Get());
}

/// <summary>
/// Line3Dを生成する
/// </summary>
void DirectXCommon::CreateLine3d()
{
	// Line3dのシェーダをコンパイルする
	line3dVertexShaderBlob_ = dxc_->CompileShader(L"YokosukaEngine/Shader/Line3d/Line3d.VS.hlsl", L"vs_6_0");
	assert(line3dVertexShaderBlob_ != nullptr);
	line3dPixelShaderBlob_ = dxc_->CompileShader(L"YokosukaEngine/Shader/Line3d/Line3d.PS.hlsl", L"ps_6_0");
	assert(line3dPixelShaderBlob_ != nullptr);

	// Line3d用のPSOの生成と初期化
	psoLine3d_[kBlendModeNone] = new Line3dBlendNone();
	psoLine3d_[kBlendModeNone]->Initialize(logging_, dxc_.get(), directXGPU_->GetDevice(), line3dVertexShaderBlob_.Get(), line3dPixelShaderBlob_.Get());

	psoLine3d_[kBlendModeNormal] = new Line3dBlendNormal();
	psoLine3d_[kBlendModeNormal]->Initialize(logging_, dxc_.get(), directXGPU_->GetDevice(), line3dVertexShaderBlob_.Get(), line3dPixelShaderBlob_.Get());

	psoLine3d_[kBlendModeAdd] = new Line3dBlendAdd();
	psoLine3d_[kBlendModeAdd]->Initialize(logging_, dxc_.get(), directXGPU_->GetDevice(), line3dVertexShaderBlob_.Get(), line3dPixelShaderBlob_.Get());

	psoLine3d_[kBlendModeSubtract] = new Line3dBlendSubtract();
	psoLine3d_[kBlendModeSubtract]->Initialize(logging_, dxc_.get(), directXGPU_->GetDevice(), line3dVertexShaderBlob_.Get(), line3dPixelShaderBlob_.Get());

	psoLine3d_[kBlendModeMultiply] = new Line3dBlendMultiply();
	psoLine3d_[kBlendModeMultiply]->Initialize(logging_, dxc_.get(), directXGPU_->GetDevice(), line3dVertexShaderBlob_.Get(), line3dPixelShaderBlob_.Get());

	psoLine3d_[kBlendModeScreen] = new Line3dBlendScreen();
	psoLine3d_[kBlendModeScreen]->Initialize(logging_, dxc_.get(), directXGPU_->GetDevice(), line3dVertexShaderBlob_.Get(), line3dPixelShaderBlob_.Get());
}

/// <summary>
/// Primitiveを生成する
/// </summary>
void DirectXCommon::CreatePrimitive()
{
	// Primitiveのシェーダをコンパイルする
	primitiveVertexShaderBlob_ = dxc_->CompileShader(L"YokosukaEngine/Shader/Primitive/Primitive.VS.hlsl", L"vs_6_0");
	assert(primitiveVertexShaderBlob_ != nullptr);
	primitivePixelShaderBlob_ = dxc_->CompileShader(L"YokosukaEngine/Shader/Primitive/Primitive.PS.hlsl", L"ps_6_0");
	assert(primitivePixelShaderBlob_ != nullptr);

	// Primitive用のPSOの生成と初期化
	psoPrimitive_[kBlendModeNone] = new PrimitiveBlendNone();
	psoPrimitive_[kBlendModeNone]->Initialize(logging_, dxc_.get(), directXGPU_->GetDevice(), primitiveVertexShaderBlob_.Get(), primitivePixelShaderBlob_.Get());

	psoPrimitive_[kBlendModeNormal] = new PrimitiveBlendNormal();
	psoPrimitive_[kBlendModeNormal]->Initialize(logging_, dxc_.get(), directXGPU_->GetDevice(), primitiveVertexShaderBlob_.Get(), primitivePixelShaderBlob_.Get());

	psoPrimitive_[kBlendModeAdd] = new PrimitiveBlendAdd();
	psoPrimitive_[kBlendModeAdd]->Initialize(logging_, dxc_.get(), directXGPU_->GetDevice(), primitiveVertexShaderBlob_.Get(), primitivePixelShaderBlob_.Get());

	psoPrimitive_[kBlendModeSubtract] = new PrimitiveBlendSubtract();
	psoPrimitive_[kBlendModeSubtract]->Initialize(logging_, dxc_.get(), directXGPU_->GetDevice(), primitiveVertexShaderBlob_.Get(), primitivePixelShaderBlob_.Get());

	psoPrimitive_[kBlendModeMultiply] = new PrimitiveBlendMultiply();
	psoPrimitive_[kBlendModeMultiply]->Initialize(logging_, dxc_.get(), directXGPU_->GetDevice(), primitiveVertexShaderBlob_.Get(), primitivePixelShaderBlob_.Get());

	psoPrimitive_[kBlendModeScreen] = new PrimitiveBlendScreen();
	psoPrimitive_[kBlendModeScreen]->Initialize(logging_, dxc_.get(), directXGPU_->GetDevice(), primitiveVertexShaderBlob_.Get(), primitivePixelShaderBlob_.Get());
}

/// <summary>
/// CopyImageを生成する
/// </summary>
void DirectXCommon::CreateCopyImage()
{
	// Primitiveのシェーダをコンパイルする
	fullscreenVertexShaderBlob_ = dxc_->CompileShader(L"YokosukaEngine/Shader/PostEffect/Fullscreen.VS.hlsl", L"vs_6_0");
	assert(fullscreenVertexShaderBlob_ != nullptr);
	copyImagePixelShaderBlob_ = dxc_->CompileShader(L"YokosukaEngine/Shader/PostEffect/CopyImage.PS.hlsl", L"ps_6_0");
	assert(copyImagePixelShaderBlob_ != nullptr);

	// Primitive用のPSOの生成と初期化
	psoCopyImage_[kBlendModeNone] = new CopyImageBlendNone();
	psoCopyImage_[kBlendModeNone]->Initialize(logging_, dxc_.get(), directXGPU_->GetDevice(), fullscreenVertexShaderBlob_.Get(), copyImagePixelShaderBlob_.Get());

	psoCopyImage_[kBlendModeNormal] = new CopyImageBlendNormal();
	psoCopyImage_[kBlendModeNormal]->Initialize(logging_, dxc_.get(), directXGPU_->GetDevice(), fullscreenVertexShaderBlob_.Get(), copyImagePixelShaderBlob_.Get());

	psoCopyImage_[kBlendModeAdd] = new CopyImageBlendAdd();
	psoCopyImage_[kBlendModeAdd]->Initialize(logging_, dxc_.get(), directXGPU_->GetDevice(), fullscreenVertexShaderBlob_.Get(), copyImagePixelShaderBlob_.Get());

	psoCopyImage_[kBlendModeSubtract] = new CopyImageBlendSubtract();
	psoCopyImage_[kBlendModeSubtract]->Initialize(logging_, dxc_.get(), directXGPU_->GetDevice(), fullscreenVertexShaderBlob_.Get(), copyImagePixelShaderBlob_.Get());

	psoCopyImage_[kBlendModeMultiply] = new CopyImageBlendMultiply();
	psoCopyImage_[kBlendModeMultiply]->Initialize(logging_, dxc_.get(), directXGPU_->GetDevice(), fullscreenVertexShaderBlob_.Get(), copyImagePixelShaderBlob_.Get());

	psoCopyImage_[kBlendModeScreen] = new CopyImageBlendScreen();
	psoCopyImage_[kBlendModeScreen]->Initialize(logging_, dxc_.get(), directXGPU_->GetDevice(), fullscreenVertexShaderBlob_.Get(), copyImagePixelShaderBlob_.Get());
}

/// <summary>
/// PostEffectを生成する
/// </summary>
void DirectXCommon::CreatePostEffect()
{
	psoPostEffect_[kCopyImage] = new CopyImagePipeline();
	psoPostEffect_[kCopyImage]->Initialize(logging_, dxc_.get(), directXGPU_->GetDevice(), fullscreenVertexShaderBlob_.Get(), copyImagePixelShaderBlob_.Get());

	// GrayScaleのシェーダをコンパイルする
	grayScalePixelShaderBlob_ = dxc_->CompileShader(L"YokosukaEngine/Shader/PostEffect/GrayScale.PS.hlsl", L"ps_6_0");
	assert(grayScalePixelShaderBlob_ != nullptr);
	psoPostEffect_[kGrayScale] = new GrayScale();
	psoPostEffect_[kGrayScale]->Initialize(logging_, dxc_.get(), directXGPU_->GetDevice(), fullscreenVertexShaderBlob_.Get(), grayScalePixelShaderBlob_.Get());

	// Sepiaのシェーダをコンパイルする
	sepiaPixelShaderBlob_ = dxc_->CompileShader(L"YokosukaEngine/Shader/PostEffect/Sepia.PS.hlsl", L"ps_6_0");
	assert(sepiaPixelShaderBlob_ != nullptr);
	psoPostEffect_[kSepia] = new Sepia();
	psoPostEffect_[kSepia]->Initialize(logging_, dxc_.get(), directXGPU_->GetDevice(), fullscreenVertexShaderBlob_.Get(), sepiaPixelShaderBlob_.Get());

	// Vignetteのシェーダをコンパイルする
	vignettePixelShaderBlob_ = dxc_->CompileShader(L"YokosukaEngine/Shader/PostEffect/Vignette.PS.hlsl", L"ps_6_0");
	assert(vignettePixelShaderBlob_ != nullptr);
	psoPostEffect_[kVignetteing] = new Vignette();
	psoPostEffect_[kVignetteing]->Initialize(logging_, dxc_.get(), directXGPU_->GetDevice(), fullscreenVertexShaderBlob_.Get(), vignettePixelShaderBlob_.Get());

	// Smoothingのシェーダをコンパイルする
	smoothingPixelShaderBlob_ = dxc_->CompileShader(L"YokosukaEngine/Shader/PostEffect/Smoothing.PS.hlsl", L"ps_6_0");
	assert(smoothingPixelShaderBlob_ != nullptr);
	psoPostEffect_[kSmoothing] = new Smoothing();
	psoPostEffect_[kSmoothing]->Initialize(logging_, dxc_.get(), directXGPU_->GetDevice(), fullscreenVertexShaderBlob_.Get(), smoothingPixelShaderBlob_.Get());

	// GaussianFilterのシェーダをコンパイルする
	gaussianFilterPixelShaderBlob_ = dxc_->CompileShader(L"YokosukaEngine/Shader/PostEffect/GaussianFilter.PS.hlsl", L"ps_6_0");
	assert(gaussianFilterPixelShaderBlob_ != nullptr);
	psoPostEffect_[kGaussianFilter] = new GaussianFilter();
	psoPostEffect_[kGaussianFilter]->Initialize(logging_, dxc_.get(), directXGPU_->GetDevice(), fullscreenVertexShaderBlob_.Get(), gaussianFilterPixelShaderBlob_.Get());

	// LuminanceBaseOutlineのシェーダをコンパイルする
	outlinePixelShaderBlob_ = dxc_->CompileShader(L"YokosukaEngine/Shader/PostEffect/LuminaceBaseOutline.PS.hlsl", L"ps_6_0");
	assert(outlinePixelShaderBlob_ != nullptr);
	psoPostEffect_[kLuminanceBaseOutline] = new LuminanceBaseOutline();
	psoPostEffect_[kLuminanceBaseOutline]->Initialize(logging_, dxc_.get(), directXGPU_->GetDevice(), fullscreenVertexShaderBlob_.Get(), outlinePixelShaderBlob_.Get());

	// BrightnessExtractionのシェーダをコンパイルする
	brightnessExtractionPixelShaderBlob_ = dxc_->CompileShader(L"YokosukaEngine/Shader/PostEffect/BrightnessExtraction.PS.hlsl", L"ps_6_0");
	assert(brightnessExtractionPixelShaderBlob_ != nullptr);
	psoPostEffect_[kBrightnessExtraction] = new BrightnessExtraction();
	psoPostEffect_[kBrightnessExtraction]->Initialize(logging_, dxc_.get(), directXGPU_->GetDevice(), fullscreenVertexShaderBlob_.Get(), brightnessExtractionPixelShaderBlob_.Get());
	luminanceResource_ = CreateBufferResource(directXGPU_->GetDevice(), sizeof(GPUforLuminance));

	// Hideのシェーダをコンパイルする
	hidePixelShaderBlob_ = dxc_->CompileShader(L"YokosukaEngine/Shader/PostEffect/hide.PS.hlsl", L"ps_6_0");
	assert(hidePixelShaderBlob_ != nullptr);
	psoPostEffect_[kHide] = new Hide();
	psoPostEffect_[kHide]->Initialize(logging_, dxc_.get(), directXGPU_->GetDevice(), fullscreenVertexShaderBlob_.Get(), hidePixelShaderBlob_.Get());

	// RasterScrollのシェーダをコンパイルする
	rasterScrollPixelShaderBlob_ = dxc_->CompileShader(L"YokosukaEngine/Shader/PostEffect/RasterScroll.PS.hlsl", L"ps_6_0");
	assert(rasterScrollPixelShaderBlob_ != nullptr);
	psoPostEffect_[kRasterScroll] = new RasterScroll();
	psoPostEffect_[kRasterScroll]->Initialize(logging_, dxc_.get(), directXGPU_->GetDevice(), fullscreenVertexShaderBlob_.Get(), rasterScrollPixelShaderBlob_.Get());
	rasterScrollResource_ = CreateBufferResource(directXGPU_->GetDevice(), sizeof(GPUforRasterScroll));
}

/// <summary>
/// RTV通常コピー
/// </summary>
void DirectXCommon::DrawCopyImage()
{
	// 既にオフスクリーンを使用していたら上書きする
	if (useNumOffscreen_ <= 0)
		return;

	// RenderTarget -> PixelShader
	ChangeResourceState(directXCommand_->GetCommandList(),
		offscreen_[useNumOffscreen_ - 1].renderTextureResource, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	// PSOの設定
	psoPostEffect_[kCopyImage]->CommandListSet(directXCommand_->GetCommandList());

	// 形状を設定
	directXCommand_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// RenderTextureのRTVを張り付ける
	directXCommand_->GetCommandList()->SetGraphicsRootDescriptorTable(0, offscreen_[useNumOffscreen_ - 1].renderTextureSrvGPUHandle);

	// 頂点3つ描画
	directXCommand_->GetCommandList()->DrawInstanced(3, 1, 0, 0);

	// PixelShader -> RenderTarget
	ChangeResourceState(directXCommand_->GetCommandList(),
		offscreen_[useNumOffscreen_ - 1].renderTextureResource, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);

}

/// <summary>
/// グレースケール
/// </summary>
void DirectXCommon::DrawGrayScale()
{
	// 既にオフスクリーンを使用していたら上書きする
	if (useNumOffscreen_ <= 0)
		return;

	// RenderTarget -> PixelShader
	ChangeResourceState(directXCommand_->GetCommandList(),
		offscreen_[useNumOffscreen_ - 1].renderTextureResource, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	// PSOの設定
	psoPostEffect_[kGrayScale]->CommandListSet(directXCommand_->GetCommandList());

	// 形状を設定
	directXCommand_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// RenderTextureのRTVを張り付ける
	directXCommand_->GetCommandList()->SetGraphicsRootDescriptorTable(0, offscreen_[useNumOffscreen_ - 1].renderTextureSrvGPUHandle);

	// 頂点3つ描画
	directXCommand_->GetCommandList()->DrawInstanced(3, 1, 0, 0);

	// PixelShader -> RenderTarget
	ChangeResourceState(directXCommand_->GetCommandList(),
		offscreen_[useNumOffscreen_ - 1].renderTextureResource, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
}

/// <summary>
/// セピア
/// </summary>
void DirectXCommon::DrawSepia()
{
	// 既にオフスクリーンを使用していたら上書きする
	if (useNumOffscreen_ <= 0)
		return;

	// RenderTarget -> PixelShader
	ChangeResourceState(directXCommand_->GetCommandList(),
		offscreen_[useNumOffscreen_ - 1].renderTextureResource, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	// PSOの設定
	psoPostEffect_[kSepia]->CommandListSet(directXCommand_->GetCommandList());

	// 形状を設定
	directXCommand_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// RenderTextureのRTVを張り付ける
	directXCommand_->GetCommandList()->SetGraphicsRootDescriptorTable(0, offscreen_[useNumOffscreen_ - 1].renderTextureSrvGPUHandle);

	// 頂点3つ描画
	directXCommand_->GetCommandList()->DrawInstanced(3, 1, 0, 0);

	// PixelShader -> RenderTarget
	ChangeResourceState(directXCommand_->GetCommandList(),
		offscreen_[useNumOffscreen_ - 1].renderTextureResource, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
}

/// <summary>
/// ヴィネッティング
/// </summary>
void DirectXCommon::DrawVignetting()
{
	// 既にオフスクリーンを使用していたら上書きする
	if (useNumOffscreen_ <= 0)
		return;


	// RenderTarget -> PixelShader
	ChangeResourceState(directXCommand_->GetCommandList(),
		offscreen_[useNumOffscreen_ - 1].renderTextureResource, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	// PSOの設定
	psoPostEffect_[kVignetteing]->CommandListSet(directXCommand_->GetCommandList());

	// 形状を設定
	directXCommand_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// RenderTextureのRTVを張り付ける
	directXCommand_->GetCommandList()->SetGraphicsRootDescriptorTable(0, offscreen_[useNumOffscreen_ - 1].renderTextureSrvGPUHandle);

	// 頂点3つ描画
	directXCommand_->GetCommandList()->DrawInstanced(3, 1, 0, 0);

	// PixelShader -> RenderTarget
	ChangeResourceState(directXCommand_->GetCommandList(),
		offscreen_[useNumOffscreen_ - 1].renderTextureResource, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
}

/// <summary>
/// 平滑化
/// </summary>
void DirectXCommon::DrawSmoothing()
{
	// 既にオフスクリーンを使用していたら上書きする
	if (useNumOffscreen_ <= 0)
		return;

	// RenderTarget -> PixelShader
	ChangeResourceState(directXCommand_->GetCommandList(),
		offscreen_[useNumOffscreen_ - 1].renderTextureResource, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	// PSOの設定
	psoPostEffect_[kSmoothing]->CommandListSet(directXCommand_->GetCommandList());

	// 形状を設定
	directXCommand_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// RenderTextureのRTVを張り付ける
	directXCommand_->GetCommandList()->SetGraphicsRootDescriptorTable(0, offscreen_[useNumOffscreen_ - 1].renderTextureSrvGPUHandle);

	// 頂点3つ描画
	directXCommand_->GetCommandList()->DrawInstanced(3, 1, 0, 0);

	// PixelShader -> RenderTarget
	ChangeResourceState(directXCommand_->GetCommandList(),
		offscreen_[useNumOffscreen_ - 1].renderTextureResource, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
}

/// <summary>
/// ガウシアンフィルター
/// </summary>
void DirectXCommon::DrawGaussianFilter()
{
	// 既にオフスクリーンを使用していたら上書きする
	if (useNumOffscreen_ <= 0)
		return;

	// RenderTarget -> PixelShader
	ChangeResourceState(directXCommand_->GetCommandList(),
		offscreen_[useNumOffscreen_ - 1].renderTextureResource, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	// PSOの設定
	psoPostEffect_[kGaussianFilter]->CommandListSet(directXCommand_->GetCommandList());

	// 形状を設定
	directXCommand_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// RenderTextureのRTVを張り付ける
	directXCommand_->GetCommandList()->SetGraphicsRootDescriptorTable(0, offscreen_[useNumOffscreen_ - 1].renderTextureSrvGPUHandle);

	// 頂点3つ描画
	directXCommand_->GetCommandList()->DrawInstanced(3, 1, 0, 0);

	// PixelShader -> RenderTarget
	ChangeResourceState(directXCommand_->GetCommandList(),
		offscreen_[useNumOffscreen_ - 1].renderTextureResource, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
}

/// <summary>
/// アウトライン
/// </summary>
void DirectXCommon::DrawOutline()
{
	// 既にオフスクリーンを使用していたら上書きする
	if (useNumOffscreen_ <= 0)
		return;

	// RenderTarget -> PixelShader
	ChangeResourceState(directXCommand_->GetCommandList(),
		offscreen_[useNumOffscreen_ - 1].renderTextureResource, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	// PSOの設定
	psoPostEffect_[kLuminanceBaseOutline]->CommandListSet(directXCommand_->GetCommandList());

	// 形状を設定
	directXCommand_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// RenderTextureのRTVを張り付ける
	directXCommand_->GetCommandList()->SetGraphicsRootDescriptorTable(0, offscreen_[useNumOffscreen_ - 1].renderTextureSrvGPUHandle);

	// 頂点3つ描画
	directXCommand_->GetCommandList()->DrawInstanced(3, 1, 0, 0);

	// PixelShader -> RenderTarget
	ChangeResourceState(directXCommand_->GetCommandList(),
		offscreen_[useNumOffscreen_ - 1].renderTextureResource, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
}

/// <summary>
/// 高輝度抽出
/// </summary>
void DirectXCommon::DrawBrightnessExtraction()
{
	// 既にオフスクリーンを使用していたら上書きする
	if (useNumOffscreen_ <= 0)
		return;

	// RenderTarget -> PixelShader
	ChangeResourceState(directXCommand_->GetCommandList(),
		offscreen_[useNumOffscreen_ - 1].renderTextureResource, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	// PSOの設定
	psoPostEffect_[kBrightnessExtraction]->CommandListSet(directXCommand_->GetCommandList());

	// 形状を設定
	directXCommand_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Luminanceの設定
	GPUforLuminance* luminanceData = nullptr;
	luminanceResource_->Map(0, nullptr, reinterpret_cast<void**>(&luminanceData));
	luminanceData->threshold = 0.5f;
	directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(1, luminanceResource_->GetGPUVirtualAddress());

	// RenderTextureのRTVを張り付ける
	directXCommand_->GetCommandList()->SetGraphicsRootDescriptorTable(0, offscreen_[useNumOffscreen_ - 1].renderTextureSrvGPUHandle);

	// 頂点3つ描画
	directXCommand_->GetCommandList()->DrawInstanced(3, 1, 0, 0);

	// PixelShader -> RenderTarget
	ChangeResourceState(directXCommand_->GetCommandList(),
		offscreen_[useNumOffscreen_ - 1].renderTextureResource, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
}


/// <summary>
/// 隠す
/// </summary>
void DirectXCommon::DrawHide()
{
	// 既にオフスクリーンを使用していたら上書きする
	if (useNumOffscreen_ <= 0)
		return;

	// RenderTarget -> PixelShader
	ChangeResourceState(directXCommand_->GetCommandList(),
		offscreen_[useNumOffscreen_ - 1].renderTextureResource, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	// PSOの設定
	psoPostEffect_[kHide]->CommandListSet(directXCommand_->GetCommandList());

	// 形状を設定
	directXCommand_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 頂点3つ描画
	directXCommand_->GetCommandList()->DrawInstanced(3, 1, 0, 0);

	// PixelShader -> RenderTarget
	ChangeResourceState(directXCommand_->GetCommandList(),
		offscreen_[useNumOffscreen_ - 1].renderTextureResource, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
}

/// <summary>
/// ラスタースクロール
/// </summary>
void DirectXCommon::DrawRasterScroll()
{
	// 既にオフスクリーンを使用していたら上書きする
	if (useNumOffscreen_ <= 0)
		return;

	// RenderTarget -> PixelShader
	ChangeResourceState(directXCommand_->GetCommandList(),
		offscreen_[useNumOffscreen_ - 1].renderTextureResource, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

	// PSOの設定
	psoPostEffect_[kRasterScroll]->CommandListSet(directXCommand_->GetCommandList());

	// 形状を設定
	directXCommand_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// RasterScrollの設定
	GPUforRasterScroll* rasterScrollData = nullptr;
	rasterScrollResource_->Map(0, nullptr, reinterpret_cast<void**>(&rasterScrollData));
	rasterScrollData->amplitude = 0.2f;
	rasterScrollData->frequency = 12.0f;
	rasterScrollData->phaseOffset = 0.0f;
	rasterScrollData->scrollSpeed = 0.0f;
	rasterScrollData->time = 0.0f;
	directXCommand_->GetCommandList()->SetGraphicsRootConstantBufferView(1, rasterScrollResource_->GetGPUVirtualAddress());

	// RenderTextureのRTVを張り付ける
	directXCommand_->GetCommandList()->SetGraphicsRootDescriptorTable(0, offscreen_[useNumOffscreen_ - 1].renderTextureSrvGPUHandle);

	// 頂点3つ描画
	directXCommand_->GetCommandList()->DrawInstanced(3, 1, 0, 0);

	// PixelShader -> RenderTarget
	ChangeResourceState(directXCommand_->GetCommandList(),
		offscreen_[useNumOffscreen_ - 1].renderTextureResource, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
}
