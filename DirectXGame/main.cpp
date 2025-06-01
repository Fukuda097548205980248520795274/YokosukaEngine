#include "YokosukaEngine/Include/YokosukaEngine.h"

// Windowsアプリでのエントリーポイント（main関数）
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// エンジンの生成と初期化
	YokosukaEngine* yokosukaEngine = new YokosukaEngine();
	yokosukaEngine->Initialize(1280, 720, "LE2A_11_フクダ_ソウワ");


	/*--------------
	    変数を作る
	--------------*/

	// 色
	Vector4 color = { 1.0f , 1.0f , 1.0f , 1.0f };

	// ワールドトランスフォーム
	WorldTransform* worldTransform = new WorldTransform();
	worldTransform->Initialize();
	worldTransform->scale_ = { 6.0f , 6.0f , 6.0f };

	// UVトランスフォーム
	WorldTransform* uvTransform = new WorldTransform();
	uvTransform->Initialize();

	// カメラ
	Camera3D* camera3d = new Camera3D();
	camera3d->Initialize(1280.0f , 720.0f);

	// 平行光源
	DirectionalLight directionalLight;
	directionalLight.color = { 1.0f , 1.0f ,1.0f , 1.0f };
	directionalLight.direction = { 0.0f , -1.0f , 0.0f };
	directionalLight.intensity = 1.0f;

	// 点光源
	PointLight pointLight;
	pointLight.color = { 1.0f , 1.0f , 1.0f , 1.0f };
	pointLight.position = { 0.0f , 2.0f , 0.0f };
	pointLight.intensity = 0.0f;
	pointLight.radius = 2.0f;
	pointLight.decay = 1.0f;

	// スポットライト
	SpotLight spotLight;
	spotLight.color = { 1.0f , 1.0f , 1.0f , 1.0f };
	spotLight.position = { 2.0f , 1.25f , 0.0f };
	spotLight.distance = 7.0f;
	spotLight.direction = Normalize({ -1.0f , -1.0f , 0.0f });
	spotLight.intensity = 4.0f;
	spotLight.decay = 2.0f;
	spotLight.cosAngle = std::cos(std::numbers::pi_v<float> / 3.0f);
	spotLight.fallofStart = 1.0f;

	// モデル
	uint32_t modelHandle = yokosukaEngine->LoadModelData("./Resources/Models/terrain" , "terrain.obj");


	// メインループ
	while (yokosukaEngine->ProcessMessage())
	{
		// 全キーの入力情報を取得する
		yokosukaEngine->CheckAllKeyInfo();

		// フレーム開始
		yokosukaEngine->BeginFrame();

		///
		/// ↓ 更新処理ここから
		/// 

		ImGui::Begin("Sphere");
		ImGui::DragFloat3("translation", &worldTransform->translation_.x, 0.1f);
		ImGui::DragFloat3("rotation", &worldTransform->rotation_.x, 0.01f);
		ImGui::DragFloat3("scale", &worldTransform->scale_.x, 0.01f);
		ImGui::End();
		
		ImGui::Begin("PointLight");
		ImGui::ColorEdit4("Color", &pointLight.color.x);
		ImGui::DragFloat3("Position", &pointLight.position.x, -1.0f, 1.0f);
		ImGui::DragFloat("Intensity", &pointLight.intensity, 0.1f);
		ImGui::DragFloat("Radius", &pointLight.radius, 0.1f);
		ImGui::DragFloat("Decay", &pointLight.decay, 0.01f);
		ImGui::End();

		ImGui::Begin("SpotLight");
		ImGui::DragFloat3("position", &spotLight.position.x, 0.1f);
		ImGui::DragFloat("intensity", &spotLight.intensity, 0.1f);
		ImGui::DragFloat("decay", &spotLight.decay, 0.1f);
		ImGui::DragFloat("cosAngle", &spotLight.cosAngle, 0.01f);
		ImGui::DragFloat("distance", &spotLight.distance, 0.01f);
		ImGui::DragFloat3("direction", &spotLight.direction.x, 0.01f);
		ImGui::DragFloat("fallofStart", &spotLight.fallofStart, 0.01f);
		ImGui::End();

		ImGui::Begin("camera");
		ImGui::DragFloat3("translation", &camera3d->translation_.x, 0.1f);
		ImGui::DragFloat3("rotation", &camera3d->rotation_.x, 0.01f);
		ImGui::End();

		// カメラ更新
		
		camera3d->UpdateMatrix();

		worldTransform->UpdateWorldMatrix();
		uvTransform->UpdateWorldMatrix();

		///
		/// ↑ 更新処理ここまで
		/// 

		///
		/// ↓ 描画処理ここから
		/// 

		yokosukaEngine->DrawModel(worldTransform, uvTransform, camera3d, modelHandle, color , directionalLight, pointLight, spotLight);

		///
		/// ↑ 描画処理ここまで
		/// 

		// フレーム終了
		yokosukaEngine->EndFrame();

		// 全てのキーの入力情報をコピーする
		yokosukaEngine->CopyAllKeyInfo();
	}

	delete uvTransform;
	delete worldTransform;
	delete camera3d;


	// エンジンの解放
	delete yokosukaEngine;

	return 0;
}