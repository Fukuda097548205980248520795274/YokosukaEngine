#include "CenterAxis.h"
#include "../../GameScene.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
void CenterAxis::Initliaze(const YokosukaEngine* engine, const Camera3D* camera3d)
{
	// nullptrチェック
	assert(engine);
	assert(camera3d);

	// 引数を受けとる
	engine_ = engine;
	camera3d_ = camera3d;

	// ワールドトランスフォームの生成
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
	worldTransform_->translation_.y = 10.0f;

	// UVトランスフォーム
	uvTransform_ = std::make_unique<UvTransform>();
	uvTransform_->Initialize();

	// テクスチャを読み込む
	textureHandle_ = engine_->LoadTexture("./Resources/Textures/white2x2.png");
}

/// <summary>
/// 更新処理
/// </summary>
void CenterAxis::Update()
{

	//t_ += 0.0005f * (*gameTimer_);

	// ゴールしたかどうか
	if (t_ + 0.0001f > 1.0f)
	{
		isGoal_ = true;
		return;
	}

	// 現在の位置
	worldTransform_->translation_ = engine_->CatmullRomPosition(controlPoint_, t_);
	Vector3 nextPos = engine_->CatmullRomPosition(controlPoint_, t_ + 0.0001f);

	// 次の位置
	Vector3 toNextPos = nextPos - worldTransform_->translation_;

	// 進む方向を向く
	worldTransform_->rotation_.y = std::atan2(toNextPos.x, toNextPos.z);
	float length = std::sqrt(std::pow(toNextPos.x, 2.0f) + std::pow(toNextPos.z, 2.0f));
	worldTransform_->rotation_.x = std::atan2(-toNextPos.y, length);

	// ワールドトラスフォームの更新
	worldTransform_->UpdateWorldMatrix();
	uvTransform_->UpdateWorldMatrix();
}

/// <summary>
/// 描画処理
/// </summary>
void CenterAxis::Draw()
{
	// 制御点の描画
	engine_->DrwaCatmullRomSpline(controlPoint_, Vector4(1.0f, 0.0f, 0.0f, 1.0f), camera3d_);
}


/// <summary>
/// ワールド座標のGetter
/// </summary>
/// <returns></returns>
Vector3 CenterAxis::GetWorldPosition()const
{
	// ワールド座標
	Vector3 worldPosition;

	worldPosition.x = worldTransform_->worldMatrix_.m[3][0];
	worldPosition.y = worldTransform_->worldMatrix_.m[3][1];
	worldPosition.z = worldTransform_->worldMatrix_.m[3][2];

	return worldPosition;
}

/// <summary>
/// 制御点スクリプトを読み込む
/// </summary>
/// <param name="filePath"></param>
void CenterAxis::LoadControlPointScript(const char* filePath)
{
	// nullptrチェック
	assert(filePath);

	// ファイルを開く
	std::ifstream file;
	file.open(filePath);
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	controlPointStream_ << file.rdbuf();

	// ファイルを閉じる
	file.close();


	// 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (std::getline(controlPointStream_, line))
	{
		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		// 文字
		std::string word;

		// カッコまでの文字を取得する
		std::getline(line_stream, word, '(');

		// "//"から始まるのはコメントのため無視する
		if (word.find("//") == 0)
		{
			continue;
		}


		// 点
		if (word.find("P") == 0)
		{
			// 3次元ベクトル
			Vector3 pos = Vector3(0.0f, 0.0f, 0.0f);

			// X軸
			std::getline(line_stream, word, ',');
			pos.x = (float)std::atof(word.c_str());

			// Y軸
			std::getline(line_stream, word, ',');
			pos.y = (float)std::atof(word.c_str());

			// Z軸
			std::getline(line_stream, word, ')');
			pos.z = (float)std::atof(word.c_str());

			// 追加する
			controlPoint_.push_back(pos);
		}
	}
}