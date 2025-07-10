#include "GameScene.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine">エンジン</param>
void GameScene::Initialize(const YokosukaEngine* engine)
{
	// nullptrチェック
	assert(engine);

	// Scene更新
	Scene::Initialize(engine);

	// サウンドを読み込む
	soundHandle_ = engine_->LoadSound("./Resources/Sounds/Bgm/Addictive_Waveform.mp3");


	// 平行光源の生成と初期化
	directionalLight_ = std::make_unique<DirectionalLight>();
	directionalLight_->Initialize();

	// 天球の生成と初期化
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(engine_, camera3d_.get());

	// プレイヤーの生成と初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(engine_, camera3d_.get());
	player_->SetGameScene(this);
}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update()
{
	// Scene更新
	Scene::Update();

	if (!engine_->IsSoundPlay(playHandle_) || playHandle_ == 0)
	{
		playHandle_ = engine_->PlaySoundData(soundHandle_ , 0.3f);
	}


	// 天球の更新
	skydome_->Update();

	// プレイヤーの更新
	player_->Update();
}

/// <summary>
/// 描画処理
/// </summary>
void GameScene::Draw()
{
	// 平行光源の設置
	engine_->SetDirectionalLight(directionalLight_.get());

	// 天球の描画
	skydome_->Draw();

	// プレイヤーの描画
	player_->Draw();

	engine_->SetOffscreenEffect(kBrightnessExtraction);
	engine_->SetOffscreenEffect(kGaussianFilter);
	engine_->SetOffscreenEffect(kHide);

	engine_->CopyRtvImage(0);

	engine_->SetCopyImageBlendMode(kBlendModeAdd);
	engine_->CopyRtvImage(2);
	engine_->SetCopyImageBlendMode(kBlendModeNormal);

	// Scene描画
	Scene::Draw();
}


/// <summary>
/// プレイヤーの弾を発射する
/// </summary>
void GameScene::PlayerBulletShot(std::unique_ptr<BasePlayerBullet> playerBullet)
{
	
}