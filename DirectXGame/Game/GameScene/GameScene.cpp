#include "GameScene.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine">エンジン</param>
void GameScene::Initialize(const YokosukaEngine* engine)
{
	// nullptrチェック
	assert(engine);

	// Scene更新
	Scene::Initialize(engine);

	// プレイヤーの生成と初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(engine_, camera3d_.get(), Vector3(0.0f, 8.0f, 0.0f));

	// 惑星の生成と初期化
	planet_ = std::make_unique<Planet>();
	planet_->Initialize(engine_ , camera3d_.get() , 8.0f);

	// 重力場の生成と初期化
	gravitationalField_ = std::make_unique<GravitationalField>();
	gravitationalField_->Initialize(engine_ , camera3d_.get() , 32.0f);
}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update()
{
	// Scene更新
	Scene::Update();

	// プレイヤーの更新
	player_->Update();

	// 惑星の更新
	planet_->Update();

	// 重力場の更新
	gravitationalField_->Update();


	// 全ての当たり判定を行う
	CheckAllCollisions();
}

/// <summary>
/// 描画処理
/// </summary>
void GameScene::Draw()
{
	// Scene描画
	Scene::Draw();

	// プレイヤーの描画
	player_->Draw();

	// 惑星の描画
	planet_->Draw();

	// 重力場の描画
	gravitationalField_->Draw();
}

/// <summary>
/// 全ての当たり判定を行う
/// </summary>
void GameScene::CheckAllCollisions()
{
	// 球
	Sphere sphere1,sphere2;


#pragma region // プレイヤー と 球

	sphere1 = player_->GetCollisionSphere();
	sphere2 = planet_->GetCollisionSphere();

	if (IsCollision(sphere1, sphere2))
	{
		player_->OnCollision(planet_.get());
	}

#pragma endregion


#pragma region // プレイヤー と 重力場

	sphere1 = player_->GetCollisionSphere();
	sphere2 = gravitationalField_->GetCollisionSphere();

	if (IsCollision(sphere1, sphere2))
	{
		player_->OnCollision(gravitationalField_.get());
	}

#pragma endregion
}