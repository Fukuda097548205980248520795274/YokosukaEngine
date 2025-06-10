#pragma once
#include "../YokosukaEngine/Include/YokosukaEngine.h"
#include "GameCamera/GameCamera.h"

class Game
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine">エンジン</param>
	void Initialize(const YokosukaEngine* engine);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();


private:

	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// デバッグカメラ有効化
	bool isDebugCameraActive_ = false;

	// 3Dカメラ
	std::unique_ptr<Camera3D> camera3d_ = nullptr;

	// 2Dカメラ
	std::unique_ptr<Camera2D> camera2d_ = nullptr;

	// ゲームカメラ
	std::unique_ptr<GameCamera> gameCamera_ = nullptr;

	// デバッグカメラの表示
#ifdef _DEBUG

	std::unique_ptr<WorldTransform> debugCameraWorldTransform_ = nullptr;
	std::unique_ptr<UvTransform> debugCameraUvTransform_ = nullptr;
	uint32_t debugCameraModelHandle_ = 0;

#endif



	/*-------------------
		ゲームで使用する
	-------------------*/


	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;

	// UVトランスフォーム
	std::unique_ptr<UvTransform> uvTransform_ = nullptr;

	// モデルハンドル
	uint32_t modelHandle_ = 0;

	// 平行光源
	std::unique_ptr<DirectionalLight> directionalLight_ = nullptr;

	// ポイントライト
	std::unique_ptr<PointLight> pointLight_ = nullptr;

	// スポットライト
	std::unique_ptr<SpotLight> spotLight_ = nullptr;
};