#include "YokosukaEngine/Include/YokosukaEngine.h"
#include "Game/Game.h"

// Windowsアプリでのエントリーポイント（main関数）
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// エンジンの生成と初期化
	YokosukaEngine* engine = new YokosukaEngine();
	engine->Initialize(1280, 720, "LE2A_11_フクダ_ソウワ_時を紡ぐ千早振り");

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

	// リソースリークチェッカー
	Microsoft::WRL::ComPtr<IDXGIDebug1> debug;
	if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug))))
	{
		debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
	}

	return 0;
}