#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"
#include "Emitter/Emitter.h"

class Player
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="camera">カメラ</param>
	void Initialize(YokosukaEngine* engine, Camera3D* camera);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// ワールドトランスフォームのGetter
	/// </summary>
	/// <returns></returns>
	WorldTransform* GetWorldTransform() { return worldTransform_; }


private:

	// エンジン
	YokosukaEngine* engine_ = nullptr;

	// カメラ
	Camera3D* camera_ = nullptr;

	// ワールドトランスフォーム
	WorldTransform* worldTransform_ = nullptr;

	// UVトランスフォーム
	WorldTransform* uvTransform_ = nullptr;

	// テクスチャ
	uint32_t textureHandle_;
	uint32_t textureHandleTable_[3];

	// テクスチャの種類
	enum class TextureHandle
	{
		UvChecker,
		MonsterBall,
		White
	};

	// 色
	Engine::Vector4 color_{ 1.0f , 1.0f , 1.0f , 1.0f };

	// エミッターのリスト
	std::list<Emitter*> emitters_;
};

