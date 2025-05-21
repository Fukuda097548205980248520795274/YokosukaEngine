#include "YokosukaEngine/Include/YokosukaEngine.h"

// Windowsアプリでのエントリーポイント（main関数）
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// エンジンの生成と初期化
	YokosukaEngine* yokosukaEngine = new YokosukaEngine();
	yokosukaEngine->Initialize(1280, 720, "LE2A_11_フクダ_ソウワ");


	/*----------------
	    変数を作る
	----------------*/

	// ワールドトランスフォーム
	WorldTransform* worldTransform = new WorldTransform();
	worldTransform->Initialize();

	// UVトランスフォーム
	WorldTransform* uvTransform = new WorldTransform();
	uvTransform->Initialize();

	// カメラ
	Camera3D* camera = new Camera3D();
	camera->Initialize(1280.0f , 720.0f);
	camera->translation_.z = -10.0f;

	// モデル
	uint32_t modelHandle = yokosukaEngine->LoadModelData("./Resources/Models/Suzanne", "Suzanne.obj");



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

		ImGui::Begin("Model");
		ImGui::DragFloat3("Scale", &worldTransform->scale_.x, 0.01f);
		ImGui::DragFloat3("Rotation", &worldTransform->rotation_.x, 0.01f);
		ImGui::DragFloat3("Translation", &worldTransform->translation_.x, 0.1f);
		ImGui::End();

		// カメラを更新する
		camera->UpdateMatrix();

		// トランスフォームを更新する
		worldTransform->UpdateWorldMatrix();
		uvTransform->UpdateWorldMatrix();

		///
		/// ↑ 更新処理ここまで
		/// 

		///
		/// ↓ 描画処理ここから
		/// 

		// モデルを描画する
		yokosukaEngine->DrawModel(worldTransform, uvTransform, camera, modelHandle, { 1.0f , 0.0f , 0.0f , 1.0f });

		///
		/// ↑ 描画処理ここまで
		/// 

		// フレーム終了
		yokosukaEngine->EndFrame();

		// 全てのキーの入力情報をコピーする
		yokosukaEngine->CopyAllKeyInfo();
	}

	// エンジンの解放
	delete yokosukaEngine;

	return 0;
}