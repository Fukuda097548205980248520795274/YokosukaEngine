#include "MiniTriangle.h"

/// <summary>
/// デストラクタ
/// </summary>
MiniTriangle::~MiniTriangle()
{
	// UVトランスフォーム
	delete uvTransform_;

	// ワールドトランスフォーム
	delete worldTransform_;
}

/// <summary>
/// 初期化
/// </summary>
void MiniTriangle::Initialize(YokosukaEngine* engine, Camera3D* camera, WorldTransform* parent, const Engine::Vector3& position, uint32_t* textureHandle)
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
	worldTransform_->scale_ = { 0.5f, 0.5f , 0.5f };

	// UVトランスフォームを初期化する
	uvTransform_ = new WorldTransform();
	uvTransform_->Initialize();

	// 親 : プレイヤーをセットする
	worldTransform_->SetParent(parent);
}

/// <summary>
/// 更新処理
/// </summary>
void MiniTriangle::Update()
{
	// 回転させる
	worldTransform_->rotation_ += {0.01f, 0.05f, 0.0f};
	uvTransform_->rotation_.z += 0.05f;

	// ワールドトランスフォームを更新する
	worldTransform_->UpdateWorldMatrix();

	// UVトランスフォームを更新する
	uvTransform_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void MiniTriangle::Draw()
{
	// 三角形を描画する
	engine_->DrawTriangle(worldTransform_, uvTransform_, camera_, *textureHandle_, color);
}

/// <summary>
/// ワールド座標のGetter
/// </summary>
/// <returns></returns>
Engine::Vector3 MiniTriangle::GetWorldPosition()
{
	Engine::Vector3 worldPosition;

	worldPosition.x = worldTransform_->worldMatrix_.m[3][0];
	worldPosition.y = worldTransform_->worldMatrix_.m[3][1];
	worldPosition.z = worldTransform_->worldMatrix_.m[3][2];

	return worldPosition;
}