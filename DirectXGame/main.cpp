#include "YokosukaEngine/Include/YokosukaEngine.h"

// Windowsアプリでのエントリーポイント（main関数）
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// エンジンの生成と初期化
	YokosukaEngine* yokosukaEngine = new YokosukaEngine();
	yokosukaEngine->Initialize(1280, 720, "LE2A_11_フクダ_ソウワ");

	int x = 0;
	int y = 0;
	int z = 0;


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

		ImGui::Begin("mouse");
		ImGui::Text("x : %d , y : %d , z : %d", x, y, z);
		ImGui::End();

		if (yokosukaEngine->GetMouseButtonRelease(kMouseButtonLeft))
		{
			x++;
		}

		if (yokosukaEngine->GetMouseButtonRelease(kMouseButtonRight))
		{
			y++;
		}

		if (yokosukaEngine->GetMouseButtonRelease(kMouseButtonCenter))
		{
			z++;
		}

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