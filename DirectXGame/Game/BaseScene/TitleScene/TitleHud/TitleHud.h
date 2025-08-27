#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"
#include "../../../ModelHandleStore/ModelHandleStore.h"
#include "../../../TextureHandleStore/TextureHandleStore.h"

#include "../../../BaseSprite/BaseSprite.h"
#include "../../../Model/Model.h"

class TitleHud
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera3d"></param>
	/// <param name="camera2d"></param>
	void Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const Camera2D* camera2d,
		const ModelHandleStore* modelHandleStore, const TextureHandleStore* textureHandleStore);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();


private:

	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// 3Dカメラ
	const Camera3D* camera3d_ = nullptr;

	// 2Dカメラ
	const Camera2D* camera2d_ = nullptr;

	// モデルハンドル格納場所
	const ModelHandleStore* modelHandleStore_ = nullptr;

	// テクスチャハンドル格納場所
	const TextureHandleStore* textureHandleStore_ = nullptr;


	// プレイヤーのジェット
	std::unique_ptr<Model> playerJet_ = nullptr;


	// ゲームパッド : スタート
	std::unique_ptr<Sprite> spriteGamepadStart_ = nullptr;

	// キーボード : スタート
	std::unique_ptr<Sprite> spritekeyboardStart_ = nullptr;

	// 白背景
	std::unique_ptr<Sprite> spriteBgWhite_ = nullptr;

	// 黒背景
	std::unique_ptr<Sprite> spriteBgBlack_ = nullptr;
	float blackScaleY_ = 220.0f;



public:

	/*----------------
	    ジェット飛行
	----------------*/

	/// <summary>
	/// ジェット飛行 : 初期化
	/// </summary>
	void PhaseFlyJetInitialize(float maxParameter);

	/// <summary>
	/// ジェット飛行 : 更新処理
	/// </summary>
	void PhaseFlyJetUpdate();


private:

	// ジェット飛行のパラメータ
	float flyJetParameter_ = 0.0f;

	// ジェット飛行のパラメータ最大値
	float maxFlyJetOperationParameter = 0.0f;

	// ジェットの初期位置
	Vector3 jetStartPos_ = Vector3(15.0f, 1.5f, -29.0f);

	// ジェットが止まる場所
	Vector3 jetStopPos_ = Vector3(-4.7f, 1.5f, -29.0f);

	// ジェットの到着地点
	Vector3 jetGoalPos_ = Vector3(-19.7f, 1.5f, -29.0f);

	
public:


	/*-----------
	    操作中
	-----------*/

	/// <summary>
	/// 操作中 : 初期化
	/// </summary>
	void PhaseOperationInitialize();

	/// <summary>
	/// 操作中 : 更新処理
	/// </summary>
	void PhaseOperationUpdate();

private:

	// 操作中のパラメータ
	float operationParameter_ = 0.0f;

	// 操作中のパラメータ最大値
	const float kMaxOperationParameter = 1.0f;


public:

	/*----------------
	    フェードアウト
	----------------*/

	/// <summary>
	/// フェードアウト : 初期化
	/// </summary>
	void PhaseFadeOutInitialize(float maxParameter);

	/// <summary>
	/// フェードアウト : 更新処理
	/// </summary>
	void PhaseFadeOutUpdate();

private:
	
	// フェードアウトのパラメータ
	float fadeOutParameter_ = 0.0f;

	// フェードアウトの最大パラメータ
	float maxFadeOutParameter_ = 0.0f;
};

