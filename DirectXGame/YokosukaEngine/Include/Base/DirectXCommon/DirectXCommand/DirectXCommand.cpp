#include "DirectXCommand.h"

#pragma comment(lib , "d3d12.lib")
#pragma comment(lib , "dxgi.lib")

/// <summary>
/// 初期化
/// </summary>
/// <param name="directXGPU"></param>
void DirectXCommand::Initialize(Logging* logging, DirectXGPU* directXGPU)
{
	// nullptrチェック
	assert(logging);
	assert(directXGPU);

	// 引数を受け取る
	logging_ = logging;
	directXGPU_ = directXGPU;


	/*------------------------
	    コマンドキューの生成
	------------------------*/

	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};

	HRESULT hr = directXGPU_->GetDevice()->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue_));
	assert(SUCCEEDED(hr));

	// コマンドキューを生成した旨のログ
	logging_->Log("Create CommandQueue \n");


	/*--------------------------
	    コマンドアロケータの生成
	--------------------------*/

	hr = directXGPU_->GetDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator_));
	assert(SUCCEEDED(hr));

	// コマンドアロケータを生成した旨のログ
	logging_->Log("Create CommandAllocator \n");


	/*------------------------
	    コマンドリストの生成
	------------------------*/

	hr = directXGPU_->GetDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator_.Get(), nullptr, IID_PPV_ARGS(&commandList_));
	assert(SUCCEEDED(hr));

	// コマンドリストを生成した旨のログ
	logging_->Log("Create CommandList \n");
}