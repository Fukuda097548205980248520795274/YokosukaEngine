#pragma once
#include "../BaseCharacter.h"
#include "../../../../Model/Model.h"

class Enemy : public BaseCharacter
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera3d"></param>
	/// <param name="position"></param>
	void Initialize(const YokosukaEngine* engine, const ModelHandleStore* modelHandleStore, const TextureHandleStore* textureHandleStore,
		const Camera3D* camera3d, const Vector3& position) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 拡縮のSetter
	/// </summary>
	/// <param name="scale"></param>
	void SetScale(const Vector3& scale) { worldTransform_->scale_ = scale; }

	/// <summary>
	/// 拡縮のGetter
	/// </summary>
	/// <returns></returns>
	Vector3 GetScale()const { return worldTransform_->scale_; }


private:

	// 本体のモデル
	std::unique_ptr<Model> modelBody_ = nullptr;
};

