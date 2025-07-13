#include "LoadTexture.h"

#pragma comment(lib , "d3d12.lib")
#pragma comment(lib , "dxgi.lib")
#pragma comment(lib , "dxcompiler.lib")

/// <summary>
/// テクスチャデータを読み込む
/// </summary>
/// <param name="filePath">ファイルパス</param>
/// <returns></returns>
DirectX::ScratchImage LoadTexture(const std::string& filePath)
{
	// テクスチャファイルを読んで、プログラムで扱えるようにする
	DirectX::ScratchImage image{};
	std::wstring filePathW = ConvertString(filePath);
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));

	// ミップマップの作成
	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
	assert(SUCCEEDED(hr));

	// ミップマップ付きのデータを返す
	return mipImages;
}

/// <summary>
/// テクスチャリソースを作成する
/// </summary>
/// <param name="device">デバイス</param>
/// <param name="metadata">テクスチャのデータ</param>
/// <returns></returns>
Microsoft::WRL::ComPtr<ID3D12Resource> CreateTextureResource(Microsoft::WRL::ComPtr<ID3D12Device> device, const DirectX::TexMetadata& metadata)
{
	/*------------------------------------
	    メタデータを基にリソースを作成する
	------------------------------------*/

	D3D12_RESOURCE_DESC resourceDesc{};

	// テクスチャのサイズ
	resourceDesc.Width = UINT(metadata.width);
	resourceDesc.Height = UINT(metadata.height);

	// ミップマップの数
	resourceDesc.MipLevels = UINT16(metadata.mipLevels);

	// 奥行き or 配列テクスチャの配列数
	resourceDesc.DepthOrArraySize = UINT16(metadata.arraySize);

	// テクスチャのフォーマット
	resourceDesc.Format = metadata.format;

	// サンプリングカウント　1固定
	resourceDesc.SampleDesc.Count = 1;

	// テクスチャの次元数
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension);


	/*------------------------
	    利用するヒープの設定
	------------------------*/

	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;


	/*----------------------------
	    リソースを生成して返却する
	----------------------------*/

	Microsoft::WRL::ComPtr<ID3D12Resource> resource = nullptr;
	HRESULT hr = device->CreateCommittedResource(
		// ヒープの設定
		&heapProperties,

		// ヒープの特殊な設定
		D3D12_HEAP_FLAG_NONE,

		// リソースの設定
		&resourceDesc,

		// データ転送される設定
		D3D12_RESOURCE_STATE_COPY_DEST,

		// Clear最適地
		nullptr,

		// 作成するリソースポインタへのポインタ
		IID_PPV_ARGS(&resource)
	);
	assert(SUCCEEDED(hr));

	return resource;
}

/// <summary>
/// 
/// </summary>
/// <param name="texture"></param>
/// <param name="mipImages"></param>
/// <param name="device"></param>
/// <param name="commandList"></param>
/// <returns></returns>
[[nodiscard]]
Microsoft::WRL::ComPtr<ID3D12Resource> UploadTextureData(Microsoft::WRL::ComPtr<ID3D12Resource> texture, const DirectX::ScratchImage& mipImages,
	ID3D12Device* device, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList)
{
	std::vector<D3D12_SUBRESOURCE_DATA> subresources;
	DirectX::PrepareUpload(device, mipImages.GetImages(), mipImages.GetImageCount(), mipImages.GetMetadata(), subresources);
	uint64_t intermediateSize = GetRequiredIntermediateSize(texture.Get(), 0, UINT(subresources.size()));
	Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource = CreateBufferResource(device, intermediateSize);
	UpdateSubresources(commandList.Get(), texture.Get(), intermediateResource.Get(), 0, 0, UINT(subresources.size()), subresources.data());

	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = texture.Get();
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;

	commandList->ResourceBarrier(1, &barrier);
	return intermediateResource;
}

/// <summary>
/// バッファリソースを作成する
/// </summary>
/// <param name="device"></param>
/// <param name="sizeInBytes"></param>
/// <returns></returns>
Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(ID3D12Device* device, size_t sizeInBytes)
{
	// リソースのヒープ設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;

	// リソースの設定
	D3D12_RESOURCE_DESC bufferResourceDesc{};

	// バッファリソース
	bufferResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;

	// リソースのサイズ
	bufferResourceDesc.Width = sizeInBytes;

	// バッファの場合はこれらを0にする
	bufferResourceDesc.Height = 1;
	bufferResourceDesc.DepthOrArraySize = 1;
	bufferResourceDesc.MipLevels = 1;
	bufferResourceDesc.SampleDesc.Count = 1;

	// バッファの場合はこれにする
	bufferResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// バッファリソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> bufferResource = nullptr;
	HRESULT hr = device->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &bufferResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&bufferResource));
	assert(SUCCEEDED(hr));

	return bufferResource;
}

/// <summary>
/// デプスステンシルテクスチャのリソースを作成する
/// </summary>
/// <param name="device"></param>
/// <param name="width"></param>
/// <param name="height"></param>
/// <returns></returns>
Microsoft::WRL::ComPtr<ID3D12Resource> CreateDepthStencilTextureResource(ID3D12Device* device, int32_t width, int32_t height)
{
	/*--------------------
	    リソースの設定
	--------------------*/

	D3D12_RESOURCE_DESC resourceDesc{};

	// テクスチャのサイズ
	resourceDesc.Width = width;
	resourceDesc.Height = height;

	// ミップマップの数
	resourceDesc.MipLevels = 1;

	// 奥行き or 配列Textureの配列数
	resourceDesc.DepthOrArraySize = 1;

	// デプスステンシルとして利用可能なフォーマット
	resourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// サンプリングカウント
	resourceDesc.SampleDesc.Count = 1;

	// 2次元
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

	// デプスステンシルとして使う通知
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;


	/*----------------
	    ヒープの設定
	----------------*/

	D3D12_HEAP_PROPERTIES heapProperties{};

	// VRAM上に作る
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;


	/*---------------------------
	    深度値のクリア最適化設定
	---------------------------*/

	D3D12_CLEAR_VALUE depthClearValue{};

	// 1.0f（最大値）でクリア
	depthClearValue.DepthStencil.Depth = 1.0f;

	// フォーマット
	depthClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;


	/*-----------------------
	    リソースを生成する
	-----------------------*/

	Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilResource = nullptr;
	HRESULT hr = device->CreateCommittedResource(
		// ヒープの設定
		&heapProperties,

		// ヒープの特殊な設定
		D3D12_HEAP_FLAG_NONE,

		// リソースの設定
		&resourceDesc,

		// 深度値を書き込む状態にしておく
		D3D12_RESOURCE_STATE_DEPTH_WRITE,

		// クリア最適地
		&depthClearValue,

		// 作成するリソースポインタへのポインタ
		IID_PPV_ARGS(&depthStencilResource)
	);
	assert(SUCCEEDED(hr));

	return depthStencilResource;
}