#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"
#include "../../BaseScene/BaseScene.h"

class TitleScene : public BaseScene
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	void Initialize(const YokosukaEngine* engine, const ModelHandleStore* modelHandleStore) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

private:


	/*---------------
		ビヘイビア
	---------------*/

	// ビヘイビア
	enum Behavior
	{
		kFadeIn,
		kOperation,
		kFadeOut
	};

	// 現在のビヘイビア
	Behavior behavior_ = kFadeIn;

	// 次のビヘイビアのリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;



	/*----------------------------
		ビヘイビア : フェードイン
	----------------------------*/

	/// <summary>
	/// ビヘイビア : フェードイン : 初期化
	/// </summary>
	void BehaviorFadeInInitialize();

	/// <summary>
	/// ビヘイビア : フェードイン : 更新処理
	/// </summary>
	void BehaviorFadeInUpdate();

	/// <summary>
	/// ビヘイビア : フェードイン : 描画処理
	/// </summary>
	void BehaviorFadeInDraw();

	// フェードインパラメータ
	float fadeInParameter_ = 0.0f;

	// フェードんインパラメータの最大値
	const float kFadeInPrameterMax = 2.0f;


	// ガラスが割れる音
	uint32_t shGlassBreaks_ = 0;
	uint32_t phGlassBreaks_ = 0;
	float phGlassBreaksVolume_ = 0.7f;
	float phGlassBreaksPitch_ = 1.0f;


	/*---------------------
		ビヘイビア : 操作
	---------------------*/

	/// <summary>
	/// ビヘイビア : 操作 : 初期化
	/// </summary>
	void BehaviorOperationInitialize();

	/// <summary>
	/// ビヘイビア : 操作 : 更新処理
	/// </summary>
	void BehaviorOperationUpdate();

	/// <summary>
	/// ビヘイビア : 操作 : 描画処理
	/// </summary>
	void BehaviorOperationDraw();


	/*-----------------------------
	    ビヘイビア : フェードアウト
	-----------------------------*/

	/// <summary>
	/// ビヘイビア : フェードアウト : 初期化
	/// </summary>
	void BehaviorFadeOutInitialize();

	/// <summary>
	/// ビヘイビア : フェードアウト : 更新処理
	/// </summary>
	void BehaviorFadeOutUpdate();

	/// <summary>
	/// ビヘイビア : フェードアウト : 描画処理
	/// </summary>
	void BehaviorFadeOutDraw();

	// フェードアウトパラメータ
	float fadeOutParameter_ = 0.0f;

	// フェードアウトパラメータの最大値
	const float kFadeOutPrameterMax = 2.0f;
};