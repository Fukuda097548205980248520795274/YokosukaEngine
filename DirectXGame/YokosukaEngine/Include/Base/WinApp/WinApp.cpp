#include "WinApp.h"

// ウィンドウプロシージャ
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	// ImGuiのカーソル操作
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
	{
		return true;
	}

	// メッセージに応じて固有の処理を行う
	switch (msg)
	{
		// ウィンドウが破棄された
	case WM_DESTROY:

		// OSに対してアプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	// 標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

/// <summary>
/// デストラクタ
/// </summary>
WinApp::~WinApp()
{
	// ウィンドウハンドルを閉じる
	CloseWindow(hwnd_);
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="kWindowWidth">ウィンドウの横幅</param>
/// <param name="kWindowHeight">ウィンドウの縦幅</param>
void WinApp::Initialize(const int32_t kWindowWidth, const int32_t kWindowHeight , const std::wstring& titleName)
{
	/*-----------------------------
	    ウィンドウクラスを登録する
	-----------------------------*/

	// ウィンドウプロシージャ
	wc_.lpfnWndProc = WindowProc;

	// ウィンドウクラス名
	wc_.lpszClassName = L"DirectXGame";

	// インスタンスハンドル
	wc_.hInstance = GetModuleHandle(nullptr);

	// カーソル
	wc_.hCursor = LoadCursor(nullptr, IDC_ARROW);

	// ウィンドウクラスを登録する
	RegisterClass(&wc_);


	/*--------------------------
	   ウィンドウサイズを決める
	--------------------------*/

	// クライアント領域のサイズ
	clientWidth_ = kWindowWidth;
	clientHeight_ = kWindowHeight;

	// ウィンドウサイズを表す構造体にクライアント領域を入れる
	wrc_ = { 0,0,clientWidth_,clientHeight_ };

	// クライアント領域を元に実際のサイズにwrcを変更してもらう
	AdjustWindowRect(&wrc_, WS_OVERLAPPEDWINDOW, false);


	/*------------------------------
	    ウィンドウを生成して表示する
	------------------------------*/

	// ウィンドウハンドルの設定
	hwnd_ = CreateWindow(
		// 利用するクラス名
		wc_.lpszClassName,

		// タイトルバーの文字
		titleName.c_str(),

		// よく見るウィンドウスタイル
		WS_OVERLAPPEDWINDOW,

		// 表示X座標
		CW_USEDEFAULT,

		// 表示Y座標
		CW_USEDEFAULT,

		// ウィンドウの横幅
		wrc_.right - wrc_.left,

		// ウィンドウの縦幅
		wrc_.bottom - wrc_.top,

		// 親ウィンドウハンドル
		nullptr,

		// メニューハンドル
		nullptr,

		// インスタンスハンドル
		wc_.hInstance,

		// オプション
		nullptr
	);

	// ウィンドウを表示する
	ShowWindow(hwnd_, SW_SHOW);
}

/// <summary>
/// メインループ
/// </summary>
bool WinApp::ProcessMessage()
{
	// ウィンドウのxボタンを押されるまでループ
	while (msg_.message != WM_QUIT)
	{
		// ウィンドウにメッセージが来てたら最優先で処理させる
		if (PeekMessage(&msg_, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg_);
			DispatchMessage(&msg_);
		}
		else
		{
			// ゲーム処理
			
			return true;
		}
	}

	return false;
}