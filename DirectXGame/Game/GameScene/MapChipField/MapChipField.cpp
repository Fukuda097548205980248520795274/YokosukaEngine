#include "MapChipField.h"

namespace
{
	// マップチップの番号 と CSVの番号 の対照表
	std::map < std::string, MapChipType > mapChipTable =
	{
		{"0",MapChipType::kBlank},
		{"1",MapChipType::kBlock}
	};
}

/// <summary>
/// 読み込み前のリセット
/// </summary>
void MapChipField::ResetMapChipData()
{
	// マップチップデータのリセット
	mapChipData_.data.clear();
	mapChipData_.data.resize(kBlockRow);
	for (std::vector<MapChipType>& mapChipDataLine : mapChipData_.data)
	{
		mapChipDataLine.resize(kBlockColumn);
	}
}

/// <summary>
/// マップチップのCSVファイルを読み込む
/// </summary>
/// <param name="filePath"></param>
void MapChipField::LoadMapChipCsv(const std::string& filePath)
{
	// マップチップデータをリセットする
	ResetMapChipData();

	// ファイルを開く
	std::ifstream file;
	file.open(filePath);
	assert(file.is_open());

	// マップチップCSV
	std::stringstream mapChipCsv;

	// ファイルの内容を文字列ストリームにコピー
	mapChipCsv << file.rdbuf();

	// ファイルを閉じる
	file.close();


	// CSVからマップチップデータを読み込む
	for (uint32_t row = 0; row < kBlockRow; ++row)
	{
		// CSVの行
		std::string line;
		std::getline(mapChipCsv, line);

		// 1行目の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		for (uint32_t column = 0; column < kBlockColumn; ++column)
		{
			// 文字（文字列）
			std::string word;

			// カンマ区切り
			std::getline(line_stream, word, ',');

			// 対照表にあるデータを探す
			if (mapChipTable.contains(word))
			{
				// 対照表に当たる文字を入れる
				mapChipData_.data[row][column] = mapChipTable[word];
			}
		}
	}
}

/// <summary>
/// マップチップ番号で、指定したマップチップの種類を取得する
/// </summary>
/// <param name="xIndex"></param>
/// <param name="yIndex"></param>
/// <returns></returns>
MapChipType MapChipField::GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex) const
{
	// x番号がマップチップデータの範囲外だったら、空白を返す
	if (xIndex < 0 || kBlockColumn - 1 < xIndex)
	{
		return MapChipType::kBlank;
	}

	// y番号がマップチップデータの範囲外だったら、空白を返す
	if (yIndex < 0 || kBlockRow - 1 < yIndex)
	{
		return MapChipType::kBlank;
	}

	return mapChipData_.data[yIndex][xIndex];
}

/// <summary>
/// マップチップ番号で、指定したマップチップの座標を取得する
/// </summary>
/// <param name="xIndex"></param>
/// <param name="yIndex"></param>
/// <returns></returns>
Vector3 MapChipField::GetMapCihpPositionByIndex(uint32_t xIndex, uint32_t yIndex) const
{
	Vector3 mapChipPosition =
	{
		kBlockSize * xIndex,
		(kBlockRow - 1 - yIndex) * kBlockSize,
		0.0f
	};

	return mapChipPosition;
}


/// <summary>
/// 座標で、マップチップ番号を取得する
/// </summary>
/// <param name="poisition"></param>
/// <returns></returns>
MapChipField::IndexSet MapChipField::GetMapChipIndexSetByPosition(const Vector3& position) const
{
	IndexSet indexSet;
	indexSet.xIndex = static_cast<uint32_t>((position.x + (kBlockSize / 2.0f)) / kBlockSize);
	indexSet.yIndex = kBlockRow - 1 - static_cast<uint32_t>((position.y + (kBlockSize / 2.0f)) / kBlockSize);

	return indexSet;
}


/// <summary>
/// マップチップ番号で、ブロックの矩形範囲を求める
/// </summary>
/// <param name="xIndex"></param>
/// <param name="yIndex"></param>
/// <returns></returns>
MapChipField::Rect MapChipField::GetRectByIndex(uint32_t xIndex, uint32_t yIndex) const
{

	// ブロックの中心点
	Vector3 center = GetMapCihpPositionByIndex(xIndex, yIndex);

	Rect rect;
	rect.left = center.x - kBlockSize / 2.0f;
	rect.right = center.x + kBlockSize / 2.0f;
	rect.bottom = center.y - kBlockSize / 2.0f;
	rect.top = center.y + kBlockSize / 2.0f;

	return rect;
}