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
#include <list>
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
	uint32_t SelectHandlePlayAudio(uint32_t soundHandle, float soundVolume);

	/// <summary>
	/// 音声を停止する
	/// </summary>
	/// <param name="playHandle"></param>
	void StopSound(uint32_t playHandle);

	/// <summary>
	/// 音量を設定する
	/// </summary>
	/// <param name="playHandle"></param>
	/// <param name="setVolume"></param>
	void SetVolume(uint32_t playHandle, float setVolume);

	/// <summary>
	/// 音楽が再生されているかどうか
	/// </summary>
	/// <param name="playHandle"></param>
	bool IsSoundPlay(uint32_t playHandle);

	/// <summary>
	/// 停止した曲を削除する
	/// </summary>
	void DeleteStopAudio();


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
	uint32_t SoundPlayWave(uint32_t index, float soundVolume);

	// XAudio2
	Microsoft::WRL::ComPtr<IXAudio2> xAudio2_ = nullptr;

	// マスターボイス
	IXAudio2MasteringVoice* masterVoice_ = nullptr;

	// ロードに使用する構造体
	struct LoadAudioStructure
	{
		// ファイルパス
		std::string filePath;

		// フォーマット
		WAVEFORMATEX* waveFormat;

		// メディアデータ
		std::vector<BYTE> mediaData;

		// サウンドハンドル
		uint32_t soundHandle;
	};

	// サウンドプレイに使用する構造体
	class SoundPlayStructure
	{
	public:

		// プレイハンドル
		uint32_t playHandle;

		// ソースボイス
		IXAudio2SourceVoice* pSourceVoice;
	};


	// ロードに使用する構造体
	LoadAudioStructure loadAudioStructure_[512] = {};

	// サウンドプレイに使用する構造体
	std::list<SoundPlayStructure*> soundPlayStructure_;


	// 使用した音声データ数
	uint32_t useNumSoundData_ = 0;
};