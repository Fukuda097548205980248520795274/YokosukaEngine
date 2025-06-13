#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"
#include <cstdint>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <cassert>

// マップチップの種類
enum class MapChipType
{
	// 空白
	kBlank,

	// ブロック
	kBlock
};

// マップチップデータの構造体
struct MapChipData
{
	std::vector<std::vector<MapChipType>> data;
};

class MapChipField
{
public:

	// マップ番号
	struct IndexSet
	{
		uint32_t xIndex;
		uint32_t yIndex;
	};

	// 範囲矩形
	struct Rect
	{
		float left;
		float right;
		float bottom;
		float top;
	};

public:

	/// <summary>
	/// 読み込み前のリセット
	/// </summary>
	void ResetMapChipData();

	/// <summary>
	/// マップチップのCSVファイルを読み込む
	/// </summary>
	/// <param name="filePath"></param>
	void LoadMapChipCsv(const std::string& filePath);

	/// <summary>
	/// マップチップ番号で、指定したマップチップの種類を取得する
	/// </summary>
	/// <param name="xIndex"></param>
	/// <param name="yIndex"></param>
	/// <returns></returns>
	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex) const;

	/// <summary>
	/// マップチップ番号で、指定したマップチップの座標を取得する
	/// </summary>
	/// <param name="xIndex"></param>
	/// <param name="yIndex"></param>
	/// <returns></returns>
	Vector3 GetMapCihpPositionByIndex(uint32_t xIndex, uint32_t yIndex) const;

	/// <summary>
	/// ブロックの行数のGetter
	/// </summary>
	/// <returns></returns>
	uint32_t GetBlockRow() const { return kBlockRow; }

	/// <summary>
	/// ブロックの列数のGetter
	/// </summary>
	/// <returns></returns>
	uint32_t GetBlockColumn() const { return kBlockColumn; }

	/// <summary>
	/// 座標で、マップチップ番号を取得する
	/// </summary>
	/// <param name="poisition"></param>
	/// <returns></returns>
	IndexSet GetMapChipIndexSetByPosition(const Vector3& position) const;

	/// <summary>
	/// マップチップ番号で、ブロックの矩形範囲を求める
	/// </summary>
	/// <param name="xIndex"></param>
	/// <param name="yIndex"></param>
	/// <returns></returns>
	Rect GetRectByIndex(uint32_t xIndex, uint32_t yIndex) const;


private:

	// マップチップデータの配列
	MapChipData mapChipData_;

	// ブロックのサイズ
	const float kBlockSize = 2.0f;

	// ブロックの行数
	const uint32_t kBlockRow = 20;

	// ブロックの列数
	const uint32_t kBlockColumn = 100;
};

