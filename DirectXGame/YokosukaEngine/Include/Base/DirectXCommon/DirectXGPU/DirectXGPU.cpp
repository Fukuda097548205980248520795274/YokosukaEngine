#include "DirectXGPU.h"

#pragma comment(lib , "d3d12.lib")
#pragma comment(lib , "dxgi.lib")

/// <summary>
/// 初期化
/// </summary>
void DirectXGPU::Initialize(Logging* logging)
{
	// nullptrチェック
	assert(logging);

	// 引数を受け取る
	logging_ = logging;


	/*-------------------------
	   DXGIファクトリーの生成
	-------------------------*/

	HRESULT hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_));
	assert(SUCCEEDED(hr));

	// 生成完了ログ
	logging_->Log("Create DXGIFactory \n");


	/*-----------------------------------
	    使用するアダプタ（GPU）を取得する
	-----------------------------------*/

	// 良い順にアダプタを頼む
	for (UINT i = 0;
		dxgiFactory_->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&useAdapter_)) != DXGI_ERROR_NOT_FOUND;
		++i)
	{
		// アダプタ情報を取得する
		DXGI_ADAPTER_DESC3 adapterDesc{};
		HRESULT hr = useAdapter_->GetDesc3(&adapterDesc);
		assert(SUCCEEDED(hr));

		// ソフトウェアアダプタでなければ採用
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE))
		{
			// 採用したアダプタの情報をログに出力
			logging_->Log(ConvertString(std::format(L"Use Adapter : {} \n", adapterDesc.Description)));
			break;
		}

		// ソフトウェアアダプタの場合はなかったことにする
		useAdapter_ = nullptr;
	}

	assert(useAdapter_ != nullptr);


	/*------------------
	    デバイスの生成
	------------------*/


	// 機能レベル
	D3D_FEATURE_LEVEL featureLevels[] =
	{ D3D_FEATURE_LEVEL_12_2 , D3D_FEATURE_LEVEL_12_1 , D3D_FEATURE_LEVEL_12_0 };

	// ログ出力用の文字列
	const char* featureLevelStrings[] = { "12.2" , "12.1", "12.0" };

	// 高い順に生成できるか試していく
	for (size_t i = 0; i < _countof(featureLevels); ++i)
	{
		// 採用したアダプタでデバイスを生成する
		HRESULT hr = D3D12CreateDevice(useAdapter_.Get(), featureLevels[i], IID_PPV_ARGS(&device_));

		// 指定した機能レベルでデバイスが生成できたかどうか
		if (SUCCEEDED(hr))
		{
			// 生成できた旨を伝えるログ
			logging_->Log(std::format("FeatureLevel : {} \n", featureLevelStrings[i]));
			break;
		}
	}

	assert(device_ != nullptr);

	// 初期化完了
	logging_->Log("Complate create D3D12Device!!! \n");
}