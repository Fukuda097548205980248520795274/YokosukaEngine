#pragma once
#include "../BaseModelHandles.h"

class PlayerBulletStrongModelHandles : public BaseModelHandles
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	void Initialize(const YokosukaEngine* engine) override;
};

