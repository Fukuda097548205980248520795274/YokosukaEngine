#pragma once
#include <wrl.h>
#include <xaudio2.h>
#include <fstream>
#include <assert.h>
#include <stdlib.h>
#pragma comment(lib , "xaudio2.lib")

// チャンクヘッダ
typedef struct ChunkHeader
{
	// チャンク毎のID
	char id[4];

	// チャンクサイズ
	int32_t size;

}ChunkHeader;

// RIFFヘッダチャンク
typedef struct RiffHeader
{
	// "RIFF"
	ChunkHeader chunk;

	// "WAVE"
	char type[4];

}RiffHeader;

// FMTチャンク
typedef struct formatChunk
{
	// "fmt"
	ChunkHeader chunk;

	// 波形フォーマット
	WAVEFORMATEX fmt;
}formatChunk;

// 音声データ
typedef struct SoundData
{
	// 波形フォーマット
	WAVEFORMATEX wfex;

	// バッファの先頭アドレス
	BYTE* pBuffer;

	// バッファのサイズ
	unsigned int bufferSize;
}SoundData;

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
	void SelectHandlePlayAudio(uint32_t soundHandle);


private:

	/// <summary>
	/// 音声データ読み込み
	/// </summary>
	/// <param name="fileName"></param>
	/// <returns></returns>
	SoundData SoundLoadWave(const std::string& fileName);

	/// <summary>
	/// 音声データを解放する
	/// </summary>
	/// <param name="soundData"></param>
	void SoundUnload(SoundData* soundData);

	/// <summary>
	/// 音声を再生する
	/// </summary>
	/// <param name="soundData"></param>
	void SoundPlayWave(const SoundData& soundData);

	// XAudio2
	Microsoft::WRL::ComPtr<IXAudio2> xAudio2_ = nullptr;

	// マスターボイス
	IXAudio2MasteringVoice* masterVoice_ = nullptr;

	// ファイルパス
	std::string filePath_[512];

	// 音声データ
	SoundData soundData_[512];

	// サウンドハンドル
	uint32_t soundHandle_[512] = { 0 };

	// 使用した音声データ数
	uint32_t useNumSoundData_ = 0;
};