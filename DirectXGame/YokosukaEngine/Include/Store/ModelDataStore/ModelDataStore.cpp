#include "ModelDataStore.h"

/// <summary>
/// デストラクタ
/// </summary>
ModelDataStore::~ModelDataStore()
{
	// モデルの情報
	for (ModelInfo* modelInfo : modelInfo_)
	{
		delete modelInfo;
	}
	modelInfo_.clear();
}

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
	ID3D12Device* device, Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap ,
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList)
{
	// 同じモデルデータを使われているか
	for (uint32_t i = 0; i < useModelDataNum_; i++)
	{
		if (strcmp(directoryPath.c_str(), modelInfo_[i]->directoryPath.c_str())) { continue; }
		if (strcmp(fileName.c_str(), modelInfo_[i]->fileName.c_str())) { continue; }

		return modelInfo_[i]->modelHandle;
	}

	// 新規モデル情報
	ModelInfo* modelInfo = new ModelInfo();

	// モデルハンドルを決める
	while (modelInfo->modelHandle == 0)
	{
		modelInfo->modelHandle = rand() % 100000 + 1;

		// モデルハンドルが被らないようにする
		for (uint32_t j = 0; j < useModelDataNum_; j++)
		{
			if (useModelDataNum_ == j)
				continue;

			if (modelInfo->modelHandle == modelInfo_[j]->modelHandle)
			{
				modelInfo->modelHandle = 0;
				break;
			}
		}
	}

	// パスを受け取る
	modelInfo->directoryPath = directoryPath;
	modelInfo->fileName = fileName;

	// モデルを読み込む
	modelInfo->modelData = LoadObjFile(directoryPath, fileName);

	// 頂点リソースを作る
	modelInfo->vertexResource =
		CreateBufferResource(device, sizeof(VertexData) * modelInfo->modelData.vertices.size());

	// テクスチャハンドルを取得する
	if (strcmp(modelInfo->modelData.material.textureFilePath.c_str(), "") == 0)
	{
		modelInfo->textureHandle = textureStore_->GetTextureHandle("./Resources/Textures/white2x2.png",
			device, srvDescriptorHeap, commandList);
	}
	else
	{
		modelInfo->textureHandle =
			textureStore_->GetTextureHandle(modelInfo->modelData.material.textureFilePath,device, srvDescriptorHeap, commandList);
	}


	// ベクトルに追加する
	modelInfo_.push_back(modelInfo);

	// カウントする
	useModelDataNum_++;

	return modelInfo_[useModelDataNum_ - 1]->modelHandle;
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
		if (modelHandle == modelInfo_[i]->modelHandle)
		{
			return modelInfo_[i]->modelData;
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
		if (modelHandle == modelInfo_[i]->modelHandle)
		{
			return modelInfo_[i]->vertexResource;
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
		if (modelHandle == modelInfo_[i]->modelHandle)
		{
			return modelInfo_[i]->textureHandle;
		}
	}

	assert(false);
	return 0;
}