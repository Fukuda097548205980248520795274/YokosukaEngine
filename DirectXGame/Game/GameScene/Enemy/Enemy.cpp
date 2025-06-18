#include "Enemy.h"
#include "BaseEnemyPhase/EnemyPhaseApproach/EnemyPhaseApproach.h"
#include "BaseEnemyPhase/EnemyPhaseLeave/EnemyPhaseLeave.h"

/// <summary>
/// デストラクタ
/// </summary>
Enemy::~Enemy()
{
	// 弾のリスト
	for (EnemyBullet* bullet : bullets_)
	{
		delete bullet;
	}
	bullets_.clear();
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
/// <param name="directionalLight"></param>
void Enemy::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const DirectionalLight* directionalLight,
	const Vector3& position)
{
	// nullptrチェック
	assert(engine);
	assert(camera3d);
	assert(directionalLight);

	// 引数を受け取る
	engine_ = engine;
	camera3d_ = camera3d;
	directionalLight_ = directionalLight;


	// ワールドトランスフォームの生成と初期化
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
	worldTransform_->translation_ = position;

	// UVトランスフォームの生成と初期化
	uvTransform_ = std::make_unique<UvTransform>();
	uvTransform_->Initialize();

	// モデルを読み込む
	modelHandle_ = engine_->LoadModelData("./Resources/Models/Enemy", "Enemy.obj");

	// ポイントライトの生成と初期化
	pointLight_ = std::make_unique<PointLight>();
	pointLight_->Initialize();
	pointLight_->intensity_ = 0.0f;

	// スポットライトの生成と初期化
	spotLight_ = std::make_unique<SpotLight>();
	spotLight_->Initialize();
	spotLight_->intensity_ = 0.0f;


	// 接近フェーズを初期化する
	ChangePhase(std::make_unique<EnemyPhaseApproach>(this));
}

/// <summary>
/// 更新処理
/// </summary>
void Enemy::Update()
{
	// 行動フェーズでの動き
	phase_->Update();

	// 弾の更新
	for (EnemyBullet* bullet : bullets_)
	{
		bullet->Update();
	}

	// 消滅した弾をリストから削除する
	bullets_.remove_if([](EnemyBullet* bullet)
		{
			if (bullet->IsFinished())
			{
				delete bullet;
				return true;
			}
			return false;
		}
	);

	// トランスフォームを更新する
	worldTransform_->UpdateWorldMatrix();
	uvTransform_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void Enemy::Draw()
{
	// モデルを描画する
	engine_->DrawModel(worldTransform_.get(), uvTransform_.get(), camera3d_, modelHandle_, { 1.0f, 0.0f, 0.0f , 1.0f },
		directionalLight_, pointLight_.get(), spotLight_.get());

	// 弾の描画
	for (EnemyBullet* bullet : bullets_)
	{
		bullet->Draw();
	}
}

/// <summary>
/// フェーズの状態を切り替える
/// </summary>
/// <param name="phaseState"></param>
void Enemy::ChangePhase(std::unique_ptr<BaseEnemyPhase> phase)
{
	phase_ = std::move(phase);
}

/// <summary>
/// 弾を発射する
/// </summary>
void Enemy::BulletShot()
{
	// 弾の移動速度
	const float kBulletMoveSpeed = 1.0f;

	// 速度ベクトル
	Vector3 velocity = { 0.0f , 0.0f , -kBulletMoveSpeed };

	// 弾の生成と初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(engine_, camera3d_, directionalLight_, GetWorldPosition(), velocity);

	// リストに登録する
	bullets_.push_back(newBullet);


	// 発射タイマーを初期化する
	shotTiemer_ = 0.0f;
}

/// <summary>
/// ワールド座標のGetter
/// </summary>
/// <returns></returns>
Vector3 Enemy::GetWorldPosition()
{
	// ワールド座標
	Vector3 worldPosition;

	worldPosition.x = worldTransform_->worldMatrix_.m[3][0];
	worldPosition.y = worldTransform_->worldMatrix_.m[3][1];
	worldPosition.z = worldTransform_->worldMatrix_.m[3][2];

	return worldPosition;
}