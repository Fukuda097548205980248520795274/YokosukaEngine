#pragma once
#include "../Collider.h"

// 前方宣言
class GameScene;

class Hammer final : public Collider
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera3d"></param>
	void Initialize(const YokosukaEngine* engine, const Camera3D* camera3d , WorldTransform* parent);

	/// <summary>
	/// ゲームシーンのSetter
	/// </summary>
	/// <param name="gameScene"></param>
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; };

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// ワールドトランスフォームの回転のGetter
	/// </summary>
	/// <returns></returns>
	const Vector3& GetRotation() { return worldTransform_->rotation_; }

	/// <summary>
	/// ワールドトランスフォームの回転のSetter
	/// </summary>
	/// <param name="rotation"></param>
	void SetRotation(const Vector3& rotation) { worldTransform_->rotation_ = rotation; }

	/// <summary>
	/// 衝突判定応答
	/// </summary>
	void OnCollision([[maybe_unused]] Collider* other) override;

	/// <summary>
	/// 中心座標のGetter
	/// </summary>
	/// <returns></returns>
	Vector3 GetCenterPosition() const override;

	/// <summary>
	/// 攻撃フラグのSetter
	/// </summary>
	/// <param name="isAttack"></param>
	void SetIsAttack(bool isAttack) { isAttack_ = isAttack; }


private:


	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;

	// UVトランスフォーム
	std::unique_ptr<UvTransform> uvTransform_ = nullptr;

	// モデルハンドル
	uint32_t modelHandle_ = 0;

	// 攻撃フラグ
	bool isAttack_ = false;

	// ゲームシーン
	GameScene* gameScene_ = nullptr;
};

