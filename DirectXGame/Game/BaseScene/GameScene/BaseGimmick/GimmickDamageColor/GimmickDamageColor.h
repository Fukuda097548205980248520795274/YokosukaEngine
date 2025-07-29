#pragma once
#include "../BaseGimmick.h"
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"

class GimmickDamageColor : public BaseGimmick
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(WorldTransform* worldTransform, float parameterVelocity) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera3d"></param>
	/// <param name="modelHandle"></param>
	/// <param name="color"></param>
	void Draw(const YokosukaEngine* engine, const Camera3D* camera3d, uint32_t modelHandle, const Vector3& color);

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();


private:

};

