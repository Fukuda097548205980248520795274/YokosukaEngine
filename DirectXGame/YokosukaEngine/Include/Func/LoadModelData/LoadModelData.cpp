#include "LoadModelData.h"

/// <summary>
/// Objファイルを読み込む
/// </summary>
/// <param name="directoryPath">ディレクトリのパス</param>
/// <param name="fileName">ファイル名</param>
/// <returns></returns>
ModelData LoadObjFile(const std::string& directoryPath, const std::string& fileName)
{
	/*------------------------
	    必要となる変数の宣言
	------------------------*/

	// 構築するモデルデータ
	ModelData modelData;

	// 位置（ローカル座標）
	std::vector<Vector4> positions;

	// 法線
	std::vector<Vector3> normals;

	// テクスチャ座標
	std::vector<Vector2> texcoords;

	// ファイルから読んだ1行を格納するもの
	std::string line;


	/*------------------
	    ファイルを開く
	------------------*/

	// ファイルを開く
	std::ifstream file(directoryPath + "/" + fileName);
	assert(file.is_open());


	/*--------------------------------------
	    ファイルを読み、モデルデータを構築する
	--------------------------------------*/

	while (std::getline(file, line))
	{
		std::string identifier;
		std::istringstream s(line);

		// 先頭の識別子を読む
		s >> identifier;

		
		/*   頂点情報を読む   */

		// 位置（ローカル座標）
		if (identifier == "v")
		{
			Vector4 position;
			s >> position.x >> position.y >> position.z;
			position.x *= -1.0f;
			position.w = 1.0f;
			positions.push_back(position);
		}
		else if (identifier == "vt")
		{
			// テクスチャ座標
			Vector2 texcoord;
			s >> texcoord.x >> texcoord.y;
			texcoord.y = 1.0f - texcoord.y;
			texcoords.push_back(texcoord);
		}
		else if (identifier == "vn")
		{
			// 法線
			Vector3 normal;
			s >> normal.x >> normal.y >> normal.z;
			normal.x *= -1.0f;

			normals.push_back(normal);
		}
		else if (identifier == "f")
		{
			/*   三角形を作る   */

			VertexData triangle[3] = {};

			// 面は三角形限定
			for (uint32_t faceVertex = 0; faceVertex < 3; ++faceVertex)
			{
				std::string vertexDefinition;
				s >> vertexDefinition;

				// 頂点の要素へのインデックスは「位置 /UV/法線」で格納されているので、分解してインデックスを取得する
				std::istringstream v(vertexDefinition);
				uint32_t elementIndices[3];
				for (uint32_t element = 0; element < 3; ++element)
				{
					std::string index;

					// 区切りでインデックスを読んでいく
					std::getline(v, index, '/');
					elementIndices[element] = std::stoi(index);
				}

				// 要素へのインデックスから、実際の要素の値を取得して、頂点を構築する
				Vector4 position = positions[elementIndices[0] - 1];
				Vector2 texcoord = texcoords[elementIndices[1] - 1];
				Vector3 normal = normals[elementIndices[2] - 1];

				triangle[faceVertex] = {position , texcoord , normal};
			}

			modelData.vertices.push_back(triangle[2]);
			modelData.vertices.push_back(triangle[1]);
			modelData.vertices.push_back(triangle[0]);
		}
		else if (identifier == "mtllib")
		{
			std::string materialFileName;
			s >> materialFileName;
			modelData.material = LoadMtlFile(directoryPath, materialFileName);
		}
	}

	return modelData;
}