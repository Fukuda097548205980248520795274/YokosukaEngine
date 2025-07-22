#include "CollisionRecord.h"

/// <summary>
/// 履歴を追加する
/// </summary>
/// <param name="record"></param>
void CollisionRecord::AddRecord(uint32_t record)
{
	records_.push_back(record);
}

/// <summary>
/// 履歴をチェックする
/// </summary>
/// <param name="number"></param>
/// <returns></returns>
bool CollisionRecord::RecordCheck(uint32_t number)
{
	for (uint32_t& record : records_)
	{
		if (record == number)
			return true;
	}

	return false;
}

/// <summary>
/// 履歴を抹消する
/// </summary>
void CollisionRecord::RecordClear()
{
	records_.clear();
}