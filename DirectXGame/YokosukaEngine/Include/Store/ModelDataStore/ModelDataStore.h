#pragma once
#include <vector>
#include <string>
#include <cassert>
#include <fstream>
#include <sstream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../../Math/Matrix4x4/Matrix4x4.h"
#include "../../Base/DirectXCommon/Resource/Resource.h"
#include "../TextureStore/TextureStore.h"
#include <stdlib.h>

// ノード
struct Node
{
	// ローカル行列
	Matrix4x4 localMatrix;

	// ノードの名前
	std::string name;

	// 子ノード
	std::vector<Node> children;
};

// マテリアルデータ
struct MaterialData
{
	// テクスチャのファイルパス
	std::string textureFilePath;
};

// モデルデータ
struct ModelData
{
	// 頂点
	std::vector<VertexData> vertices;

	// マテリアルデータ
	MaterialData material;

	Node rootNode;
};

// モデルの情報
class ModelInfo
{
public:

	// ディレクトリパス
	std::string directoryPath;

	// ファイル名
	std::string fileName;

	// モデルデータ
	std::vector<ModelData> modelData{};

	// 頂点バッファビュー
	std::vector<D3D12_VERTEX_BUFFER_VIEW> vertexBufferView{};

	// 頂点リソース
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> vertexResource;

	// 頂点データ
	std::vector<VertexData*> vertexData;

	// モデルハンドル
	uint32_t modelHandle = 0;

	// テクスチャハンドル
	std::vector<uint32_t> textureHandle;
};

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
	/// モデル情報のGetter
	/// </summary>
	/// <param name="modelHandle"></param>
	/// <returns></returns>
	ModelInfo* GetModelInfo(uint32_t modelHandle);


private:

	// テクスチャ格納場所
	TextureStore* textureStore_ = nullptr;
	

	// モデルの情報の構造体
	std::vector<ModelInfo*> modelInfo_;
	

	// 使用したモデルデータをカウントする
	uint32_t useModelDataNum_ = 0;
};


/// <summary>
/// Objファイルを読み込む
/// </summary>
/// <param name="directoryPath">ディレクトリのパス</param>
/// <param name="fileName">ファイル名</param>
/// <returns></returns>
std::vector<ModelData> LoadObjFile(const std::string& directoryPath, const std::string& fileName);

/// <summary>
/// ノードを読む
/// </summary>
/// <param name="node"></param>
/// <returns></returns>
Node ReadNode(aiNode* node);

/// <summary>
/// Mtlファイルを読み込む
/// </summary>
/// <param name="directoryPath"></param>
/// <param name="fileName"></param>
/// <returns></returns>
MaterialData LoadMtlFile(const std::string& directoryPath, const std::string& fileName);

/// <summary>
/// Mtlファイルを読み込む
/// </summary>
/// <param name="directoryPath"></param>
/// <param name="fileName"></param>
/// <returns></returns>
MaterialData LoadMtlFile(const std::string& directoryPath, const std::string& fileName, std::string& materialName);