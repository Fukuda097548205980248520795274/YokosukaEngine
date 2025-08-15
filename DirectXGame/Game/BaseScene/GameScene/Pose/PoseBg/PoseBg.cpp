#include "PoseBg.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera2d"></param>
void PoseBg::Initialize(const YokosukaEngine* engine, const Camera2D* camera2d)
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
	worldTransform_->scale_ = Vector3(static_cast<float>(engine_->GetScreenWidth() /2), static_cast<float>(engine_->GetScreenHeight() / 2), 0.0f);
	worldTransform_->translation_ = Vector3(static_cast<float>(engine_->GetScreenWidth() / 2), static_cast<float>(engine_->GetScreenHeight() / 2), 0.0f);

	// UVトランスフォームの生成と初期化
	uvTransform_ = std::make_unique<UvTransform>();
	uvTransform_->Initialize();

	// テクスチャを読み込む
	whiteTextureHandle_ = engine_->LoadTexture("./Resources/Textures/white2x2.png");
}

/// <summary>
/// 更新処理
/// </summary>
void PoseBg::Update()
{

	// タイマーを進める
	timer_ += 1.0f / 60.0f;
	timer_ = std::min(timer_, maxTime_);

	// モードに合わせて更新
	switch (mode_)
	{
	case kColored:
		// 色付け

		ColoredUpdate();

		break;

	case kColorFabes:
		// 色消し

		ColorFabesUpdate();

		break;
	}


	// トランスフォームの更新
	worldTransform_->UpdateWorldMatrix();
	uvTransform_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void PoseBg::Draw()
{
	// 白い図形を描画する
	engine_->DrawSprite(worldTransform_.get(), uvTransform_.get(), camera2d_, whiteTextureHandle_, Vector4(1.0f, 1.0f, 1.0f, alpha_), FillMode::kSolid);
}

/// <summary>
/// 色付けのリセット
/// </summary>
void PoseBg::ColoredReset(float time)
{
	// 透明度
	alpha_ = 0.0f;

	timer_ = 0.0f;

	// 時間を受け取る
	maxTime_ = time;
	maxTime_ = std::max(maxTime_, 0.0f);

	// モード切替
	mode_ = kColored;
}

/// <summary>
/// 色消しのリセット
/// </summary>
void PoseBg::ColorFabesReset(float time)
{
	// 透明度
	alpha_ = 0.5f;

	timer_ = 0.0f;

	// 時間を受け取る
	maxTime_ = time;
	maxTime_ = std::max(maxTime_, 0.0f);

	// モード切替
	mode_ = kColorFabes;
}


/// <summary>
/// 色付けの更新処理
/// </summary>
void PoseBg::ColoredUpdate()
{
	// キーフレーム
	float keyFrame[2] = { 0.0f , maxTime_ };


	// 横に広がる
	if (timer_ >= keyFrame[0] && timer_ <= keyFrame[1])
	{
		// 補間
		float t = (timer_ - keyFrame[0]) / (keyFrame[1] - keyFrame[0]);
		float easing = t;

		alpha_ = Lerp(0.0f, 0.1f, easing);
	}
}

/// <summary>
/// 色消しの更新処理
/// </summary>
void PoseBg::ColorFabesUpdate()
{
	// キーフレーム
	float keyFrame[2] = { 0.0f , maxTime_ };

	// 横に閉じる
	if (timer_ >= keyFrame[0] && timer_ <= keyFrame[1])
	{
		// 補間
		float t = (timer_ - keyFrame[0]) / (keyFrame[1] - keyFrame[0]);
		float easing = t;

		alpha_ = Lerp(0.1f, 0.0f, easing);
	}
}