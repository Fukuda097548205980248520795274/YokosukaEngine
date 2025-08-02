#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"

class CenterAxis
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera3d"></param>
	void Initliaze(const YokosukaEngine* engine , const Camera3D* camera3d);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// ワールドトランスフォームのGetter
	/// </summary>
	/// <returns></returns>
	WorldTransform* GetWorldTransform() const { return worldTransform_.get(); }

	/// <summary>
	/// ワールド座標のGetter
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition()const;

	/// <summary>
	/// ゲームタイマーのSetter
	/// </summary>
	/// <param name="gameTimer"></param>
	void SetGameTimer(const float* gameTimer) { gameTimer_ = gameTimer; }


private:

	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// カメラ
	const Camera3D* camera3d_ = nullptr;

	// ゲームタイム
	const float* gameTimer_ = nullptr;


	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;

	// UVトランスフォーム
	std::unique_ptr<UvTransform> uvTransform_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;



	Vector3 controlPointTable[7] =
	{
		{0.0f , 20.0f , 0.0f},
		{0.0f , 20.0f , 450.0f},
		{100.0f , 20.0f , 573.0f},
		{200.0f , 20.0f , 660.0f},
		{250.0f , 20.0f , 750.0f},
		{230.0f , 20.0f , 800.0f},
		{100.0f , 20.0f , 900.0f}

	};

	float t_ = 0.0f;

	std::vector<Vector3> controlPoint_;
};

