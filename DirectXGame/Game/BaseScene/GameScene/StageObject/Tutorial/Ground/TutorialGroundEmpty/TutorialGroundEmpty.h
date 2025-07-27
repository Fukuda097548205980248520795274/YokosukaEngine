#pragma once
#include "../../../StageObject.h"

class TutorialGroundEmpty : public StageObject
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera3d"></param>
	void Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const Vector3& position) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;


private:


	// 地面のモデル
	uint32_t groundModelHandle_ = 0;

	// 地面のワールドトランスフォーム
	std::unique_ptr<WorldTransform> groundWorldTrasnform_ = nullptr;

	// 地面のUVトランスフォーム
	std::unique_ptr<UvTransform> groundUvTransform_ = nullptr;
};

