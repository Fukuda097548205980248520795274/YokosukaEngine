#pragma once
#include <Windows.h>
#include <strsafe.h>
#include <DbgHelp.h>
#pragma comment(lib,"Dbghelp.lib")

/// <summary>
/// CrashHandlerの設定
/// </summary>
/// <param name="exception"></param>
/// <returns></returns>
static LONG WINAPI ExportDump(EXCEPTION_POINTERS* exception)
{
	// 時刻を取得して、時刻を名前に入れたファイルを作成
	SYSTEMTIME time;
	GetLocalTime(&time);
	wchar_t filePath[MAX_PATH] = { 0 };
	CreateDirectory(L"YokosukaEngine/Output/Dumps", nullptr);
	StringCchPrintfW(filePath, MAX_PATH, L"YokosukaEngine/Output/Dumps/%04d-%02d%02d-%02d%02d.dmp",
		time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute);
	HANDLE dumpFileHandle = CreateFile(filePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);

	// processID（このexeのID）とクラッシュ（例外）の発生したthreadIDを取得
	DWORD processId = GetCurrentProcessId();
	DWORD threadId = GetCurrentThreadId();

	// 設定情報を入力
	MINIDUMP_EXCEPTION_INFORMATION minidumpInformation{ 0 };
	minidumpInformation.ThreadId = threadId;
	minidumpInformation.ExceptionPointers = exception;
	minidumpInformation.ClientPointers = TRUE;

	// Dumpを出力
	MiniDumpWriteDump(GetCurrentProcess(), processId, dumpFileHandle, MiniDumpNormal, &minidumpInformation, nullptr, nullptr);

	// 他に関連付けられているSEH例外ハンドラがあれば実行
	return EXCEPTION_EXECUTE_HANDLER;
}
