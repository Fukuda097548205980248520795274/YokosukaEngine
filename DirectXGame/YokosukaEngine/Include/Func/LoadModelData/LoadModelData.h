#pragma once
#include <vector>
#include <string>
#include <cassert>
#include <fstream>
#include <sstream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../../Math/Matrix4x4/Matrix4x4.h"
#include "../../Draw/VertexData/VertexData.h"
#include "../LoadMaterialData/LoadMaterialData.h"

struct Node
{
	// ローカル行列
	Matrix4x4 localMatrix;

	// ノードの名前
	std::string name;

	// 子ノード
	std::vector<Node> children;
};

// モデルデータ
struct ModelData
{
	// 頂点
	std::vector<VertexData> vertices;

	// マテリアルデータ
	MaterialData material;

	Node rootNode;
};

/// <summary>
/// Objファイルを読み込む
/// </summary>
/// <param name="directoryPath">ディレクトリのパス</param>
/// <param name="fileName">ファイル名</param>
/// <returns></returns>
ModelData LoadObjFile(const std::string& directoryPath, const std::string& fileName);

/// <summary>
/// ノードを読む
/// </summary>
/// <param name="node"></param>
/// <returns></returns>
Node ReadNode(aiNode* node);