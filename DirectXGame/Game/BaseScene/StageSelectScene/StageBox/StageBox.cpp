#include "StageBox.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera2d"></param>
void StageBox::Initialize(const YokosukaEngine* engine, const Camera2D* camera2d)
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
	worldTransform_->translation_ = Vector3(static_cast<float>((engine_->GetScreenWidth() / 2) / 2), static_cast<float>(engine_->GetScreenHeight() / 2), 0.0f);

	// UVトランスフォームの生成と初期化
	uvTransform_ = std::make_unique<UvTransform>();
	uvTransform_->Initialize();

	// テクスチャを読み込む
	whiteTextureHandle_ = engine_->LoadTexture("./Resources/Textures/white2x2.png");
}

/// <summary>
/// 更新処理
/// </summary>
void StageBox::Update()
{

	// タイマーを進める
	timer_ += 1.0f / 60.0f;
	timer_ = std::min(timer_, maxTime_);

	// モードに合わせて更新
	switch (mode_)
	{
	case kOpen:
		// 開き

		OpenUpdate();

		break;

	case kClose:
		// 閉じ

		CloseUpdate();

		break;
	}


	// トランスフォームの更新
	worldTransform_->UpdateWorldMatrix();
	uvTransform_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void StageBox::Draw()
{
	// 白い図形を描画する
	engine_->DrawSprite(worldTransform_.get(), uvTransform_.get(), camera2d_, whiteTextureHandle_, Vector4(1.0f, 1.0f, 1.0f, 1.0f), FillMode::kWireFrame);
}

/// <summary>
/// 開きのリセット
/// </summary>
void StageBox::OpenReset(float time)
{
	worldTransform_->scale_ = Vector3(0.0f, 0.0f, 1.0f);

	timer_ = 0.0f;

	// 時間を受け取る
	maxTime_ = time;
	maxTime_ = std::max(maxTime_, 0.0f);

	// モード切替
	mode_ = kOpen;
}

/// <summary>
/// 閉じのリセット
/// </summary>
void StageBox::CloseReset(float time)
{
	worldTransform_->scale_ = Vector3(maxSize.x, maxSize.y, 1.0f);

	timer_ = 0.0f;

	// 時間を受け取る
	maxTime_ = time;
	maxTime_ = std::max(maxTime_, 0.0f);

	// モード切替
	mode_ = kClose;
}


/// <summary>
/// 開きの更新処理
/// </summary>
void StageBox::OpenUpdate()
{
	// キーフレーム
	float keyFrame[2][2] =
	{
		{0.0f , maxTime_ / 2.0f},
		{maxTime_ / 2.0f , maxTime_}
	};


	// 横に広がる
	if (timer_ >= keyFrame[0][0] && timer_ <= keyFrame[0][1])
	{
		// 補間
		float t = (timer_ - keyFrame[0][0]) / (keyFrame[0][1] - keyFrame[0][0]);
		float easing = 1.0f - std::powf(1.0f - t, 3.0f);

		worldTransform_->scale_.x = Lerp(0.0f, maxSize.x, easing);
	}

	// 縦に広がる
	if (timer_ >= keyFrame[1][0] && timer_ <= keyFrame[1][1])
	{
		// 補間
		float t = (timer_ - keyFrame[1][0]) / (keyFrame[1][1] - keyFrame[1][0]);
		float easing = 1.0f - std::powf(1.0f - t, 3.0f);

		worldTransform_->scale_.y = Lerp(0.0f, maxSize.y, easing);
	}
}

/// <summary>
/// 閉じの更新処理
/// </summary>
void StageBox::CloseUpdate()
{
	// キーフレーム
	float keyFrame[2][2] =
	{
		{0.0f , maxTime_ / 2.0f},
		{maxTime_ / 2.0f , maxTime_}
	};

	// 横に閉じる
	if (timer_ >= keyFrame[0][0] && timer_ <= keyFrame[0][1])
	{
		// 補間
		float t = (timer_ - keyFrame[0][0]) / (keyFrame[0][1] - keyFrame[0][0]);
		float easing = 1.0f - std::powf(1.0f - t, 3.0f);

		worldTransform_->scale_.x = Lerp(maxSize.x, 0.0f, easing);
	}

	// 縦に閉じる
	if (timer_ >= keyFrame[1][0] && timer_ <= keyFrame[1][1])
	{
		// 補間
		float t = (timer_ - keyFrame[1][0]) / (keyFrame[1][1] - keyFrame[1][0]);
		float easing = 1.0f - std::powf(1.0f - t, 3.0f);

		worldTransform_->scale_.y = Lerp(maxSize.y, 0.0f, easing);
	}
}