#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"
#include <sstream>

// 前方宣言
class GameScene;

class CenterAxis
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera3d"></param>
	void Initliaze(const YokosukaEngine* engine , const Camera3D* camera3d);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// ゲームシーンのSetter
	/// </summary>
	/// <param name="gameScene"></param>
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	/// <summary>
	/// ワールドトランスフォームのGetter
	/// </summary>
	/// <returns></returns>
	WorldTransform* GetWorldTransform() const { return worldTransform_.get(); }

	/// <summary>
	/// ワールド座標のGetter
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition()const;

	/// <summary>
	/// ゲームタイマーのSetter
	/// </summary>
	/// <param name="gameTimer"></param>
	void SetGameTimer(const float* gameTimer) { gameTimer_ = gameTimer; }

	/// <summary>
	/// ゴールフラグのGetter
	/// </summary>
	/// <returns></returns>
	bool IsGoal()const { return isGoal_; }

	/// <summary>
	/// 制御点スクリプトを読み込む
	/// </summary>
	/// <param name="filePath"></param>
	void LoadControlPointScript(const char* filePath);


private:

	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// カメラ
	const Camera3D* camera3d_ = nullptr;

	// ゲームタイム
	const float* gameTimer_ = nullptr;

	// ゲームシーン
	GameScene* gameScene_ = nullptr;


	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;

	// UVトランスフォーム
	std::unique_ptr<UvTransform> uvTransform_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;



	/*--------------------
	    制御点スクリプト
	--------------------*/

	// 制御点スクリプトの文字ストリーム
	std::stringstream controlPointStream_;

	// ゴールフラグ
	bool isGoal_ = false;

	float t_ = 0.0f;

	std::vector<Vector3> controlPoint_;
};

