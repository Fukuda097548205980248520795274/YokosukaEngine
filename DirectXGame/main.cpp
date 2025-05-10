#include "../Externals/YokosukaEngine/Include/YokosukaEngine.h"

// Windowsアプリでのエントリーポイント（main関数）
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// エンジンの生成と初期化
	YokosukaEngine* yokosukaEngine = new YokosukaEngine();
	yokosukaEngine->Initialize(1280, 720);


	/*--------------
	    変数を作る
	--------------*/

	// 図形
	WorldTransform* worldTransform = new WorldTransform();
	worldTransform->Initialize();
	worldTransform->scale_ = { 5.0f , 5.0f , 5.0f };

	// カメラ
	Camera* camera = new Camera();
	camera->Initialize(1280.0f , 720.0f);
	camera->translation_.z = -50.0f;


	// メインループ
	while (yokosukaEngine->ProcessMessage())
	{
		// フレーム開始
		yokosukaEngine->BeginFrame();

		///
		/// ↓ 更新処理ここから
		/// 

		ImGui::Begin("Window");
		ImGui::DragFloat3("translation", &worldTransform->translation_.x, 0.1f);
		ImGui::End();

		// 図形を回転させる
		worldTransform->rotation_.y += 0.03f;

		// ワールドトランスフォームを更新する
		worldTransform->UpdateWorldMatrix();

		// カメラを更新する
		camera->UpdateMatrix();


		///
		/// ↑ 更新処理ここまで
		/// 

		///
		/// ↓ 描画処理ここから
		/// 

		// 三角形を描画する
		yokosukaEngine->DrawTriangle(worldTransform, camera);

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