#pragma once
#include "../BaseStageObject.h"

class StageObjectSaturnPlanet : public BaseStageObject
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera3d"></param>
	/// <param name="modelHandleStore"></param>
	/// <param name="gameFrame"></param>
	/// <param name="time"></param>
	/// <param name="position"></param>
	/// <param name="rotation"></param>
	void Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const ModelHandleStore* modelHandleStore, const float* gameFrame,
		float time, const Vector3& position, const Vector3& rotation) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;


private:

	// 惑星のモデルハンドル
	uint32_t mhSaturnPlanet_ = 0;
};

