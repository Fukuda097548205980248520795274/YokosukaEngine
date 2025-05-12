#include "../Engine/Include/YokosukaEngine.h"

// Windowsアプリでのエントリーポイント（main関数）
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// エンジンの生成と初期化
	YokosukaEngine* yokosukaEngine = new YokosukaEngine();
	yokosukaEngine->Initialize(1280, 720);


	/*--------------
	    変数を作る
	--------------*/

	// uvChecker
	uint32_t ghUvChecker = yokosukaEngine->LoadTexture("./Resources/Textures/uvChecker.png");
	uint32_t ghMonsterBall = yokosukaEngine->LoadTexture("./Resources/Textures/monsterBall.png");

	// 図形
	WorldTransform* worldTransform = new WorldTransform();
	worldTransform->Initialize();
	worldTransform->scale_ = { 5.0f , 5.0f , 5.0f };

	// 3Dカメラ
	Camera3D* camera3d = new Camera3D();
	camera3d->Initialize(1280.0f , 720.0f);
	camera3d->translation_.z = -50.0f;

	bool flag = true;;


	// メインループ
	while (yokosukaEngine->ProcessMessage())
	{
		// フレーム開始
		yokosukaEngine->BeginFrame();

		///
		/// ↓ 更新処理ここから
		/// 

		ImGui::Begin("Triangle");
		ImGui::DragFloat3("translation", &worldTransform->translation_.x, 0.1f);
		ImGui::DragFloat3("rotatoin", &worldTransform->rotation_.x, 0.01f);
		if (ImGui::Button("texture"))
		{
			if (flag)
			{
				flag = false;
			}
			else
			{
				flag = true;
			}
		}
		ImGui::End();


		// ワールドトランスフォームを更新する
		worldTransform->UpdateWorldMatrix();

		// カメラを更新する
		camera3d->UpdateMatrix();


		///
		/// ↑ 更新処理ここまで
		/// 

		///
		/// ↓ 描画処理ここから
		/// 

		// テクスチャ切り替え
		if (flag)
		{
			// 三角形を描画する
			yokosukaEngine->DrawTriangle(worldTransform, camera3d, ghUvChecker);
		}
		else
		{
			// 三角形を描画する
			yokosukaEngine->DrawTriangle(worldTransform, camera3d, ghMonsterBall);
		}

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