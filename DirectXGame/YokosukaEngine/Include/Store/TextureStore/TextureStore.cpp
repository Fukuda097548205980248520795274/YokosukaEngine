#include "TextureStore.h"
#include "../../Base/DirectXCommon/DirectXCommon.h"

/// <summary>
/// 初期化
/// </summary>
void TextureStore::Initialize(DirectXCommon* directXCommon)
{
	// nullptrチェック
	assert(directXCommon);

	// 引数を受け取る
	directXCommon_ = directXCommon;
}

/// <summary>
/// テクスチャを読み込みテクスチャハンドルを取得する
/// </summary>
/// <param name="filePath">ファイルパス</param>
/// <returns></returns>
uint32_t TextureStore::GetTextureHandle( const std::string& filePath, Microsoft::WRL::ComPtr<ID3D12Device> device,
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList)
{
	// 既に格納されているファイルパスを探す
	for (uint32_t i = 0; i < textureNum_; i++)
	{
		if (strcmp(filePath.c_str(), textureData_[i].filePath.c_str()) == 0)
		{
			return textureData_[i].textureHandle;
		}
	}

	// テクスチャハンドルを設定する
	while (textureData_[textureNum_].textureHandle == 0)
	{
		textureData_[textureNum_].textureHandle = rand() % 100000 + 1;

		for (uint32_t j = 0; j < textureNum_; j++)
		{
			// 同じ要素番号は比較しない
			if (textureNum_ == j)
				continue;

			// 同じテクスチャハンドルはやり直し
			if (textureData_[textureNum_].textureHandle == textureData_[j].textureHandle)
			{
				textureData_[textureNum_].textureHandle = 0;
				break;
			}
		}
	}

	// ファイルパスを所得する
	textureData_[textureNum_].filePath = filePath;

	// テクスチャ情報を取得する
	textureData_[textureNum_].mipImages = LoadTexture(textureData_[textureNum_].filePath);

	const DirectX::TexMetadata& metadata = textureData_[textureNum_].mipImages.GetMetadata();
	textureData_[textureNum_].textureResource = CreateTextureResource(device, metadata);
	textureData_[textureNum_].intermediateResource = 
		UploadTextureData(textureData_[textureNum_].textureResource, textureData_[textureNum_].mipImages, device, commandList);

	// メタデータを元にSRVを設定する
	textureData_[textureNum_].srvDesc.Format = metadata.format;
	textureData_[textureNum_].srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	textureData_[textureNum_].srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	textureData_[textureNum_].srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);

	// SRVを作成するディスクリプタヒープの場所を確保する
	textureData_[textureNum_].textureSrvHandleCPU = directXCommon_->GetCPUDescriptorHandle(srvDescriptorHeap,
		device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV), directXCommon_->GetNumSrvDescriptors());

	textureData_[textureNum_].textureSrvHandleGPU = directXCommon_->GetGPUDescriptorHandle(srvDescriptorHeap,
		device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV), directXCommon_->GetNumSrvDescriptors());

	// SRVを生成する
	device->CreateShaderResourceView(textureData_[textureNum_].textureResource.Get(),
		&textureData_[textureNum_].srvDesc, textureData_[textureNum_].textureSrvHandleCPU);

	// 格納したテクスチャとSRVの番号をカウントする
	directXCommon_->CountNumSrvDescriptors();
	textureNum_++;

	return textureData_[textureNum_ - 1].textureHandle;
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
		if (textureHandle == textureData_[i].textureHandle)
		{
			commandList->SetGraphicsRootDescriptorTable(2, textureData_[i].textureSrvHandleGPU);

			break;
		}
	}
}