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

	// テクスチャ
	uint32_t textureHandle = yokosukaEngine->LoadTexture("./Resources/Textures/uvChecker.png");
	uint32_t textureHandle1 = yokosukaEngine->LoadTexture("./Resources/Textures/uvChecker.png");

	// カメラ
	Camera3D* camera = new Camera3D();
	camera->Initialize(1280.0f , 720.0f);
	camera->translation_.z = -10.0f;


	// メインループ
	while (yokosukaEngine->ProcessMessage())
	{
		// フレーム開始
		yokosukaEngine->BeginFrame();

		///
		/// ↓ 更新処理ここから
		/// 

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

		// 球を描画する
		yokosukaEngine->DrawSphere(worldTransform, uvTransform, camera, textureHandle, { 1.0f , 1.0f , 1.0f , 1.0f });

		///
		/// ↑ 描画処理ここまで
		/// 

		// フレーム終了
		yokosukaEngine->EndFrame();
	}

	// エンジンの解放
	delete yokosukaEngine;

	return 0;
}