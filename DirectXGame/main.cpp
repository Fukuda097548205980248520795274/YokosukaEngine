#include "YokosukaEngine/Include/YokosukaEngine.h"
#include "Game/Game.h"
#include "Game/GameScene/GlobalVariables/GlobalVariables.h"

// Windowsアプリでのエントリーポイント（main関数）
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// エンジンの生成と初期化
	YokosukaEngine* engine = new YokosukaEngine();
	engine->Initialize(1280, 720, "LE2A_11_フクダ_ソウワ");

	// ゲームインスタンスの生成と初期化
	Game* game = new Game();
	game->Initialize(engine);


	// メインループ
	while (engine->ProcessMessage())
	{
		// 全キーの入力情報を取得する
		engine->CheckAllKeyInfo();

		// オーディオストアの更新処理
		engine->UpdateAudioStore();

		// フレーム開始
		engine->BeginFrame();

		///
		/// ↓ 更新処理ここから
		/// 

		// ゲームの更新
		game->Update();

		// 調整項目 ImGui
		GlobalVariables::GetInstance()->Update();

		///
		/// ↑ 更新処理ここまで
		/// 

		///
		/// ↓ 描画処理ここから
		/// 

		// ゲームの描画
		game->Draw();

		///
		/// ↑ 描画処理ここまで
		/// 

		// フレーム終了
		engine->EndFrame();

		// 全てのキーの入力情報をコピーする
		engine->CopyAllKeyInfo();
	}

	// ゲームの解放
	delete game;
	game = nullptr;

	// エンジンの解放
	delete engine;
	engine = nullptr;

	return 0;
}