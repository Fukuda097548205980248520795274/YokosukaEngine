#include "GameScene.h"

/// <summary>
/// デストラクタ
/// </summary>
GameScene::~GameScene()
{
	// ヒットエフェクト
	for (HitEffect* hitEffect : hitEffects_)
	{
		delete hitEffect;
	}
	hitEffects_.clear();
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine">エンジン</param>
void GameScene::Initialize(const YokosukaEngine* engine)
{
	// nullptrチェック
	assert(engine);

	// Scene初期化
	Scene::Initialize(engine);


	// 平行光源の生成と初期化
	directionalLight_ = std::make_unique<DirectionalLight>();
	directionalLight_->Initialize();

	// 天球の生成と初期化
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(engine_, camera3d_.get());

	// 地面の生成と初期化
	ground_ = std::make_unique<Ground>();
	ground_->Initialize(engine_, camera3d_.get());

	// 衝突マネージャの生成
	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->Initialize();

	// プレイヤーの生成と初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(engine_, camera3d_.get(), Vector3(0.0f, 0.0f, 0.0f));
	player_->SetMainCamera(mainCamera_.get());
	player_->SetGameScene(this);

	// ロックオンの生成と初期化
	lockOn_ = std::make_unique<LockOn>();
	lockOn_->Initialize(engine_, camera3d_.get(), camera2d_.get());
	mainCamera_->SetLockOn(lockOn_.get());
	player_->SetLockOn(lockOn_.get());

	// プレイヤーにメインカメラを追従させる
	mainCamera_->SetTarget(player_->GetWorldTransform());

	// 敵を生成する
	for (int i = 0; i < 5; i++)
	{
		std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();
		enemy->Initialize(engine_, camera3d_.get(), Vector3(-20.0f + i * 10.0f, 0.0f, 30.0f));
		enemies_.push_back(std::move(enemy));
	}
}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update()
{
	// Scene更新
	Scene::Update();


	// 天球の更新
	skydome_->Update();

	// 地面の更新
	ground_->Update();

	// プレイヤー更新
	player_->Update();

	// ロックオンの更新
	lockOn_->Update(enemies_);

	// 敵の更新
	for (std::unique_ptr<Enemy>& enemy : enemies_)
	{
		enemy->Update();
	}

	// ヒットエフェクトの更新
	for (HitEffect* hitEffect : hitEffects_)
	{
		hitEffect->Update();
	}

	// 終了したヒットエフェクトをリストから削除する
	hitEffects_.remove_if([](HitEffect* hitEffect)
		{
			if (hitEffect->IsFinished())
			{
				delete hitEffect;
				return true;
			}
			return false;
		}
	);

	// 衝突マネージャの更新
	collisionManager_->UpdateTransform();

	// 当たり判定
	CheckAllCollision();
}

/// <summary>
/// ヒットエフェクトを放出する
/// </summary>
void GameScene::EmitHitEffect(HitEffect* hitEffect)
{
	// リストに追加する
	hitEffects_.push_back(hitEffect);
}

/// <summary>
/// 描画処理
/// </summary>
void GameScene::Draw()
{
	// Scene描画
	Scene::Draw();

	// 平行光源の設置
	engine_->SetDirectionalLight(directionalLight_.get());

	// 天球の描画
	skydome_->Draw();

	// 地面の描画
	ground_->Draw();

	// 衝突マネージャの描画
	collisionManager_->Draw();

	// プレイヤー描画
	player_->Draw();

	// ロックオンの描画
	lockOn_->Draw();

	// ヒットエフェクトの描画
	for (HitEffect* hitEffect : hitEffects_)
	{
		hitEffect->Draw();
	}

	// 敵の描画
	for (std::unique_ptr<Enemy>& enemy : enemies_)
	{
		enemy->Draw();
	}
}


/// <summary>
/// 全ての衝突判定を行う
/// </summary>
void GameScene::CheckAllCollision()
{
	// 衝突マネージャのリセット
	collisionManager_->Reset();

	// コライダーをリストに登録
	collisionManager_->AddCollider(player_.get());
	collisionManager_->AddCollider(player_->GetHammerInstance());
	for (std::unique_ptr<Enemy>& enemy : enemies_)
	{
		collisionManager_->AddCollider(enemy.get());
	}

	// 衝突判定と応答
	collisionManager_->CheckAllCollisions();
}