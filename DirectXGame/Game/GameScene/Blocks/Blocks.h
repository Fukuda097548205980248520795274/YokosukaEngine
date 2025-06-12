#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"
#include "../MapChipField/MapChipField.h"

class Blocks
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Blocks();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="mapChipField"></param>
	/// <param name="camera3d"></param>
	/// <param name="directionalLight"></param>
	void Initialize(const YokosukaEngine* engine, const MapChipField* mapChipField, const Camera3D* camera3d , const DirectionalLight* directionalLight);

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

	// マップチップフィールド
	const MapChipField* mapChipField_ = nullptr;

	// 3Dカメラ
	const Camera3D* camera3d_ = nullptr;

	// 平行光源
	const DirectionalLight* directionalLight_ = nullptr;


	// ブロックのワールドトランスフォーム
	std::vector<std::vector<WorldTransform*>> worldTransforms_;

	// ブロックのUVトランスフォーム
	std::unique_ptr<UvTransform> uvTransform_ = nullptr;

	// モデルハンドル
	uint32_t modelHandle_ = 0;

	// ポイントライト
	std::unique_ptr<PointLight> pointLight_ = nullptr;

	// スポットライト
	std::unique_ptr<SpotLight> spotLight_ = nullptr;

	// 行の数
	uint32_t numBlockRow = 0;

	// 列の数
	uint32_t numBlockColumn = 0;

	// ブロック1つのサイズ
	const float kBlockSize = 2.0f;
};

