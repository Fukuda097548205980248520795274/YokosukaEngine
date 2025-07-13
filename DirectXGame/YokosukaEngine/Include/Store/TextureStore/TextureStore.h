#pragma once
#include <stdlib.h>
#include <time.h>
#include "../../Func/LoadTexture/LoadTexture.h"

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