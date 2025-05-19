#include "ModelDataStore.h"

/// <summary>
/// 初期化
/// </summary>
void ModelDataStore::Initialize(TextureStore* textureStore)
{
	// nullptrチェック
	assert(textureHandle_);

	// 引数を受け取る
	textureStore_ = textureStore;
}

/// <summary>
/// モデルハンドルを取得する
/// </summary>
/// <param name="directoryPath"></param>
/// <param name="filePath"></param>
uint32_t ModelDataStore::GetModelHandle(const std::string& directoryPath, const std::string& fileName,
	Microsoft::WRL::ComPtr<ID3D12Device> device, Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap ,
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList)
{
	// 同じモデルデータを使われているか
	for (uint32_t i = 0; i < useModelDataNum_; i++)
	{
		if (strcmp(directoryPath.c_str(), directoryPath_[i].c_str())) { continue; }
		if (strcmp(fileName.c_str(), fileName_[i].c_str())) { continue; }

		return modelHandle_[i];
	}

	// モデルハンドルを決める
	while (modelHandle_[useModelDataNum_] == 0)
	{
		modelHandle_[useModelDataNum_] = rand() % 100000 + 1;

		// モデルハンドルが被らないようにする
		for (uint32_t j = 0; j < useModelDataNum_; j++)
		{
			if (useModelDataNum_ == j)
				continue;

			if (modelHandle_[useModelDataNum_] == modelHandle_[j])
			{
				modelHandle_[useModelDataNum_] = 0;
				break;
			}
		}
	}

	// パスを受け取る
	directoryPath_[useModelDataNum_] = directoryPath;
	fileName_[useModelDataNum_] = fileName;

	// モデルを読み込む
	modelData_[useModelDataNum_] = LoadObjFile(directoryPath, fileName);

	// 頂点リソースを作る
	vertexResource_[useModelDataNum_] = CreateBufferResource(device, sizeof(VertexData) * modelData_[useModelDataNum_].vertices.size());

	// テクスチャハンドルを取得する
	textureHandle_[useModelDataNum_] = textureStore_->GetTextureHandle(modelData_[useModelDataNum_].material.textureFilePath,
		device, srvDescriptorHeap, commandList);


	// カウントする
	useModelDataNum_++;

	return modelHandle_[useModelDataNum_ - 1];
}

/// <summary>
/// モデルハンドルのGetter
/// </summary>
/// <param name="modelHandle"></param>
/// <returns></returns>
ModelData ModelDataStore::GetModelData(uint32_t modelHandle)
{
	for (uint32_t i = 0; i < useModelDataNum_; i++)
	{
		if (modelHandle == modelHandle_[i])
		{
			return modelData_[i];
		}
	}

	assert(false);

	ModelData missData;
	return missData;
}

/// <summary>
/// 頂点リソースのGetter
/// </summary>
/// <param name="modelHandle"></param>
/// <returns></returns>
Microsoft::WRL::ComPtr<ID3D12Resource> ModelDataStore::GetVertexResource(uint32_t modelHandle)
{
	for (uint32_t i = 0; i < useModelDataNum_; i++)
	{
		if (modelHandle == modelHandle_[i])
		{
			return vertexResource_[i];
		}
	}

	assert(false);
	return nullptr;
}

/// <summary>
/// テクスチャハンドルを取得する
/// </summary>
/// <param name="modelHandle"></param>
/// <returns></returns>
uint32_t ModelDataStore::GetTextureHandle(uint32_t modelHandle)
{
	for (uint32_t i = 0; i < useModelDataNum_; i++)
	{
		if (modelHandle == modelHandle_[i])
		{
			return textureHandle_[i];
		}
	}

	assert(false);
	return 0;
}