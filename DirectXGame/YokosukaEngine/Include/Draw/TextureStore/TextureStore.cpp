#include "TextureStore.h"

/// <summary>
/// テクスチャを読み込みテクスチャハンドルを取得する
/// </summary>
/// <param name="filePath">ファイルパス</param>
/// <returns></returns>
uint32_t TextureStore::GetTextureHandle(const std::string& filePath, Microsoft::WRL::ComPtr<ID3D12Device> device,
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList)
{
	// 既に格納されているファイルパスを探す
	for (uint32_t i = 0; i < textureNum_; i++)
	{
		if (strcmp(filePath.c_str(), filePath_[i].c_str()) == 0)
		{
			return textureHandle_[i];
		}
	}

	// テクスチャハンドルを設定する
	while (textureHandle_[textureNum_] == 0)
	{
		textureHandle_[textureNum_] = rand() % 100000 + 1;

		for (uint32_t j = 0; j < textureNum_; j++)
		{
			// 同じ要素番号は比較しない
			if (textureNum_ == j)
				continue;

			// 同じテクスチャハンドルはやり直し
			if (textureHandle_[textureNum_] == textureHandle_[j])
			{
				textureHandle_[textureNum_] = 0;
				break;
			}
		}
	}

	// ファイルパスを所得する
	filePath_[textureNum_] = filePath;

	// テクスチャ情報を取得する
	mipImages_[textureNum_] = LoadTexture(filePath_[textureNum_]);

	const DirectX::TexMetadata& metadata = mipImages_[textureNum_].GetMetadata();
	textureResource_[textureNum_] = CreateTextureResource(device, metadata);
	intermediateResource_[textureNum_] = UploadTextureData(textureResource_[textureNum_], mipImages_[textureNum_], device, commandList);

	// メタデータを元にSRVを設定する
	srvDesc_[textureNum_].Format = metadata.format;
	srvDesc_[textureNum_].Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	// 2Dテクスチャ
	srvDesc_[textureNum_].ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc_[textureNum_].Texture2D.MipLevels = UINT(metadata.mipLevels);

	// SRVを作成するディスクリプタヒープの場所を確保する
	textureSrvHandleCPU_[textureNum_] = GetCPUDescriptorHandle(srvDescriptorHeap,
		device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV), 4 + textureNum_);

	textureSrvHandleGPU_[textureNum_] = GetGPUDescriptorHandle(srvDescriptorHeap,
		device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV), 4 + textureNum_);

	// SRVを生成する
	device->CreateShaderResourceView(textureResource_[textureNum_].Get(), &srvDesc_[textureNum_], textureSrvHandleCPU_[textureNum_]);

	// 格納した数をカウントする
	textureNum_++;

	return textureHandle_[textureNum_ - 1];
}

/// <summary>
/// 使用するテクスチャを選択する
/// </summary>
/// <param name="commandList">コマンドリスト</param>
/// <param name="textureHandle">テクスチャハンドル</param>
void TextureStore::SelectTexture(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList, uint32_t textureHandle)
{
	for (int i = 0; i < 512; i++)
	{
		if (textureHandle == textureHandle_[i])
		{
			commandList->SetGraphicsRootDescriptorTable(2, textureSrvHandleGPU_[i]);

			break;
		}
	}
}

/// <summary>
/// 指定したディスクリプタヒープに格納するためのポインタを取得する（CPU）
/// </summary>
/// <param name="descriptorHeap">ディスクリプタヒープ</param>
/// <param name="descriptorSize">ディスクリプタのサイズ</param>
/// <param name="index">配列番号</param>
/// <returns></returns>
D3D12_CPU_DESCRIPTOR_HANDLE TextureStore::GetCPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap,
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
D3D12_GPU_DESCRIPTOR_HANDLE TextureStore::GetGPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap,
	uint32_t descriptorSize, uint32_t index)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize * index);
	return handleGPU;
}