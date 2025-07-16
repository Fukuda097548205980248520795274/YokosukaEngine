#include "Resource.h"

#pragma comment(lib , "d3d12.lib")
#pragma comment(lib , "dxgi.lib")

/// <summary>
/// バッファリソースを作成する
/// </summary>
/// <param name="device"></param>
/// <param name="sizeInBytes"></param>
/// <returns></returns>
Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(ID3D12Device* device, size_t sizeInBytes)
{
	// リソースのヒープ設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;

	// リソースの設定
	D3D12_RESOURCE_DESC bufferResourceDesc{};

	// バッファリソース
	bufferResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;

	// リソースのサイズ
	bufferResourceDesc.Width = sizeInBytes;

	// バッファの場合はこれらを0にする
	bufferResourceDesc.Height = 1;
	bufferResourceDesc.DepthOrArraySize = 1;
	bufferResourceDesc.MipLevels = 1;
	bufferResourceDesc.SampleDesc.Count = 1;

	// バッファの場合はこれにする
	bufferResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// バッファリソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> bufferResource = nullptr;
	HRESULT hr = device->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &bufferResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&bufferResource));
	assert(SUCCEEDED(hr));

	return bufferResource;
}



/// <summary>
/// 初期化
/// </summary>
/// <param name="device"></param>
void PlaneResources::Initialize(ID3D12Device* device)
{
	// インデックスリソースのバッファを作成する
	indexResource_ = CreateBufferResource(device, sizeof(uint32_t) * 6);
	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * 6;
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	// 頂点リソースのバッファを作成する
	vertexResource_ = CreateBufferResource(device, sizeof(VertexData) * 4);
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 4;
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	// マテリアルリソースのバッファを作成する
	materialResource_ = CreateBufferResource(device, sizeof(Material));

	// 座標変換リソースのバッファを作成する
	transformationResource_ = CreateBufferResource(device, sizeof(TransformationMatrix));

	// カメラリソースのバッファを作成する
	cameraResource_ = CreateBufferResource(device, sizeof(CameraForGPU));
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="device"></param>
void RingResources::Initialize(ID3D12Device* device)
{
	// インデックスリソースのバッファを作成する
	indexResource_ = CreateBufferResource(device, sizeof(uint32_t) * (kMaxSubdivisions * 6));
	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * (kMaxSubdivisions * 6);
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	// 頂点リソースのバッファを作成する
	vertexResource_ = CreateBufferResource(device, sizeof(VertexData) * (kMaxSubdivisions * 4));
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * (kMaxSubdivisions * 4);
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	// マテリアルリソースのバッファを作成する
	materialResource_ = CreateBufferResource(device, sizeof(Material));

	// 座標変換リソースのバッファを作成する
	transformationResource_ = CreateBufferResource(device, sizeof(TransformationMatrix));

	// カメラリソースのバッファを作成する
	cameraResource_ = CreateBufferResource(device, sizeof(CameraForGPU));
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="device"></param>
void CylinderResources::Initialize(ID3D12Device* device)
{
	// インデックスリソースのバッファを作成する
	indexResource_ = CreateBufferResource(device, sizeof(uint32_t) * (kMaxSubdivisions * 6));
	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * (kMaxSubdivisions * 6);
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	// 頂点リソースのバッファを作成する
	vertexResource_ = CreateBufferResource(device, sizeof(VertexData) * (kMaxSubdivisions * 4));
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * (kMaxSubdivisions * 4);
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	// マテリアルリソースのバッファを作成する
	materialResource_ = CreateBufferResource(device, sizeof(Material));

	// 座標変換リソースのバッファを作成する
	transformationResource_ = CreateBufferResource(device, sizeof(TransformationMatrix));

	// カメラリソースのバッファを作成する
	cameraResource_ = CreateBufferResource(device, sizeof(CameraForGPU));
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="device"></param>
void SphereResources::Initialize(ID3D12Device* device)
{
	// インデックスリソースのバッファを作成する
	indexResource_ = CreateBufferResource(device, sizeof(uint32_t) * (kMaxSubdivisions * kMaxSubdivisions) * 6);
	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * (kMaxSubdivisions * kMaxSubdivisions) * 6;
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	// 頂点リソースのバッファを作成する
	vertexResource_ = CreateBufferResource(device, sizeof(VertexData) * (kMaxSubdivisions * kMaxSubdivisions) * 4);
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * (kMaxSubdivisions * kMaxSubdivisions) * 4;
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	// マテリアルリソースのバッファを作成する
	materialResource_ = CreateBufferResource(device, sizeof(Material));

	// 座標変換リソースのバッファを作成する
	transformationResource_ = CreateBufferResource(device, sizeof(TransformationMatrix));

	// カメラリソースのバッファを作成する
	cameraResource_ = CreateBufferResource(device, sizeof(CameraForGPU));
}

/// <summary>
/// 初期化
/// </summary>
void LineResources::Initliaze(ID3D12Device* device)
{
	// 頂点リソースのバッファを作成する
	vertexResource_ = CreateBufferResource(device, sizeof(Vector4) * kVertexNum);
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = sizeof(Vector4) * kVertexNum;
	vertexBufferView_.StrideInBytes = sizeof(Vector4);

	// マテリアルリソースのバッファを作成する
	materialResource_ = CreateBufferResource(device, sizeof(Vector4));

	// 座標変換リソースのバッファを作成する
	transformationResource_ = CreateBufferResource(device, sizeof(Matrix4x4));
}