#pragma once
#include <stdlib.h>
#include <time.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxcapi.h>
#include <cassert>
#include <vector>
#include <wrl.h>
#include "../../Base/Logging/Logging.h"
#include "../../Func/String/String.h"
#include "../../../../Externals/DirectXTex/DirectXTex.h"
#include "../../../../Externals/DirectXTex/d3dx12.h"
#include "../../Base/DirectXCommon/Resource/Resource.h"

// 前方宣言
class DirectXCommon;

class TextureStore
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TextureStore();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(DirectXCommon* directXCommon);

	/// <summary>
	/// テクスチャを読み込みテクスチャハンドルを取得する
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <returns></returns>
	uint32_t GetTextureHandle(const std::string& filePath , ID3D12Device* device,
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap , Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList);

	/// <summary>
	/// 使用するテクスチャを選択する
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void SelectTexture(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList, uint32_t textureHandle);


private:

	// DirectXCommon
	DirectXCommon* directXCommon_ = nullptr;


	// テクスチャ格納データ
	class TextureData
	{
	public:

		// ファイルパス
		std::string filePath;

		// ミップマップ
		DirectX::ScratchImage mipImages;

		// テクスチャリソース
		Microsoft::WRL::ComPtr<ID3D12Resource> textureResource = nullptr;

		// 中間リソース
		Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource = nullptr;

		// srvの設定
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};

		// CPUハンドル
		D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU{};

		// GPUハンドル
		D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU{};

		// テクスチャハンドル
		uint32_t textureHandle = 0;
	};

	// テクスチャ格納のデータ
	std::vector<TextureData*> textureData_;

	// 格納したテクスチャの数
	uint32_t textureNum_ = 0;
};



/// <summary>
/// テクスチャデータを読み込む
/// </summary>
/// <param name="filePath">ファイルパス</param>
/// <returns></returns>
DirectX::ScratchImage LoadTexture(const std::string& filePath);

/// <summary>
/// テクスチャリソースを作成する
/// </summary>
/// <param name="device">デバイス</param>
/// <param name="metadata">テクスチャのデータ</param>
/// <returns></returns>
Microsoft::WRL::ComPtr<ID3D12Resource> CreateTextureResource(Microsoft::WRL::ComPtr<ID3D12Device> device, const DirectX::TexMetadata& metadata);

/// <summary>
/// 
/// </summary>
/// <param name="texture"></param>
/// <param name="mipImages"></param>
/// <param name="device"></param>
/// <param name="commandList"></param>
/// <returns></returns>
[[nodiscard]]
Microsoft::WRL::ComPtr<ID3D12Resource> UploadTextureData(Microsoft::WRL::ComPtr<ID3D12Resource> texture, const DirectX::ScratchImage& mipImages,
	ID3D12Device* device, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList);

/// <summary>
/// デプスステンシルテクスチャのリソースを作成する
/// </summary>
/// <param name="device"></param>
/// <param name="width"></param>
/// <param name="height"></param>
/// <returns></returns>
Microsoft::WRL::ComPtr<ID3D12Resource> CreateDepthStencilTextureResource(ID3D12Device* device, int32_t width, int32_t height);