#include "YokosukaEngine/Include/YokosukaEngine.h"

// Windowsアプリでのエントリーポイント（main関数）
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// エンジンの生成と初期化
	YokosukaEngine* engine = new YokosukaEngine();
	engine->Initialize(1280, 720, "LE2A_11_フクダ_ソウワ");


	/*----------------
	    変数を作る
	----------------*/

	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform = std::make_unique<WorldTransform>();
	worldTransform->Initialize();

	//  UVトランスフォーム
	std::unique_ptr<WorldTransform> uvTransform = std::make_unique<WorldTransform>();
	uvTransform->Initialize();

	// カメラ
	std::unique_ptr<Camera3D> camera = std::make_unique<Camera3D>();
	camera->Initialize(1280.0f , 720.0f);

	// 平行光源
	std::unique_ptr<DirectionalLight> directionalLight = std::make_unique<DirectionalLight>();
	directionalLight->Initialize();
	directionalLight->intensity_ = 0.0f;

	// ポイントライト
	std::unique_ptr<PointLight> pointLight = std::make_unique<PointLight>();
	pointLight->Initialize();
	pointLight->intensity_ = 0.0f;

	// スポットライト
	std::unique_ptr<SpotLight> spotLight = std::make_unique<SpotLight>();
	spotLight->Initialize();

	// モデル
	uint32_t modelHandle = engine->LoadModelData("./Resources/Models/terrain", "terrain.obj");

	// bgm
	uint32_t soundHandle1 = engine->LoadSound("./Resources/Sounds/Bgm/oboreruKaiba.mp3");
	uint32_t soundHandle2 = engine->LoadSound("./Resources/Sounds/Bgm/ZinroNoTameNoKomoriuta.mp3");
	uint32_t soundHandle3 = engine->LoadSound("./Resources/Sounds/Bgm/oboreruKaiba.mp3");

	engine->PlayerSoundData(soundHandle1 , 0.5f);


	// メインループ
	while (engine->ProcessMessage())
	{
		// 全キーの入力情報を取得する
		engine->CheckAllKeyInfo();

		// フレーム開始
		engine->BeginFrame();

		///
		/// ↓ 更新処理ここから
		/// 

		ImGui::Begin("spotLight");
		ImGui::DragFloat3("position", &spotLight->position_.x, 0.01f);
		ImGui::End();

		engine->DebugCameraUpdate();
		camera->UpdateDebugCameraData(engine->GetDebugCameraInstance());

		worldTransform->UpdateWorldMatrix();
		uvTransform->UpdateWorldMatrix();

		if (engine->GetKeyTrigger(DIK_SPACE))
		{
			engine->PlayerSoundData(soundHandle2, 0.3f);
		}

		///
		/// ↑ 更新処理ここまで
		/// 

		///
		/// ↓ 描画処理ここから
		/// 

		engine->DrawModel(worldTransform.get(), uvTransform.get(), camera.get(), modelHandle, { 1.0f , 1.0f , 1.0f , 1.0f },
			directionalLight.get(), pointLight.get(), spotLight.get());

		///
		/// ↑ 描画処理ここまで
		/// 

		// フレーム終了
		engine->EndFrame();

		// 全てのキーの入力情報をコピーする
		engine->CopyAllKeyInfo();
	}


	// エンジンの解放
	delete engine;

	return 0;
}