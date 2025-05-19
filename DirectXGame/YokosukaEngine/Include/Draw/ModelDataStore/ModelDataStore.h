#pragma once
#include "../../Func/LoadModelData/LoadModelData.h"
#include "../../Draw/TextureStore/TextureStore.h"
#include <stdlib.h>

class ModelDataStore
{
public:

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
		Microsoft::WRL::ComPtr<ID3D12Device> device, Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap,
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


	// ディレクトリパス
	std::string directoryPath_[256];

	// ファイル名
	std::string fileName_[256];

	// モデルデータ
	ModelData modelData_[256];

	// 頂点リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_[256] = { nullptr };

	// モデルハンドル
	uint32_t modelHandle_[256];

	// テクスチャハンドル
	uint32_t textureHandle_[256];

	// 使用したモデルデータをカウントする
	uint32_t useModelDataNum_ = 0;
};

