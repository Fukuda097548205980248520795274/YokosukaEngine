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
	DirectionalLight directionalLight;
	directionalLight.color = { 1.0f , 1.0f , 1.0f ,1.0f };
	directionalLight.direction = Normalize({ 0.0f , -1.0f , 1.0f });
	directionalLight.intensity = 1.0f;

	// ポイントライト
	PointLight pointLight;
	pointLight.intensity = 1.0f;
	pointLight.color = { 1.0f , 1.0f , 1.0f , 1.0f };
	pointLight.position = { -2.0f , 2.0f , 0.0f};
	pointLight.radius = 32.0f;
	pointLight.decay = 2.0f;

	// スポットライト
	SpotLight spotLight;
	spotLight.intensity = 0.0f;
	spotLight.color = { 1.0f , 1.0f , 1.0f , 1.0f };
	spotLight.direction = { 0.0f , -1.0f , 0.0f };
	spotLight.fallofStart = 0.0f;
	spotLight.cosAngle = 2.0f;
	spotLight.decay = 2.0f;
	spotLight.position = { 0.0f , 10.0f , -10.0f };
	spotLight.distance = 32.0f;

	// モデル
	uint32_t modelHandle = engine->LoadModelData("./Resources/Models/UtahTeapot", "UtahTeapot.gltf");

	// bgm
	uint32_t soundHandle1 = engine->LoadSound("./Resources/Sounds/Bgm/oboreruKaiba.mp3");
	uint32_t soundHandle2 = engine->LoadSound("./Resources/Sounds/Bgm/ZinroNoTameNoKomoriuta.mp3");
	uint32_t soundHandle3 = engine->LoadSound("./Resources/Sounds/Bgm/oboreruKaiba.mp3");

	engine->PlayerSoundData(soundHandle2 , 0.3f);


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
		
		ImGui::Begin("Plane");
		ImGui::DragFloat3("rotation", &worldTransform->rotation_.x, 0.01f);
		ImGui::End();

		ImGui::Begin("Camera");
		ImGui::DragFloat3("translation", &camera->translation_.x, 0.1f);
		ImGui::DragFloat3("rotation", &camera->rotation_.x, 0.01f);
		ImGui::End();


		camera->UpdateMatrix();

		worldTransform->UpdateWorldMatrix();
		uvTransform->UpdateWorldMatrix();

		if (engine->GetKeyTrigger(DIK_SPACE))
		{
			engine->PlayerSoundData(soundHandle3, 0.0f);
		}

		///
		/// ↑ 更新処理ここまで
		/// 

		///
		/// ↓ 描画処理ここから
		/// 

		engine->DrawModel(worldTransform.get(), uvTransform.get(), camera.get(), modelHandle, { 1.0f , 1.0f , 1.0f , 1.0f },
			directionalLight, pointLight, spotLight);

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