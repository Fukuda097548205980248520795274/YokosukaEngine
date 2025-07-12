#pragma once
#include <wrl.h>
#include <cassert>
#include <d3d12.h>
#include <dxgi1_6.h>

#include "../../Logging/Logging.h"
#include "../../../Func/String/String.h"

class DirectXGPU
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Logging* logging);

	/// <summary>
	/// DXGIファクトリのGetter
	/// </summary>
	/// <returns></returns>
	IDXGIFactory7* GetDXGIfactory() { return dxgiFactory_.Get(); }

	/// <summary>
	/// アダプターのGetter
	/// </summary>
	/// <returns></returns>
	IDXGIAdapter4* GetAdapter() { return useAdapter_.Get(); }

	/// <summary>
	/// デバイスのGetter
	/// </summary>
	/// <returns></returns>
	ID3D12Device* GetDevice() { return device_.Get(); }


private:

	// ロギング
	Logging* logging_ = nullptr;


	// DXGIファクトリー
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory_ = nullptr;

	// 使用するアダプタ
	Microsoft::WRL::ComPtr<IDXGIAdapter4> useAdapter_ = nullptr;

	// デバイス
	Microsoft::WRL::ComPtr<ID3D12Device> device_ = nullptr;
};

