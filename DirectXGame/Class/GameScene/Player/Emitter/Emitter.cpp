#include "Emitter.h"

/// <summary>
/// デストラクタ
/// </summary>
Emitter::~Emitter()
{
	// パーティクル
	for (Particle* particle : particles_)
	{
		delete particle;
	}
	particles_.clear();

	// UVトランスフォーム
	delete uvTransform_;

	// ワールドトランスフォーム
	delete worldTransform_;
}

/// <summary>
/// 初期化
/// </summary>
void Emitter::Initialize(YokosukaEngine* engine,Camera3D* camera,WorldTransform* parent, const Engine::Vector3& position,uint32_t* textureHandle)
{
	// nullptrチェック
	assert(engine);
	assert(camera);
	assert(parent);
	assert(textureHandle);

	// 引数を受け取る
	engine_ = engine;
	camera_ = camera;
	textureHandle_ = textureHandle;

	// ワールドトランスフォームを初期化する
	worldTransform_ = new WorldTransform();
	worldTransform_->Initialize();
	worldTransform_->translation_ = position;
	worldTransform_->scale_ = { 0.1f, 0.1f , 0.1f };

	// UVトランスフォームを初期化する
	uvTransform_ = new WorldTransform();
	uvTransform_->Initialize();

	// 親 : プレイヤーをセットする
	worldTransform_->SetParent(parent);
}

/// <summary>
/// 更新処理
/// </summary>
void Emitter::Update()
{
	// 回転させる
	worldTransform_->rotation_ += {0.01f, 0.05f, 0.0f};
	uvTransform_->rotation_.z += 0.05f;

	// ワールドトランスフォームを更新する
	worldTransform_->UpdateWorldMatrix();

	// UVトランスフォームを更新する
	uvTransform_->UpdateWorldMatrix();



	// 放出の復活処理を進める
	emissionRespawnTimer += 1.0f / 60.0f;

	if (emissionRespawnTimer > kEmissionTime)
	{
		// パーティクルを放出する
		Particle* newParticle = new Particle();
		newParticle->Initialize(engine_, camera_, GetWorldPosition(), textureHandle_);
		particles_.push_back(newParticle);

		// 復活処理を初期化する
		emissionRespawnTimer = 0.0f;
	}

	// 終了したパーティクルを消す
	particles_.remove_if([](Particle* particle)
		{
			if (particle->IsFinished())
			{
				delete particle;
				return true;
			}
			return false;
		}
	);

	// パーティクル
	for (Particle* particle : particles_)
	{
		particle->Update();
	}
}

/// <summary>
/// 描画処理
/// </summary>
void Emitter::Draw()
{
	// 三角形を描画する
	engine_->DrawTriangle(worldTransform_,uvTransform_, camera_, *textureHandle_, color);

	// パーティクル
	for (Particle* particle : particles_)
	{
		particle->Draw();
	}
}

/// <summary>
/// ワールド座標のGetter
/// </summary>
/// <returns></returns>
Engine::Vector3 Emitter::GetWorldPosition()
{
	Engine::Vector3 worldPosition;

	worldPosition.x = worldTransform_->worldMatrix_.m[3][0];
	worldPosition.y = worldTransform_->worldMatrix_.m[3][1];
	worldPosition.z = worldTransform_->worldMatrix_.m[3][2];

	return worldPosition;
}