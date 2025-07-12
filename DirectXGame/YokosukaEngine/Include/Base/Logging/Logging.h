#pragma once
#include <Windows.h>
#include <format>
#include <string>
#include <filesystem>
#include <fstream>
#include <chrono>

class Logging
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// ログを出力する
	/// </summary>
	/// <param name="message">メッセージ</param>
	void Log(const std::string& message);


private:

	/// <summary>
	/// ログファイルを生成する
	/// </summary>
	std::ofstream CreateLogFile();

	// ログファイルの情報
	std::ofstream logStream_;
};

