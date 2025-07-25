#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"

class SphereModel
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera3d"></param>
	void Initialize(const YokosukaEngine* engine, const Camera3D* camera3d);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(bool enableLighting, bool enableHalfLambert);


private:

	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// カメラ
	const Camera3D* camera3d_ = nullptr;


	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;

	// UVトランスフォーム
	std::unique_ptr<UvTransform> uvTransform_ = nullptr;

	// 色
	Vector4 color_ = { 1.0f,1.0f,1.0f,1.0f };

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;

	// 分割数
	int segment = 16;
	int ring = 8;
};

