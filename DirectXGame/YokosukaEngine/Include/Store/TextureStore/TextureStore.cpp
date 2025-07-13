#include "TextureStore.h"
#include "../../Base/DirectXCommon/DirectXCommon.h"

/// <summary>
/// デストラクタ
/// </summary>
TextureStore::~TextureStore()
{
	// テクスチャデータのリスト
	for (TextureData* textureData : textureData_)
	{
		delete textureData;
	}
	textureData_.clear();
}

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
uint32_t TextureStore::GetTextureHandle( const std::string& filePath, ID3D12Device* device,
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList)
{
	// 既に格納されているファイルパスを探す
	for (uint32_t i = 0; i < textureNum_; i++)
	{
		if (strcmp(filePath.c_str(), textureData_[i]->filePath.c_str()) == 0)
		{
			return textureData_[i]->textureHandle;
		}
	}

	// 新規テクスチャデータ
	TextureData* textureData = new TextureData();


	// テクスチャハンドルを設定する
	while (textureData->textureHandle == 0)
	{
		textureData->textureHandle = rand() % 100000 + 1;

		for (uint32_t j = 0; j < textureNum_; j++)
		{
			// 同じ要素番号は比較しない
			if (textureNum_ == j)
				continue;

			// 同じテクスチャハンドルはやり直し
			if (textureData->textureHandle == textureData_[j]->textureHandle)
			{
				textureData->textureHandle = 0;
				break;
			}
		}
	}

	// ファイルパスを所得する
	textureData->filePath = filePath;

	// テクスチャ情報を取得する
	textureData->mipImages = LoadTexture(textureData->filePath);

	const DirectX::TexMetadata& metadata = textureData->mipImages.GetMetadata();
	textureData->textureResource = CreateTextureResource(device, metadata);
	textureData->intermediateResource =
		UploadTextureData(textureData->textureResource, textureData->mipImages, device, commandList);

	// メタデータを元にSRVを設定する
	textureData->srvDesc.Format = metadata.format;
	textureData->srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	textureData->srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	textureData->srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);

	// SRVを作成するディスクリプタヒープの場所を確保する
	textureData->textureSrvHandleCPU = directXCommon_->GetSRVCPUDescriptorHandle(srvDescriptorHeap,device);
	textureData->textureSrvHandleGPU = directXCommon_->GetSRVGPUDescriptorHandle(srvDescriptorHeap,device);

	// SRVを生成する
	device->CreateShaderResourceView(textureData->textureResource.Get(),
		&textureData->srvDesc, textureData->textureSrvHandleCPU);

	// ベクトルに追加する
	textureData_.push_back(textureData);

	textureNum_++;

	return textureData_[textureNum_ - 1]->textureHandle;
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
		if (textureHandle == textureData_[i]->textureHandle)
		{
			commandList->SetGraphicsRootDescriptorTable(2, textureData_[i]->textureSrvHandleGPU);

			break;
		}
	}
}