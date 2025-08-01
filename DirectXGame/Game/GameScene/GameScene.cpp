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

	// 平行光源の生成と初期化
	directionalLight_ = std::make_unique<DirectionalLight>();
	directionalLight_->Initialize();

	// 平面の生成と初期化
	plane_ = std::make_unique<Plane>();
	plane_->Initialize(engine_ , camera3d_.get());

	// スプライトの生成と初期化
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize(engine_ , camera2d_.get());

	// 球の生成と初期化
	sphere_ = std::make_unique<SphereModel>();
	sphere_->Initialize(engine_, camera3d_.get());

	// スザンヌの生成と初期化
	suzanne_ = std::make_unique<Suzanne>();
	suzanne_->Initialize(engine_, camera3d_.get());

	// スタンフォードバニーの生成と初期化
	stanfordBunny_ = std::make_unique<StanfordBunny>();
	stanfordBunny_->Initialize(engine_ , camera3d_.get());

	// ユタティーポットの生成と初期化
	utahTeapot_ = std::make_unique<UtahTeapot>();
	utahTeapot_->Initialize(engine_, camera3d_.get());

	// マルチメッシュの生成と初期化
	multiMesh_ = std::make_unique<MultiMesh>();
	multiMesh_->Initialize(engine_, camera3d_.get());

	// マルチマテリアルの生成と初期化
	multiMaterial_ = std::make_unique<MultiMaterial>();
	multiMaterial_->Initialize(engine_, camera3d_.get());


	// Bgmを読み込む
	soundHandle_ = engine_->LoadSound("./Resources/Sounds/Bgm/Jinro_No_Tameno_Komoriuta.mp3");
	playHandle_ = engine_->PlaySoundData(soundHandle_, 0.3f);
}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update()
{
	// Scene更新
	Scene::Update();

	ImGui::Begin("CG2");

	// 平面の更新
	plane_->Update();

	// スプライトの更新
	sprite_->Update();

	cosCave += 0.01f;
	directionalLight_->direction_.z = std::cos(cosCave);

	if (ImGui::TreeNode("Light"))
	{
		ImGui::SliderFloat3("direction", &directionalLight_->direction_.x, -1.0f, 1.0f);
		ImGui::ColorEdit4("color", &directionalLight_->color_.x);
		ImGui::SliderFloat("intensity", &directionalLight_->intensity_, 0.0f, 1.0f);
		ImGui::TreePop();
	}
	directionalLight_->direction_ = Normalize(directionalLight_->direction_);

	// 球の更新
	sphere_->Update();

	// スザンヌの更新
	suzanne_->Update();

	// スタンフォーダニーの更新
	stanfordBunny_->Update();

	// ユタティーポットの更新
	utahTeapot_->Update();

	// マルチメッシュの更新
	multiMesh_->Update();

	// マルチマテリアルの更新
	multiMaterial_->Update();

	// オーディオオプション
	if (ImGui::TreeNode("AudioOption"))
	{
		if (ImGui::Button("PlayAudio"))
		{
			if (!engine_->IsSoundPlay(playHandle_) || playHandle_ == 0)
			{
				playHandle_ = engine_->PlaySoundData(soundHandle_ , 0.3f);
			}
		}

		if (ImGui::Button("StopAudio"))
		{
			if (engine_->IsSoundPlay(playHandle_))
			{
				engine_->StopSound(playHandle_);
			}
		}

		ImGui::TreePop();
	}

	lightingSwithingTimer_ += 1.0f / 60.0f;
	lightingSwithingTimer_ = std::fmod(lightingSwithingTimer_, kLightingSwichingTime);

	if (lightingSwithingTimer_ < 2.0f)
	{
		enableLighting_ = false;
		enableHalfLambert_ = false;
	}
	else if (lightingSwithingTimer_ < 4.0f)
	{
		enableLighting_ = true;
		enableHalfLambert_ = false;
	}
	else if (lightingSwithingTimer_ <= 6.0f)
	{
		enableLighting_ = true;
		enableHalfLambert_ = true;
	}

	// ライティングオプション
	if (ImGui::TreeNode("LightingOption"))
	{
		// ライティング無効
		if (ImGui::Button("DisableLight"))
		{
			enableLighting_ = false;
			enableHalfLambert_ = false;
			lightingSwithingTimer_ = 0.0f;
		}

		// ランバーテインリフレクタンス
		if (ImGui::Button("LambertainReflectance"))
		{
			enableLighting_ = true;
			enableHalfLambert_ = false;
			lightingSwithingTimer_ = 2.0f;
		}

		// ハーフランバート
		if (ImGui::Button("HalfLambert"))
		{
			enableLighting_ = true;
			enableHalfLambert_ = true;
			lightingSwithingTimer_ = 4.0f;
		}

		ImGui::TreePop();
	}

	ImGui::End();
}

/// <summary>
/// 描画処理
/// </summary>
void GameScene::Draw()
{
	// Scene描画
	Scene::Draw();

	// 平行光源を設置する
	engine_->SetDirectionalLight(directionalLight_.get());

	// 平面の描画
	plane_->Draw(enableLighting_ , enableHalfLambert_);

	// スプライトの描画
	sprite_->Draw();

	// 球の描画
	sphere_->Draw(enableLighting_, enableHalfLambert_);

	// スザンヌの描画
	suzanne_->Draw(enableLighting_, enableHalfLambert_);

	// スタンフォードバニーの描画
	stanfordBunny_->Draw(enableLighting_, enableHalfLambert_);

	// ユタティーポットの描画
	utahTeapot_->Draw(enableLighting_, enableHalfLambert_);

	// マルチメッシュの描画
	multiMesh_->Draw(enableLighting_, enableHalfLambert_);

	// マルチマテリアルの描画
	multiMaterial_->Draw(enableLighting_, enableHalfLambert_);
}