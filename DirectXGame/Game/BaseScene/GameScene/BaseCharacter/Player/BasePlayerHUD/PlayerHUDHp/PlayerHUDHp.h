#pragma once
#include "../BasePlayerHUD.h"

class PlayerHUDHp : public BasePlayerHUD
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
	/// 体力のSetter
	/// </summary>
	/// <param name="hp"></param>
	void SetHp(const int32_t* hp) { hp_ = hp; }

	/// <summary>
	/// 最大体力値のSetter
	/// </summary>
	/// <param name="maxHp"></param>
	void SetMaxHp(int32_t maxHp) { maxHp_ = maxHp; }


private:

	// 最大HP
	int32_t maxHp_ = 0;

	// HP
	const int32_t* hp_ = nullptr;
};

