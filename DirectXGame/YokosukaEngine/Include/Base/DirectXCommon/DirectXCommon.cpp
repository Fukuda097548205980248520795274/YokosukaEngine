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
	delete pso_;

	// DXC
	delete dxc_;

	// イベント
	CloseHandle(fenceEvent_);

	// テクスチャストア
	delete textureStore_;
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="log">ログ</param>
void DirectXCommon::Initialize(OutputLog* log, WinApp* windowApplication)
{
	//nullptrチェック
	assert(log);
	assert(windowApplication);

	// 引数を受け取る
	log_ = log;
	windowApplication_ = windowApplication;

	// テクスチャストアを初期化する
	textureStore_ = new TextureStore();

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

	// PSOの生成と初期化
	pso_ = new PipelineStateObjectNormal();
	pso_->Initialize(log_, dxc_, device_);

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
	float clearColor[] = { 0.1f , 0.25f , 0.5f , 1.0f };
	commandList_->ClearRenderTargetView(rtvHandles_[backBuffeIndex], clearColor, 0, nullptr);

	// 指定した深度で画面全体をクリアする
	commandList_->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	// 描画用のディスクリプタヒープの設定
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeaps[] = { srvDescriptorHeap_ };
	commandList_->SetDescriptorHeaps(1, descriptorHeaps->GetAddressOf());
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

	// 使用したリソースを削除する
	for (int i = 0  ; i < useResourceNum ; i++)
	{
		if (useResources_[i])
		{
			useResources_[i] = nullptr;
		}
	}
	useResourceNum = 0;
}

/// <summary>
/// 三角形を描画する
/// </summary>
void DirectXCommon::DrawTriangle(const WorldTransform* worldTransform , const Camera3D* camera , uint32_t textureHandle)
{
	/*----------
		頂点
	----------*/

	// 頂点リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource = CreateBufferResource(device_, sizeof(Engine::VertexData) * 3);


	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

	// リソースの先頭のアドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();

	// 使用するリソースのサイズ
	vertexBufferView.SizeInBytes = sizeof(Engine::VertexData) * 3;

	// 1頂点当たりのサイズ
	vertexBufferView.StrideInBytes = sizeof(Engine::VertexData);


	// 頂点データを書き込む
	Engine::VertexData* vertexData = nullptr;
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	vertexData[0].position = { 0.0f , 0.5f , 0.0f , 1.0f };
	vertexData[0].texcoord = { 0.5f , 0.0f };
	vertexData[1].position = { 0.5f , -0.5f , 0.0f , 1.0f };
	vertexData[1].texcoord = { 1.0f , 1.0f };
	vertexData[2].position = { -0.5f , -0.5f , 0.0f , 1.0f };
	vertexData[2].texcoord = { 0.0f , 1.0f };


	/*---------------
		マテリアル
	---------------*/

	// マテリアル用のリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource = CreateBufferResource(device_, sizeof(Engine::Material));

	// データを書き込む
	Engine::Material* materialData = nullptr;
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	materialData->color = { 1.0f , 1.0f , 1.0f , 1.0f };
	materialData->enableLighting = false;


	/*------------------
	    座標変換の行列
	------------------*/

	// 座標変換用のリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationResource = CreateBufferResource(device_, sizeof(Engine::TransformationMatrix));

	// データを書き込む
	Engine::TransformationMatrix* transformationData = nullptr;
	transformationResource->Map(0, nullptr, reinterpret_cast<void**>(&transformationData));
	transformationData->worldViewProjection = Multiply(worldTransform->worldMatrix_, Multiply(camera->viewMatrix_, camera->projectionMatrix_));
	transformationData->world = worldTransform->worldMatrix_;



	/*------------------
	    コマンドを積む
	------------------*/

	// ビューポート設定
	commandList_->RSSetViewports(1, &viewport_);
	
	// シザーレクト設定
	commandList_->RSSetScissorRects(1, &scissorRect_);

	// ルートシグネチャやPSOの設定
	pso_->CommandListSet(commandList_);

	// VBVを設定する
	commandList_->IASetVertexBuffers(0, 1, &vertexBufferView);

	// 形状を設定
	commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// マテリアル用のCBVを設定
	commandList_->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());

	// 座標変換用のCBVを設定
	commandList_->SetGraphicsRootConstantBufferView(1, transformationResource->GetGPUVirtualAddress());

	// テクスチャ
	textureStore_->SelectTexture(commandList_, textureHandle);

	// 描画する
	commandList_->DrawInstanced(3, 1, 0, 0);


	/*-----------------------------
	    使用したリソースを記録する
	-----------------------------*/
	
	// 頂点リソース
	for (int i = 0; i < _countof(useResources_); i++)
	{
		if (useResources_[i] == nullptr)
		{
			useResources_[i] = vertexResource;
			vertexResource = nullptr;

			// 記録したリソースをカウントする
			useResourceNum++;

			break;
		}
	}

	// マテリアル用リソース
	for (int i = 0; i < _countof(useResources_); i++)
	{
		if (useResources_[i] == nullptr)
		{
			useResources_[i] = materialResource;
			materialResource = nullptr;

			// 記録したリソースをカウントする
			useResourceNum++;

			break;
		}
	}

	// 座標変換用リソース
	for (int i = 0; i < _countof(useResources_); i++)
	{
		if (useResources_[i] == nullptr)
		{
			useResources_[i] = transformationResource;
			transformationResource = nullptr;

			// 記録したリソースをカウントする
			useResourceNum++;

			break;
		}
	}
}

/// <summary>
/// 球を描画する
/// </summary>
/// <param name="worldTransform"></param>
/// <param name="camera"></param>
/// <param name="textureHandle"></param>
void DirectXCommon::DrawSphere(const WorldTransform* worldTransform, const Camera3D* camera, uint32_t subdivisions, uint32_t textureHandle)
{
	/*----------
		頂点
	----------*/

	// 頂点数
	const uint32_t kVertexNum = subdivisions * subdivisions * 6;

	// 頂点リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource = CreateBufferResource(device_, kVertexNum * sizeof(Engine::VertexData));


	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = sizeof(Engine::VertexData) * kVertexNum;
	vertexBufferView.StrideInBytes = sizeof(Engine::VertexData);


	// 頂点データを書き込む
	Engine::VertexData* vertexData = nullptr;
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	// 経度分割1つ分の角度φ
	const float kLonEvery = float(std::numbers::pi) * 2.0f / float(subdivisions);

	// 緯度分割1つ分の角度Θ
	const float kLatEvery = float(std::numbers::pi) / float(subdivisions);
	
	// 緯度の方向に分割
	for (uint32_t latIndex = 0; latIndex < subdivisions; ++latIndex)
	{
		// 緯度
		float lat = -float(std::numbers::pi) / 2.0f + kLatEvery * latIndex;

		// 経度の方向に分割
		for (uint32_t lonIndex = 0; lonIndex < subdivisions; ++lonIndex)
		{
			// 経度
			float lon = lonIndex * kLonEvery;

			// 要素番号
			uint32_t index = (latIndex * subdivisions + lonIndex) * 6;

			vertexData[index].position.x = std::cos(lat) * std::cos(lon);
			vertexData[index].position.y = std::sin(lat);
			vertexData[index].position.z = std::cos(lat) * std::sin(lon);
			vertexData[index].position.w = 1.0f;
			vertexData[index].texcoord.x = static_cast<float>(lonIndex) / static_cast<float>(subdivisions);
			vertexData[index].texcoord.y = 1.0f - static_cast<float>(latIndex) / static_cast<float>(subdivisions);
			vertexData[index].normal.x = vertexData[index].position.x;
			vertexData[index].normal.y = vertexData[index].position.y;
			vertexData[index].normal.z = vertexData[index].position.z;

			vertexData[index + 1].position.x = std::cos(lat + kLatEvery) * std::cos(lon);
			vertexData[index + 1].position.y = std::sin(lat + kLatEvery);
			vertexData[index + 1].position.z = std::cos(lat + kLatEvery) * std::sin(lon);
			vertexData[index + 1].position.w = 1.0f;
			vertexData[index + 1].texcoord.x = static_cast<float>(lonIndex) / static_cast<float>(subdivisions);
			vertexData[index + 1].texcoord.y = 1.0f - static_cast<float>(latIndex + 1) / static_cast<float>(subdivisions);
			vertexData[index + 1].normal.x = vertexData[index + 1].position.x;
			vertexData[index + 1].normal.y = vertexData[index + 1].position.y;
			vertexData[index + 1].normal.z = vertexData[index + 1].position.z;

			vertexData[index + 2].position.x = std::cos(lat) * std::cos(lon + kLonEvery);
			vertexData[index + 2].position.y = std::sin(lat);
			vertexData[index + 2].position.z = std::cos(lat) * std::sin(lon + kLonEvery);
			vertexData[index + 2].position.w = 1.0f;
			vertexData[index + 2].texcoord.x = static_cast<float>(lonIndex + 1) / static_cast<float>(subdivisions);
			vertexData[index + 2].texcoord.y = 1.0f - static_cast<float>(latIndex) / static_cast<float>(subdivisions);
			vertexData[index + 2].normal.x = vertexData[index + 2].position.x;
			vertexData[index + 2].normal.y = vertexData[index + 2].position.y;
			vertexData[index + 2].normal.z = vertexData[index + 2].position.z;

			vertexData[index + 3].position.x = std::cos(lat) * std::cos(lon + kLonEvery);
			vertexData[index + 3].position.y = std::sin(lat);
			vertexData[index + 3].position.z = std::cos(lat) * std::sin(lon + kLonEvery);
			vertexData[index + 3].position.w = 1.0f;
			vertexData[index + 3].texcoord.x = static_cast<float>(lonIndex + 1) / static_cast<float>(subdivisions);
			vertexData[index + 3].texcoord.y = 1.0f - static_cast<float>(latIndex) / static_cast<float>(subdivisions);
			vertexData[index + 3].normal.x = vertexData[index + 3].position.x;
			vertexData[index + 3].normal.y = vertexData[index + 3].position.y;
			vertexData[index + 3].normal.z = vertexData[index + 3].position.z;

			vertexData[index + 4].position.x = std::cos(lat + kLatEvery) * std::cos(lon);
			vertexData[index + 4].position.y = std::sin(lat + kLatEvery);
			vertexData[index + 4].position.z = std::cos(lat + kLatEvery) * std::sin(lon);
			vertexData[index + 4].position.w = 1.0f;
			vertexData[index + 4].texcoord.x = static_cast<float>(lonIndex) / static_cast<float>(subdivisions);
			vertexData[index + 4].texcoord.y = 1.0f - static_cast<float>(latIndex + 1) / static_cast<float>(subdivisions);
			vertexData[index + 4].normal.x = vertexData[index + 4].position.x;
			vertexData[index + 4].normal.y = vertexData[index + 4].position.y;
			vertexData[index + 4].normal.z = vertexData[index + 4].position.z;

			vertexData[index + 5].position.x = std::cos(lat + kLatEvery) * std::cos(lon + kLonEvery);
			vertexData[index + 5].position.y = std::sin(lat + kLatEvery);
			vertexData[index + 5].position.z = std::cos(lat + kLatEvery) * std::sin(lon + kLonEvery);
			vertexData[index + 5].position.w = 1.0f;
			vertexData[index + 5].texcoord.x = static_cast<float>(lonIndex + 1) / static_cast<float>(subdivisions);
			vertexData[index + 5].texcoord.y = 1.0f - static_cast<float>(latIndex + 1) / static_cast<float>(subdivisions);
			vertexData[index + 5].normal.x = vertexData[index + 5].position.x;
			vertexData[index + 5].normal.y = vertexData[index + 5].position.y;
			vertexData[index + 5].normal.z = vertexData[index + 5].position.z;
		}
	}


	/*---------------
		マテリアル
	---------------*/

	// マテリアル用のリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource = CreateBufferResource(device_, sizeof(Engine::Material));

	// データを書き込む
	Engine::Material* materialData = nullptr;
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	materialData->color = { 1.0f , 1.0f , 1.0f , 1.0f };
	materialData->enableLighting = true;


	/*------------------
		座標変換の行列
	------------------*/

	// 座標変換用のリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationResource = CreateBufferResource(device_, sizeof(Engine::TransformationMatrix));

	// データを書き込む
	Engine::TransformationMatrix* transformationData = nullptr;
	transformationResource->Map(0, nullptr, reinterpret_cast<void**>(&transformationData));
	transformationData->worldViewProjection = Multiply(worldTransform->worldMatrix_, Multiply(camera->viewMatrix_, camera->projectionMatrix_));
	transformationData->world = worldTransform->worldMatrix_;


	/*-------------
	    平行光源
	-------------*/

	// 平行光源用リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResource = CreateBufferResource(device_, sizeof(Engine::DirectionalLight));

	// データを書き込む
	Engine::DirectionalLight* directionalLightData = nullptr;
	directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));
	directionalLightData->color = { 1.0f , 1.0f , 1.0f , 1.0f };
	directionalLightData->direction = Normalize({ 0.0f , -1.0f , 0.0f });
	directionalLightData->intensity = 1.0f;


	/*------------------
		コマンドを積む
	------------------*/

	// ビューポート設定
	commandList_->RSSetViewports(1, &viewport_);

	// シザーレクト設定
	commandList_->RSSetScissorRects(1, &scissorRect_);

	// ルートシグネチャやPSOの設定
	pso_->CommandListSet(commandList_);

	// VBVを設定する
	commandList_->IASetVertexBuffers(0, 1, &vertexBufferView);

	// 形状を設定
	commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// マテリアル用のCBVを設定
	commandList_->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());

	// 座標変換用のCBVを設定
	commandList_->SetGraphicsRootConstantBufferView(1, transformationResource->GetGPUVirtualAddress());

	// 平行光源用のCBVを設定
	commandList_->SetGraphicsRootConstantBufferView(3, directionalLightResource->GetGPUVirtualAddress());

	// テクスチャ
	textureStore_->SelectTexture(commandList_, textureHandle);

	// 描画する
	commandList_->DrawInstanced(kVertexNum, 1, 0, 0);


	/*-----------------------------
		使用したリソースを記録する
	-----------------------------*/

	// 頂点リソース
	for (int i = 0; i < _countof(useResources_); i++)
	{
		if (useResources_[i] == nullptr)
		{
			useResources_[i] = vertexResource;
			vertexResource = nullptr;

			// 記録したリソースをカウントする
			useResourceNum++;

			break;
		}
	}

	// マテリアル用リソース
	for (int i = 0; i < _countof(useResources_); i++)
	{
		if (useResources_[i] == nullptr)
		{
			useResources_[i] = materialResource;
			materialResource = nullptr;

			// 記録したリソースをカウントする
			useResourceNum++;

			break;
		}
	}

	// 座標変換用リソース
	for (int i = 0; i < _countof(useResources_); i++)
	{
		if (useResources_[i] == nullptr)
		{
			useResources_[i] = transformationResource;
			transformationResource = nullptr;

			// 記録したリソースをカウントする
			useResourceNum++;

			break;
		}
	}

	// 平行光源用リソース
	for (int i = 0; i < _countof(useResources_); i++)
	{
		if (useResources_[i] == nullptr)
		{
			useResources_[i] = directionalLightResource;
			directionalLightResource = nullptr;

			// 記録したリソースをカウントする
			useResourceNum++;

			break;
		}
	}
}

/// <summary>
/// スプライトを描画する
/// </summary>
/// <param name="worldTransform"></param>
/// <param name="camera"></param>
/// <param name="textureHandle"></param>
void DirectXCommon::DrawSprite(const WorldTransform* worldTransform, const Camera2D* camera, uint32_t textureHandle)
{
	/*----------
	    頂点
	----------*/

	// 頂点リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource = CreateBufferResource(device_, sizeof(Engine::VertexData) * 6);

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = sizeof(Engine::VertexData) * 6;
	vertexBufferView.StrideInBytes = sizeof(Engine::VertexData);

	// 頂点データを書き込む
	Engine::VertexData* vertexData = nullptr;
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	vertexData[0].position = { 0.0f , 360.0f , 0.0f , 1.0f };
	vertexData[0].texcoord = { 0.0f , 1.0f };
	vertexData[0].normal = { 0.0f , 0.0f , -1.0f };
	vertexData[1].position = { 0.0f , 0.0f , 0.0f , 1.0f };
	vertexData[1].texcoord = { 0.0f , 0.0f };
	vertexData[1].normal = { 0.0f , 0.0f , -1.0f };
	vertexData[2].position = { 640.0f , 360.0f , 0.0f , 1.0f };
	vertexData[2].texcoord = { 1.0f , 1.0f };
	vertexData[2].normal = { 0.0f , 0.0f , -1.0f };

	vertexData[3].position = { 0.0f , 0.0f , 0.0f , 1.0f };
	vertexData[3].texcoord = { 0.0f , 0.0f };
	vertexData[3].normal = { 0.0f , 0.0f , -1.0f };
	vertexData[4].position = { 640.0f , 0.0f , 0.0f , 1.0f };
	vertexData[4].texcoord = { 1.0f , 0.0f };
	vertexData[4].normal = { 0.0f , 0.0f , -1.0f };
	vertexData[5].position = { 640.0f , 360.0f , 0.0f , 1.0f };
	vertexData[5].texcoord = { 1.0f , 1.0f };
	vertexData[5].normal = { 0.0f , 0.0f , -1.0f };


	/*---------------
	    マテリアル
	---------------*/

	// マテリアルリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource = CreateBufferResource(device_, sizeof(Engine::Material));

	// マテリアルデータを書き込む
	Engine::Material* materialData = nullptr;
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	materialData->color = { 1.0f , 1.0f , 1.0f , 1.0f };
	materialData->enableLighting = false;


	/*--------------
	    座標変換
	--------------*/

	// 座標変換リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationResource = CreateBufferResource(device_, sizeof(Engine::TransformationMatrix));

	// 座標変換用の行列に書き込む
	Engine::TransformationMatrix* transformationData = nullptr;
	transformationResource->Map(0, nullptr, reinterpret_cast<void**>(&transformationData));
	transformationData->worldViewProjection = Multiply(worldTransform->worldMatrix_, Multiply(camera->viewMatrix_, camera->projectionMatrix_));
	transformationData->world = worldTransform->worldMatrix_;


	/*------------------
	    コマンドを積む
	------------------*/

	// ビューポート設定
	commandList_->RSSetViewports(1, &viewport_);

	// シザーレクト設定
	commandList_->RSSetScissorRects(1, &scissorRect_);

	// ルートシグネチャやPSOの設定
	pso_->CommandListSet(commandList_);

	// VBVを設定する
	commandList_->IASetVertexBuffers(0, 1, &vertexBufferView);

	// 形状を設定
	commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// マテリアル用のCBVを設定
	commandList_->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());

	// 座標変換用のCBVを設定
	commandList_->SetGraphicsRootConstantBufferView(1, transformationResource->GetGPUVirtualAddress());

	// テクスチャ
	textureStore_->SelectTexture(commandList_, textureHandle);

	// 描画する
	commandList_->DrawInstanced(6, 1, 0, 0);


	/*-----------------------------
		使用したリソースを記録する
	-----------------------------*/

	// 頂点リソース
	for (int i = 0; i < _countof(useResources_); i++)
	{
		if (useResources_[i] == nullptr)
		{
			useResources_[i] = vertexResource;
			vertexResource = nullptr;

			// 記録したリソースをカウントする
			useResourceNum++;

			break;
		}
	}

	// マテリアル用リソース
	for (int i = 0; i < _countof(useResources_); i++)
	{
		if (useResources_[i] == nullptr)
		{
			useResources_[i] = materialResource;
			materialResource = nullptr;

			// 記録したリソースをカウントする
			useResourceNum++;

			break;
		}
	}

	// 座標変換用リソース
	for (int i = 0; i < _countof(useResources_); i++)
	{
		if (useResources_[i] == nullptr)
		{
			useResources_[i] = transformationResource;
			transformationResource = nullptr;

			// 記録したリソースをカウントする
			useResourceNum++;

			break;
		}
	}
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