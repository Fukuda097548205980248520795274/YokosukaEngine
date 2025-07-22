#include "Hammer.h"
#include "../../BaseCharacter/Enemy/Enemy.h"
#include "../../GameScene.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
void Hammer::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, WorldTransform* parent)
{
	// コライダーの初期化
	Collider::Initialize(engine, camera3d);
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kPlayerHammer));

	// ワールドトランスフォームの生成と初期化
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
	worldTransform_->SetParent(parent);
	worldTransform_->translation_.y = 1.0f;

	// UVトランスフォームの生成と初期化
	uvTransform_ = std::make_unique<UvTransform>();
	uvTransform_->Initialize();

	// モデルハンドル
	modelHandle_ = engine_->LoadModelData("./Resources/Models/Player/weapon", "weapon.obj");

	// 接触履歴の生成と初期化
	collisionRecord_ = std::make_unique<CollisionRecord>();
}

/// <summary>
/// 更新処理
/// </summary>
void Hammer::Update()
{
	// トランスフォームの更新
	worldTransform_->UpdateWorldMatrix();
	uvTransform_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void Hammer::Draw()
{
	// モデルの描画
	engine_->DrawModel(worldTransform_.get(), uvTransform_.get(), camera3d_, modelHandle_, Vector4(0.2f, 0.2f, 0.2f, 1.0f), false);
}

/// <summary>
/// 衝突判定応答
/// </summary>
void Hammer::OnCollision([[maybe_unused]] Collider* other)
{
	// 攻撃中のみ判定する
	if (isAttack_ == false)
		return;

	uint32_t typeID = other->GetTypeID();

	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kEnemy))
	{
		Enemy* enemy = static_cast<Enemy*>(other);
		Vector3 worldPosition = enemy->GetWorldPosition();

		// 履歴
		uint32_t serialNumber = enemy->GetSerialNumber();

		// 接触履歴があれば何もせずに抜ける
		if (collisionRecord_->RecordCheck(serialNumber))
			return;

		// 接触履歴に登録
		collisionRecord_->AddRecord(serialNumber);

		// ヒットエフェクトの生成と初期化
		HitEffect* hitEffect;
		hitEffect = new HitEffect();
		hitEffect->Initialize(engine_, camera3d_, worldPosition);
		gameScene_->EmitHitEffect(hitEffect);
	}
}

/// <summary>
/// 中心座標のGetter
/// </summary>
/// <returns></returns>
Vector3 Hammer::GetCenterPosition() const
{
	// オフセット
	Vector3 offset = { 0.0f , 3.0f , 0.0f };

	// ワールド座標に変換
	Vector3 worldPosition = Transform(offset, worldTransform_->worldMatrix_);
	return worldPosition;
}

/// <summary>
/// 接触履歴の抹消
/// </summary>
void Hammer::CollisionRecordClear()
{
	collisionRecord_->RecordClear();
}