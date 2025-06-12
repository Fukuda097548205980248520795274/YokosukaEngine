#include "Blocks.h"

/// <summary>
/// デストラクタ
/// </summary>
Blocks::~Blocks()
{
	// ワールドトランスフォーム
	for (std::vector<WorldTransform*>& worldTransformBlocksRow : worldTransforms_)
	{
		for (WorldTransform* worldTransformBlock : worldTransformBlocksRow)
		{
			delete worldTransformBlock;
		}
	}
	worldTransforms_.clear();
}

/// <summary>
/// 
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
/// <param name="directionalLight"></param>
void Blocks::Initialize(const YokosukaEngine* engine, const MapChipField* mapChipField, const Camera3D* camera3d, const DirectionalLight* directionalLight)
{
	// nullptrチェック
	assert(engine);
	assert(mapChipField);
	assert(camera3d);
	assert(directionalLight);

	// 引数を受け取る
	engine_ = engine;
	mapChipField_ = mapChipField;
	camera3d_ = camera3d;
	directionalLight_ = directionalLight;


	// ワールドトランスフォームを生成・初期化し、ブロックを配置する
	for (uint32_t row = 0; row < mapChipField_->GetBlockRow(); ++row)
	{
		// 行
		std::vector<WorldTransform*> worldTransformBlocksRow;

		for (uint32_t column = 0; column < mapChipField_->GetBlockColumn(); ++column)
		{
			// 何もないところには配置しない
			if (mapChipField_->GetMapChipTypeByIndex(column, row) == MapChipType::kBlank)
				continue;

			// ワールドトランスフォームの生成と初期化
			WorldTransform* worldTransformBlock = new WorldTransform();
			worldTransformBlock->Initialize();

			// 配置する
			worldTransformBlock->translation_ = mapChipField_->GetMapCihpPositionByIndex(column, row);

			// 登録する
			worldTransformBlocksRow.push_back(worldTransformBlock);
		}

		// 行を登録する
		worldTransforms_.push_back(worldTransformBlocksRow);
	}

	// UVトランスフォームの生成と初期化
	uvTransform_ = std::make_unique<UvTransform>();
	uvTransform_->Initialize();

	// モデルを読み込む
	modelHandle_ = engine_->LoadModelData("./Resources/Models/Cube", "Cube.obj");


	// ポイントライトの生成と初期化
	pointLight_ = std::make_unique<PointLight>();
	pointLight_->Initialize();
	pointLight_->intensity_ = 0.0f;

	// スポットライトの生成と初期化
	spotLight_ = std::make_unique<SpotLight>();
	spotLight_->Initialize();
	spotLight_->intensity_ = 0.0f;
}

/// <summary>
/// 更新処理
/// </summary>
void Blocks::Update()
{
	// ワールドトランスフォームを更新する
	for (std::vector<WorldTransform*>& worldTransformBlocksRow : worldTransforms_)
	{
		for (WorldTransform* worldTransformBlock : worldTransformBlocksRow)
		{
			worldTransformBlock->UpdateWorldMatrix();
		}
	}

	// UVトランスフォームの更新
	uvTransform_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void Blocks::Draw()
{
	// ブロックを描画する
	for (std::vector<WorldTransform*>& worldTransformBlocksRow : worldTransforms_)
	{
		for (WorldTransform* worldTransformBlock : worldTransformBlocksRow)
		{
			engine_->DrawModel(worldTransformBlock, uvTransform_.get(), camera3d_, modelHandle_, { 1.0f , 1.0f , 1.0f , 1.0f },
				directionalLight_, pointLight_.get(), spotLight_.get());
		}
	}
}