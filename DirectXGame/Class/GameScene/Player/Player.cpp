#include "Player.h"

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{
	// エミッター
	for (Emitter* emitter : emitters_)
	{
		delete emitter;
	}
	emitters_.clear();

	// UVトランスフォーム
	delete uvTransform_;

	// ワールドトランスフォーム
	delete worldTransform_;
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="camera">カメラ</param>
void Player::Initialize(YokosukaEngine* engine, Camera3D* camera)
{
	// nullptrチェック
	assert(engine);
	assert(camera);

	// 引数を受け取る
	engine_ = engine;
	camera_ = camera;

	// ワールドトランスフォームの生成と初期化
	worldTransform_ = new WorldTransform();
	worldTransform_->Initialize();
	worldTransform_->scale_ = { 1.0f , 1.0f , 1.0f };

	// UVトランスフォームの生成と初期化
	uvTransform_ = new WorldTransform();
	uvTransform_->Initialize();

	// テクスチャを読み込む
	textureHandleTable_[0] = engine_->LoadTexture("./Resources/Textures/uvChecker.png");
	textureHandleTable_[1] = engine_->LoadTexture("./Resources/Textures/monsterBall.png");
	textureHandleTable_[2] = engine_->LoadTexture("./Resources/Textures/white.png");

	textureHandle_ = textureHandleTable_[int(TextureHandle::UvChecker)];


	// エミッターを登録する
	for (uint32_t i = 0; i < 4; i++)
	{
		Emitter* newEmitter = new Emitter();

		if (i == 0)
		{
			newEmitter->Initialize(engine_, camera_, worldTransform_, { 0.0f , 2.0f , 0.0f } , &textureHandle_);
		}
		else if (i == 1)
		{
			newEmitter->Initialize(engine_, camera_, worldTransform_, { 0.0f , -2.0f , 0.0f }, &textureHandle_);
		}
		else if (i == 2)
		{
			newEmitter->Initialize(engine_, camera_, worldTransform_, { 0.0f , 0.0f , -2.0f }, &textureHandle_);
		} else if (i == 3)
		{
			newEmitter->Initialize(engine_, camera_, worldTransform_, { 0.0f , 0.0f , 2.0f }, &textureHandle_);
		}
		

		emitters_.push_back(newEmitter);
	}
}

/// <summary>
/// 更新処理
/// </summary>
void Player::Update()
{
	ImGui::Begin("Player");
	ImGui::ColorEdit3("color", &color_.x);
	ImGui::DragFloat3("scale", &worldTransform_->scale_.x, 0.1f);
	ImGui::DragFloat3("rotation", &worldTransform_->rotation_.x, 0.01f); 
	ImGui::DragFloat3("translation", &worldTransform_->translation_.x, 0.01f);
	if (ImGui::Button("texture"))
	{

		if (textureHandle_ == textureHandleTable_[int(TextureHandle::UvChecker)])
		{
			textureHandle_ = textureHandleTable_[int(TextureHandle::MonsterBall)];
		}
		else if (textureHandle_ == textureHandleTable_[int(TextureHandle::MonsterBall)])
		{
			textureHandle_ = textureHandleTable_[int(TextureHandle::White)];
		}
		else if (textureHandle_ == textureHandleTable_[int(TextureHandle::White)])
		{
			textureHandle_ = textureHandleTable_[int(TextureHandle::UvChecker)];
		}
	}
	ImGui::End();

	// 回転させる
	worldTransform_->rotation_ += {0.05f, 0.01f, 0.0f};

	// テクスチャを回転させる
	uvTransform_->rotation_.z += 0.05f;


	// ワールドトランスフォームを更新する
	worldTransform_->UpdateWorldMatrix();

	// UVトランスフォームを更新する
	uvTransform_->UpdateWorldMatrix();


	// エミッター
	for (Emitter* emitter : emitters_)
	{
		emitter->Update();
	}
}

/// <summary>
/// 描画処理
/// </summary>
void Player::Draw()
{
	// 三角形を描画する
	engine_->DrawTriangle(worldTransform_,uvTransform_, camera_, textureHandle_, color_);

	// エミッター
	for (Emitter* emitter : emitters_)
	{
		emitter->Draw();
	}
}