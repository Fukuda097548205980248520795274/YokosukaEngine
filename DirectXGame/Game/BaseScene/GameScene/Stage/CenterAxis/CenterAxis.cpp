#include "CenterAxis.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
void CenterAxis::Initliaze(const YokosukaEngine* engine, const Camera3D* camera3d)
{
	// nullptrチェック
	assert(engine);
	assert(camera3d);

	// 引数を受けとる
	engine_ = engine;
	camera3d_ = camera3d;

	// ワールドトランスフォームの生成
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
	worldTransform_->translation_.y = 10.0f;

	// UVトランスフォーム
	uvTransform_ = std::make_unique<UvTransform>();
	uvTransform_->Initialize();

	// テクスチャを読み込む
	textureHandle_ = engine_->LoadTexture("./Resources/Textures/white2x2.png");


	// 制御点の登録
	for (uint32_t i = 0; i < _countof(controlPointTable); ++i)
	{
		controlPoint_.push_back(controlPointTable[i]);
	}
}

/// <summary>
/// 更新処理
/// </summary>
void CenterAxis::Update()
{
	ImGui::Begin("controlPoint");
	ImGui::DragFloat3("0", &controlPoint_[0].x);
	ImGui::DragFloat3("1", &controlPoint_[1].x);
	ImGui::DragFloat3("2", &controlPoint_[2].x);
	ImGui::DragFloat3("3", &controlPoint_[3].x);
	ImGui::DragFloat3("4", &controlPoint_[4].x);
	ImGui::End();

	t_ += 0.0005f * (*gameTimer_);

	worldTransform_->translation_ = engine_->CatmullRomPosition(controlPoint_, t_);
	Vector3 nextPos = engine_->CatmullRomPosition(controlPoint_, t_ + 0.0001f);

	Vector3 toNextPos = nextPos - worldTransform_->translation_;

	worldTransform_->rotation_.y = std::atan2(toNextPos.x, toNextPos.z);
	float length = std::sqrt(std::pow(toNextPos.x, 2.0f) + std::pow(toNextPos.z, 2.0f));
	worldTransform_->rotation_.x = std::atan2(-toNextPos.y, length);

	// ワールドトラスフォームの更新
	worldTransform_->UpdateWorldMatrix();
	uvTransform_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void CenterAxis::Draw()
{
	// 制御点の描画
	engine_->DrwaCatmullRomSpline(controlPoint_, Vector4(1.0f, 0.0f, 0.0f, 1.0f), camera3d_);

	// 弾を描画する
	//engine_->DrawSphere(worldTransform_.get(), uvTransform_.get(), camera3d_, textureHandle_, 10, 5, Vector4(1.0f, 0.0f, 0.0f, 1.0f), false);
}


/// <summary>
/// ワールド座標のGetter
/// </summary>
/// <returns></returns>
Vector3 CenterAxis::GetWorldPosition()const
{
	// ワールド座標
	Vector3 worldPosition;

	worldPosition.x = worldTransform_->worldMatrix_.m[3][0];
	worldPosition.y = worldTransform_->worldMatrix_.m[3][1];
	worldPosition.z = worldTransform_->worldMatrix_.m[3][2];

	return worldPosition;
}