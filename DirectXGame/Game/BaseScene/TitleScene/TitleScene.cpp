#include "TitleScene.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
void TitleScene::Initialize(const YokosukaEngine* engine, const ModelHandleStore* modelHandleStore)
{
	// 基底クラス初期化
	BaseScene::Initialize(engine , modelHandleStore);

	// 効果音を読み込む
	shGlassBreaks_ = engine_->LoadSound("./Resources/Sounds/Se/title/glass.mp3");

	// サウンドを読み込む
	soundHandle_ = engine_->LoadSound("./Resources/Sounds/Bgm/Sagittarius_2.mp3");


	// フェードイン初期化
	BehaviorFadeInInitialize();
}

/// <summary>
/// 更新処理
/// </summary>
void TitleScene::Update()
{
	// 基底クラス更新処理
	BaseScene::Update();


	// 次のビヘイビアのリクエストがあるとき
	if (behaviorRequest_)
	{
		// ビヘイビアを切り替える
		behavior_ = behaviorRequest_.value();

		// ビヘイビア初期化
		switch (behavior_)
		{
		case kFadeIn:
			// フェードイン
			BehaviorFadeInInitialize();

			break;

		case kOperation:
			// 操作
			BehaviorOperationInitialize();

			break;

		case kFadeOut:
			// フェードアウト
			BehaviorFadeOutInitialize();

			break;
		}

		// ビヘイビアリクエストを初期化
		behaviorRequest_ = std::nullopt;
	}

	// ビヘイビア更新処理
	switch (behavior_)
	{
	case kFadeIn:
		// フェードイン
		BehaviorFadeInUpdate();

		break;

	case kOperation:
		// 操作
		BehaviorOperationUpdate();

		break;

	case kFadeOut:
		// フェードアウト
		BehaviorFadeOutUpdate();

		break;
	}
}

/// <summary>
/// 描画処理
/// </summary>
void TitleScene::Draw()
{
	// ビヘイビア描画処理
	switch (behavior_)
	{
	case kFadeIn:
		// フェードイン
		BehaviorFadeInDraw();

		break;

	case kOperation:
		// 操作
		BehaviorOperationDraw();

		break;

	case kFadeOut:
		// フェードアウト
		BehaviorFadeOutDraw();

		break;
	}

	// 基底クラス描画処理
	BaseScene::Draw();
}





/*----------------------------
	ビヘイビア : フェードイン
----------------------------*/

/// <summary>
/// ビヘイビア : フェードイン : 初期化
/// </summary>
void TitleScene::BehaviorFadeInInitialize()
{
	// フェードインパラメータ
	fadeInParameter_ = 0.0f;



	// ガラスの効果音
	phGlassBreaks_ = engine_->PlaySoundData(shGlassBreaks_, 0.5f);

	// ガラスの効果音のピッチを通常にする
	phGlassBreaksPitch_ = 1.0f;
	engine_->SetPitch(phGlassBreaks_, phGlassBreaksPitch_);
}

/// <summary>
/// ビヘイビア : フェードイン : 更新処理
/// </summary>
void TitleScene::BehaviorFadeInUpdate()
{
	// パラメータを進める
	fadeInParameter_ += 1.0f / 60.0f;
	fadeInParameter_ = std::min(fadeInParameter_, kFadeInPrameterMax);


	// ピッチの速度を上げる
	phGlassBreaksPitch_ -= 1.0f / 60.0f;
	phGlassBreaksPitch_ = std::max(phGlassBreaksPitch_, 0.0f);
	engine_->SetPitch(phGlassBreaks_, phGlassBreaksPitch_);


	// 最大値になったら操作に遷移する
	if (fadeInParameter_ >= kFadeInPrameterMax)
	{
		// ガラス音を止める
		engine_->StopSound(phGlassBreaks_);

		behaviorRequest_ = kOperation;
		return;
	}
}

/// <summary>
/// ビヘイビア : フェードイン : 描画処理
/// </summary>
void TitleScene::BehaviorFadeInDraw()
{

}






/*---------------------
	ビヘイビア : 操作
---------------------*/

/// <summary>
/// ビヘイビア : 操作 : 初期化
/// </summary>
void TitleScene::BehaviorOperationInitialize()
{

}

/// <summary>
/// ビヘイビア : 操作 : 更新処理
/// </summary>
void TitleScene::BehaviorOperationUpdate()
{
	// BGMをループさせる
	if (!engine_->IsSoundPlay(playHandle_) || playHandle_ == 0)
	{
		playHandle_ = engine_->PlaySoundData(soundHandle_, 0.3f);
	}

	if (engine_->GetKeyTrigger(DIK_A))
	{
		behaviorRequest_ = kFadeOut;

		// BGMを止める
		engine_->StopSound(playHandle_);
	}
}

/// <summary>
/// ビヘイビア : 操作 : 描画処理
/// </summary>
void TitleScene::BehaviorOperationDraw()
{

}






/*-----------------------------
	ビヘイビア : フェードアウト
-----------------------------*/

/// <summary>
/// ビヘイビア : フェードアウト : 初期化
/// </summary>
void TitleScene::BehaviorFadeOutInitialize()
{
	// フェードアウトパラメータ
	fadeOutParameter_ = 0.0f;



	// ガラスの効果音
	phGlassBreaks_ = engine_->PlaySoundData(shGlassBreaks_, 0.5f);

	// ガラスの効果音のピッチを下げる
	phGlassBreaksPitch_ = 0.0f;
	engine_->SetPitch(phGlassBreaks_, phGlassBreaksPitch_);
}

/// <summary>
/// ビヘイビア : フェードアウト : 更新処理
/// </summary>
void TitleScene::BehaviorFadeOutUpdate()
{
	// パラメータを進める
	fadeOutParameter_ += 1.0f / 60.0f;
	fadeOutParameter_ = std::min(fadeOutParameter_, kFadeOutPrameterMax);

	// 最大値になったら終了する
	if (fadeOutParameter_ >= kFadeOutPrameterMax)
	{
		isFinished_ = true;
		return;
	}



	// ピッチの速度を上げる
	phGlassBreaksPitch_ += 1.0f / 60.0f;
	phGlassBreaksPitch_ = std::min(phGlassBreaksPitch_, 1.0f);
	engine_->SetPitch(phGlassBreaks_, phGlassBreaksPitch_);
}

/// <summary>
/// ビヘイビア : フェードアウト : 描画処理
/// </summary>
void TitleScene::BehaviorFadeOutDraw()
{

}