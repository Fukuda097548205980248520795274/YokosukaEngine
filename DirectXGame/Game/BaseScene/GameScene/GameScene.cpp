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

	// 敵の生成と初期化
	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize(engine_, modelHandleStore_, textureHandleStore_, camera3d_, Vector3(10.0f, 0.0f, 0.0f));

	// 平行光源の生成と初期化
	directionalLight_ = std::make_unique<DirectionalLight>();
	directionalLight_->Initialize();
	
}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update()
{
	// プレイヤーの更新処理
	player_->Update();

	// 敵の更新処理
	enemy_->Update();


	// 敵の当たり判定の球
	Sphere enemySphere;
	enemySphere.center = enemy_->GetWorldPosition();
	enemySphere.radius = 1.0f;

	if (IsCollision(player_->GetBackPlane(), enemySphere))
	{
		if (IsCollision(player_->GetTopPlane(), enemySphere) && IsCollision(player_->GetBottomPlane(), enemySphere))
		{
			ImGui::Text("hit");
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

	// 敵の描画処理
	enemy_->Draw();

	// Scene描画
	BaseScene::Draw();
}