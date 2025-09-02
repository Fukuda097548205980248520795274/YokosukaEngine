#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"
#include "../../BaseScene/BaseScene.h"
#include "../../BaseSprite/BaseSprite.h"

#include "BaseCharacter/BaseCharacter.h"
#include "BaseCharacter/Player/Player.h"
#include "BaseCharacter/Enemy/Enemy.h"

class GameScene : public BaseScene
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine">エンジン</param>
	void Initialize(const YokosukaEngine* engine, const ModelHandleStore* modelHandleStore, const TextureHandleStore* textureHandleStore) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw()override;

	/// <summary>
	/// カメラをシェイクする
	/// </summary>
	/// <param name="shakeTime"></param>
	/// <param name="shakeSize"></param>
	void CameraShake(float shakeTime, float shakeSize) { mainCamera_->SetShake(shakeTime, shakeSize); }


private:


	// プレイヤー
	std::unique_ptr<Player> player_ = nullptr;

	// 敵のリスト
	std::list<std::unique_ptr<Enemy>> enemies_;

	// 平行光源
	std::unique_ptr<DirectionalLight> directionalLight_ = nullptr;
	
};
