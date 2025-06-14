#include "DeathParticle.h"

/// <summary>
/// デストラクタ
/// </summary>
DeathParticle::~DeathParticle()
{
	// ワールドトランスフォーム
	for (uint32_t i = 0 ; i < 8 ; ++i)
	{
		delete worldTransforms_[i];
	}
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
void DeathParticle::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d , const Vector3& position)
{
	// nullptrチェック
	assert(engine);
	assert(camera3d);

	// 引数を受け取る
	engine_ = engine;
	camera3d_ = camera3d;

	// ワールドトランスフォームの生成と初期化
	for (uint32_t i = 0 ; i < 8 ; ++i)
	{
		worldTransforms_[i] = new WorldTransform();
		worldTransforms_[i]->Initialize();

		worldTransforms_[i]->translation_ = position;
	}

	// UVトランスフォーム
	uvTransform_ = std::make_unique<UvTransform>();
	uvTransform_->Initialize();

	// モデルを読み込む
	modelHandle_ = engine_->LoadModelData("./Resources/Models/DeathParticle", "DeathParticle.obj");
}

/// <summary>
/// 更新処理
/// </summary>
void DeathParticle::Update()
{
	// 消滅したら処理しない
	if (isFinished_)
		return;

	// 経過時間カウントを進める
	counter_ += 1.0f / 60.0f;

	// 存続時間を越えたら消滅する
	if (counter_ >= kDuration)
	{
		isFinished_ = true;
		return;
	}

	// 徐々に透明になる
	color_.w = std::clamp(1.0f - (counter_ / kDuration), 0.0f, 1.0f);


	// ワールドトランスフォーム
	for (uint32_t i = 0 ; i < 8 ;++i)
	{
		// 基本となる速度ベクトル
		Vector3 velocity = { kSpeed , 0.0f , 0.0f };

		// 回転角を計算する
		float angle = kAngleUnit * i;

		// Z軸回転行列
		Matrix4x4 rotateZMatrix = MakeRotateZMatrix(angle);

		// 基本ベクトルを回転させて速度ベクトルを得る
		velocity = Transform(velocity, rotateZMatrix);

		// 移動処理
		worldTransforms_[i]->translation_ += velocity;

		// ワールドトランスフォームを更新する
		worldTransforms_[i]->UpdateWorldMatrix();
	}

	// UVトランスフォームを更新する
	uvTransform_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void DeathParticle::Draw()
{
	// 消滅したら処理しない
	if (isFinished_)
		return;

	// モデルを描画する
	for (uint32_t i = 0 ; i < 8 ; ++i)
	{
		engine_->DrawModel(worldTransforms_[i], uvTransform_.get(), camera3d_, modelHandle_, color_);
	}
}