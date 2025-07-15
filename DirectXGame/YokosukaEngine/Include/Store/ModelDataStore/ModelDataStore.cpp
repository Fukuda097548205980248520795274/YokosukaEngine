#include "ModelDataStore.h"

/// <summary>
/// デストラクタ
/// </summary>
ModelDataStore::~ModelDataStore()
{
	// モデルの情報
	for (ModelInfo* modelInfo : modelInfo_)
	{
		delete modelInfo;
	}
	modelInfo_.clear();
}

/// <summary>
/// 初期化
/// </summary>
void ModelDataStore::Initialize(TextureStore* textureStore)
{
	// nullptrチェック
	assert(textureStore);

	// 引数を受け取る
	textureStore_ = textureStore;
}

/// <summary>
/// モデルハンドルを取得する
/// </summary>
/// <param name="directoryPath"></param>
/// <param name="filePath"></param>
uint32_t ModelDataStore::GetModelHandle(const std::string& directoryPath, const std::string& fileName,
	ID3D12Device* device, Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap ,
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList)
{
	// 同じモデルデータを使われているか
	for (uint32_t i = 0; i < useModelDataNum_; i++)
	{
		if (strcmp(directoryPath.c_str(), modelInfo_[i]->directoryPath.c_str())) { continue; }
		if (strcmp(fileName.c_str(), modelInfo_[i]->fileName.c_str())) { continue; }

		return modelInfo_[i]->modelHandle;
	}

	// 新規モデル情報
	ModelInfo* modelInfo = new ModelInfo();

	// モデルハンドルを決める
	while (modelInfo->modelHandle == 0)
	{
		modelInfo->modelHandle = rand() % 100000 + 1;

		// モデルハンドルが被らないようにする
		for (uint32_t j = 0; j < useModelDataNum_; j++)
		{
			if (useModelDataNum_ == j)
				continue;

			if (modelInfo->modelHandle == modelInfo_[j]->modelHandle)
			{
				modelInfo->modelHandle = 0;
				break;
			}
		}
	}

	// パスを受け取る
	modelInfo->directoryPath = directoryPath;
	modelInfo->fileName = fileName;

	// モデルを読み込む
	modelInfo->modelData = LoadObjFile(directoryPath, fileName);

	// 頂点リソースを作る
	modelInfo->vertexResource =
		CreateBufferResource(device, sizeof(VertexData) * modelInfo->modelData.vertices.size());

	// 頂点バッファビューを設定する
	modelInfo->vertexBufferView.BufferLocation = modelInfo->vertexResource->GetGPUVirtualAddress();
	modelInfo->vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * modelInfo->modelData.vertices.size());
	modelInfo->vertexBufferView.StrideInBytes = sizeof(VertexData);

	// 頂点データを書き込む
	modelInfo->vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&modelInfo->vertexData));
	std::memcpy(modelInfo->vertexData, modelInfo->modelData.vertices.data(),
		sizeof(VertexData) * modelInfo->modelData.vertices.size());

	// テクスチャハンドルを取得する
	if (strcmp(modelInfo->modelData.material.textureFilePath.c_str(), "") == 0)
	{
		modelInfo->textureHandle = textureStore_->GetTextureHandle("./Resources/Textures/white2x2.png",
			device, srvDescriptorHeap, commandList);
	}
	else
	{
		modelInfo->textureHandle =
			textureStore_->GetTextureHandle(modelInfo->modelData.material.textureFilePath,device, srvDescriptorHeap, commandList);
	}


	// ベクトルに追加する
	modelInfo_.push_back(modelInfo);

	// カウントする
	useModelDataNum_++;

	return modelInfo_[useModelDataNum_ - 1]->modelHandle;
}

/// <summary>
/// モデルハンドルのGetter
/// </summary>
/// <param name="modelHandle"></param>
/// <returns></returns>
ModelData ModelDataStore::GetModelData(uint32_t modelHandle)
{
	for (uint32_t i = 0; i < useModelDataNum_; i++)
	{
		if (modelHandle == modelInfo_[i]->modelHandle)
		{
			return modelInfo_[i]->modelData;
		}
	}

	assert(false);

	ModelData missData;
	return missData;
}

/// <summary>
/// 頂点リソースのGetter
/// </summary>
/// <param name="modelHandle"></param>
/// <returns></returns>
Microsoft::WRL::ComPtr<ID3D12Resource> ModelDataStore::GetVertexResource(uint32_t modelHandle)
{
	for (uint32_t i = 0; i < useModelDataNum_; i++)
	{
		if (modelHandle == modelInfo_[i]->modelHandle)
		{
			return modelInfo_[i]->vertexResource;
		}
	}

	assert(false);
	return nullptr;
}

/// <summary>
/// テクスチャハンドルを取得する
/// </summary>
/// <param name="modelHandle"></param>
/// <returns></returns>
uint32_t ModelDataStore::GetTextureHandle(uint32_t modelHandle)
{
	for (uint32_t i = 0; i < useModelDataNum_; i++)
	{
		if (modelHandle == modelInfo_[i]->modelHandle)
		{
			return modelInfo_[i]->textureHandle;
		}
	}

	assert(false);
	return 0;
}

/// <summary>
/// 頂点バッファビューを取得する
/// </summary>
/// <param name="modelHandle"></param>
/// <returns></returns>
D3D12_VERTEX_BUFFER_VIEW ModelDataStore::GetVertexBufferView(uint32_t modelHandle)
{
	for (uint32_t i = 0; i < useModelDataNum_; i++)
	{
		if (modelHandle == modelInfo_[i]->modelHandle)
		{
			return modelInfo_[i]->vertexBufferView;
		}
	}

	assert(false);
	D3D12_VERTEX_BUFFER_VIEW unknown{};
	return unknown;
}



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


	/*   ノードを解析する   */

	modelData.rootNode = ReadNode(scene->mRootNode);


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

/// <summary>
/// ノードを読む
/// </summary>
/// <param name="node"></param>
/// <returns></returns>
Node ReadNode(aiNode* node)
{
	Node result;

	// ノードのローカル行列を取得
	aiMatrix4x4 aiLocalMatrix = node->mTransformation;

	// 列ベクトルを行ベクトルに転置
	aiLocalMatrix.Transpose();

	// 行列の値を渡す
	for (uint32_t i = 0; i < 4; i++)
	{
		for (uint32_t j = 0; j < 4; j++)
		{
			result.localMatrix.m[i][j] = aiLocalMatrix[i][j];
		}
	}

	// ノード名を格納
	result.name = node->mName.C_Str();

	// 子供の数だけを確保
	result.children.resize(node->mNumChildren);

	for (uint32_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex)
	{
		// 再帰的に読み込んで階層構造を作っていく
		result.children[childIndex] = ReadNode(node->mChildren[childIndex]);
	}

	return result;
}


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