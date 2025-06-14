#pragma once
#include "../../YokosukaEngine/Include/YokosukaEngine.h"
#include "../MainCamera/MainCamera.h"
#include "../Axis/Axis.h"
#include "../Fade/Fade.h"

class TitleScene
{
public:

	// フェーズ
	enum class Phase
	{
		// フェードイン
		kFadeIn,

		// メイン部
		kMain,

		// フェードアウト
		kFadeOut
	};


public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	void Initialize(const YokosukaEngine* engine);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 終了フラグのGetter
	/// </summary>
	/// <returns></returns>
	bool IsFinished() { return isFinished_; }

	/// <summary>
	/// フェーズの状態遷移
	/// </summary>
	void ChangePhase();

	/// <summary>
	/// メイン部の更新処理
	/// </summary>
	void MainUpdate();


private:


	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// デバッグカメラ有効化
	bool isDebugCameraActive_ = false;

	// 3Dカメラ
	std::unique_ptr<Camera3D> camera3d_ = nullptr;

	// ゲームカメラ
	std::unique_ptr<MainCamera> mainCamera_ = nullptr;

	// デバッグのみで使用する
#ifdef _DEBUG

	// 軸方向表示
	std::unique_ptr<Axis> axis_ = nullptr;

#endif

	// フェード
	std::unique_ptr<Fade> fade_ = nullptr;

	// 現在のフェーズ
	Phase phase_ = Phase::kFadeOut;

	// フェードする時間
	const float kFadeTime = 1.0f;


	// 終了フラグ
	bool isFinished_ = false;
};

