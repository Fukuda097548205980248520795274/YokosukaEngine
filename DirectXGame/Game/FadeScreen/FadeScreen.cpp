#include "FadeScreen.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera2d"></param>
void FadeScreen::Initialize(const YokosukaEngine* engine, const Camera2D* camera2d)
{
	// nullptrチェック
	assert(engine);
	assert(camera2d);

	// 引数を受け取る
	engine_ = engine;
	camera2d_ = camera2d;

	// ワールドトランスフォームの生成と初期化
	worldTransform_ = std::make_unique<WorldTransform2D>();
	worldTransform_->Initialize();
	worldTransform_->translation_ = Vector3(static_cast<float>(engine_->GetScreenWidth() / 2.0f), static_cast<float>(engine_->GetScreenHeight() / 2.0f), 0.0f);
	worldTransform_->scale_ = Vector3(static_cast<float>(engine_->GetScreenWidth() / 2.0f), static_cast<float>(engine_->GetScreenHeight() / 2.0f), 1.0f);

	// UVトランスフォームの生成と初期化
	uvTransform_ = std::make_unique<UvTransform>();
	uvTransform_->Initialize();

	// テクスチャを読み込む
	textureHandle_ = engine_->LoadTexture("./Resources/Textures/white2x2.png");
}

/// <summary>
/// 更新処理
/// </summary>
void FadeScreen::Update()
{
	// タイマーを進める
	fadeTimer_ += 1.0f / 60.0f;
	fadeTimer_ = std::min(fadeTimer_, fadeTime_);

	switch (fade_)
	{
	case kIn:
		// フェードイン
		FadeInUpdate();

		break;

	case kOut:
		// フェードアウト
		FadeOutUpdate();

		break;
	}

	// トランスフォームの更新
	worldTransform_->UpdateWorldMatrix();
	uvTransform_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void FadeScreen::Draw()
{
	engine_->DrawSprite(worldTransform_.get(), uvTransform_.get(), camera2d_, textureHandle_, color_, FillMode::kSolid);
}


/// <summary>
/// フェードインのリセット
/// </summary>
/// <param name="timer"></param>
void FadeScreen::ResetFadeIn(float timer)
{
	fadeTimer_ = 0.0f;
	fade_ = kIn;

	fadeTime_ = timer;
	fadeTime_ = std::max(fadeTime_, 0.0f);
}

/// <summary>
/// フェードアウトのリセット
/// </summary>
/// <param name="timer"></param>
void FadeScreen::ResetFadeOut(float timer)
{
	fadeTimer_ = 0.0f;
	fade_ = kOut;

	fadeTime_ = timer;
	fadeTime_ = std::max(fadeTime_, 0.0f);
}




/// <summary>
/// フェードインの更新処理
/// </summary>
void FadeScreen::FadeInUpdate()
{
	if (fadeTime_ == 0.0f)
	{
		color_.w = 0.0f;
		return;
	}

	float t = fadeTimer_ / fadeTime_;
	color_.w = Lerp(1.0f, 0.0f, t);
}

/// <summary>
/// フェードアウトの更新処理
/// </summary>
void FadeScreen::FadeOutUpdate()
{
	if (fadeTime_ == 0.0f)
	{
		color_.w = 1.0f;
		return;
	}

	float t = fadeTimer_ / fadeTime_;
	color_.w = Lerp(0.0f, 1.0f, t);
}