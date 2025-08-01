#include "DirectXShaderCompiler.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="log">ログ</param>
void DirectXShaderCompiler::Initialize(Logging* logging)
{
	// nullptrチェック
	assert(logging);

	// 引数を受け取る
	logging_ = logging;

	// DXCユーティリティの初期化
	HRESULT hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils_));
	assert(SUCCEEDED(hr));

	// コンパイラの初期化
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler_));
	assert(SUCCEEDED(hr));

	// インクルードハンドラーの初期化
	hr = dxcUtils_->CreateDefaultIncludeHandler(&includeHandler_);
	assert(SUCCEEDED(hr));

	// 初期化成功のログを出力
	logging_->Log("Succeeded DXC Initialize \n");
}

/// <summary>
/// シェーダをコンパイルする
/// </summary>
/// <param name="filePath">ファイルパス</param>
/// <param name="profile">プロフィール</param>
IDxcBlob* DirectXShaderCompiler::CompileShader(const std::wstring& filePath, const wchar_t* profile)
{
	/*--------------------------
	    HLSLファイルを読み込む
	--------------------------*/

	// これからシェーダをコンパイルする旨をログに出す
	logging_->Log(ConvertString(std::format(L"Begin CompileShader , path : {} , profile : {} \n", filePath, profile)));

	// HLSLファイルを読む
	IDxcBlobEncoding* shaderSource = nullptr;
	HRESULT hr = dxcUtils_->LoadFile(filePath.c_str(), nullptr, &shaderSource);
	assert(SUCCEEDED(hr));

	// 読み込んだファイルの内容を設定する
	DxcBuffer shaderSourceBuffer;
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();

	// UTF8の文字コードであることを通知する
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;


	/*-------------------
	    コンパイルする
	-------------------*/

	// コンパイルオプション
	LPCWSTR arguments[] = {
		// コンパイル対象のHLSLファイル名
		filePath.c_str(),

		// エントリーポイントの指定
		L"-E" , L"main" ,

		// シェーダプロフィールの設定
		L"-T" , profile ,

		// デバッグ用の情報を埋め込む
		L"-Zi" , L"-Qembed_debug" ,

		// 最適化を外しておく
		L"-Od" ,

		// メモリレイアウトは行優先
		L"-Zpr"
	};

	// 実際にシェーダをコンパイルする
	IDxcResult* shaderResult = nullptr;
	hr = dxcCompiler_->Compile(
		// 読み込んだファイル
		&shaderSourceBuffer,

		// コンパイルオプション
		arguments,

		// コンパイルオプションの数
		_countof(arguments),

		// インクルードが含まれた諸々
		includeHandler_,

		// コンパイル結果
		IID_PPV_ARGS(&shaderResult)
	);

	// DXCを起動できたかどうか
	assert(SUCCEEDED(hr));


	/*-----------------------------------
	    警告・エラーがでていないか確認する
	-----------------------------------*/

	// 警告・エラーが出てたらログに出して見せる
	IDxcBlobUtf8* shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if(shaderError != nullptr && shaderError->GetStringLength() != 0)
	{
		logging_->Log(shaderError->GetStringPointer());
		assert(false);
	}


	/*--------------------------------
	    コンパイル結果を受け取って返す
	--------------------------------*/

	// コンパイル結果から実行用のバイナリ部分を取得
	IDxcBlob* shaderBlob = nullptr;
	shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(hr));

	// 成功した旨をログに出す
	logging_->Log(ConvertString(std::format(L"Compile Succeeded , path : {} , profile : {} \n", filePath, profile)));

	// もう使わないリソースを解放する
	shaderSource->Release();
	shaderResult->Release();

	// 実行用のバイナリを返却する
	return shaderBlob;
}