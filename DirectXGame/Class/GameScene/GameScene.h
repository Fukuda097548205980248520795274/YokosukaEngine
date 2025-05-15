#pragma once
#include "../../YokosukaEngine/Include/YokosukaEngine.h"
#include "Player/Player.h"

class GameScene
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(YokosukaEngine* engine);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();


private:

	// カメラ
	Camera3D* camera_ = nullptr;

	// エンジン
	YokosukaEngine* engine_ = nullptr;

	// プレイヤー
	Player* player_ = nullptr;
};

