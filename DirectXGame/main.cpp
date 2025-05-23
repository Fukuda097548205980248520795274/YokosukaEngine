#include "YokosukaEngine/Include/YokosukaEngine.h"

// Windowsアプリでのエントリーポイント（main関数）
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// エンジンの生成と初期化
	YokosukaEngine* yokosukaEngine = new YokosukaEngine();
	yokosukaEngine->Initialize(1280, 720, "LE2A_11_フクダ_ソウワ");


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

		///
		/// ↑ 更新処理ここまで
		/// 

		///
		/// ↓ 描画処理ここから
		/// 

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