#include "PlayerBulletWeek.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
/// <param name="position"></param>
void PlayerBulletWeek::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const ModelHandleStore* modelHandleStore,
	const Vector3& position)
{
	// 基底クラス初期化
	BasePlayerBullet::Initialize(engine, camera3d, modelHandleStore , position);


	// 本体の生成と初期化
	bulletWorldTransform_ = std::make_unique<WorldTransform>();
	bulletWorldTransform_->Initialize();
	bulletWorldTransform_->SetParent(worldTransform_.get());

	// 弾のポイントライトの生成と初期化
	bulletPointLight_ = std::make_unique<PointLight>();
	bulletPointLight_->Initialize();
	bulletPointLight_->color_ = Vector4(0.2f, 1.0f, 0.2f, 1.0f);
	bulletPointLight_->radius_ = 16.0f;


	/*--------------
	    弾の設定
	--------------*/

	// モデルハンドル
	bulletModelHandle_ = modelHandleStore_->GetModelHandle(ModelHandleStore::kPlayerBulletWeek)[0];

	// 発射時間
	shotTime_ = 0.75f;

	// 攻撃力
	power_ = 2;

	// 発射速度
	speed = 5.0f;
}

/// <summary>
/// 更新処理
/// </summary>
void PlayerBulletWeek::Update()
{
	// 基底クラス更新
	BasePlayerBullet::Update();

	// 本体の更新
	bulletWorldTransform_->UpdateWorldMatrix();

	// 弾の位置にポイントライトを配置する
	bulletPointLight_->position_ = GetBulletWorldTransform();
}

/// <summary>
/// 描画処理
/// </summary>
void PlayerBulletWeek::Draw()
{
	// ポイントライトを設置する
	engine_->SetPointLight(bulletPointLight_.get());

	// 本体を描画する
	engine_->DrawModel(bulletWorldTransform_.get(), camera3d_, bulletModelHandle_, Vector4(0.2f, 1.0f, 0.2f, 1.0f), false);
}

/// <summary>
/// 弾本体のワールド座標のGetter
/// </summary>
/// <returns></returns>
Vector3 PlayerBulletWeek::GetBulletWorldTransform()
{
	// ワールド座標
	Vector3 worldPosition;

	worldPosition.x = bulletWorldTransform_->worldMatrix_.m[3][0];
	worldPosition.y = bulletWorldTransform_->worldMatrix_.m[3][1];
	worldPosition.z = bulletWorldTransform_->worldMatrix_.m[3][2];

	return worldPosition;
}