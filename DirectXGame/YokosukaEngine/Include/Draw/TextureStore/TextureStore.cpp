#include "TextureStore.h"

/// <summary>
/// テクスチャを読み込みテクスチャハンドルを取得する
/// </summary>
/// <param name="filePath">ファイルパス</param>
/// <returns></returns>
uint32_t TextureStore::GetTextureHandle(const std::string& filePath ,Microsoft::WRL::ComPtr<ID3D12Device> device ,
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList)
{
	// 既に格納されているファイルパスを探す
	for (uint32_t i = 0; i < textureNum_; i++)
	{
		if (strcmp(filePath.c_str(),filePath_[i].c_str()))
		{
			return textureHandle_[i];
		}
	}

	// 格納されていない場所から格納する
	for (uint32_t i = 0; i < 256; i++)
	{
		if (textureResource_[i].Get())
			continue;

		// テクスチャハンドルを設定する
		while (textureHandle_[i] == 0)
		{
			textureHandle_[i] = rand() % 100000 + 1;

			for (uint32_t j = 0; j < textureNum_; j++)
			{
				// 同じ要素番号は比較しない
				if (i == j)
					continue;

				// 同じテクスチャハンドルはやり直し
				if (textureHandle_[i] == textureHandle_[j])
				{
					textureHandle_[i] = 0;
					break;
				}
			}
		}

		// テクスチャ情報を取得する
		mipImages_[i] = LoadTexture(filePath);

		const DirectX::TexMetadata& metadata = mipImages_[i].GetMetadata();
		textureResource_[i] = CreateTextureResource(device, metadata);
		intermediateResource_[i] = UploadTextureData(textureResource_[i], mipImages_[i], device, commandList);

		// メタデータを元にSRVを設定する
		srvDesc_[i].Format = metadata.format;
		srvDesc_[i].Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

		// 2Dテクスチャ
		srvDesc_[i].ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc_[i].Texture2D.MipLevels = UINT(metadata.mipLevels);

		// SRVを作成するディスクリプタヒープの場所を確保する
		textureSrvHandleCPU_[i] = GetCPUDescriptorHandle(srvDescriptorHeap, 
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV),1 + textureNum_);

		textureSrvHandleGPU_[i] = GetGPUDescriptorHandle(srvDescriptorHeap,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV), 1 + textureNum_);

		// SRVを生成する
		device->CreateShaderResourceView(textureResource_[i].Get(), &srvDesc_[i], textureSrvHandleCPU_[i]);

		// 格納した数をカウントする
		textureNum_++;

		return textureHandle_[i];
	}

	// 失敗
	assert(false);
	return 0;
}

/// <summary>
/// 使用するテクスチャを選択する
/// </summary>
/// <param name="commandList">コマンドリスト</param>
/// <param name="textureHandle">テクスチャハンドル</param>
void TextureStore::SelectTexture(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList, uint32_t textureHandle)
{
	for (int i = 0; i < 256; i++)
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