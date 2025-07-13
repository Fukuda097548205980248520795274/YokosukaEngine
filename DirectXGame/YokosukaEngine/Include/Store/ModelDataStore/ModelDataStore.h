#pragma once
#include "../../Func/LoadModelData/LoadModelData.h"
#include "../TextureStore/TextureStore.h"
#include <stdlib.h>

class ModelDataStore
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ModelDataStore();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(TextureStore* textureStore);

	/// <summary>
	/// モデルハンドルを取得する
	/// </summary>
	/// <param name="directoryPath"></param>
	/// <param name="filePath"></param>
	uint32_t GetModelHandle(const std::string& directoryPath, const std::string& fileName,
		ID3D12Device* device, Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap,
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList);

	/// <summary>
	/// モデルハンドルのGetter
	/// </summary>
	/// <param name="modelHandle"></param>
	/// <returns></returns>
	ModelData GetModelData(uint32_t modelHandle);

	/// <summary>
	/// 頂点リソースのGetter
	/// </summary>
	/// <param name="modelHandle"></param>
	/// <returns></returns>
	Microsoft::WRL::ComPtr<ID3D12Resource> GetVertexResource(uint32_t modelHandle);

	/// <summary>
	/// テクスチャハンドルを取得する
	/// </summary>
	/// <param name="modelHandle"></param>
	/// <returns></returns>
	uint32_t GetTextureHandle(uint32_t modelHandle);


private:

	// テクスチャ格納場所
	TextureStore* textureStore_ = nullptr;


	// モデルの情報
	class ModelInfo
	{
	public:

		// ディレクトリパス
		std::string directoryPath;

		// ファイル名
		std::string fileName;

		// モデルデータ
		ModelData modelData{};

		// 頂点リソース
		Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource = nullptr;

		// モデルハンドル
		uint32_t modelHandle = 0;

		// テクスチャハンドル
		uint32_t textureHandle = 0;
	};

	// モデルの情報の構造体
	std::vector<ModelInfo*> modelInfo_;
	

	// 使用したモデルデータをカウントする
	uint32_t useModelDataNum_ = 0;
};

