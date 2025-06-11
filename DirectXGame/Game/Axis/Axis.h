#pragma once
#include "../../YokosukaEngine/Include/YokosukaEngine.h"

class Axis
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const YokosukaEngine* engine);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(struct Vector3& cameraRotation);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();


private:

	// エンジン
	const YokosukaEngine* engine_ = nullptr;


	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;

	// UVトランスフォーム
	std::unique_ptr<UvTransform> uvTransform_ = nullptr;

	// 3Dカメラ
	std::unique_ptr<Camera3D> camera3d_ = nullptr;

	// モデルハンドル
	uint32_t modelHandle_ = 0;
};

