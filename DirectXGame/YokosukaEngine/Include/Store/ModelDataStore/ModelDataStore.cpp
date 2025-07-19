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


	int i = 0;

	for (ModelData& aModelData : modelInfo->modelData)
	{
		// 頂点リソースを作る
		Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource =
			CreateBufferResource(device, sizeof(VertexData) * modelInfo->modelData[i].vertices.size());

		// 頂点バッファビューを設定する
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
		vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
		vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * modelInfo->modelData[i].vertices.size());
		vertexBufferView.StrideInBytes = sizeof(VertexData);
		modelInfo->vertexBufferView.push_back(vertexBufferView);

		// 頂点データを書き込む
		VertexData* vertexData;
		vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
		std::memcpy(vertexData, modelInfo->modelData[i].vertices.data(),
			sizeof(VertexData) * modelInfo->modelData[i].vertices.size());

		modelInfo->vertexResource.push_back(vertexResource);
		modelInfo->vertexData.push_back(vertexData);


		int textureHandle = 0;

		// テクスチャハンドルを取得する
		if (strcmp(modelInfo->modelData[i].material.textureFilePath.c_str(), "") == 0)
		{
			textureHandle = textureStore_->GetTextureHandle("./Resources/Textures/white2x2.png",
				device, srvDescriptorHeap, commandList);
		} 
		else
		{
			textureHandle =
				textureStore_->GetTextureHandle(modelInfo->modelData[i].material.textureFilePath, device, srvDescriptorHeap, commandList);
		}

		modelInfo->textureHandle.push_back(textureHandle);

		i++;
	}


	// ベクトルに追加する
	modelInfo_.push_back(modelInfo);

	// カウントする
	useModelDataNum_++;

	return modelInfo->modelHandle;
}

/// <summary>
/// モデル情報のGetter
/// </summary>
/// <param name="modelHandle"></param>
/// <returns></returns>
ModelInfo* ModelDataStore::GetModelInfo(uint32_t modelHandle)
{
	for (uint32_t i = 0; i < useModelDataNum_; i++)
	{
		if (modelHandle == modelInfo_[i]->modelHandle)
		{
			return modelInfo_[i];
		}
	}

	assert(false);

	return nullptr;
}



/// <summary>
/// Objファイルを読み込む
/// </summary>
/// <param name="directoryPath">ディレクトリのパス</param>
/// <param name="fileName">ファイル名</param>
/// <returns></returns>
std::vector<ModelData> LoadObjFile(const std::string& directoryPath, const std::string& fileName)
{
	// 構築するモデルデータ
	std::vector<ModelData> modelDataVector;


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
		ModelData modelData;

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

		// メッシュをカウントする
		modelDataVector.push_back(modelData);
	}


	/*   ノードを解析する   */

	for (ModelData& modelData : modelDataVector)
	{
		modelData.rootNode = ReadNode(scene->mRootNode);
	}

	/*   マテリアルを解析する   */

	// ファイルを開く
	std::ifstream ifs(filePath);
	assert(ifs.is_open());

	// 1行
	std::string line;

	// マテリアルファイル名
	std::string materialFileName;

	// マテリアルの名前
	std::vector<std::string> materialNames;

	// メッシュ宣言の後かどうかを判断する
	bool isMeshProcessed = false;

	while (std::getline(ifs, line))
	{
		std::string identifer;
		std::istringstream s(line);

		// 先頭の識別子を読む
		s >> identifer;

		if (identifer == "v" || identifer == "vt" || identifer == "vn")
		{
			isMeshProcessed = true;
		}

		if (isMeshProcessed)
		{
			if (identifer == "o")
			{
				materialNames.push_back("");
				isMeshProcessed = false;
			}
		}

		// mtlファイル名
		if (identifer == "mtllib")
		{
			// mtlファイル名を取得する
			s >> materialFileName;
			isMeshProcessed = false;
		}

		// mtl名
		if (identifer == "usemtl")
		{
			std::string materialName;
			s >> materialName;
			materialNames.push_back(materialName);
			isMeshProcessed = false;
		}
	}

	// ファイルを閉じる
	ifs.close();

	for (int i = 0; i < materialNames.size(); ++i)
	{
		modelDataVector[i].material = LoadMtlFile(directoryPath + "/", materialFileName, materialNames[i]);
	}


	return modelDataVector;
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


/// <summary>
/// Mtlファイルを読み込む
/// </summary>
/// <param name="directoryPath"></param>
/// <param name="fileName"></param>
/// <returns></returns>
MaterialData LoadMtlFile(const std::string& directoryPath, const std::string& fileName , std::string& materialName)
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
	if (!file.is_open())return materialData;

	// 読み込み準備ができたかどうか
	bool isLoad = false;

	while (std::getline(file, line))
	{
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		if (identifier == "newmtl")
		{
			std::string mtlName;
			s >> mtlName;
			if (mtlName == materialName)
			{
				isLoad = true;
			}
		}

		if (isLoad)
		{
			if (identifier == "map_Kd")
			{
				std::string textureFileName;
				s >> textureFileName;

				// 連結してファイルパスにする
				materialData.textureFilePath = directoryPath + "/" + textureFileName;

				break;
			}
		}
	}

	return materialData;
}