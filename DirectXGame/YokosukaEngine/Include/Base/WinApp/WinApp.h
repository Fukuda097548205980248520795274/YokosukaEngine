#pragma once
#include <Windows.h>
#include <cstdint>
#include <dxgidebug.h>
#include <string>
#include "../../../../Externals/ImGui/imgui.h"
#include "../../../../Externals/ImGui/imgui_impl_dx12.h"
#include "../../../../Externals/ImGui/imgui_impl_win32.h"

#pragma comment(lib,"dxguid.lib")

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

class WinApp
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~WinApp();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="kWindowWidth">ウィンドウの横幅</param>
	/// <param name="kWindowHeight">ウィンドウの縦幅</param>
	/// <param name="titleName">ウィンドウのタイトル名</param>
	void Initialize(const int32_t kWindowWidth , const int32_t kWindowHeight, const std::wstring& titleName);

	/// <summary>
	/// メッセージを処理する
	/// </summary>
	bool ProcessMessage();

	/// <summary>
	/// 画面の横幅のGetter
	/// </summary>
	/// <returns></returns>
	int32_t GetWindowWidth() { return clientWidth_; }

	/// <summary>
	/// 画面の縦幅のGetter
	/// </summary>
	/// <returns></returns>
	int32_t GetWindowHeight() { return clientHeight_; }

	/// <summary>
	/// ウィンドウハンドルのGetter
	/// </summary>
	/// <returns></returns>
	HWND GetHwnd() { return hwnd_; }

	/// <summary>
	/// ウィンドウクラスのGetter
	/// </summary>
	/// <returns></returns>
	WNDCLASS GetWndClass() { return wc_; }


private:

	// ウィンドウクラス
	WNDCLASS wc_{};

	// クライアント領域の横幅
	int32_t clientWidth_ = 0;

	// クライアント領域の縦幅
	int32_t clientHeight_ = 0;

	// ウィンドウズサイズ
	RECT wrc_{};

	// ウィンドウハンドル
	HWND hwnd_{};

	// メッセージ
	MSG msg_{};
};

