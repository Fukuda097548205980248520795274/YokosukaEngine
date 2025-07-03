#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"

// 前方宣言
class Planet;
class GravitationalField;

class Player
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera3d"></param>
	void Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const Vector3& position);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// ワールド座標のGetter
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition();

	/// <summary>
	/// 当たり判定の球のGetter
	/// </summary>
	/// <returns></returns>
	Sphere GetCollisionSphere();

	/// <summary>
	/// 衝突判定応答
	/// </summary>
	/// <param name="planet"></param>
	void OnCollision(const Planet* planet);

	/// <summary>
	/// 衝突判定応答
	/// </summary>
	void OnCollision(const GravitationalField* gravitationalField);


private:

	/// <summary>
	/// 入力操作
	/// </summary>
	void Input();

	/// <summary>
	/// 移動操作
	/// </summary>
	void MoveInput();

	/// <summary>
	/// 最も近い惑星を探す
	/// </summary>
	void SarchNearPlanet();


	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// カメラ
	const Camera3D* camera3d_ = nullptr;


	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;

	// UVトランスフォーム
	std::unique_ptr<UvTransform> uvTransform_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;

	// デルタタイム
	const float kDeltaTime = 1.0f / 60.0f;


	// 半径
	float radius_ = 1.0f;

	// 角度
	float theta_ = 0.0f;
	float phi_ = 0.0f;

	// 角速度
	float anglerTheta_ = 0.0f;

	// 移動角速度
	const float kAnglerTheta = 0.01f;

	// 移動フラグ
	bool isMove_ = false;

	// 速度ベクトル
	Vector3 velocity_ = {0.0f , 0.0f , 0.0f};



	// 速度
	float speed_ = 0.0f;

	// ジャンプの初速
	const float kJumpStartSpeed = 6.0f;

	// 落下速度
	float fallUpSpeed = 0.0f;

	// 落下上昇ベクトル
	Vector3 fallUpVelocity_ = { 0.0f , 0.0f ,0.0f };



	// 重力に引っ張られているかどうか
	bool isGravityPull_ = false;

	// 着地したかどうか
	bool isGround_ = false;

	// 重力場の中心方向ベクトル
	Vector3 toGravity_ = { 0.0f , 0.0f , 0.0f };

	// 落下速度
	const float kFallSpeed = 4.9f;



	// 惑星の位置ベクトル
	Vector3 planetPosition_ = { 0.0f , 0.0f , 0.0f };

	// プレイヤーから惑星の距離
	float toPlanetLength_ = 0.0f;


	// 最も近い惑星リスト
	std::list<std::pair<float, Planet*>> nearPlanets_;

	// 乗っている惑星
	Planet* ridePlanet_ = nullptr;

	// 惑星を切り替える経過タイマー
	float planetChangeTimer_ = 0.0f;

	// 惑星を切り替える時間
	const float kPlanetChangeTime = 0.8f;
};

