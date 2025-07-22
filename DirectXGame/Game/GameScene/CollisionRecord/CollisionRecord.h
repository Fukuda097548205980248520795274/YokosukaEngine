#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"

class CollisionRecord
{
public:

	/// <summary>
	/// 履歴を追加する
	/// </summary>
	/// <param name="record"></param>
	void AddRecord(uint32_t record);

	/// <summary>
	/// 履歴をチェックする
	/// </summary>
	/// <param name="number"></param>
	/// <returns></returns>
	bool RecordCheck(uint32_t number);

	/// <summary>
	/// 履歴を抹消する
	/// </summary>
	void RecordClear();


private:

	// 履歴
	std::vector<uint32_t> records_;
};

