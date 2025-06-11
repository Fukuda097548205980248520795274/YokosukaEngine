#pragma once
#define NOMINMAX
#include <wrl.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <xaudio2.h>
#include <fstream>
#include <assert.h>
#include <stdlib.h>
#include <vector>
#include "../../Func/String/String.h"

#pragma comment(lib, "Mf.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "Mfreadwrite.lib")
#pragma comment(lib, "mfuuid.lib")
#pragma comment(lib , "xaudio2.lib")


class AudioStore
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~AudioStore();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// ファイル名でサウンドハンドルを取得する
	/// </summary>
	/// <param name="fileName"></param>
	/// <returns></returns>
	uint32_t GetSoundHandle(const std::string& fileName);

	/// <summary>
	/// 指定したハンドルで音声データを再生する
	/// </summary>
	/// <param name="soundHandle"></param>
	void SelectHandlePlayAudio(uint32_t soundHandle, float soundVolume);


private:

	/// <summary>
	/// 音声データ読み込み
	/// </summary>
	/// <param name="fileName"></param>
	/// <returns></returns>
	void SoundLoadWave(const std::string& filePath);

	/// <summary>
	/// 音声を再生する
	/// </summary>
	/// <param name="soundData"></param>
	void SoundPlayWave(uint32_t index, float soundVolume);

	/// <summary>
	/// 音声を停止する
	/// </summary>
	/// <param name="index"></param>
	void SoundStopWave(uint32_t index);

	// XAudio2
	Microsoft::WRL::ComPtr<IXAudio2> xAudio2_ = nullptr;

	// マスターボイス
	IXAudio2MasteringVoice* masterVoice_ = nullptr;

	// ファイルパス
	std::string filePath_[512];

	// フォーマット
	WAVEFORMATEX* waveFormat_[512];


	std::vector<BYTE> mediaData_[512];

	// サウンドハンドル
	uint32_t soundHandle_[512] = { 0 };

	// 使用した音声データ数
	uint32_t useNumSoundData_ = 0;
};