#include "AudioStore.h"

/// <summary>
/// デストラクタ
/// </summary>
AudioStore::~AudioStore()
{
	for (uint32_t i = 0; i < useNumSoundData_; i++)
	{
		CoTaskMemFree(loadAudioStructure_[i].waveFormat);
	}

	MFShutdown();
	xAudio2_.Reset();
}

/// <summary>
/// 初期化
/// </summary>
void AudioStore::Initialize()
{
	MFStartup(MF_VERSION, MFSTARTUP_NOSOCKET);

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
void AudioStore::SoundLoadWave(const std::string& filePath)
{
	const std::wstring filePathW = ConvertString(filePath);
	IMFSourceReader* pMFSourceReader{ nullptr };
	MFCreateSourceReaderFromURL(filePathW.c_str(), NULL, &pMFSourceReader);


	IMFMediaType* pMFMediaType{ nullptr };
	MFCreateMediaType(&pMFMediaType);
	pMFMediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
	pMFMediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
	pMFSourceReader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, nullptr, pMFMediaType);

	pMFMediaType->Release();
	pMFMediaType = nullptr;
	pMFSourceReader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, &pMFMediaType);

	MFCreateWaveFormatExFromMFMediaType(pMFMediaType, &loadAudioStructure_[useNumSoundData_].waveFormat, nullptr);


	while (true)
	{
		IMFSample* pMFSample{ nullptr };
		DWORD dwStreamFlags{ 0 };
		pMFSourceReader->ReadSample(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, nullptr, &dwStreamFlags, nullptr, &pMFSample);

		if (dwStreamFlags & MF_SOURCE_READERF_ENDOFSTREAM)
		{
			break;
		}

		IMFMediaBuffer* pMFMediaBuffer{ nullptr };
		pMFSample->ConvertToContiguousBuffer(&pMFMediaBuffer);

		BYTE* pBuffer{ nullptr };
		DWORD cbCurrentLength{ 0 };
		pMFMediaBuffer->Lock(&pBuffer, nullptr, &cbCurrentLength);

		loadAudioStructure_[useNumSoundData_].mediaData.resize(loadAudioStructure_[useNumSoundData_].mediaData.size() + cbCurrentLength);
		memcpy(loadAudioStructure_[useNumSoundData_].mediaData.data() + loadAudioStructure_[useNumSoundData_].mediaData.size() - cbCurrentLength, pBuffer, cbCurrentLength);

		pMFMediaBuffer->Unlock();

		pMFMediaBuffer->Release();
		pMFSample->Release();
	}


	pMFMediaType->Release();
	pMFSourceReader->Release();
}

/// <summary>
/// 音声を再生する
/// </summary>
/// <param name="soundData"></param>
uint32_t AudioStore::SoundPlayWave(uint32_t index , float soundVolume)
{
	SoundPlayStructure* soundPlayStructure = new SoundPlayStructure();

	// プレイハンドルを作成する
	while (soundPlayStructure->playHandle == 0)
	{
		soundPlayStructure->playHandle = rand() % 1000000 + 1;

		for (SoundPlayStructure* soundPlayHandle : soundPlayStructure_)
		{
			if (soundPlayStructure->playHandle == soundPlayHandle->playHandle)
			{
				soundPlayStructure->playHandle = 0;
				break;
			}
		}
	}

	xAudio2_->CreateSourceVoice(&soundPlayStructure->pSourceVoice, loadAudioStructure_[index].waveFormat);

	XAUDIO2_BUFFER buffer{ 0 };
	buffer.pAudioData = loadAudioStructure_[index].mediaData.data();
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.AudioBytes = sizeof(BYTE) * static_cast<UINT32>(loadAudioStructure_[index].mediaData.size());
	soundPlayStructure->pSourceVoice->SubmitSourceBuffer(&buffer);

	const float kMaxSoundVolume = 1.0f;
	const float kMinSoundVolume = 0.0f;

	// 規格外の音にならぬようにする
	soundVolume = std::max(kMinSoundVolume, soundVolume);
	soundVolume = std::min(kMaxSoundVolume, soundVolume);

	soundPlayStructure->pSourceVoice->SetVolume(soundVolume);

	soundPlayStructure->pSourceVoice->Start(0);

	// リストに登録する
	soundPlayStructure_.push_back(soundPlayStructure);

	return soundPlayStructure->playHandle;
}

/// <summary>
/// 停止した曲を削除する
/// </summary>
void AudioStore::DeleteStopAudio()
{
	soundPlayStructure_.remove_if([](SoundPlayStructure* soundPlayStructure)
		{
			// 音楽が終了したとき
			if (soundPlayStructure->pSourceVoice)
			{
				XAUDIO2_VOICE_STATE state;
				soundPlayStructure->pSourceVoice->GetState(&state);

				if (state.BuffersQueued <= 0)
				{
					delete soundPlayStructure;
					return true;
				}
			}
			else
			{
				// 音楽を停止させたとき
				delete soundPlayStructure;
				return true;
			}

			return false;
		}
	);
}

/// <summary>
/// 音声を停止する
/// </summary>
/// <param name="playHandle"></param>
void AudioStore::SoundStop(uint32_t playHandle)
{
	for (SoundPlayStructure* soundPlayStructure : soundPlayStructure_)
	{
		if (playHandle == soundPlayStructure->playHandle)
		{
			soundPlayStructure->pSourceVoice->Stop(0);
			soundPlayStructure->pSourceVoice = nullptr;

			return;
		}
	}

	return;
}

/// <summary>
/// 音楽が再生しているかどうか
/// </summary>
/// <param name="playHandle"></param>
bool AudioStore::IsSoundPlay(uint32_t playHandle)
{
	for (SoundPlayStructure* soundPlayStructure : soundPlayStructure_)
	{
		if (playHandle == soundPlayStructure->playHandle)
		{
			return true;
		}
	}

	return false;
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
		if (strcmp(filePath.c_str(), loadAudioStructure_[i].filePath.c_str()) == 0)
		{
			return loadAudioStructure_[i].soundHandle;
		}
	}

	// サウンドハンドルを作成する
	while (loadAudioStructure_[useNumSoundData_].soundHandle == 0)
	{
		loadAudioStructure_[useNumSoundData_].soundHandle = rand() % 1000000 + 1;

		for (uint32_t i = 0; i < useNumSoundData_; i++)
		{
			if (loadAudioStructure_[useNumSoundData_].soundHandle == loadAudioStructure_[i].soundHandle)
			{
				loadAudioStructure_[useNumSoundData_].soundHandle = 0;
				break;
			}
		}
	}

	// ファイルパス
	loadAudioStructure_[useNumSoundData_].filePath = filePath;

	// 読み込む
	SoundLoadWave(filePath);

	// 使用した音声データをカウントする
	useNumSoundData_++;

	return loadAudioStructure_[useNumSoundData_ - 1].soundHandle;
}

/// <summary>
/// 指定したハンドルで音声データを流す
/// </summary>
/// <param name="soundHandle"></param>
uint32_t AudioStore::SelectHandlePlayAudio(uint32_t soundHandle, float soundVolume)
{
	// 指定したハンドルのある要素をさがす
	for (uint32_t i = 0; i < useNumSoundData_; i++)
	{
		if (soundHandle == loadAudioStructure_[i].soundHandle)
		{
			return SoundPlayWave(i, soundVolume);
		}
	}

	assert(false);
	return 0;
}