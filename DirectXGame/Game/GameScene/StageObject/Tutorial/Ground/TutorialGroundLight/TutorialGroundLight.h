#pragma once
#include "../../../StageObject.h"

class TutorialGroundLight : public StageObject
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

	// ライトのワールドトランスフォーム
	std::unique_ptr<WorldTransform> lightWorldTrasnform_ = nullptr;

	// ライトのUVトランスフォーム
	std::unique_ptr<UvTransform> lightUvTransform_ = nullptr;

	// ライトのテクスチャ
	uint32_t lightTextureHandle_ = 0;
};

