#include "LoadModelData.h"

/// <summary>
/// Objファイルを読み込む
/// </summary>
/// <param name="directoryPath">ディレクトリのパス</param>
/// <param name="fileName">ファイル名</param>
/// <returns></returns>
ModelData LoadObjFile(const std::string& directoryPath, const std::string& fileName)
{
	// 構築するモデルデータ
	ModelData modelData;


	/*------------------
	    ファイルを開く
	------------------*/

	// ファイルを開く
	Assimp::Importer importer;
	std::string filePath = directoryPath + "/" + fileName;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);

	// メッシュがないのは対応しない
	assert(scene->HasMeshes());


	/*--------------------
	    データを読み込む
	--------------------*/

	/*   メッシュを解析する   */

	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
	{

		aiMesh* mesh = scene->mMeshes[meshIndex];

		// 法線がないMeshは今回は非対応
		assert(mesh->HasNormals());

		// テクスチャ座標系がないMeshは非対応
		assert(mesh->HasTextureCoords(0));


		/*   フェイスを解析する   */

		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex)
		{
			aiFace& face = mesh->mFaces[faceIndex];

			// 三角形のみサポート
			assert(face.mNumIndices == 3);


			/*   フェイスの中身（頂点）の解析を行う   */

			for (uint32_t element = 0; element < face.mNumIndices; ++element)
			{
				uint32_t vertexIndex = face.mIndices[element];
				aiVector3D& position = mesh->mVertices[vertexIndex];
				aiVector3D& normal = mesh->mNormals[vertexIndex];
				aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];

				// 頂点
				VertexData vertex;
				vertex.position = { position.x , position.y , position.z , 1.0f };
				vertex.normal = { normal.x , normal.y , normal.z };
				vertex.texcoord = { texcoord.x , texcoord.y };

				// 右手座標系 -> 左手座標系
				vertex.position.x *= -1.0f;
				vertex.normal.x *= -1.0f;

				// 頂点を登録する
				modelData.vertices.push_back(vertex);
			}
		}
	}


	/*   マテリアルを解析する   */

	for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex)
	{
		aiMaterial* material = scene->mMaterials[materialIndex];

		if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0)
		{
			aiString textureFilePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
			modelData.material.textureFilePath = directoryPath + "/" + textureFilePath.C_Str();
		}
	}


	return modelData;
}