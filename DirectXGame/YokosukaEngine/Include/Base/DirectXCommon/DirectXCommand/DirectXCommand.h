#pragma once
#include <wrl.h>
#include <cassert>
#include <d3d12.h>
#include <dxgi1_6.h>

#include "../../Logging/Logging.h"
#include "../../../Func/String/String.h"
#include "../DirectXGPU/DirectXGPU.h"

class DirectXCommand
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="directXGPU"></param>
	void Initialize(Logging* logging, DirectXGPU* directXGPU);

	/// <summary>
	/// コマンドキューのGetter
	/// </summary>
	/// <returns></returns>
	ID3D12CommandQueue* GetCommandQueue() { return commandQueue_.Get(); }

	/// <summary>
	/// コマンドアロケータのGetter
	/// </summary>
	/// <returns></returns>
	ID3D12CommandAllocator* GetCommandAllocator() { return commandAllocator_.Get(); }

	/// <summary>
	/// コマンドリストのGetter
	/// </summary>
	/// <returns></returns>
	ID3D12GraphicsCommandList* GetCommandList() { return commandList_.Get(); }


private:


	// ロギング
	Logging* logging_ = nullptr;

	// DirectXGPU
	DirectXGPU* directXGPU_ = nullptr;


	// コマンドキュー
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_ = nullptr;

	// コマンドアロケータ
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_ = nullptr;

	// コマンドリスト
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_ = nullptr;
};

