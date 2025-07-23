#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"

class MultiMesh
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

	// UVトランスフォームの可変子配列
	std::vector<std::unique_ptr<UvTransform>> uvTransforms_;

	// モデルハンドル
	uint32_t modelHandle_ = 0;
};

