#include "Particle.h"

/// <summary>
/// デストラクタ
/// </summary>
Particle::~Particle()
{
	// UVトランスフォーム
	delete uvTransform_;

	// ワールドトランスフォーム
	delete worldTransform_;
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera"></param>
/// <param name="worldPosition"></param>
void Particle::Initialize(YokosukaEngine* engine, Camera3D* camera, const Engine::Vector3& worldPosition, uint32_t* textureHandle)
{
	// nullptrチェック
	assert(engine);
	assert(camera);
	assert(textureHandle);

	// 引数を受け取る
	engine_ = engine;
	camera_ = camera;
	textureHandle_ = textureHandle;

	// ワールドトランスフォームの生成と初期化
	worldTransform_ = new WorldTransform();
	worldTransform_->Initialize();
	worldTransform_->translation_ = worldPosition;

	// UVトランスフォームを初期化する
	uvTransform_ = new WorldTransform();
	uvTransform_->Initialize();
}

/// <summary>
/// 更新処理
/// </summary>
void Particle::Update()
{
	// デスタイマーを進める
	deathTimer_ += 1.0f / 60.0f;

	// 寿命を越えたら終了する
	if (deathTimer_ >= kLifeTime)
	{
		isFinished_ = true;
	}

	// 徐々に小さくする
	float rate = deathTimer_ / kLifeTime;
	float easeIn = std::pow(rate, 4.0f);
	worldTransform_->scale_ = (1.0f - easeIn) * Engine::Vector3(0.1f, 0.1f, 0.1f) + easeIn * Engine::Vector3(0.0f, 0.0f, 0.0f);

	// 明るい青 -> 青
	color_ = (1.0f - rate) * Engine::Vector4(0.5f, 0.5f, 1.0f , 1.0f) + rate * Engine::Vector4(0.0f, 0.0f, 1.0f , 1.0f);

	// 回転させる
	worldTransform_->rotation_ += {0.1f, 0.05f, 0.0f};

	// テクスチャを回転させる
	uvTransform_->rotation_.z += -0.05f;

	// ワールドトランスフォームを更新する
	worldTransform_->UpdateWorldMatrix();

	// UVトランスフォームを更新する
	uvTransform_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void Particle::Draw()
{
	// 三角形を描画する
	if (static_cast<int>(deathTimer_ * 100) % 5 >= 3)
	engine_->DrawTriangle(worldTransform_,uvTransform_, camera_, *textureHandle_, color_);
}