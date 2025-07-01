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


	// 星を生成する
	SetPlanet(Vector3(0.0f, 0.0f, 0.0f), 8.0f, 32.0f);
	SetPlanet(Vector3(16.0f, 16.0f, 0.0f), 8.0f, 32.0f);
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
	for (std::unique_ptr<Planet>& planet : planets_)
	{
		planet->Update();
	}

	// 重力場の更新
	for (std::unique_ptr<GravitationalField>& gravitationalField : gravitationalFields_)
	{
		gravitationalField->Update();
	}


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
	for (std::unique_ptr<Planet>& planet : planets_)
	{
		planet->Draw();
	}

	// 重力場の描画
	for (std::unique_ptr<GravitationalField>& gravitationalField : gravitationalFields_)
	{
		gravitationalField->Draw();
	}

}

/// <summary>
/// 全ての当たり判定を行う
/// </summary>
void GameScene::CheckAllCollisions()
{
	// 球
	Sphere sphere1,sphere2;


#pragma region // プレイヤー と 惑星

	sphere1 = player_->GetCollisionSphere();

	for (std::unique_ptr<Planet>& planet : planets_)
	{
		sphere2 = planet->GetCollisionSphere();

		if (IsCollision(sphere1, sphere2))
		{
			player_->OnCollision(planet.get());
		}
	}

#pragma endregion


#pragma region // プレイヤー と 重力場

	sphere1 = player_->GetCollisionSphere();
	
	for (std::unique_ptr<GravitationalField>& gravitationalField : gravitationalFields_)
	{
		sphere2 = gravitationalField->GetCollisionSphere();

		if (IsCollision(sphere1, sphere2))
		{
			player_->OnCollision(gravitationalField.get());
		}
	}

#pragma endregion
}



/// <summary>
/// 惑星を設置する
/// </summary>
void GameScene::SetPlanet(const Vector3& position, float planetRadius , float gravitationalFieldRadius)
{
	// 惑星の生成と初期化
	std::unique_ptr<Planet> planet = std::make_unique<Planet>();
	planet->Initialize(engine_, camera3d_.get(),position, planetRadius);

	// 重力場の生成と初期化
	std::unique_ptr<GravitationalField> gravitationalField = std::make_unique<GravitationalField>();
	gravitationalField->Initialize(engine_, camera3d_.get(), position, gravitationalFieldRadius);
	gravitationalField->SetPlanetInstance(planet.get());

	// リストに登録する
	planets_.push_back(std::move(planet));
	gravitationalFields_.push_back(std::move(gravitationalField));
}
