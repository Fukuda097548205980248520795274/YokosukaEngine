#pragma once
#include <stdlib.h>
#include <time.h>
#include "../../Func/LoadTexture/LoadTexture.h"

class TextureStore
{
public:

	/// <summary>
	/// テクスチャを読み込みテクスチャハンドルを取得する
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <returns></returns>
	uint32_t GetTextureHandle(const std::string& filePath , Microsoft::WRL::ComPtr<ID3D12Device> device,
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap , Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList);

	/// <summary>
	/// 使用するテクスチャを選択する
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void SelectTexture(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList, uint32_t textureHandle);


private:

	/// <summary>
	/// 指定したディスクリプタヒープに格納するためのポインタを取得する（CPU）
	/// </summary>
	/// <param name="descriptorHeap">ディスクリプタヒープ</param>
	/// <param name="descriptorSize">ディスクリプタのサイズ</param>
	/// <param name="index">配列番号</param>
	/// <returns></returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap,
		uint32_t descriptorSize, uint32_t index);

	/// <summary>
	/// 指定したディスクリプタヒープに格納するためのポインタを取得する（GPU）
	/// </summary>
	/// <param name="descriptorHeap">ディスクリプタヒープ</param>
	/// <param name="descriptorSize">ディスクリプタのサイズ</param>
	/// <param name="index">配列番号</param>
	/// <returns></returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap,
		uint32_t descriptorSize, uint32_t index);


	// ファイルパス
	std::string filePath_[512];

	// ミップマップ
	DirectX::ScratchImage mipImages_[512]{};

	// テクスチャリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> textureResource_[512] = { nullptr };

	// 中間リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource_[512] = { nullptr };

	// srvの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc_[512]{};

	// CPUハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU_[512];

	// GPUハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_[512];

	// テクスチャハンドル
	uint32_t textureHandle_[512] = { 0 };

	// 格納したテクスチャの数
	uint32_t textureNum_ = 0;
};

