#include "TimedCall.h"

/// <summary>
/// 更新処理
/// </summary>
void TimedCall::Update()
{
	// 完了したら処理しない
	if (isComplate_)
		return;

	// 時間を進める
	time_ -= 1.0f / 60.0f;

	// 時間が終了したら、コールバック関数を呼び出し、完了する
	if (time_ <= 0.0f)
	{
		// コールバック関数を呼び出し
		f_();

		isComplate_ = true;
	}

}