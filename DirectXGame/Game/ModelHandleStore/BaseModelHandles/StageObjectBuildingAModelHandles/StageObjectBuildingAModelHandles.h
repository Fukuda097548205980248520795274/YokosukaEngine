#pragma once
#include "../BaseModelHandles.h"

class StageObjectBuildingAModelHandles : public BaseModelHandles
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	void Initialize(const YokosukaEngine* engine) override;
};

