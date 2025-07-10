#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <vector>
#include <numbers>
#include <list>
#include <random>
#include <algorithm>
#include "../../Math/Vector4/Vector4.h"
#include "../../Math/Transform2D/Transform2D.h"
#include "../../Camera/Camera3D/Camera3D.h"
#include "../../Camera/Camera2D/Camera2D.h"
#include "../../Func/String/String.h"
#include "../../Func/LoadModelData/LoadModelData.h"
#include "../OutputLog/OutputLog.h"
#include "../../Func/Collision/Collision.h"

#include "../../PipelineStateObject/Object3d/BaseObject3d.h"
#include "../../PipelineStateObject/Object3d/BlendNone/Object3dBlendNone.h"
#include "../../PipelineStateObject/Object3d/BlendNormal/Object3dBlendNormal.h"
#include "../../PipelineStateObject/Object3d/BlendAdd/Object3dBlendAdd.h"
#include "../../PipelineStateObject/Object3d/BlendSubtract/Object3dBlendSubtract.h"
#include "../../PipelineStateObject/Object3d/BlendMultiply/Object3dBlendMultiply.h"
#include "../../PipelineStateObject/Object3d/BlendScreen/Object3dBlendScreen.h"

#include "../../PipelineStateObject/Line3d/BaseLine3d.h"
#include "../../PipelineStateObject/Line3d/BlendNone/Line3dBlendNone.h"
#include "../../PipelineStateObject/Line3d/BlendNormal/Line3dBlendNormal.h"
#include "../../PipelineStateObject/Line3d/BlendAdd/Line3dBlendAdd.h"
#include "../../PipelineStateObject/Line3d/BlendSubtract/Line3dBlendSubtract.h"
#include "../../PipelineStateObject/Line3d/BlendMultiply/Line3dBlendMultiply.h"
#include "../../PipelineStateObject/Line3d/BlendScreen/Line3dBlendScreen.h"

#include "../../ParticleEffect/Particle/Particle.h"
#include "../../Draw/DirectXShaderCompiler/DirectXShaderCompiler.h"

#include "../../PipelineStateObject/Particle/BaseParticle.h"
#include "../../PipelineStateObject/Particle/BlendNone/ParticleBlendNone.h"
#include "../../PipelineStateObject/Particle/BlendNormal/ParticleBlendNormal.h"
#include "../../PipelineStateObject/Particle/BlendAdd/ParticleBlendAdd.h"
#include "../../PipelineStateObject/Particle/BlendSubtract/ParticleBlendSubtract.h"
#include "../../PipelineStateObject/Particle/BlendMultiply/ParticleBlendMultiply.h"
#include "../../PipelineStateObject/Particle/BlendScreen/ParticleBlendScreen.h"

#include "../../PipelineStateObject/Primitive/BasePrimitive.h"
#include "../../PipelineStateObject/Primitive/BlendNone/PrimitiveBlendNone.h"
#include "../../PipelineStateObject/Primitive/BlendNormal/PrimitiveBlendNormal.h"
#include "../../PipelineStateObject/Primitive/BlendAdd/PrimitiveBlendAdd.h"
#include "../../PipelineStateObject/Primitive/BlendSubtract/PrimitiveBlendSubtract.h"
#include "../../PipelineStateObject/Primitive/BlendMultiply/PrimitiveBlendMultiply.h"
#include "../../PipelineStateObject/Primitive/BlendScreen/PrimitiveBlendScreen.h"

#include "../../PipelineStateObject/CopyImage/BaseCopyImage.h"
#include "../../PipelineStateObject/CopyImage/CopyImageBlendNone/CopyImageBlendNone.h"
#include "../../PipelineStateObject/CopyImage/CopyImageBlendNormal/CopyImageBlendNormal.h"
#include "../../PipelineStateObject/CopyImage/CopyImageBlendAdd/CopyImageBlendAdd.h"
#include "../../PipelineStateObject/CopyImage/CopyImageBlendSubtract/CopyImageBlendSubtract.h"
#include "../../PipelineStateObject/CopyImage/CopyImageBlendMultiply/CopyImageBlendMultiply.h"
#include "../../PipelineStateObject/CopyImage/CopyImageBlendScreen/CopyImageBlendScreen.h"

#include "../../PipelineStateObject/PostEffect/PostEffect.h"
#include "../../PipelineStateObject/PostEffect/CopyImage/CopyImage.h"
#include "../../PipelineStateObject/PostEffect/GrayScale/GrayScale.h"
#include "../../PipelineStateObject/PostEffect/Sepia/Sepia.h"
#include "../../PipelineStateObject/PostEffect/Vignette/Vignette.h"
#include "../../PipelineStateObject/PostEffect/Smoothing/Smoothing.h"
#include "../../PipelineStateObject/PostEffect/GaussianFillter/GaussianFilter.h"
#include "../../PipelineStateObject/PostEffect/LuminanceBaseOutline/LuminanceBaseOutline.h"
#include "../../PipelineStateObject/PostEffect/BrightnessExtraction/BrightnessExtraction.h"
#include "../../PipelineStateObject/PostEffect/Hide/Hide.h"

#include "../../Transform/WorldTransform/WorldTransform.h"
#include "../../Transform/WorldTransform2D/WorldTransform2D.h"
#include "../../Transform/UvTransform/UvTransform.h"


#include "../../Draw/VertexData/VertexData.h"
#include "../../Store/TextureStore/TextureStore.h"
#include "../../Store/ModelDataStore/ModelDataStore.h"
#include "../../Draw/Material/Material.h"
#include "../../Draw/TransformationMatrix/TransformationMatrix.h"
#include "../../Draw/GPUforPostEffect/GPUforPostEffect.h"

#include "../../Light/DirectionalLight/DirectionalLight.h"
#include "../../Light/PointLight/PointLight.h"
#include "../../Light/SpotLight/SpotLight.h"

#pragma comment(lib , "d3d12.lib")
#pragma comment(lib , "dxgi.lib")

// 前方宣言
class WinApp;

// エフェクト
enum Effect
{
	// 通常コピー
	kCopyImage,

	// グレースケール
	kGrayScale,

	// セピア
	kSepia,

	// ヴィネッティング
	kVignetteing,

	// 平滑化
	kSmoothing,

	// ガウシアンフィルター
	kGaussianFilter,

	// アウトライン : 輝度
	kLuminanceBaseOutline,

	// 高輝度抽出
	kBrightnessExtraction,

	// 隠す
	kHide,

	// エフェクトの数
	kEfectCount,
};

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
	/// 平行光源を設置する
	/// </summary>
	/// <param name="directionalLight"></param>
	void SetDirectionalLight(const DirectionalLight* directionalLight);

	/// <summary>
	/// ポイントライトを設置する
	/// </summary>
	/// <param name="pointLight"></param>
	void SetPointLight(const PointLight* pointLight);

	/// <summary>
	/// スポットライトを設置する
	/// </summary>
	/// <param name="spotLight"></param>
	void SetSpotLight(const SpotLight* spotLight);

	/// <summary>
	/// オフスクリーンにエフェクトをかける
	/// </summary>
	/// <param name="effect"></param>
	void SetOffscreenEffect(Effect effect);

	/// <summary>
	/// RTVに描画したテクスチャをコピーする
	/// </summary>
	void CopyRtvImage(uint32_t rtvNum);

	/// <summary>
	/// 平面を描画する
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="uvTransform"></param>
	/// <param name="camera"></param>
	/// <param name="textureHandle"></param>
	/// <param name="color"></param>
	/// <param name="isLighting"></param>
	void DrawPlane(const WorldTransform* worldTransform, const UvTransform* uvTransform,
		const Camera3D* camera, uint32_t textureHandle, Vector4 color, bool isLighting);

	/// <summary>
	/// 球を描画する
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="uvTransform"></param>
	/// <param name="camera"></param>
	/// <param name="color"></param>
	/// <param name="isLighting"></param>
	void DrawSphere(const WorldTransform* worldTransform, const UvTransform* uvTransform,
		const Camera3D* camera, uint32_t textureHandle, uint32_t segment, uint32_t ring, Vector4 color, bool isLighting);

	/// <summary>
	/// リングを描画する
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="uvTransform"></param>
	/// <param name="camera"></param>
	/// <param name="textureHandle"></param>
	/// <param name="subdivisions"></param>
	/// <param name="outRadius"></param>
	/// <param name="inRadius"></param>
	/// <param name="color"></param>
	/// <param name="isLighting"></param>
	void DrawRing(const WorldTransform* worldTransform, const UvTransform* uvTransform,
		const Camera3D* camera, uint32_t textureHandle, uint32_t subdivisions, float outRadius, float inRadius, Vector4 color, bool isLighting);

	/// <summary>
	/// 円柱を描画する
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="uvTransform"></param>
	/// <param name="camera"></param>
	/// <param name="textureHandle"></param>
	/// <param name="subdivisions"></param>
	/// <param name="height"></param>
	/// <param name="radius"></param>
	/// <param name="color"></param>
	/// <param name="isLighting"></param>
	void DrawCylinder(const WorldTransform* worldTransform, const UvTransform* uvTransform,
		const Camera3D* camera, uint32_t textureHandle, uint32_t subdivisions, float height, float radius, Vector4 color, bool isLighting);

	/// <summary>
	/// モデルを描画する
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="uvTransform"></param>
	/// <param name="camera"></param>
	/// <param name="modelHandle"></param>
	/// <param name="color"></param>
	/// <param name="isLighting"></param>
	void DrawModel(const WorldTransform* worldTransform, const UvTransform* uvTransform,
		const Camera3D* camera, uint32_t modelHandle, Vector4 color ,bool isLighting);

	/// <summary>
	/// パーティクルを描画する
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="uvTransform"></param>
	/// <param name="camera"></param>
	/// <param name="color"></param>
	void DrawParticle(const Camera3D* camera, uint32_t modelHandle, Vector4 color);

	/// <summary>
	/// 線を描画する
	/// </summary>
	/// <param name="start">始点</param>
	/// <param name="end">終点</param>
	/// <param name="camera">カメラ</param>
	/// <param name="color">色</param>
	void DrawLine(const Vector3& start, const Vector3& end, const Camera3D* camera, Vector4 color);

	/// <summary>
	/// スプライトを描画する
	/// </summary>
	/// <param name="worldTransform">ワールドトランスフォーム</param>
	/// <param name="uvTransform">UVトランスフォーム</param>
	/// <param name="camera">カメラ</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <param name="color">色</param>
	void DrawSprite(const Vector2 v1, const Vector2 v2, const Vector2 v3, const Vector2 v4,
		const UvTransform* uvTransform, const Camera2D* camera, uint32_t textureHandle, Vector4 color);

	/// <summary>
	/// ブレンドモードのSetter
	/// </summary>
	/// <param name="blendMode"></param>
	void SetObject3dBlendMode(uint32_t blendMode) { useObject3dBlendMode_ = blendMode; }

	/// <summary>
	/// ブレンドモードのSetter
	/// </summary>
	/// <param name="blendMode"></param>
	void SetParticleBlendMode(uint32_t blendMode) { useParticleBlendMode_ = blendMode; }

	/// <summary>
	/// ブレンドモードのSetter
	/// </summary>
	/// <param name="blendMode"></param>
	void SetLine3dBlendMode(uint32_t blendMode) { useLine3dBlendMode_ = blendMode; }

	/// <summary>
	/// ブレンドモードのSetter
	/// </summary>
	/// <param name="blendMode"></param>
	void SetCopyImageBlendMode(uint32_t blendMode) { useCopyImageBlendMode_ = blendMode; }

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
	D3D12_CPU_DESCRIPTOR_HANDLE GetRTVCPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap,
		Microsoft::WRL::ComPtr<ID3D12Device> device);

	/// <summary>
	/// 指定したディスクリプタヒープに格納するためのポインタを取得する（CPU）
	/// </summary>
	/// <param name="descriptorHeap">ディスクリプタヒープ</param>
	/// <param name="descriptorSize">ディスクリプタのサイズ</param>
	/// <param name="index">配列番号</param>
	/// <returns></returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetSRVCPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap,
		Microsoft::WRL::ComPtr<ID3D12Device> device);

	/// <summary>
	/// 指定したディスクリプタヒープに格納するためのポインタを取得する（GPU）
	/// </summary>
	/// <param name="descriptorHeap">ディスクリプタヒープ</param>
	/// <param name="descriptorSize">ディスクリプタのサイズ</param>
	/// <param name="index">配列番号</param>
	/// <returns></returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetSRVGPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap,
		Microsoft::WRL::ComPtr<ID3D12Device> device);

	/// <summary>
	/// RenderTextureを作成する
	/// </summary>
	/// <param name="device"></param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <param name="format"></param>
	/// <param name="clearColor"></param>
	/// <returns></returns>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateRenderTextureResource(Microsoft::WRL::ComPtr<ID3D12Device> device,
		uint32_t width, uint32_t height, DXGI_FORMAT format, const Vector4& clearColor);

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

	/// <summary>
	/// Object3Dを生成する
	/// </summary>
	void CreateObject3d();

	/// <summary>
	/// Particleを生成する
	/// </summary>
	void CreateParticle();

	/// <summary>
	/// Line3Dを生成する
	/// </summary>
	void CreateLine3d();

	/// <summary>
	/// Primitiveを生成する
	/// </summary>
	void CreatePrimitive();

	/// <summary>
	/// CopyImageを生成する
	/// </summary>
	void CreateCopyImage();

	/// <summary>
	/// PostEffectを生成する
	/// </summary>
	void CreatePostEffect();
	


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
	const UINT kMaxNumRtvDescriptors = 256;
	UINT numRtvCPUDescriptors = 0;

	// SRV用ディスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap_ = nullptr;
	const UINT kMaxNumSrvDescriptors = 256;
	UINT numSrvCPUDescriptors = 1;
	UINT numSrvGPUDescriptors = 1;

	// DSV用ディスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeap_ = nullptr;
	const UINT kMaxNunDsvDescriptors = 1;

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


	/*----------------------------
	    オフスクリーンレンダリング
	----------------------------*/

	// オフスクリーン
	struct Offscreen
	{
		// レンダーテクスチャ
		Microsoft::WRL::ComPtr<ID3D12Resource> renderTextureResource = nullptr;

		// RTV用CPUハンドル
		D3D12_CPU_DESCRIPTOR_HANDLE renderTextureRtvCPUHnalde{};

		// SRV用ハンドル
		D3D12_CPU_DESCRIPTOR_HANDLE renderTextureSrvCPUHandle{};
		D3D12_GPU_DESCRIPTOR_HANDLE renderTextureSrvGPUHandle{};
	};

	// オフスクリーンの最大数
	const uint32_t kMaxNumOffscreen = 124;

	// 使用したオフスクリーンの数
	uint32_t useNumOffscreen_ = 0;

	// オフスクリーン
	Offscreen offscreen_[124];


	/*-----------------------
	    パイプラインステート
	-----------------------*/

	// Object3d用のPSO
	BaseObject3d* psoObject3d_[kBlendModekCountOfBlendMode] = { nullptr };
	uint32_t useObject3dBlendMode_ = kBlendModeNormal;
	Microsoft::WRL::ComPtr<IDxcBlob> object3dVertexShaderBlob_ = nullptr;
	Microsoft::WRL::ComPtr<IDxcBlob> object3dPixelShaderBlob_ = nullptr;

	// Particle用のPSO
	BaseParticle* psoParticle_[kBlendModekCountOfBlendMode] = { nullptr };
	uint32_t useParticleBlendMode_ = kBlendModeAdd;
	Microsoft::WRL::ComPtr<IDxcBlob> particleVertexShaderBlob_ = nullptr;
	Microsoft::WRL::ComPtr<IDxcBlob> particlePixelShaderBlob_ = nullptr;

	// Line3d用のPSO
	BaseLine3d* psoLine3d_[kBlendModekCountOfBlendMode] = { nullptr };
	uint32_t useLine3dBlendMode_ = kBlendModeNormal;
	Microsoft::WRL::ComPtr<IDxcBlob> line3dVertexShaderBlob_ = nullptr;
	Microsoft::WRL::ComPtr<IDxcBlob> line3dPixelShaderBlob_ = nullptr;

	// Primitive用のPSO
	BasePrimitive* psoPrimitive_[kBlendModekCountOfBlendMode] = { nullptr };
	uint32_t usePrimitiveBlendMode_ = kBlendModeNormal;
	Microsoft::WRL::ComPtr<IDxcBlob> primitiveVertexShaderBlob_ = nullptr;
	Microsoft::WRL::ComPtr<IDxcBlob> primitivePixelShaderBlob_ = nullptr;

	// CopyImage用のPSO
	BaseCopyImage* psoCopyImage_[kBlendModekCountOfBlendMode] = { nullptr };
	uint32_t useCopyImageBlendMode_ = kBlendModeNormal;
	Microsoft::WRL::ComPtr<IDxcBlob> fullscreenVertexShaderBlob_ = nullptr;
	Microsoft::WRL::ComPtr<IDxcBlob> copyImagePixelShaderBlob_ = nullptr;


	/*-------------------
	    ポストエフェクト
	-------------------*/

	/// <summary>
	/// RTV通常コピー
	/// </summary>
	void DrawCopyImage();

	/// <summary>
	/// グレースケール
	/// </summary>
	void DrawGrayScale();

	/// <summary>
	/// セピア
	/// </summary>
	void DrawSepia();

	/// <summary>
	/// ヴィネッティング
	/// </summary>
	void DrawVignetting();

	/// <summary>
	/// 平滑化
	/// </summary>
	void DrawSmoothing();

	/// <summary>
	/// ガウシアンフィルター
	/// </summary>
	void DrawGaussianFilter();

	/// <summary>
	/// アウトライン
	/// </summary>
	void DrawOutline();

	/// <summary>
	/// 高輝度抽出
	/// </summary>
	void DrawBrightnessExtraction();

	/// <summary>
	/// 隠す
	/// </summary>
	void DrawHide();


	// PostEffectピクセルシェーダ
	PostEffect* psoPostEffect_[kEfectCount] = { nullptr };
	Microsoft::WRL::ComPtr<IDxcBlob> grayScalePixelShaderBlob_ = nullptr;
	Microsoft::WRL::ComPtr<IDxcBlob> sepiaPixelShaderBlob_ = nullptr;
	Microsoft::WRL::ComPtr<IDxcBlob> vignettePixelShaderBlob_ = nullptr;
	Microsoft::WRL::ComPtr<IDxcBlob> smoothingPixelShaderBlob_ = nullptr;
	Microsoft::WRL::ComPtr<IDxcBlob> gaussianFilterPixelShaderBlob_ = nullptr;
	Microsoft::WRL::ComPtr<IDxcBlob> outlinePixelShaderBlob_ = nullptr;
	Microsoft::WRL::ComPtr<IDxcBlob> brightnessExtractionPixelShaderBlob_ = nullptr;
	Microsoft::WRL::ComPtr<IDxcBlob> hidePixelShaderBlob_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> luminanceResource_ = nullptr;

	// リソースの最大数
	const uint32_t kMaxNumResource = 1024;


	/*------------------------
	    平面で使用するリソース
	------------------------*/

	// 使用したリソースをカウントする
	uint32_t useNumResourcePlane_ = 0;

	// インデックスリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResourcePlane_ = { nullptr };

	// 頂点バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBufferResourcePlane_ = { nullptr };

	// マテリアルリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResourcePlane_[1024] = { nullptr };

	// 座標変換リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationResourcePlane_[1024] = { nullptr };

	// カメラリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> cameraResourcePlane_[1024] = { nullptr };


	/*----------------------
	    球で使用するリソース
	----------------------*/

	// 使用したリソースをカウントする
	uint32_t useNumResourceSphere_ = 0;

	//最大 分割数
	const uint32_t kSphereMaxSubdivisions = 32;

	// インデックスリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResourceSphere_ = { nullptr };

	// 頂点バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBufferResourceSphere_ = { nullptr };

	// マテリアルリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResourceSphere_[1024] = { nullptr };

	// 座標変換リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationResourceSphere_[1024] = { nullptr };

	// カメラリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> cameraResourceSphere_[1024] = { nullptr };


	/*--------------------------
	    リングで使用するリソース
	--------------------------*/

	// 使用したリソースをカウントする
	uint32_t useNumResourceRing_ = 0;

	//最大 分割数
	const uint32_t kRingMaxSubdivisions = 32;

	// インデックスリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResourceRing_ = { nullptr };

	// 頂点バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBufferResourceRing_ = { nullptr };

	// マテリアルリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResourceRing_[1024] = { nullptr };

	// 座標変換リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationResourceRing_[1024] = { nullptr };

	// カメラリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> cameraResourceRing_[1024] = { nullptr };


	/*------------------------
	    円柱で使用するリソース
	------------------------*/

	// 使用したリソースをカウントする
	uint32_t useNumResourceCylinder_ = 0;

	//最大 分割数
	const uint32_t kCylinderMaxSubdivisions = 32;

	// インデックスリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResourceCylinder_ = { nullptr };

	// 頂点バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBufferResourceCylinder_ = { nullptr };

	// マテリアルリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResourceCylinder_[1024] = { nullptr };

	// 座標変換リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationResourceCylinder_[1024] = { nullptr };

	// カメラリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> cameraResourceCylinder_[1024] = { nullptr };


	/*----------------------------
	    モデルで使用するリソース
	----------------------------*/

	// 使用したリソースをカウントする
	uint32_t useNumResourceModel_ = 0;

	// マテリアルリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResourceModel_[1024] = { nullptr };

	// 座標変換リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationResourceModel_[1024] = { nullptr };

	// カメラリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> cameraResourceModel_[1024] = { nullptr };


	/*-----------------------------
	    スプライトで使用するリソース
	-----------------------------*/

	// 使用したリソースをカウントする
	uint32_t useNumResourceSprite_ = 0;

	// インデックスリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResourceSprite_ = { nullptr };

	// 頂点バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBufferResourceSprite_ = { nullptr };

	// マテリアルリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResourceSprite_[1024] = { nullptr };

	// 座標変換リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationResourceSprite_[1024] = { nullptr };


	/*-----------------------
	    線で使用するリソース
	-----------------------*/

	// 使用したリソースをカウントする
	uint32_t useNumResourceLine_ = 0;

	// 頂点バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBufferResourceLine_[1024] = { nullptr };

	// マテリアルリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResourceLine_[1024] = { nullptr };

	// 座標変換リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationResourceLine_[1024] = { nullptr };


	/*--------------
	    平行光源
	--------------*/

	// 最大数
	const uint32_t kMaxNumDirectionalLight = 1024;

	// 使用数
	uint32_t useNumDirectionalLightCount_ = 0;

	// 使用数リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> useNumDirectionalLightResource_ = nullptr;
	UseNumDirectionalLight* useNumDirectionLightData_ = nullptr;

	// インスタンシングリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> instancingDirectionalLightResource_ = nullptr;
	DirectionalLightForGPU* directionalLightData_ = nullptr;

	// ハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE instancingDirectionalLightSrvHandleCPU_{};
	D3D12_GPU_DESCRIPTOR_HANDLE instancingDirectionalLightSrvHandleGPU_{};


	/*------------------
	    ポイントライト
	------------------*/

	// 最大数
	const uint32_t kMaxNumPointLight = 1024;

	// 使用数
	uint32_t useNumPointLightCount_ = 0;

	// 使用数リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> useNumPointLightResource_ = nullptr;
	UseNumPointLight* useNumPointLightData_ = nullptr;

	// インスタンシングリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> instancingPointLightResource_ = nullptr;
	PointLightForGPU* pointLightData_ = nullptr;

	// ハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE instancingPointLightSrvHandleCPU_{};
	D3D12_GPU_DESCRIPTOR_HANDLE instancingPointLightSrvHandleGPU_{};


	/*------------------
	    スポットライト 
	------------------*/

	// 最大数
	const uint32_t kMaxNumSpotLight = 1024;

	// 使用数
	uint32_t useNumSpotLightCount_ = 0;

	// 使用数リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> useNumSpotLightResource_ = nullptr;
	UseNumSpotLight* useNumSpotLightData_ = nullptr;

	// インスタンシングリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> instancingSpotLightResource_ = nullptr;
	SpotLightForGPU* spotLightData_ = nullptr;

	// ハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE instancingSpotLightSrvHandleCPU_{};
	D3D12_GPU_DESCRIPTOR_HANDLE instancingSpotLightSrvHandleGPU_{};


	/*--------------------------------
	    パーティクルで使用するリソース
	--------------------------------*/

	// デルタタイム
	const float kDeltaTime = 1.0f / 60.0f;

	// パーティクルの数
	const uint32_t kNumMaxInstance = 1000000;

	// マテリアル用のリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResourceParticle_ = nullptr;

	// インスタンシングリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> instancingResourcesParticle_ = nullptr;

	// ハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU_{};
	D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU_{};

	// パーティクル
	std::list<Particle> particles_ = {};

	// エミッター
	Emitter emitter_{};

	// 場
	Feild accelerationFeild_{};
};

