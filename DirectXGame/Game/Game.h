#pragma once
#include "../YokosukaEngine/Include/YokosukaEngine.h"
#include "GameScene/GameScene.h"
#include "TitleScene/TitleScene.h"

class Game
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine">エンジン</param>
	void Initialize(const YokosukaEngine* engine);

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

	// シーン
	std::unique_ptr<Scene> scene_ = nullptr;


	/*-------------
	    フェーズ
	-------------*/

	enum Phase
	{
		kTitle,
		kGame
	};

	// 現在のフェーズ
	Phase phase_ = kTitle;

	// 次のフェーズのリクエスト
	std::optional<Phase> phaseRequest_ = std::nullopt;


};