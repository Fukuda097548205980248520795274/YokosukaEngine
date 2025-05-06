#include "../Externals/YokosukaEngine/Include/YokosukaEngine.h"

// Windowsアプリでのエントリーポイント（main関数）
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// エンジンの生成と初期化
	YokosukaEngine* yokosukaEngine = new YokosukaEngine();
	yokosukaEngine->Initialize(1280, 720);

	// メインループ
	while (yokosukaEngine->ProcessMessage())
	{

	}

	// エンジンの解放
	delete yokosukaEngine;

	return 0;
}