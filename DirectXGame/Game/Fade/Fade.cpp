#include "Fade.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
void Fade::Initialize(const YokosukaEngine* engine)
{
	// nullptrチェック
	assert(engine);

	// 引数を受け取る
	engine_ = engine;

	// 2Dカメラの生成と初期化
	camera2d_ = std::make_unique<Camera2D>();
	camera2d_->Initialize(static_cast<float>(engine_->GetScreenWidth()), static_cast<float>(engine_->GetScreenHeight()));

	// ワールドトランスフォームの生成と初期化
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
	
	// 中心に配置する
	worldTransform_->translation_ = { static_cast<float>(engine_->GetScreenWidth()) / 2.0f, static_cast<float>(engine_->GetScreenHeight()) / 2.0f ,0.0f };
	worldTransform_->scale_ = { static_cast<float>(engine_->GetScreenWidth()) / 2.0f, static_cast<float>(engine_->GetScreenHeight()) / 2.0f ,0.0f };

	// UVトランスフォームの生成と初期化
	uvTransform_ = std::make_unique<UvTransform>();
	uvTransform_->Initialize();

	// テクスチャを読み込む
	textureHandle_ = engine_->LoadTexture("./Resources/Textures/white2x2.png");
}

/// <summary>
/// 更新処理
/// </summary>
void Fade::Update()
{
	// フェード状態による分岐
	switch (status_)
	{
	case Status::kNone:
		// 何もしない

		break;

	case Status::kFadeOut:
		// フェードアウト

		// 経過時間をカウントする
		counter_ += 1.0f / 60.0f;

		// 経過時間を越えないようにする
		counter_ = std::min(counter_, duration_);

		// α値を求める
		color_.w = 1.0f - (counter_ / duration_);

		break;

	case Status::kFadeIn:
		// フェードイン

		// 経過時間をカウントする
		counter_ += 1.0f / 60.0f;

		// 経過時間を越えないようにする
		counter_ = std::min(counter_, duration_);

		// α値を求める
		color_.w = counter_ / duration_;

		break;
	}

	//  トランスフォームを更新する
	worldTransform_->UpdateWorldMatrix();
	uvTransform_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void Fade::Draw()
{
	if (status_ == Status::kNone)
		return;

	if (color_.w == 0.0f)
		return;

	// テクスチャを描画する
	engine_->DrawSprite(worldTransform_.get(), uvTransform_.get(), camera2d_.get(), textureHandle_, color_);
}

/// <summary>
/// フェード開始
/// </summary>
/// <param name="status">フェード状態</param>
/// <param name="duration">フェードする時間</param>
void Fade::Start(Status status, float duration)
{
	status_ = status;
	duration_ = duration;
	counter_ = 0.0f;
}

/// <summary>
/// フェード停止
/// </summary>
void Fade::Stop()
{
	status_ = Status::kNone;
}

/// <summary>
/// フェード終了判定
/// </summary>
/// <returns></returns>
bool Fade::IsFinished()
{
	switch (status_)
	{
	case Status::kFadeOut:
	case Status::kFadeIn:

		if (counter_ >= duration_)
		{
			return true;
		}
		else
		{
			return false;
		}

		break;
	}

	return false;
}