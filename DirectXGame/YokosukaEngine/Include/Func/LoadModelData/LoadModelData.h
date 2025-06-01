#pragma once
#include <vector>
#include <string>
#include <cassert>
#include <fstream>
#include <sstream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../../Draw/VertexData/VertexData.h"
#include "../LoadMaterialData/LoadMaterialData.h"

// モデルデータ
typedef struct ModelData
{
	// 頂点
	std::vector<VertexData> vertices;

	// マテリアルデータ
	MaterialData material;
}ModelData;

/// <summary>
/// Objファイルを読み込む
/// </summary>
/// <param name="directoryPath">ディレクトリのパス</param>
/// <param name="fileName">ファイル名</param>
/// <returns></returns>
ModelData LoadObjFile(const std::string& directoryPath, const std::string& fileName);