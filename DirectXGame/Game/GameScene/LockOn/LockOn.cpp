#include "LockOn.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera2d"></param>
void LockOn::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const Camera2D* camera2d)
{
	// nullptrチェック
	assert(engine);
	assert(camera2d);
	assert(camera3d);

	// 引数を受け取る
	engine_ = engine;
	camera2d_ = camera2d;
	camera3d_ = camera3d;

	// ワールドトランスフォームの生成と初期化
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
	worldTransform_->scale_ = { 32.0f , 32.0f , 0.0f };

	// Uvトランスフォームの生成と初期化
	uvTransform_ = std::make_unique<UvTransform>();
	uvTransform_->Initialize();

	// テクスチャを読み込む
	textureHandle_ = engine_->LoadTexture("./Resources/Textures/reticle.png");
}

/// <summary>
/// 更新処理
/// </summary>
void LockOn::Update(const std::list<std::unique_ptr<Enemy>>& enemies)
{
	// ロックオン状態
	if (target_)
	{
		// ボタンを押したらロックオンを外す
		if (engine_->GetGamepadButtonTrigger(0, XINPUT_GAMEPAD_RIGHT_SHOULDER))
		{
			target_ = nullptr;
		}
		else if (LockOnRange())
		{
			target_ = nullptr;
		}
	}
	else
	{
		// ボタンを押したらロックオンする
		if (engine_->GetGamepadButtonTrigger(0, XINPUT_GAMEPAD_RIGHT_SHOULDER))
		{
			Search(enemies);
		}
	}

	// ロックオン状態
	if (target_)
	{
		// オフセット
		Vector3 offset = { 0.0f , 0.3f , 0.0f };

		// 敵のロックオン座標を取得する
		Vector3 worldPosition = target_->GetWorldPosition() + offset;

		// ビューポート変換行列
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0.0f, 0.0f, static_cast<float>(engine_->GetScreenWidth()), static_cast<float>(engine_->GetScreenHeight()),
			0.0f, 1.0f);

		// スクリーン座標に変換する
		Vector3 screenPosition = Transform(Transform(worldPosition, camera3d_->viewMatrix_ * camera3d_->projectionMatrix_) , viewportMatrix);

		// Vector2に格納する
		Vector2 screenV2 = { screenPosition.x , screenPosition.y };

		// 移動する
		worldTransform_->translation_.x = screenV2.x;
		worldTransform_->translation_.y = screenV2.y;
	}

	// トランスフォームの更新
	worldTransform_->UpdateWorldMatrix();
	uvTransform_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void LockOn::Draw()
{
	if (target_)
	{
		// スプライトを描画する
		engine_->DrawSprite(worldTransform_.get(), uvTransform_.get(), camera2d_, textureHandle_, Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	}
}



/// <summary>
/// 最も近い敵を探索する
/// </summary>
/// <param name="enemies"></param>
void LockOn::Search(const std::list<std::unique_ptr<Enemy>>& enemies)
{
	// 目標
	std::list<std::pair<float, const Enemy*>> targets;

	// 全ての敵に対して順番にロックオン
	for (const std::unique_ptr<Enemy>& enemy : enemies)
	{
		// 敵のロックオン座標を取得
		Vector3 worldPosition = enemy->GetCenterPosition();

		// ビュー座標に変換する
		Vector3 viewPosition = Transform(worldPosition, camera3d_->viewMatrix_);


		// 距離条件チェック
		if (minDistance_ <= viewPosition.z && viewPosition.z <= maxDistance_)
		{
			// カメラ前方との角度を計算
			float arcTangent = std::atan2(std::sqrt(viewPosition.x * viewPosition.x + viewPosition.y * viewPosition.y), viewPosition.z);

			// 角度条件チェック（コーンに収まっているかどうか）
			if (std::fabs(arcTangent) <= angleRange_)
			{
				targets.emplace_back(std::make_pair(viewPosition.z, enemy.get()));
			}
		}
	}



	// ロックオン対象をリセット
	target_ = nullptr;

	if (!targets.empty())
	{
		// 距離でソード
		targets.sort([](auto& pair1, auto& pari2) {return pair1.first < pari2.first; });

		// ソートの結果いちばん近い敵をロックオン対象とする
		target_ = targets.front().second;
	}
}


/// <summary>
/// ロックオンしている範囲
/// </summary>
bool LockOn::LockOnRange()
{
	// 敵のロックオン座標を取得
	Vector3 worldPosition = target_->GetCenterPosition();

	// ビュー座標に変換する
	Vector3 viewPosition = Transform(worldPosition, camera3d_->viewMatrix_);


	// 距離条件チェック
	if (minDistance_ <= viewPosition.z && viewPosition.z <= maxDistance_)
	{
		// カメラ前方との角度を計算
		float arcTangent = std::atan2(std::sqrt(viewPosition.x * viewPosition.x + viewPosition.y * viewPosition.y), viewPosition.z);

		// 角度条件チェック（コーンに収まっているかどうか）
		if (std::fabs(arcTangent) <= angleRange_)
		{
			return false;
		}
	}

	// 範囲外
	return true;
}