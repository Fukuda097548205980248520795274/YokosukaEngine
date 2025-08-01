#pragma once
#include "../../YokosukaEngine/Include/YokosukaEngine.h"
#include "Plane/Plane.h"
#include "Sprite/Sprite.h"
#include "SphereModel/SphereModel.h"
#include "Suzanne/Suzanne.h"
#include "StanfordBunny/StanfordBunny.h"
#include "UtahTeapot/UtahTeapot.h"
#include "MultiMesh/MultiMesh.h"
#include "MultiMaterial/MultiMaterial.h"

class GameScene : public Scene
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine">エンジン</param>
	void Initialize(const YokosukaEngine* engine) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw()override;


private:


	// 平行光源
	std::unique_ptr<DirectionalLight> directionalLight_ = nullptr;

	// 明るさの波
	float cosCave = 0.0f;

	
	// 平面
	std::unique_ptr<Plane> plane_ = nullptr;

	// スプライト
	std::unique_ptr<Sprite> sprite_ = nullptr;

	// 球
	std::unique_ptr<SphereModel> sphere_ = nullptr;

	// スザンヌ
	std::unique_ptr<Suzanne> suzanne_ = nullptr;

	// スタンフォードバニー
	std::unique_ptr<StanfordBunny> stanfordBunny_ = nullptr;

	// ユタティーポット
	std::unique_ptr<UtahTeapot> utahTeapot_ = nullptr;

	// マルチメッシュ
	std::unique_ptr<MultiMesh> multiMesh_ = nullptr;

	// マルチマテリアル
	std::unique_ptr<MultiMaterial> multiMaterial_ = nullptr;


	// ライティング有効化
	bool enableLighting_ = true;

	// ハーフランバート有効化
	bool enableHalfLambert_ = true;


	// サウンドハンドル
	uint32_t soundHandle_ = 0;

	// プレイハンドル
	uint32_t playHandle_ = 0;


	// ライティング切り替えタイマー
	float lightingSwithingTimer_ = 0.0f;
	const float kLightingSwichingTime = 6.0f;
};
