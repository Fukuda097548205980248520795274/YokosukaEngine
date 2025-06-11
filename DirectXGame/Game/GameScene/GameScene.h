#pragma once
#include "../../YokosukaEngine/Include/YokosukaEngine.h"
#include "Player/Player.h"

class GameScene
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera3d"></param>
	void Initialize(const YokosukaEngine* engine , const Camera3D* camera3d);

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

	// 平行光源
	std::unique_ptr<DirectionalLight> directionalLight_ = nullptr;


	// プレイヤー
	std::unique_ptr<Player> player_ = nullptr;
};

