#include "GameScene.h"
#include "../../ModelHandleStore/ModelHandleStore.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine">エンジン</param>
void GameScene::Initialize(const YokosukaEngine* engine, const ModelHandleStore* modelHandleStore, const TextureHandleStore* textureHandleStore)
{
	// BaseScene更新
	BaseScene::Initialize(engine , modelHandleStore , textureHandleStore);

	// プレイヤーの生成と初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(engine_, modelHandleStore_, textureHandleStore_, camera3d_, Vector3(0.0f, 0.0f, 0.0f));

	// 平行光源の生成と初期化
	directionalLight_ = std::make_unique<DirectionalLight>();
	directionalLight_->Initialize();
	


	for (int i = 0; i < 3; i++)
	{
		std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();
		enemy->Initialize(engine_, modelHandleStore_, textureHandleStore_, camera3d_, Vector3(10.0f, -10.0f + 10.0f * i, 0.0f));
		enemies_.push_back(std::move(enemy));
	}
}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update()
{
	// プレイヤーの更新処理
	player_->Update();

	// 敵の更新処理
	for (std::unique_ptr<Enemy>& enemy : enemies_)
	{
		enemy->Update();
	}


	// プレイヤーの向き
	Vector3 playerDirction = player_->GetDirection();

	for (std::unique_ptr<Enemy>& enemy : enemies_)
	{
		// 敵の方向
		Vector3 toEnemy = Normalize(enemy->GetWorldPosition() - player_->GetWorldPosition());

		if (Dot(playerDirction, toEnemy) >= 0.75f)
		{
			player_->OnCollision(enemy.get());
		}
	}


	// Scene更新
	BaseScene::Update();
}

/// <summary>
/// 描画処理
/// </summary>
void GameScene::Draw()
{
	// 平行光源の接地
	engine_->SetDirectionalLight(directionalLight_.get());

	// プレイヤーの描画処理
	player_->Draw();

	// 敵の更新処理
	for (std::unique_ptr<Enemy>& enemy : enemies_)
	{
		enemy->Draw();
	}

	// Scene描画
	BaseScene::Draw();
}