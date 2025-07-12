#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxcapi.h>
#include <cassert>
#include "../../Base/Logging/Logging.h"
#include "../../Func/String/String.h"

#pragma comment(lib , "d3d12.lib")
#pragma comment(lib , "dxgi.lib")
#pragma comment(lib , "dxcompiler.lib")

class DirectXShaderCompiler
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="log">ログ</param>
	void Initialize(Logging* logging);

	/// <summary>
	/// シェーダをコンパイルする
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <param name="profile">プロフィール</param>
	IDxcBlob* CompileShader(const std::wstring& filePath, const wchar_t* profile);


private:

	// ロギング
	Logging* logging_ = nullptr;

	// DXCユーティリティ
	IDxcUtils* dxcUtils_ = nullptr;

	// コンパイラ
	IDxcCompiler3* dxcCompiler_ = nullptr;

	// インクルードハンドラ
	IDxcIncludeHandler* includeHandler_ = nullptr;
};

