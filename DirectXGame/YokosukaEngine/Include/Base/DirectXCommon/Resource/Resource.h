#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cstdint>
#include <cassert>
#include "../../../Math/Vector2/Vector2.h"
#include "../../../Math/Vector3/Vector3.h"
#include "../../../Math/Vector4/Vector4.h"
#include "../../../Math/Matrix4x4/Matrix4x4.h"


// マテリアル
struct Material
{
	// 色
	Vector4 color;

	// ライティング有効化
	int32_t  enableLighting;

	// ハーフランバート有効化
	int32_t enableHalfLambert;

	// パディング
	float padding[2];

	//  UVトランスフォーム
	Matrix4x4 uvTransform;

	// 光沢度
	float shininess;

};

//頂点情報
struct VertexData
{
	// ローカル座標
	Vector4 position;

	// テクスチャ座標
	Vector2 texcoord;

	// 頂点法線
	Vector3 normal;
};

// 座標変換用の行列
struct TransformationMatrix
{
	// ワールドビュープロジェクション行列
	Matrix4x4 worldViewProjection;

	// ワールド行列
	Matrix4x4 world;

	// 転置ワールド行列
	Matrix4x4 worldInverseTranspose;
};

// GPUに持っていくカメラのデータ
struct CameraForGPU
{
	Vector3 worldPosition;
};


/// <summary>
/// バッファリソースを作成する
/// </summary>
/// <param name="device"></param>
/// <param name="sizeInBytes"></param>
/// <returns></returns>
Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(ID3D12Device* device, size_t sizeInBytes);


// 平面で使用するリソース
class PlaneResources
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device"></param>
	void Initialize(ID3D12Device* device);

	// インデックスリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource_ = nullptr;
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};

	// 頂点リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	// マテリアルリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_ = nullptr;

	// 座標変換リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationResource_ = nullptr;

	// カメラリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> cameraResource_ = nullptr;

};

// リングで使用するリソース
class RingResources
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device"></param>
	void Initialize(ID3D12Device* device);

	// インデックスリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource_ = nullptr;
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};

	// 頂点リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	// マテリアルリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_ = nullptr;

	// 座標変換リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationResource_ = nullptr;

	// カメラリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> cameraResource_ = nullptr;


private:

	// 最大分割数
	const uint32_t kMaxSubdivisions = 32;
};

// 円柱で使用するリソース
class CylinderResources
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device"></param>
	void Initialize(ID3D12Device* device);

	// インデックスリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource_ = nullptr;
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};

	// 頂点リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	// マテリアルリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_ = nullptr;

	// 座標変換リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationResource_ = nullptr;

	// カメラリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> cameraResource_ = nullptr;



private:

	// 最大分割数
	const uint32_t kMaxSubdivisions = 32;
};


// 弾で使用するリソース
class SphereResources
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device"></param>
	void Initialize(ID3D12Device* device);

	// インデックスリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource_ = nullptr;
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};

	// 頂点リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	// マテリアルリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_ = nullptr;

	// 座標変換リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationResource_ = nullptr;

	// カメラリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> cameraResource_ = nullptr;

private:

	// 最大分割数
	const uint32_t kMaxSubdivisions = 32;
};

// 線で使用するリソース
class LineResources
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initliaze(ID3D12Device* device);

	// 頂点リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_ = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	// マテリアルリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource_ = nullptr;

	// 座標変換リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationResource_ = nullptr;

private:

	// 頂点数
	const uint32_t kVertexNum = 2;
};