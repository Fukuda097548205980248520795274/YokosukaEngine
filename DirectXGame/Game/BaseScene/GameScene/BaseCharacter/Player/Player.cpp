#include "Player.h"
#include "../Enemy/Enemy.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
/// <param name="position"></param>
void Player::Initialize(const YokosukaEngine* engine, const ModelHandleStore* modelHandleStore, const TextureHandleStore* textureHandleStore,
	const Camera3D* camera3d, const Vector3& position)
{
	// 基底クラスを初期化する
	BaseCharacter::Initialize(engine,modelHandleStore, textureHandleStore, camera3d, position);

	// 本体のモデル
	modelBody_ = std::make_unique<Model>();
	modelBody_->Initialize(engine_, camera3d_, modelHandleStore_->GetModelHandle(ModelHandleStore::kSuzanne));
	modelBody_->worldTransform_->SetParent(worldTransform_.get());

	worldTransform_->rotation_.y = std::numbers::pi_v<float> / 2.0f;


	// プレイヤーの視覚の生成と初期化
	visibility_ = std::make_unique<PlayerVisibility>();
	visibility_->Initialize(worldTransform_.get());

	// ゲームパッドの移動操作の生成と初期化
	moveActionGamepad_ = std::make_unique<MoveActionGamepad>();
	moveActionGamepad_->Initialize(engine_ , worldTransform_.get());

	// ゲームパッドの拡縮操作
	scaleActionGamepad_ = std::make_unique<ScaleActionGamepad>();
	scaleActionGamepad_->Initialize(engine_ , worldTransform_.get());

}

/// <summary>
/// 更新処理
/// </summary>
void Player::Update()
{
	// ゲームパッドの拡縮操作
	scaleActionGamepad_->SetTargets(targets_);
	scaleActionGamepad_->Update();

	// 拡縮操作中は移動操作ができない
	if (scaleActionGamepad_->IsOperation() == false)
	{
		// ゲームパッドの移動操作
		moveActionGamepad_->Update();
	}

	// 向きの値を取得する
	direction_ = scaleActionGamepad_->GetDirection();


	// 本体モデルの更新
	modelBody_->Update();

	// プレイヤーの視覚の更新
	visibility_->Update();

	// 基底クラスの更新
	BaseCharacter::Update();

	// ターゲットリストをクリアする
	targets_.clear();
}

/// <summary>
/// 描画処理
/// </summary>
void Player::Draw()
{
	// 本体モデルの描画
	modelBody_->Draw();
}


/// <summary>
/// 衝突判定応答
/// </summary>
/// <param name="enemy"></param>
void Player::OnCollision(Enemy* enemy)
{
	targets_.push_back(enemy);
}