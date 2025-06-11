#include "ModelDataStore.h"

/// <summary>
/// 初期化
/// </summary>
void ModelDataStore::Initialize(TextureStore* textureStore)
{
	// nullptrチェック
	assert(textureStore);

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
		if (strcmp(directoryPath.c_str(), modelInfoStructure_[i].directoryPath.c_str())) { continue; }
		if (strcmp(fileName.c_str(), modelInfoStructure_[i].fileName.c_str())) { continue; }

		return modelInfoStructure_[i].modelHandle;
	}

	// モデルハンドルを決める
	while (modelInfoStructure_[useModelDataNum_].modelHandle == 0)
	{
		modelInfoStructure_[useModelDataNum_].modelHandle = rand() % 100000 + 1;

		// モデルハンドルが被らないようにする
		for (uint32_t j = 0; j < useModelDataNum_; j++)
		{
			if (useModelDataNum_ == j)
				continue;

			if (modelInfoStructure_[useModelDataNum_].modelHandle == modelInfoStructure_[j].modelHandle)
			{
				modelInfoStructure_[useModelDataNum_].modelHandle = 0;
				break;
			}
		}
	}

	// パスを受け取る
	modelInfoStructure_[useModelDataNum_].directoryPath = directoryPath;
	modelInfoStructure_[useModelDataNum_].fileName = fileName;

	// モデルを読み込む
	modelInfoStructure_[useModelDataNum_].modelData = LoadObjFile(directoryPath, fileName);

	// 頂点リソースを作る
	modelInfoStructure_[useModelDataNum_].vertexResource = 
		CreateBufferResource(device, sizeof(VertexData) * modelInfoStructure_[useModelDataNum_].modelData.vertices.size());

	// テクスチャハンドルを取得する
	if (strcmp(modelInfoStructure_[useModelDataNum_].modelData.material.textureFilePath.c_str(), "") == 0)
	{
		modelInfoStructure_[useModelDataNum_].textureHandle = textureStore_->GetTextureHandle("./Resources/Textures/white2x2.png",
			device, srvDescriptorHeap, commandList);
	}
	else
	{
		modelInfoStructure_[useModelDataNum_].textureHandle = 
			textureStore_->GetTextureHandle(modelInfoStructure_[useModelDataNum_].modelData.material.textureFilePath,device, srvDescriptorHeap, commandList);
	}


	// カウントする
	useModelDataNum_++;

	return modelInfoStructure_[useModelDataNum_ - 1].modelHandle;
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
		if (modelHandle == modelInfoStructure_[i].modelHandle)
		{
			return modelInfoStructure_[i].modelData;
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
		if (modelHandle == modelInfoStructure_[i].modelHandle)
		{
			return modelInfoStructure_[i].vertexResource;
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
		if (modelHandle == modelInfoStructure_[i].modelHandle)
		{
			return modelInfoStructure_[i].textureHandle;
		}
	}

	assert(false);
	return 0;
}