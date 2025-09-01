#pragma once
#include "../../YokosukaEngine/Include/YokosukaEngine.h"

class Model
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera2d"></param>
	/// <param name="textureHandle"></param>
	void Initialize(const YokosukaEngine* engine, const Camera3D* camera2d, uint32_t modelHandle);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();


	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;

	// UVトランスフォーム
	std::unique_ptr<UvTransform> uvTransform_ = nullptr;

	// 色
	Vector4 color_ = { 1.0f , 1.0f , 1.0f , 1.0f };

	// ライティング有効
	bool enableLighting_ = true;



private:


	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// 3Dカメラ
	const Camera3D* camera3d_ = nullptr;


	// モデル
	uint32_t modelHandle_ = 0;
};

