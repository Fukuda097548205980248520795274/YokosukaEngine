#pragma once
#include "../BaseCharacter.h"
#include "../../../../Model/Model.h"

#include "PlayerVisibility/PlayerVisibility.h"
#include "MoveActionGamepad/MoveActionGamepad.h"
#include "ScaleActionGamepad/ScaleActionGamepad.h"

// 前方宣言
class Enemy;

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
	/// 向きのGetter
	/// </summary>
	/// <returns></returns>
	Vector3 GetDirection()const { return direction_; }

	/// <summary>
	/// 衝突判定応答
	/// </summary>
	/// <param name="enemy"></param>
	void OnCollision(Enemy* enemy);


private:

	// 本体のモデル
	std::unique_ptr<Model> modelBody_ = nullptr;

	// プレイヤーの視覚
	std::unique_ptr<PlayerVisibility> visibility_ = nullptr;

	// ゲームパッドの移動操作
	std::unique_ptr<MoveActionGamepad> moveActionGamepad_ = nullptr;

	// ゲームパッドの拡縮操作
	std::unique_ptr<ScaleActionGamepad> scaleActionGamepad_ = nullptr;


	// 向き
	Vector3 direction_ = { 1.0f , 0.0f , 0.0f };

	// ターゲットリスト
	std::list<Enemy*> targets_;
};

