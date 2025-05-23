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

	// ワールドトランスフォーム
	WorldTransform* worldTransform = new WorldTransform();
	worldTransform->Initialize();

	// UVトランスフォーム
	WorldTransform* uvTransform = new WorldTransform();
	uvTransform->Initialize();

	// カメラ
	Camera3D* camera3d = new Camera3D();
	camera3d->Initialize(1280.0f , 720.0f);

	// テクスチャハンドル
	uint32_t textureHandle = yokosukaEngine->LoadTexture("./Resources/Textures/uvChecker.png");


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

		yokosukaEngine->DebugCameraUpdate();
		camera3d->UpdateDebugCameraData(yokosukaEngine->GetDebugCameraInstance());

		worldTransform->UpdateWorldMatrix();
		uvTransform->UpdateWorldMatrix();

		///
		/// ↑ 更新処理ここまで
		/// 

		///
		/// ↓ 描画処理ここから
		/// 

		yokosukaEngine->DrawTriangle(worldTransform, uvTransform, camera3d, textureHandle, { 1.0f , 1.0f, 1.0f , 1.0f });

		///
		/// ↑ 描画処理ここまで
		/// 

		// フレーム終了
		yokosukaEngine->EndFrame();

		// 全てのキーの入力情報をコピーする
		yokosukaEngine->CopyAllKeyInfo();
	}

	delete camera3d;
	delete uvTransform;
	delete worldTransform;


	// エンジンの解放
	delete yokosukaEngine;

	return 0;
}