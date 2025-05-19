#include "LoadMaterialData.h"

/// <summary>
/// Mtlファイルを読み込む
/// </summary>
/// <param name="directoryPath"></param>
/// <param name="fileName"></param>
/// <returns></returns>
MaterialData LoadMtlFile(const std::string& directoryPath, const std::string& fileName)
{
	/*--------------------
	    必要な変数の宣言
	--------------------*/

	// 構築するマテリアルデータ
	MaterialData materialData;

	// ファイルから読んだ1行を格納するもの
	std::string line;


	/*------------------------------------
	    ファイルを読みモデルデータを構築する
	------------------------------------*/

	// ファイルを開く
	std::ifstream file(directoryPath + "/" + fileName);
	assert(file.is_open());

	while (std::getline(file, line))
	{
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		// identifierに応じた処理
		if (identifier == "map_Kd")
		{
			std::string textureFileName;
			s >> textureFileName;

			// 連結してファイルパスにする
			materialData.textureFilePath = directoryPath + "/" + textureFileName;
		}
	}

	return materialData;
}