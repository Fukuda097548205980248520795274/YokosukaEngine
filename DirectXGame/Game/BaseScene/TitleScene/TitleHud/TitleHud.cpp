#include "TitleHud.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
/// <param name="camera2d"></param>
void TitleHud::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const Camera2D* camera2d,
	const ModelHandleStore* modelHandleStore, const TextureHandleStore* textureHandleStore)
{
	// nullptrチェック
	assert(engine);
	assert(camera3d);
	assert(camera2d);
	assert(modelHandleStore);
	assert(textureHandleStore);

	// 引数を受け取る
	engine_ = engine;
	camera3d_ = camera3d;
	camera2d_ = camera2d;
	modelHandleStore_ = modelHandleStore;
	textureHandleStore_ = textureHandleStore;


	// プレイヤージェットの生成と初期化
	playerJet_ = std::make_unique<Model>();
	playerJet_->Initialize(engine_, camera3d_, modelHandleStore_->GetModelHandle(ModelHandleStore::kPlayer)[0]);
	playerJet_->enableLighting_ = false;
	playerJet_->color_ = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	playerJet_->worldTransform_->rotation_ = Vector3(-std::numbers::pi_v<float> / 2.0f, 0.0f, std::numbers::pi_v<float> / 2.0f);
	playerJet_->worldTransform_->translation_ = jetStartPos_;


	// ゲームパッド : スタート の生成と初期化
	spriteGamepadStart_ = std::make_unique<Sprite>();
	spriteGamepadStart_->Initialize(engine_, camera2d_, textureHandleStore_->GetTextureHandle(TextureHandleStore::kGamepadStart));
	spriteGamepadStart_->worldTransform_->translation_ =
		Vector3(static_cast<float>(engine_->GetScreenWidth()) / 2.0f, 600.0f, 0.0f);

	// キーボード : スタート の生成と初期化
	spritekeyboardStart_ = std::make_unique<Sprite>();
	spritekeyboardStart_->Initialize(engine_, camera2d_, textureHandleStore_->GetTextureHandle(TextureHandleStore::kKeyboardStart));
	spritekeyboardStart_->worldTransform_->translation_ =
		Vector3(static_cast<float>(engine_->GetScreenWidth()) / 2.0f, 600.0f, 0.0f);

	// 白背景の生成と初期化
	spriteBgWhite_ = std::make_unique<Sprite>();
	spriteBgWhite_->Initialize(engine_, camera2d_, textureHandleStore_->GetTextureHandle(TextureHandleStore::kWhite));
	spriteBgWhite_->worldTransform_->scale_ =
		Vector3(static_cast<float>(engine_->GetScreenWidth()) / 2.0f, static_cast<float>(engine_->GetScreenHeight()) / 2.0f, 0.0f);
	spriteBgWhite_->worldTransform_->translation_ =
		Vector3(static_cast<float>(engine_->GetScreenWidth()) / 2.0f, static_cast<float>(engine_->GetScreenHeight()) / 2.0f, 200.0f);

	// 黒背景の生成と初期化
	spriteBgBlack_ = std::make_unique<Sprite>();
	spriteBgBlack_->Initialize(engine_, camera2d_, textureHandleStore_->GetTextureHandle(TextureHandleStore::kWhite));
	spriteBgBlack_->worldTransform_->scale_ =
		Vector3(static_cast<float>(engine_->GetScreenWidth()) / 2.0f, blackScaleY_, 0.0f);
	spriteBgBlack_->worldTransform_->translation_ =
		Vector3(static_cast<float>(engine_->GetScreenWidth()) / 2.0f, static_cast<float>(engine_->GetScreenHeight()), 0.0f);
	spriteBgBlack_->color_ = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
}

/// <summary>
/// 更新処理
/// </summary>
void TitleHud::Update()
{
	// モデルの描画
	playerJet_->Update();

	// スプライトの更新
	spriteGamepadStart_->Update();
	spritekeyboardStart_->Update();
	spriteBgWhite_->Update();
	spriteBgBlack_->Update();
}

/// <summary>
/// 描画処理
/// </summary>
void TitleHud::Draw()
{
	// モデルの描画
	playerJet_->Draw();

	spriteBgWhite_->Draw();
	spriteBgBlack_->Draw();

	// コントローラを使用しているかどうか
	if (engine_->IsGamepadEnable(0))
	{
		spriteGamepadStart_->Draw();
	} else
	{
		spritekeyboardStart_->Draw();
	}
}




/// <summary>
/// ジェット飛行 : 初期化
/// </summary>
void TitleHud::PhaseFlyJetInitialize(float maxParameter)
{
	maxFlyJetOperationParameter = maxParameter;
}

/// <summary>
/// ジェット飛行 : 更新処理
/// </summary>
void TitleHud::PhaseFlyJetUpdate()
{
	// パラメータを進める
	flyJetParameter_ += 1.0f / 60.0f;
	flyJetParameter_ = std::min(flyJetParameter_, maxFlyJetOperationParameter);

	if (flyJetParameter_ <= maxFlyJetOperationParameter)
	{
		// 補間
		float t = flyJetParameter_ / maxFlyJetOperationParameter;
		float easing = 1.0f - std::powf(1.0f - t, 8.0f);

		playerJet_->worldTransform_->translation_ = Lerp(jetStartPos_, jetStopPos_, easing);
	}
}




/// <summary>
/// 操作中 : 初期化
/// </summary>
void TitleHud::PhaseOperationInitialize()
{
	// 色を初期化する
	spriteGamepadStart_->color_.w = 1.0f;
	spritekeyboardStart_->color_.w = 1.0f;
}

/// <summary>
/// 操作中 : 更新処理
/// </summary>
void TitleHud::PhaseOperationUpdate()
{
	// パラメータを進める
	operationParameter_ += 1.0f / 60.0f;
	operationParameter_ = std::fmod(operationParameter_, kMaxOperationParameter);

	// 前半
	if (operationParameter_ <= kMaxOperationParameter / 2.0f)
	{
		float t = operationParameter_ / (kMaxOperationParameter / 2.0f);
		spriteGamepadStart_->color_.w = Lerp(1.0f, 0.2f, t);
		spritekeyboardStart_->color_.w = Lerp(1.0f, 0.2f, t);
	}

	// 後半
	if (operationParameter_ > kMaxOperationParameter / 2.0f)
	{
		float t = (operationParameter_ - (kMaxOperationParameter / 2.0f)) / (kMaxOperationParameter / 2.0f);

		spriteGamepadStart_->color_.w = Lerp(0.2f, 1.0f, t);
		spritekeyboardStart_->color_.w = Lerp(0.2f, 1.0f, t);
	}
}





/// <summary>
/// フェードアウト : 初期化
/// </summary>
void TitleHud::PhaseFadeOutInitialize(float maxParameter)
{
	maxFadeOutParameter_ = maxParameter;
}

/// <summary>
/// フェードアウト : 更新処理
/// </summary>
void TitleHud::PhaseFadeOutUpdate()
{
	if (maxFadeOutParameter_ <= 0.0f)
		return;

	// パラメータを進める
	fadeOutParameter_ += 1.0f / 60.0f;
	fadeOutParameter_ = std::min(fadeOutParameter_, maxFadeOutParameter_);

	if (fadeOutParameter_ <= (maxFadeOutParameter_ / 3.0f))
	{
		// 補間
		float t = fadeOutParameter_ / (maxFadeOutParameter_ / 3.0f);
		float easing = std::powf(t, 8.0f);

		playerJet_->worldTransform_->translation_ = Lerp(jetStopPos_, jetGoalPos_, easing);
	}

	if (fadeOutParameter_ <= (maxFadeOutParameter_ / 2.0f))
	{
		float t = fadeOutParameter_ / (maxFadeOutParameter_ / 2.0f);
		spriteGamepadStart_->color_.w = Lerp(1.0f, 0.0f, t);
		spritekeyboardStart_->color_.w = Lerp(1.0f, 0.0f, t);
	}

	if (fadeOutParameter_ >= (maxFadeOutParameter_ / 2.0f))
	{
		float t = (fadeOutParameter_ - (maxFadeOutParameter_ / 2.0f)) / (maxFadeOutParameter_ - (maxFadeOutParameter_ / 2.0f));

		float easing = t;
		const float c1 = 1.70158f;
		const float c2 = c1 * 1.525f;
		if (t < 0.5f)
		{
			easing = (std::powf(2.0f * t, 2.0f) * ((c2 + 1.0f) * 2.0f * t - c2)) / 2.0f;
		}
		else
		{
			easing = (std::powf(2.0f * t - 2.0f, 2.0f) * ((c2 + 1.0f) * (t * 2.0f - 2.0f) + c2) + 2.0f) / 2.0f;
		}

		spriteBgBlack_->worldTransform_->scale_.y = Lerp(blackScaleY_, static_cast<float>(engine_->GetScreenHeight()), easing);
	}
}