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

	// カメラ
	Camera3D* camera3d = new Camera3D();
	camera3d->Initialize(1280.0f , 720.0f);

	// モデルハンドル
	uint32_t modelHandle = yokosukaEngine->LoadModelData("./Resources/Models/Suzanne" , "Suzanne.obj");


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

		ImGui::Begin("Suzanne");
		ImGui::ColorEdit4("color", &color.x);
		ImGui::End();

		yokosukaEngine->DebugCameraUpdate();
		camera3d->UpdateDebugCameraData(yokosukaEngine->GetDebugCameraInstance());

		///
		/// ↑ 更新処理ここまで
		/// 

		///
		/// ↓ 描画処理ここから
		/// 

		yokosukaEngine->DrawParticle( camera3d, modelHandle, color);

		///
		/// ↑ 描画処理ここまで
		/// 

		// フレーム終了
		yokosukaEngine->EndFrame();

		// 全てのキーの入力情報をコピーする
		yokosukaEngine->CopyAllKeyInfo();
	}

	delete camera3d;


	// エンジンの解放
	delete yokosukaEngine;

	return 0;
}