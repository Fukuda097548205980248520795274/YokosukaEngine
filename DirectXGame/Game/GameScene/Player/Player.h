#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"
#include "../PlayerBullet/PlayerBullet.h"
#include "../Collider/Collider.h"

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
	void Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const DirectionalLight* directionalLight , const Vector3& posision);

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
	/// 衝突コールバック関数
	/// </summary>
	void OnCollision() override;

	/// <summary>
	/// 弾のリストのGetter
	/// </summary>
	/// <returns></returns>
	const std::list<PlayerBullet*> GetBulletsInstance() const { return bullets_; }

	/// <summary>
	/// 親ワールドトランスフォームのSetter
	/// </summary>
	/// <param name="worldTransform"></param>
	void SetWorldTransformParent(const WorldTransform* worldTransform) { worldTransform_->SetParent(worldTransform); }


private:

	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// 3Dカメラ
	const Camera3D* camera3d_ = nullptr;

	// 平行光源
	const DirectionalLight* directionalLight_ = nullptr;


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


	// 弾
	std::list<PlayerBullet*> bullets_;
};

