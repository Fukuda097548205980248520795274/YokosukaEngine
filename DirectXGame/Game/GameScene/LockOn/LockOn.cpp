#include "LockOn.h"


/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
void LockOn::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const Camera2D* camera2d)
{
	// nullptrチェック
	assert(engine);
	assert(camera3d);
	assert(camera2d);

	// 引数を受け取る
	engine_ = engine;
	camera2d_ = camera2d;
	camera3d_ = camera3d;


	// UVトランスフォームの生成と初期化
	uvTransform_ = std::make_unique<UvTransform>();
	uvTransform_->Initialize();

	// テクスチャを読み込む
	textureHandle_ = engine_->LoadTexture("./Resources/Textures/reticle.png");
}

/// <summary>
/// 更新処理
/// </summary>
void LockOn::Update(Player* player,std::list<Enemy*>& enemies)
{
	// ロックオン対象リスト
	std::list<std::pair<float, Enemy*>> targets;

	isTarget_ = false;


	// プレイヤーのワールド座標
	Vector3 playerWorldPosition = player->GetWorldPosition();

	// ビュー座標に変換する
	Vector3 playerView = Transform(playerWorldPosition, camera3d_->viewMatrix_);


	// ロックオン判定処理
	for (Enemy* enemy : enemies)
	{
		// 敵のワールド座標を取得
		Vector3 enemyWorldPosition = enemy->GetWorldPosition();

		Vector3 enemyView = Transform(enemyWorldPosition, camera3d_->viewMatrix_);

		if (enemyView.z <= playerView.z)
			continue;

		// ワールド座標からスクリーン座標に変換する
		Vector3 enemyScreenPosition = Project(enemyWorldPosition, 0.0f, 0.0f, camera2d_->screenWidth_, camera2d_->screenHeight_, camera3d_->viewMatrix_, camera3d_->projectionMatrix_);

		// Vector2に格納
		Vector2 positionScreenV2(enemyScreenPosition.x, enemyScreenPosition.y);

		// スプライトの中心からの距離
		float distance = Length(player->GetPosition2DReticle() - positionScreenV2);

		// 2Dレティクルからのスクリーン距離が規定範囲内ならば
		if (distance <= 64.0f)
		{
			targets.emplace_back(std::make_pair(distance, enemy));

			isTarget_ = true;
		}
	}

	// 対象を絞り込んで座標設定する
	if (!targets.empty())
	{
		// 距離で昇順ソート
		targets.sort();

		// 距離が一番小さい相手をロックオン対象とする
		target_ = targets.front().second;

		// 敵のワールド座標を取得
		Vector3 enemyWorldPosition = target_->GetWorldPosition();

		// ワールド座標からスクリーン座標に変換する
		Vector3 enemyScreenPosition = Project(enemyWorldPosition, 0.0f, 0.0f, camera2d_->screenWidth_, camera2d_->screenHeight_, camera3d_->viewMatrix_, camera3d_->projectionMatrix_);

		// Vector2に格納
		targetPosisition_ = Vector2(enemyScreenPosition.x, enemyScreenPosition.y);
	}

	// トランスフォームを更新する
	uvTransform_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void LockOn::Draw()
{
	if (isTarget_ == false)
		return;

	engine_->DrawSprite(targetPosisition_.x - 32.0f, targetPosisition_.y - 32.0f,
		targetPosisition_.x + 32.0f, targetPosisition_.y - 32.0f,
		targetPosisition_.x - 32.0f, targetPosisition_.y + 32.0f,
		targetPosisition_.x + 32.0f, targetPosisition_.y + 32.0f,
		uvTransform_.get(), camera2d_, textureHandle_, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
}