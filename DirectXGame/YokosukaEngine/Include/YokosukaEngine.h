#pragma once
#include <stdlib.h>
#include <time.h>
#include "Func/CrashHandler/CrashHandler.h"
#include "Base/WinApp/WinApp.h"
#include "Base/DirectXCommon/DirectXCommon.h"
#include "Base/Input/Input.h"
#include "Store/AudioStore/AudioStore.h"

// ブレンドモード
enum BlendMode
{
	// ブレンドなし
	kBlendModeNone,

	// 通常αブレンド
	kBlendModeNormal,

	// 加算合成
	kBlendModeAdd,

	// 減算合成
	kBlendModeSubtract,

	// 乗算合成
	kBlendModeMultiply,

	// スクリーン合成
	kBlendModeScreen
};

// マウスボタン
enum MouseButton
{
	// 左ボタン
	kMouseButtonLeft,

	// 右ボタン
	kMouseButtonRight,

	// 中央ボタン
	kMouseButtonCenter
};

class YokosukaEngine
{

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~YokosukaEngine();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="kWindowWidth">ウィンドウの横幅</param>
	/// <param name="kWindowHeight">ウィンドウの縦幅</param>
	/// <param name="titleName">ウィンドウのタイトル名</param>
	void Initialize(const int32_t kWindowWidth, const int32_t kWindowHeight, const std::string& titleName);

	/// <summary>
	/// メッセージを処理する
	/// </summary>
	bool ProcessMessage() { return winApp_->ProcessMessage(); }

	/// <summary>
	/// フレーム開始
	/// </summary>
	void BeginFrame() { directXCommon_->PreDraw(); }

	/// <summary>
	/// フレーム終了
	/// </summary>
	void EndFrame() { directXCommon_->PostDraw(); }

	/// <summary>
	/// 画面の横幅のGetter
	/// </summary>
	/// <returns></returns>
	uint32_t GetScreenWidth() const { return winApp_->GetWindowWidth(); }

	/// <summary>
	/// 画面の縦幅のGetter
	/// </summary>
	/// <returns></returns>
	uint32_t GetScreenHeight() const { return winApp_->GetWindowHeight(); }

	/// <summary>
	/// テクスチャを読み込む
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <returns></returns>
	uint32_t LoadTexture(const std::string& filePath) const { return directXCommon_->LoadTexture(filePath); }

	/// <summary>
	/// モデルデータを読み込む
	/// </summary>
	/// <param name="directory"></param>
	/// <param name="fileName"></param>
	/// <returns></returns>
	uint32_t LoadModelData(const std::string& directory, const std::string& fileName) const { return directXCommon_->LoadModelData(directory, fileName); }

	/// <summary>
	/// 音声データを読み込む
	/// </summary>
	/// <param name="filePath"></param>
	/// <returns></returns>
	uint32_t LoadSound(const std::string& filePath) const { return audioStore_->GetSoundHandle(filePath); }

	/// <summary>
	/// オーディオストアの更新処理
	/// </summary>
	void UpdateAudioStore() { audioStore_->DeleteStopAudio(); };

	/// <summary>
	/// 音量の設定
	/// </summary>
	/// <param name="playHandle"></param>
	/// <param name="volume"></param>
	void SetVolume(uint32_t playHandle, float volume) const { audioStore_->SetVolume(playHandle, volume); }

	/// <summary>
	/// ピッチの設定
	/// </summary>
	/// <param name="playHandle"></param>
	/// <param name="pitch"></param>
	void SetPitch(uint32_t playHandle, float pitch)const { audioStore_->SetPitch(playHandle, pitch); }

	/// <summary>
	/// 音声データを再生する
	/// </summary>
	/// <param name="soundHandle"></param>
	uint32_t PlaySoundData(uint32_t soundHandle, float soundVolume) const { return audioStore_->SelectHandlePlayAudio(soundHandle, soundVolume); }

	/// <summary>
	/// 音声データを停止する
	/// </summary>
	/// <param name="playHandle"></param>
	void StopSound(uint32_t playHandle) const { audioStore_->StopSound(playHandle); }

	/// <summary>
	/// 音楽が再生されているかどうか
	/// </summary>
	/// <param name="playHandle"></param>
	/// <returns></returns>
	bool IsSoundPlay(uint32_t playHandle)const { return audioStore_->IsSoundPlay(playHandle); }

	/// <summary>
	/// 平行光源を設置する
	/// </summary>
	/// <param name="directionalLight"></param>
	void SetDirectionalLight(const DirectionalLight* directionalLight)const { directXCommon_->SetDirectionalLight(directionalLight); }

	/// <summary>
	/// ポイントライトを設置する
	/// </summary>
	/// <param name="pointLight"></param>
	void SetPointLight(const PointLight* pointLight)const { directXCommon_->SetPointLight(pointLight); }

	/// <summary>
	/// スポットライトを設置する
	/// </summary>
	/// <param name="spotLight"></param>
	void SetSpotLight(const SpotLight* spotLight)const { directXCommon_->SetSpotLight(spotLight); }

	/// <summary>
	/// オフスクリーンのエフェクトを使用する
	/// </summary>
	/// <param name="effect"></param>
	uint32_t SetOffscreenEffect(Effect effect) const { return directXCommon_->SetOffscreenEffect(effect); }

	/// <summary>
	/// RTVに描画したテクスチャをコピーする
	/// </summary>
	/// <param name="numRtv"></param>
	void CopyRtvImage(uint32_t numRtv) const { directXCommon_->CopyRtvImage(numRtv); }

	/// <summary>
	/// 平面を描画する
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="uvTransform"></param>
	/// <param name="camera"></param>
	/// <param name="textureHandle"></param>
	/// <param name="color"></param>
	/// <param name="isLighting"></param>
	void DrawPlane(const WorldTransform* worldTransform, const UvTransform* uvTransform,
		const Camera3D* camera, uint32_t textureHandle, Vector4 color, bool isLighting) const
	{
		directXCommon_->DrawPlane(worldTransform, uvTransform, camera, textureHandle, color, isLighting);
	}

	/// <summary>
	/// 球を描画する
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="uvTransform"></param>
	/// <param name="camera"></param>
	/// <param name="textureHandle"></param>
	/// <param name="color"></param>
	/// <param name="isLighting"></param>
	void DrawSphere(const WorldTransform* worldTransform, const UvTransform* uvTransform,
		const Camera3D* camera, uint32_t textureHandle, uint32_t segment, uint32_t ring, Vector4 color, bool isLighting) const
	{
		directXCommon_->DrawSphere(worldTransform, uvTransform, camera, textureHandle, segment, ring, color, isLighting);
	}

	/// <summary>
	/// リングを描画する
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="uvTransform"></param>
	/// <param name="camera"></param>
	/// <param name="textureHandle"></param>
	/// <param name="subdivisions"></param>
	/// <param name="outRadius"></param>
	/// <param name="inRadius"></param>
	/// <param name="color"></param>
	/// <param name="isLighting"></param>
	void DrawRing(const WorldTransform* worldTransform, const UvTransform* uvTransform,
		const Camera3D* camera, uint32_t textureHandle, uint32_t subdivisions, float outRadius, float inRadius, Vector4 color, bool isLighting) const
	{
		directXCommon_->DrawRing(worldTransform, uvTransform, camera, textureHandle, subdivisions, outRadius, inRadius, color, isLighting);
	}

	/// <summary>
	/// 円柱を描画する
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="uvTransform"></param>
	/// <param name="camera"></param>
	/// <param name="textureHandle"></param>
	/// <param name="subdivisions"></param>
	/// <param name="height"></param>
	/// <param name="radius"></param>
	/// <param name="color"></param>
	/// <param name="isLighting"></param>
	void DrawCylinder(const WorldTransform* worldTransform, const UvTransform* uvTransform,
		const Camera3D* camera, uint32_t textureHandle, uint32_t subdivisions, float height, float radius, Vector4 color, bool isLighting) const
	{
		directXCommon_->DrawCylinder(worldTransform, uvTransform, camera, textureHandle, subdivisions, height, radius, color, isLighting);
	}

	/// <summary>
	/// モデルを描画する
	/// </summary>
	void DrawModel(const WorldTransform* worldTransform, const std::vector<std::unique_ptr<UvTransform>>& uvTransforms,
		const Camera3D* camera, uint32_t modelHandle, Vector4 color, bool isLighting) const
	{
		directXCommon_->DrawModel(worldTransform, uvTransforms, camera, modelHandle, color, isLighting);
	}

	/// <summary>
	/// モデルを描画する
	/// </summary>
	void DrawModel(const WorldTransform* worldTransform, const UvTransform* uvTransform,
		const Camera3D* camera, uint32_t modelHandle, Vector4 color, bool isLighting) const
	{
		directXCommon_->DrawModel(worldTransform, uvTransform, camera, modelHandle, color, isLighting);
	}

	/// <summary>
	/// モデルを描画する
	/// </summary>
	void DrawModel(const WorldTransform* worldTransform, const Camera3D* camera, uint32_t modelHandle, Vector4 color, bool isLighting) const
	{
		directXCommon_->DrawModel(worldTransform, camera, modelHandle, color, isLighting);
	}

	/// <summary>
	/// レンダーテクスチャを貼りつけた平面を描画する
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="uvTransform"></param>
	/// <param name="camera"></param>
	/// <param name="textureHandle"></param>
	/// <param name="color"></param>
	/// <param name="isLighting"></param>
	void DrawRenderTexturePlane(const WorldTransform* worldTransform, const UvTransform* uvTransform,
		const Camera3D* camera, uint32_t screenHandle, Vector4 color, bool isLighting) const
	{
		directXCommon_->DrawRenderTexturePlane(worldTransform, uvTransform, camera, screenHandle, color, isLighting);
	}

	/// <summary>
	/// 線を描画する
	/// </summary>
	/// <param name="start"></param>
	/// <param name="end"></param>
	/// <param name="camera"></param>
	/// <param name="color"></param>
	void DrawLine(const Vector3& start, const Vector3& end, const Camera3D* camera, const Vector4& color) const
	{
		directXCommon_->DrawLine(start, end, camera, color);
	}

	/// <summary>
	/// スプライトを描画する
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="uvTransform"></param>
	/// <param name="camera"></param>
	/// <param name="textureHandle"></param>
	/// <param name="color"></param>
	void DrawSprite(const WorldTransform2D* worldTransform,const UvTransform* uvTransform, 
		const Camera2D* camera, uint32_t textureHandle, Vector4 color) const
	{
		// ワールドビュープロジェクション行列
		Matrix4x4 worldViewProjectionMatrix = worldTransform->worldMatrix_ * camera->viewMatrix_ * camera->projectionMatrix_;

		// ビューポート変換行列
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0.0f, 0.0f, camera->screenWidth_, camera->screenHeight_, 0.0f, 1.0f);

		Vector3 vertecies[4] =
		{
			Transform(Transform(Vector3(-1.0f , -1.0f) , worldViewProjectionMatrix), viewportMatrix),
			Transform(Transform(Vector3(1.0f , -1.0f) , worldViewProjectionMatrix), viewportMatrix),
			Transform(Transform(Vector3(-1.0f , 1.0f) , worldViewProjectionMatrix), viewportMatrix),
			Transform(Transform(Vector3(1.0f , 1.0f) , worldViewProjectionMatrix), viewportMatrix)
		};


		directXCommon_->DrawSprite(Vector2(vertecies[0].x, vertecies[0].y), Vector2(vertecies[1].x, vertecies[1].y),
			Vector2(vertecies[2].x, vertecies[2].y), Vector2(vertecies[3].x, vertecies[3].y), uvTransform, camera, textureHandle, color);
	}

	/// <summary>
	/// CatmullRomスプライン曲線を描画する
	/// </summary>
	/// <param name="controlPoints"></param>
	void DrwaCatmullRomSpline(const std::vector<Vector3>& controlPoints, const Vector4& color, const Camera3D* camera3d) const;

	/// <summary>
	/// ブレンドモードを設定する
	/// </summary>
	/// <param name="blendMode"></param>
	void SetObject3dBlendMode(BlendMode blendMode) const { directXCommon_->SetObject3dBlendMode(static_cast<uint32_t>(blendMode)); }

	/// <summary>
	/// ブレンドモードを設定する
	/// </summary>
	/// <param name="blendMode"></param>
	void SetParticleBlendMode(BlendMode blendMode) const { directXCommon_->SetParticleBlendMode(static_cast<uint32_t>(blendMode)); }

	/// <summary>
	/// ブレンドモードを設定する
	/// </summary>
	/// <param name="blendMode"></param>
	void SetLine3dBlendMode(BlendMode blendMode) const { directXCommon_->SetLine3dBlendMode(static_cast<uint32_t>(blendMode)); }

	/// <summary>
	/// ブレンドモードを設定する
	/// </summary>
	/// <param name="blendMode"></param>
	void SetCopyImageBlendMode(BlendMode blendMode) const { directXCommon_->SetCopyImageBlendMode(static_cast<uint32_t>(blendMode)); }

	/// <summary>
	/// 全てのキーの入力情報を取得する
	/// </summary>
	/// <param name="keys"></param>
	void CheckAllKeyInfo() const { input_->CheckInputInfo(); }

	/// <summary>
	/// 全てのキーの入力情報をコピーする
	/// </summary>
	void CopyAllKeyInfo() const { input_->CopyInputInfo(); }

	/// <summary>
	/// キー入力（Press）
	/// </summary>
	/// <param name="key">キー</param>
	/// <returns></returns>
	bool GetKeyPress(BYTE key) const { return input_->GetKeyPress(key); }

	/// <summary>
	/// キー入力（Trigger）
	/// </summary>
	/// <param name="key">キー</param>
	/// <returns></returns>
	bool GetKeyTrigger(BYTE key) const { return input_->GetKeyTrigger(key); }

	/// <summary>
	/// キー入力（Release）
	/// </summary>
	/// <param name="key">キー</param>
	/// <returns></returns>
	bool GetKeyRelease(BYTE key) const { return input_->GetKeyRelease(key); }

	/// <summary>
	/// マウスボタン（Press）
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	bool GetMouseButtonPress(MouseButton mouseButton) const { return input_->GetMousePress(static_cast<uint32_t>(mouseButton)); };

	/// <summary>
	/// マウスボタン（Trigger）
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	bool GetMouseButtonTrigger(MouseButton mouseButton) const { return input_->GetMouseTrigger(static_cast<uint32_t>(mouseButton)); };

	/// <summary>
	/// マウスボタン（Release）
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	bool GetMouseButtonRelease(MouseButton mouseButton) const { return input_->GetMouseRelease(static_cast<uint32_t>(mouseButton)); };

	/// <summary>
	/// マウスの移動量のGetter
	/// </summary>
	/// <returns></returns>
	Vector2 GetMouseVelocity() const { return input_->GetMouseVelocity(); }

	/// <summary>
	/// マウスホイールが上回転しているかどうか
	/// </summary>
	/// <returns></returns>
	bool GetMouseWheelUp() const { return input_->GetMouseWheelUp(); }

	/// <summary>
	/// マウスホイールが下回転しているかどうか
	/// </summary>
	/// <returns></returns>
	bool GetMouseWheelDown()  const { return input_->GetMouseWheelDown(); }

	/// <summary>
	/// マウスホイールの回転量のGetter
	/// </summary>
	/// <returns></returns>
	float GetMouseWheelVelocity() const { return input_->GetMouseWheelVelocity(); }

	/// <summary>
	/// 
	/// </summary>
	/// <param name="gamepadNumber"></param>
	/// <returns></returns>
	bool IsGamepadEnable(DWORD gamepadNumber) const { return input_->IsGamepadEnable(gamepadNumber); }

	/// <summary>
	/// ゲームパッドのボタンの入力情報（Press）
	/// </summary>
	/// <param name="gamepadNumber">ゲームパッドの番号</param>
	/// <param name="wButtons">指定のボタン</param>
	/// <returns></returns>
	bool GetGamepadButtonPress(DWORD gamepadNumber, DWORD wButtons) const { return input_->GetGamepadButtonPress(gamepadNumber, wButtons); }

	/// <summary>
	/// ゲームパッドのボタンの入力情報（Press）
	/// </summary>
	/// <param name="gamepadNumber">ゲームパッドの番号</param>
	/// <param name="wButtons">指定のボタン</param>
	/// <returns></returns>
	bool GetGamepadButtonTrigger(DWORD gamepadNumber, DWORD wButtons) const { return input_->GetGamepadButtonTrigger(gamepadNumber, wButtons); }

	/// <summary>
	/// ゲームパッドのボタンの入力情報（Press）
	/// </summary>
	/// <param name="gamepadNumber">ゲームパッドの番号</param>
	/// <param name="wButtons">指定のボタン</param>
	/// <returns></returns>
	bool GetGamepadButtonRelease(DWORD gamepadNumber, DWORD wButtons) const { return input_->GetGamepadButtonRelease(gamepadNumber, wButtons); }

	/// <summary>
	/// 
	/// </summary>
	/// <param name="gamepadNumber"></param>
	/// <returns></returns>
	Vector2 GetGamepadLeftStick(DWORD gamepadNumber) const { return input_->GetGamepadLeftStick(gamepadNumber); }

	/// <summary>
	/// 
	/// </summary>
	/// <param name="gamepadNumber"></param>
	/// <returns></returns>
	Vector2 GetGamepadRightStick(DWORD gamepadNumber) const { return input_->GetGamepadRightStick(gamepadNumber); }

	/// <summary>
	/// 
	/// </summary>
	/// <param name="gamepadNumber"></param>
	/// <returns></returns>
	float GetGamepadLeftTrigger(DWORD gamepadNumber) const { return input_->GetGamepadLeftTrigger(gamepadNumber); }

	/// <summary>
	/// 
	/// </summary>
	/// <param name="gamepadNumber"></param>
	/// <returns></returns>
	float GetGamepadRightTrigger(DWORD gamepadNumber) const { return input_->GetGamepadRightTrigger(gamepadNumber); }


	// デバッグツールメソッド
#ifdef _DEBUG

	/// <summary>
	/// グリッドを描画する
	/// </summary>
	/// <param name="camera"></param>
	void DrawGrid(const Camera3D* camera, int32_t x, int32_t z) const;

#endif


private:

	/// <summary>
	/// CatmullRomの補間点を求める
	/// </summary>
	/// <param name="p0"></param>
	/// <param name="p1"></param>
	/// <param name="p2"></param>
	/// <param name="p3"></param>
	/// <param name="t"></param>
	/// <returns></returns>
	Vector3 CatmullRomInterpolation(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t) const;

	/// <summary>
	/// CatmullRom全体で補間点を求める
	/// </summary>
	/// <param name="points"></param>
	/// <param name="t"></param>
	/// <returns></returns>
	Vector3 CatmullRomPosition(const std::vector<Vector3>& points, float t) const;


	// 現在の時間
	unsigned int currentTimer_;

	// ウィンドウアプリケーション
	std::unique_ptr<WinApp> winApp_ = nullptr;

	// ログ出力
	std::unique_ptr<Logging> logging_ = nullptr;

	// DirectXCommon
	std::unique_ptr<DirectXCommon> directXCommon_ = nullptr;

	// Input
	std::unique_ptr<Input> input_ = nullptr;

	// オーディオストア
	std::unique_ptr<AudioStore> audioStore_ = nullptr;
};


// デバッグツール
#ifdef _DEBUG

// デバッグカメラ
class DebugCamera
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const YokosukaEngine* engine);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 3Dカメラのインスタンスを取得する
	/// </summary>
	/// <returns></returns>
	Camera3D* GetCamera3DInstance() { return camera3d_.get(); }

	/// <summary>
	/// ワールド座標のGetter
	/// </summary>
	Vector3 GetWorldTransform();


private:

	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// 3Dカメラ
	std::unique_ptr<Camera3D> camera3d_ = nullptr;

	// ピボットポイント
	Vector3 pivotPoint_ = { 0.0f , 0.0f , 0.0f };

	// ピボットポイントとの距離
	float pivotPointLength_ = 50.0f;

	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;
};

// 軸方向表示
class AxialDirectionDisplay
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	void Initialize(const YokosukaEngine* engine);

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="cameraRotation"></param>
	void Update(const Vector3& cameraRotation);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();


private:

	// エンジン
	const YokosukaEngine* engine_ = nullptr;


	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;

	// UVトランスフォーム
	std::unique_ptr<UvTransform> uvTransform_ = nullptr;

	// 3Dカメラ
	std::unique_ptr<Camera3D> camera3d_ = nullptr;

	// モデルハンドル
	uint32_t modelHandle_ = 0;
};

#endif

// メインカメラ
class MainCamera
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="screenWidth">画面の横幅</param>
	/// <param name="screenHeight">画面の縦幅</param>
	void Initialize(float screenWidth, float screenHeight);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// ゲームカメラのインスタンスのGetter
	/// </summary>
	/// <returns></returns>
	Camera3D* GetGameCameraInstance() { return camera3d_.get(); }

	/// <summary>
	/// ピボットのワールド座標のGetter
	/// </summary>
	/// <returns></returns>
	Vector3 GetPivotWorldPosition();

	/// <summary>
	/// ピボットのワールドトランスフォームのGetter
	/// </summary>
	/// <returns></returns>
	WorldTransform* GetPivotWorldTransform() { return pivotWorldTransform_.get(); }

	/// <summary>
	/// ピボットの親のワールドトランスフォームのSetter
	/// </summary>
	/// <param name="worldTransform"></param>
	void SetPivotParent(WorldTransform* worldTransform) { pivotWorldTransform_->SetParent(worldTransform); }

	/// <summary>
	/// カメラの回転のSetter
	/// </summary>
	/// <param name="rotate"></param>
	void SetCameraRotate(const Vector3& rotate) { camera3d_->rotation_ = rotate; }

	/// <summary>
	/// シェイクの設定
	/// </summary>
	/// <param name="shakeTime">時間</param>
	/// <param name="shakeSize">大きさ</param>
	void SetShake(float shakeTime, float shakeSize);


private:

	/// <summary>
	/// シェイクする
	/// </summary>
	void Shake();

	// シェイクの移動量
	Vector3 shakeMove_ = { 0.0f , 0.0f , 0.0f };

	// シェイクの初期タイマー
	float shakeStartTimer_ = 0.0f;

	// シェイクタイマー
	float shakeTimer_ = 0.0f;

	// シェイクの大きさ
	float shakeSize_ = 0.0f;


	// 3Dカメラ
	std::unique_ptr<Camera3D> camera3d_ = nullptr;


	// ピボットポイント
	Vector3 pivotPoint_ = { 0.0f , 0.0f , 0.0f };

	// ピボットポイントとの距離
	float pivotPointLength_ = 50.0f;

	// ピボットのワールドトランスフォーム
	std::unique_ptr<WorldTransform> pivotWorldTransform_ = nullptr;
};

/// <summary>
/// フェード
/// </summary>
class Fade
{
public:


private:
};

/// <summary>
/// シーン
/// </summary>
class Scene
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	virtual void Initialize(const YokosukaEngine* engine);

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// 終了フラグのGetter
	/// </summary>
	/// <returns></returns>
	bool IsFinished() const { return isFinished_; }


protected:

	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// 3Dカメラ
	std::unique_ptr<Camera3D> camera3d_ = nullptr;

	// 2Dカメラ
	std::unique_ptr<Camera2D> camera2d_ = nullptr;

	// メインカメラ
	std::unique_ptr<MainCamera> mainCamera_ = nullptr;


	// 終了フラグ
	bool isFinished_ = false;


private:

	// デバッグカメラ有効化
	bool isDebugCameraActive_ = false;

	// デバッグツール
#ifdef _DEBUG

	// デバッグカメラ
	std::unique_ptr<DebugCamera> debugCamera_ = nullptr;

	// 軸方向表示
	std::unique_ptr<AxialDirectionDisplay> axialDirectoinDisplay_ = nullptr;
	
#endif
};
