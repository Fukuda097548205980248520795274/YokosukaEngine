#pragma once
#include "../YokosukaEngine/Include/YokosukaEngine.h"
#include "GameScene/GameScene.h"
#include "TitleScene/TitleScene.h"

// シーン
enum class Scene
{
	kUnknown = 0,

	// タイトルシーン
	kTitle,
	
	// ゲームシーン
	kGame
};

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

	/// <summary>
	/// シーン状態遷移
	/// </summary>
	void ChangeScene();

	/// <summary>
	/// シーンを更新する
	/// </summary>
	void UpdateScene();

	/// <summary>
	/// シーンを描画する
	/// </summary>
	void DrawScene();


private:

	// エンジン
	const YokosukaEngine* engine_ = nullptr;


	/*--------------
	    ゲーム制作
	--------------*/

	// 現在のシーン
	Scene scene_ = Scene::kTitle;

	// タイトルシーン
	std::unique_ptr<TitleScene> titleScene_ = nullptr;

	// ゲームシーン
	std::unique_ptr<GameScene> gameScene_ = nullptr;
};