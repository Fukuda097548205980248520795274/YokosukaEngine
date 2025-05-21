#include "AudioStore.h"

/// <summary>
/// デストラクタ
/// </summary>
AudioStore::~AudioStore()
{
	xAudio2_.Reset();

	// 音声データを消す
	for (uint32_t i = 0; i < useNumSoundData_; i++)
	{
		SoundUnload(&soundData_[i]);
	}
}

/// <summary>
/// 初期化
/// </summary>
void AudioStore::Initialize()
{
	// XAudio2を初期化する
	HRESULT hr = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(hr));

	// マスターボイスを生成する
	hr = xAudio2_->CreateMasteringVoice(&masterVoice_);
	assert(SUCCEEDED(hr));
}

/// <summary>
/// 音声データ読み込み
/// </summary>
/// <param name="fileName"></param>
/// <returns></returns>
SoundData AudioStore::SoundLoadWave(const std::string& fileName)
{
	/*--------------------
	    ファイルオープン
	--------------------*/

	// ファイル入力ストリームのインスタンス
	std::ifstream file;

	// .wavファイルをバイナリモードで開く
	file.open(fileName.c_str(), std::ios_base::binary);
	assert(file.is_open());


	/*-----------------------
	    .wavデータ読み込み
	-----------------------*/

	// RIFFヘッダー読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));

	// ファイルがRIFFかチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)
	{
		assert(false);
	}

	// タイプがWAVEかチェック
	if (strncmp(riff.type, "WAVE", 4) != 0)
	{
		assert(false);
	}


	// FMTチャンクの読み込み
	formatChunk format = {};

	// チャンクヘッダーの確認
	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0)
	{
		assert(false);
	}

	// チャンク本体の読み込み
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	
	// Dataチャンクの読み込み
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));

	// JUNKチャンクを検出した場合
	if (strncmp(data.id, "JUNK", 4) == 0)
	{
		// 読み取り位置をJUNKチャンクの終わりまで進める
		file.seekg(data.size, std::ios_base::cur);

		// 再読み込み
		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data", 4) != 0)
	{
		assert(false);
	}

	// Dataチャンクのデータ部（波形データ）の読み込み
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	// Waveファイルを閉じる
	file.close();


	/*----------------------------
	    読み込んだ音声データを返却
	----------------------------*/

	SoundData soundData = {};

	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;

	return soundData;
}

/// <summary>
/// 音声データを解放する
/// </summary>
/// <param name="soundData"></param>
void AudioStore::SoundUnload(SoundData* soundData)
{
	// バッファのメモリ開放
	delete[] soundData->pBuffer;

	soundData->pBuffer = 0;
	soundData->bufferSize = 0;
	soundData->wfex = {};
}

/// <summary>
/// 音声を再生する
/// </summary>
/// <param name="soundData"></param>
void AudioStore::SoundPlayWave(const SoundData& soundData)
{
	// 波形フォーマットを元にSourceVoiceを生成
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	HRESULT hr = xAudio2_->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(hr));

	// 再生する波形のデータの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;

	// 波形データのサイズ
	hr = pSourceVoice->SubmitSourceBuffer(&buf);
	assert(SUCCEEDED(hr));

	hr = pSourceVoice->Start();
	assert(SUCCEEDED(hr));
}

/// <summary>
/// ファイル名でサウンドハンドルを取得する
/// </summary>
/// <param name="filePath"></param>
/// <returns></returns>
uint32_t AudioStore::GetSoundHandle(const std::string& filePath)
{
	// 同じ音声データを探す
	for (uint32_t i = 0; i < useNumSoundData_; i++)
	{
		if (strcmp(filePath.c_str(), filePath_[i].c_str()) == 0)
		{
			return soundHandle_[i];
		}
	}

	// テクスチャハンドルを作成する
	while (soundHandle_[useNumSoundData_] == 0)
	{
		soundHandle_[useNumSoundData_] = rand() % 1000000 + 1;

		for (uint32_t i = 0; i < useNumSoundData_; i++)
		{
			if (soundHandle_[useNumSoundData_] == soundHandle_[i])
			{
				soundHandle_[useNumSoundData_] = 0;
				break;
			}
		}
	}

	// ファイルパス
	filePath_[useNumSoundData_] = filePath;

	// 読み込む
	soundData_[useNumSoundData_] = SoundLoadWave(filePath);

	// 使用した音声データをカウントする
	useNumSoundData_++;

	return soundHandle_[useNumSoundData_ - 1];
}

/// <summary>
/// 指定したハンドルで音声データを流す
/// </summary>
/// <param name="soundHandle"></param>
void AudioStore::SelectHandlePlayAudio(uint32_t soundHandle)
{
	for (uint32_t i = 0; i < useNumSoundData_; i++)
	{
		if (soundHandle == soundHandle_[i])
		{
			SoundPlayWave(soundData_[i]);

			return;
		}
	}

	assert(false);
	return;
}