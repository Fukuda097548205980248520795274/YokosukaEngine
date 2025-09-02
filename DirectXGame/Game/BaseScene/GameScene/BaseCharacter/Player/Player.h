#pragma once
#include "../BaseCharacter.h"
#include "../../../../Model/Model.h"

#include "PlayerVisibility/PlayerVisibility.h"
#include "MoveActionGamepad/MoveActionGamepad.h"

class Player : public BaseCharacter
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
	/// 後ろ向きの平面のGetter
	/// </summary>
	/// <returns></returns>
	Plane GetBackPlane()const { return visibility_->GetBackPlane(); }

	/// <summary>
	/// 上側の平面のGetter
	/// </summary>
	/// <returns></returns>
	Plane GetTopPlane() const { return visibility_->GetTopPlane(); }

	/// <summary>
	/// 下側の平面のGetter
	/// </summary>
	/// <returns></returns>
	Plane GetBottomPlane()const { return visibility_->GetBottomPlane(); }


private:

	// 本体のモデル
	std::unique_ptr<Model> modelBody_ = nullptr;

	// プレイヤーの視覚
	std::unique_ptr<PlayerVisibility> visibility_ = nullptr;

	// ゲームパッドの移動操作
	std::unique_ptr<MoveActionGamepad> moveActionGamepad_ = nullptr;
};

