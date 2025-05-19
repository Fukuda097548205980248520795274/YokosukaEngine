#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <cassert>

// マテリアルデータ
typedef struct MaterialData
{
	// テクスチャのファイルパス
	std::string textureFilePath;
}MaterialData;

/// <summary>
/// Mtlファイルを読み込む
/// </summary>
/// <param name="directoryPath"></param>
/// <param name="fileName"></param>
/// <returns></returns>
MaterialData LoadMtlFile(const std::string& directoryPath, const std::string& fileName);