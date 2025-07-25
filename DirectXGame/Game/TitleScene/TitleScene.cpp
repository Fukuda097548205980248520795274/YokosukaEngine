#include "TitleScene.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
void TitleScene::Initialize(const YokosukaEngine* engine)
{
	// 基底クラス初期化
	Scene::Initialize(engine);

	// サウンドを読み込む
	shGlassBreaks_ = engine_->LoadSound("./Resources/Sounds/Se/title/glass.mp3");
}

/// <summary>
/// 更新処理
/// </summary>
void TitleScene::Update()
{
	// 基底クラス更新処理
	Scene::Update();

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
	Scene::Draw();
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
}

/// <summary>
/// ビヘイビア : フェードイン : 更新処理
/// </summary>
void TitleScene::BehaviorFadeInUpdate()
{
	// パラメータを進める
	fadeInParameter_ += 1.0f / 60.0f;
	fadeInParameter_ = std::min(fadeInParameter_, kFadeInPrameterMax);

	// 最大値になったら操作に遷移する
	if (fadeInParameter_ >= kFadeInPrameterMax)
	{
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
	if (engine_->GetKeyTrigger(DIK_A))
	{
		behaviorRequest_ = kFadeOut;
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
}

/// <summary>
/// ビヘイビア : フェードアウト : 描画処理
/// </summary>
void TitleScene::BehaviorFadeOutDraw()
{

}