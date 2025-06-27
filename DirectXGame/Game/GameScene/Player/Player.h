#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"
#include "../PlayerBullet/PlayerBullet.h"
#include "../Collider/Collider.h"

// 前方宣言
class GameScene;
class LockOn;

class Player : public Collider
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera3d"></param>
	/// <param name="directionalLight"></param>
	void Initialize(const YokosukaEngine* engine, const Camera3D* camera3d,const Camera2D* camera2d, const DirectionalLight* directionalLight , const Vector3& posision);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 弾を発射する
	/// </summary>
	void BulletShot();

	/// <summary>
	/// ワールド座標のGetter
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition() const override;

	/// <summary>
	/// 3Dレティクルのワールド座標のGetter
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosision3DReticle() const;

	/// <summary>
	/// 2Dレティクルの座標のGetter
	/// </summary>
	/// <returns></returns>
	Vector2 GetPosition2DReticle() const { return Vector2(worldTransform2DReticle_->translation_.x, worldTransform2DReticle_->translation_.y); }

	/// <summary>
	/// 衝突コールバック関数
	/// </summary>
	void OnCollision() override;

	/// <summary>
	/// 親ワールドトランスフォームのSetter
	/// </summary>
	/// <param name="worldTransform"></param>
	void SetWorldTransformParent(const WorldTransform* worldTransform) { worldTransform_->SetParent(worldTransform); }

	/// <summary>
	/// ゲームシーンのインスタンスのSetter
	/// </summary>
	/// <param name="gameScene"></param>
	void SetGameSceneInstance(GameScene* gameScene) { gameScene_ = gameScene; }

	/// <summary>
	/// ロックオンのインスタンスのSetter
	/// </summary>
	/// <param name="lockOn"></param>
	void SetLockOnInstance(LockOn* lockOn) { lockOn_ = lockOn; }


private:

	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// 3Dカメラ
	const Camera3D* camera3d_ = nullptr;

	// 2Dカメラ
	const Camera2D* camera2d_ = nullptr;

	// 平行光源
	const DirectionalLight* directionalLight_ = nullptr;

	// ゲームシーンのポインタ
	GameScene* gameScene_ = nullptr;

	// ロックオンのポインタ
	LockOn* lockOn_ = nullptr;


	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;

	// UVトランスフォーム
	std::unique_ptr<UvTransform> uvTransform_ = nullptr;

	// モデルハンドル
	uint32_t modelHandle_ = 0;

	// ポイントライト
	std::unique_ptr<PointLight> pointLight_ = nullptr;

	// スポットライト
	std::unique_ptr<SpotLight> spotLight_ = nullptr;


	// 3Dレティクル用ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform3DReticle_;

	// 3Dレティクル用モデルハンドル
	uint32_t modelHandle3DReticle_ = 0;


	// 2Dレティクル用ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform2DReticle_;

	// 2Dレティクル用UVトランスフォーム
	std::unique_ptr<UvTransform> uvTransform2DReticle_;

	// 2Dレティクル用のテクスチャ
	uint32_t textureHandle2DReticle_ = 0;
};

