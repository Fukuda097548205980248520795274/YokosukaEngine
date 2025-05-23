#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <vector>
#include <numbers>
#include <list>
#include "../../Math/Vector4/Vector4.h"
#include "../../Math/WorldTransform/WorldTransform.h"
#include "../../Camera/Camera3D/Camera3D.h"
#include "../../Camera/Camera2D/Camera2D.h"
#include "../../Func/String/String.h"
#include "../../Func/LoadModelData/LoadModelData.h"
#include "../OutputLog/OutputLog.h"
#include "../../Draw/DirectXShaderCompiler/DirectXShaderCompiler.h"

#include "../../PipelineStateObject/Object3d/BaseObject3d.h"
#include "../../PipelineStateObject/Object3d/BlendNone/Object3dBlendNone.h"
#include "../../PipelineStateObject/Object3d/BlendNormal/Object3dBlendNormal.h"
#include "../../PipelineStateObject/Object3d/BlendAdd/Object3dBlendAdd.h"
#include "../../PipelineStateObject/Object3d/BlendSubtract/Object3dBlendSubtract.h"
#include "../../PipelineStateObject/Object3d/BlendMultiply/Object3dBlendMultiply.h"
#include "../../PipelineStateObject/Object3d/BlendScreen/Object3dBlendScreen.h"

#include "../../Draw/VertexData/VertexData.h"
#include "../../Draw/TextureStore/TextureStore.h"
#include "../../Draw/ModelDataStore/ModelDataStore.h"
#include "../../Draw/Material/Material.h"
#include "../../Draw/TransformationMatrix/TransformationMatrix.h"
#include "../../Draw/DirectionalLight/DirectionalLight.h"

#pragma comment(lib , "d3d12.lib")
#pragma comment(lib , "dxgi.lib")

// 前方宣言
class WinApp;

class DirectXCommon
{
private:

	// ブレンドモード
	enum BlendMode
	{
		// ブレンドなし
		kBlendModeNone,

		// 通常αブレンド
		kBlendModeNormal,

		// 加算合成
		kBlendModeAdd,

		// 減算合成
		kBlendModeSubtract,

		// 乗算合成
		kBlendModeMultiply,

		// スクリーン合成
		kBlendModeScreen,

		// これは使用してはいけない
		kBlendModekCountOfBlendMode
	};


public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~DirectXCommon();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="log">ログ</param>
	/// <param name="windowApplication">ウィンドウアプリケーション</param>
	void Initialize(OutputLog* log, WinApp* windowApplication);

	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	void PostDraw();

	/// <summary>
	/// テクスチャを読み込む
	/// </summary>
	/// <param name="filePath"></param>
	/// <returns></returns>
	uint32_t LoadTexture(const std::string& filePath) { return textureStore_->GetTextureHandle(filePath, device_, srvDescriptorHeap_, commandList_); }

	/// <summary>
	/// モデルデータを読み込む
	/// </summary>
	/// <param name="directory"></param>
	/// <param name="fileName"></param>
	/// <returns></returns>
	uint32_t LoadModelData(const std::string& directory , const std::string& fileName)
	{
		return modelDataStore_->GetModelHandle(directory, fileName, device_, srvDescriptorHeap_, commandList_);
	}

	/// <summary>
	/// 三角形を描画する
	/// </summary>
	void DrawTriangle(const WorldTransform* worldTransform, const WorldTransform* uvTransform,
		const Camera3D* camera, uint32_t textureHandle, Vector4 color);

	/// <summary>
	/// 球を表がする
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="uvTransform"></param>
	/// <param name="camera"></param>
	/// <param name="textureHandle"></param>
	/// <param name="color"></param>
	void DrawSphere(const WorldTransform* worldTransform, const WorldTransform* uvTransform,
		const Camera3D* camera, uint32_t textureHandle, Vector4 color);

	/// <summary>
	/// モデルを描画する
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="uvTransform"></param>
	/// <param name="camera"></param>
	/// <param name="color"></param>
	void DrawModel(const WorldTransform* worldTransform, const WorldTransform* uvTransform,
		const Camera3D* camera, uint32_t modelHandle, Vector4 color);

	/// <summary>
	/// スプライトを描画する
	/// </summary>
	/// <param name="worldTransform">ワールドトランスフォーム</param>
	/// <param name="uvTransform">UVトランスフォーム</param>
	/// <param name="camera">カメラ</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <param name="color">色</param>
	void DrawSprite(const WorldTransform* worldTransform, const WorldTransform* uvTransform,
		const Camera2D* camera, uint32_t textureHandle, Vector4 color);

	/// <summary>
	/// ブレンドモードのSetter
	/// </summary>
	/// <param name="blendMode"></param>
	void SetBlendMode(uint32_t blendMode) { useBlendMode_ = blendMode; }

	/// <summary>
	/// ディスクリプタヒープを生成する
	/// </summary>
	/// <param name="device"></param>
	/// <param name="heapType"></param>
	/// <param name="numDescriptors"></param>
	/// <param name="ShaderVisible"></param>
	/// <returns></returns>
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescritprHeap(Microsoft::WRL::ComPtr<ID3D12Device> device,
		D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool ShaderVisible);

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

	/// <summary>
	/// リソースステートを変更する（バリアを張る）
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	/// <param name="resource">リソース</param>
	/// <param name="beforeState">遷移前（現在）のリソースステート</param>
	/// <param name="afterState">遷移後のリソースステート</param>
	void ChangeResourceState(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList,
		Microsoft::WRL::ComPtr<ID3D12Resource> resource, D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState);


private:

	/// <summary>
	/// デバッグレイヤーを有効化する
	/// </summary>
	void ActiveDebugLayer();

	/// <summary>
	/// DXGIファクトリー
	/// </summary>
	void GenerateDXGIFactory();

	/// <summary>
	/// 使用するアダプタを取得する
	/// </summary>
	void SelectUseAdapter();

	/// <summary>
	/// デバイスを生成する
	/// </summary>
	void GenerateDevice();

	/// <summary>
	/// エラーと警告で停止させる
	/// </summary>
	void StopOnErrorsAndWarnings();

	/// <summary>
	/// コマンドキューを生成する
	/// </summary>
	void GenerateCommandQueue();

	/// <summary>
	/// コマンドアロケータを生成する
	/// </summary>
	void GenerateCommandAllocator();

	/// <summary>
	/// コマンドリストを生成する
	/// </summary>
	void GenerateCommandList();

	/// <summary>
	/// スワップチェーンを生成する
	/// </summary>
	void GenerateSwapChain();

	/// <summary>
	/// スワップチェーンからリソースを引っ張ってくる
	/// </summary>
	void PullSwapChainResource();

	/// <summary>
	/// RTVを生成する
	/// </summary>
	void GenerateRTV();

	/// <summary>
	/// DSVを生成する
	/// </summary>
	void GenerateDSV();

	/// <summary>
	/// フェンスを生成する
	/// </summary>
	void GenerateFence();

	/// <summary>
	/// イベントを生成する
	/// </summary>
	void GenerateEvent();

	/// <summary>
	/// GPUの実行完了を待つ
	/// </summary>
	void WaitForGPU();
	

	// ログ出力
	OutputLog* log_ = nullptr;

	// ウィンドウアプリケーション
	WinApp* windowApplication_ = nullptr;

	// テクスチャストア
	TextureStore* textureStore_ = nullptr;

	// モデルデータストア
	ModelDataStore* modelDataStore_ = nullptr;


	// デバッグコントローラ
	Microsoft::WRL::ComPtr<ID3D12Debug1> debugController_ = nullptr;

	// DXGIファクトリー
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory_ = nullptr;

	// 使用するアダプタ
	Microsoft::WRL::ComPtr<IDXGIAdapter4> useAdapter_ = nullptr;

	// デバイス
	Microsoft::WRL::ComPtr<ID3D12Device> device_ = nullptr;

	// コマンドキュー
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_ = nullptr;

	// コマンドアロケータ
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_ = nullptr;

	// コマンドリスト
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_ = nullptr;

	// RTV用ディスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvDescriptorHeap_ = nullptr;
	const UINT kNumRtvDescriptors = 2;

	// SRV用ディスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap_ = nullptr;
	const UINT kNumSrvDescriptors = 128;

	// DSV用ディスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeap_ = nullptr;
	const UINT kNunDsvDescriptors = 1;

	// スワップチェーン
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain_ = nullptr;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc_{};

	// スワップチェーン用のリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> swapChainResources_[2] = { nullptr };

	// RTV
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc_{};
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[2] = {};

	// 深度情報のリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilResource_ = nullptr;

	// フェンス
	Microsoft::WRL::ComPtr<ID3D12Fence> fence_ = nullptr;
	uint64_t fenceValue_ = 0;

	// イベント
	HANDLE fenceEvent_{};

	// DXC
	DirectXShaderCompiler* dxc_ = nullptr;

	// ビューポート
	D3D12_VIEWPORT viewport_{};
	
	// シザーレクト
	D3D12_RECT scissorRect_{};

	// Object3d用のPSO
	BaseObject3d* psoObject3d_[kBlendModekCountOfBlendMode] = { nullptr };

	// 使用しているブレンドモード
	uint32_t useBlendMode_ = kBlendModeNormal;



	/*---------------------------
	    三角錐で使用するリソース
	---------------------------*/

	// 頂点バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBufferResourceTriangularPyramid_[512] = {nullptr};

	// マテリアルリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> MaterialResourceTriangularPyramid_[512] = { nullptr };

	// 座標変換リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> TransformationResourceTriangularPyramid_[512] = { nullptr };

	// 平行光源リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResourceTriangularPyramid_[512] = { nullptr };

	// 使用したリソースをカウントする
	uint32_t useNumResourceTriangularPyramid_ = 0;


	/*-----------------------
	    球で使用するリソース
	-----------------------*/

	// 分割数
	const uint32_t kSubdivision = 16;

	// インデックスリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResourceSphere_[512] = { nullptr };

	// 頂点バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBufferResourceSphere_[512] = { nullptr };

	// マテリアルリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> MaterialResourceSphere_[512] = { nullptr };

	// 座標変換リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> TransformationResourceSphere_[512] = { nullptr };

	// 平行光源リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResourceSphere_[512] = { nullptr };

	// 使用したリソースをカウントする
	uint32_t useNumResourceSphere_ = 0;


	/*----------------------------
	    モデルで使用するリソース
	----------------------------*/

	// マテリアルリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> MaterialResourceModel_[512] = { nullptr };

	// 座標変換リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> TransformationResourceModel_[512] = { nullptr };

	// 平行光源リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> directionalLightResourceModel_[512] = { nullptr };

	// 使用したリソースをカウントする
	uint32_t useNumResourceModel_ = 0;


	/*-----------------------------
	    スプライトで使用するリソース
	-----------------------------*/

	// インデックスリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResourceSprite_[512] = { nullptr };

	// 頂点バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBufferResourceSprite_[512] = { nullptr };

	// マテリアルリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> MaterialResourceSprite_[512] = { nullptr };

	// 座標変換リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> TransformationResourceSprite_[512] = { nullptr };

	// 使用したリソースをカウントする
	uint32_t useNumResourceSprite_ = 0;
};

