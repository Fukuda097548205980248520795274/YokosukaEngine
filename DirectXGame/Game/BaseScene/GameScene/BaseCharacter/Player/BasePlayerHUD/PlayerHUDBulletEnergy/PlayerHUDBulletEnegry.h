#pragma once
#include "../BasePlayerHUD.h"

class PlayerHUDBulletEnegry : public BasePlayerHUD
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera3d"></param>
	void Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const ModelHandleStore* modelHandleStore) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 最大の弾エネルギーのSetter
	/// </summary>
	/// <param name="maxBulletEnergy"></param>
	void SetMaxBulletEnergy(float maxBulletEnergy) { maxBulletEnergy_ = maxBulletEnergy; }

	/// <summary>
	/// 弾エネルギーのSetter
	/// </summary>
	/// <param name="bulletEnergy"></param>
	void SetBulletEnergy(const float* bulletEnergy) { bulletEnergy_ = bulletEnergy; }


private:

	// 最大の弾エネルギー
	float maxBulletEnergy_ = 0.0f;

	// 弾エネルギー
	const float* bulletEnergy_ = nullptr;
};

