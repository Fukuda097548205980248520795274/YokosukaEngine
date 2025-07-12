#include "Logging.h"

/// <summary>
/// 初期化
/// </summary>
void Logging::Initialize()
{
	// ログのディレクトリを用意する
	std::filesystem::create_directory("YokosukaEngine/Output/Logs");

	// ログファイルを生成する
	logStream_ = CreateLogFile();
}

/// <summary>
/// ログを出力する
/// </summary>
/// <param name="message">メッセージ</param>
void Logging::Log(const std::string& message)
{
	logStream_ << message << std::endl;
	OutputDebugStringA(message.c_str());
}

/// <summary>
/// ログファイルを生成する
/// </summary>
std::ofstream Logging::CreateLogFile()
{
	// 現在時刻を取得（UTC時刻）
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

	// ログファイルの名前にコンマ何秒はいらないので、削って秒にする
	std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds> nowSeconds =
		std::chrono::time_point_cast<std::chrono::seconds>(now);

	// 日本時間（PCの設定時間）に変換
	std::chrono::zoned_time localTime{ std::chrono::current_zone(), nowSeconds };

	// formatを使って年月日_時分秒の文字列に変換
	std::string dateString = std::format("{:%Y%m%d_%H%M%S}", localTime);

	// 時刻を使ってファイル名を決定
	std::string logFilePath = std::string("YokosukaEngine/Output/Logs/") + dateString + ".log";

	// ファイルを作って書き込み準備
	std::ofstream logStream(logFilePath);

	return logStream;
}