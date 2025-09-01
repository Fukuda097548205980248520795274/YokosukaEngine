#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"
#include "PoseBox/PoseBox.h"
#include "PoseBg/PoseBg.h"

#include "../../../TextureHandleStore/TextureHandleStore.h"

#include "../../../BaseSprite/BaseSprite.h"

class Pose
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	void Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const TextureHandleStore* textureHandleStore);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// ポーズボタン
	/// </summary>
	void PoseButton();

	/// <summary>
	/// ポーズ解除ボタン
	/// </summary>
	void PoseReleaseButton();

	/// <summary>
	/// ポーズフラグのGetter
	/// </summary>
	/// <returns></returns>
	bool IsPose()const { return isPose_; }

	/// <summary>
	/// ゲーム終了フラグのGetter
	/// </summary>
	/// <returns></returns>
	bool IsEndGame()const { return isEndGame_; }


private:

	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// 3Dカメラ
	const Camera3D* camera3d_ = nullptr;

	// テクスチャハンドル格納場所
	const TextureHandleStore* textureHandleStore_ = nullptr;

	// 2Dカメラ
	std::unique_ptr<Camera2D> camera2d_ = nullptr;




	// ポーズフラグ
	bool isPose_ = false;

	// フェーズ
	enum Phase
	{
		kFadeIn,
		kOperation,
		kFadeOut
	};

	// 現在のフェーズ
	Phase phase_ = kFadeIn;

	// ポーズの箱
	std::unique_ptr<PoseBox> poseBox_ = nullptr;

	// ポーズの背景
	std::unique_ptr<PoseBg> poseBg_ = nullptr;

	// テキスト : ポーズ
	std::unique_ptr<Sprite> spritePose_ = nullptr;

	// テキスト : ゲームに戻る
	std::unique_ptr<Sprite> spriteReturnGame_ = nullptr;

	// テキスト : ゲームをやめる
	std::unique_ptr<Sprite> spriteEndGame_ = nullptr;


	// ポーズ音
	uint32_t shPose_ = 0;

	// ポーズ解除
	uint32_t shPoseRelease_ = 0;


	/// <summary>
	/// ポーズの初期化
	/// </summary>
	void PoseInitialize();

	
	/*   フェードイン   */

	/// <summary>
	/// フェードインの初期化
	/// </summary>
	void FadeInInitialize();

	/// <summary>
	/// フェードインの更新処理
	/// </summary>
	void FadeInUpdate();

	// フェードインパラメータ
	float parameterFadeIn_ = 0.0f;

	// フェードインパラメータ最大値
	const float kMaxParameterFadeIn = 0.5f;


	/*   操作   */

	/// <summary>
	/// 操作
	/// </summary>
	void Operation();

	/// <summary>
	/// 操作 : ゲームパッド
	/// </summary>
	void OperationGamepad();

	/// <summary>
	/// 操作 : キーボード
	/// </summary>
	void OperationKeyboard();

	enum Menu
	{
		returnGame,
		returnStageSelect
	};

	// 選んでいるメニュー
	int32_t selectMenu = returnGame;

	// ゲーム終了フラグ
	bool isEndGame_ = false;



	/*   フェードアウト   */

	/// <summary>
	/// フェードアウトの初期化
	/// </summary>
	void FadeOutInitialize();

	/// <summary>
	/// フェードアウトの更新処理
	/// </summary>
	void FadeOutUpdate();

	// フェードアウトパラメータ
	float parameterFadeOut_ = 0.0f;

	// フェードアウトパラメータ最大値
	float kMaxParameterFadeOut = 0.5f;
};

