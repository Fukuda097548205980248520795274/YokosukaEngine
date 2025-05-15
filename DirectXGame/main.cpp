#include "YokosukaEngine/Include/YokosukaEngine.h"
#include "Class/GameScene/GameScene.h"

// Windowsアプリでのエントリーポイント（main関数）
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// エンジンの生成と初期化
	YokosukaEngine* yokosukaEngine = new YokosukaEngine();
	yokosukaEngine->Initialize(1280, 720, "LE2A_11_フクダ_ソウワ_AL3");

	// ゲームシーンの生成と初期化
	GameScene* gameScene = new GameScene();
	gameScene->Initialize(yokosukaEngine);

	// メインループ
	while (yokosukaEngine->ProcessMessage())
	{
		// フレーム開始
		yokosukaEngine->BeginFrame();

		///
		/// ↓ 更新処理ここから
		/// 

		// ゲームシーン更新
		gameScene->Update();

		///
		/// ↑ 更新処理ここまで
		/// 

		///
		/// ↓ 描画処理ここから
		/// 

		// ゲームシーン描画
		gameScene->Draw();

		///
		/// ↑ 描画処理ここまで
		/// 

		// フレーム終了
		yokosukaEngine->EndFrame();
	}

	// ゲームシーン解放
	delete gameScene;

	// エンジンの解放
	delete yokosukaEngine;

	return 0;
}