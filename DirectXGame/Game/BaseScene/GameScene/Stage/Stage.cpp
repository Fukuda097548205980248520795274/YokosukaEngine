#include "Stage.h"
#include "../GameScene.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
void Stage::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d,
	const TextureHandleStore* textureHandleStore, const ModelHandleStore* modelHandleStore, const float* gameFrame, GameScene* gameScene)
{
	// nullptrチェック
	assert(engine);
	assert(camera3d);
	assert(textureHandleStore);
	assert(modelHandleStore);
	assert(gameFrame);
	assert(gameScene);

	// 引数を受け取る
	engine_ = engine;
	camera3d_ = camera3d;
	textureHandleStore_ = textureHandleStore;
	modelHandleStore_ = modelHandleStore;
	gameFrame_ = gameFrame;
	gameScene_ = gameScene;

	// 中心軸の生成と初期化
	centerAxis_ = std::make_unique<CenterAxis>();
	centerAxis_->Initliaze(engine_, camera3d_);
	centerAxis_->SetGameTimer(gameFrame_);
	centerAxis_->SetGameScene(gameScene_);
}

/// <summary>
/// 更新処理
/// </summary>
void Stage::Update()
{
	// 敵スクリプトを更新する
	EnemyScriptUpdate();

	// ステージオブジェクトスクリプトを更新する
	StageObjectScriptUpdate();

	// 中心軸の更新処理
	centerAxis_->Update();

	// ゴールしたらクリア
	if (centerAxis_->IsGoal())
	{
		isClear_ = true;
		return;
	}


	// ステージオブジェクトの更新
	for (std::unique_ptr<BaseStageObject>& stageObject : stageObjects_)
	{
		stageObject->Update();
	}

	// 終了したステージオブジェクトをリストから除外する
	stageObjects_.remove_if([](std::unique_ptr<BaseStageObject>& stageObject)
		{
			if (stageObject->IsFinished())
			{
				stageObject.release();
				return true;
			}
			return false;
		}
	);
}

/// <summary>
/// 描画処理
/// </summary>
void Stage::Draw()
{
	// 中心軸の描画処理
	centerAxis_->Draw();

	// ステージオブジェクトの描画
	for (std::unique_ptr<BaseStageObject>& stageObject : stageObjects_)
	{
		stageObject->Draw();
	}
}

/// <summary>
/// ステージのスクリプトを読み込む
/// </summary>
/// <param name="filePath"></param>
void Stage::LoadEnemyScript(const char* filePath)
{
	// nullptrチェック
	assert(filePath);

	// ファイルを開く
	std::ifstream file;
	file.open(filePath);
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	enemyStream_ << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

/// <summary>
/// ステージオブジェクトのスクリプトを読み込む
/// </summary>
/// <param name="filePath"></param>
void Stage::LoadStageObjectScript(const char* filePath)
{
	// nullptrチェック
	assert(filePath);

	// ファイルを開く
	std::ifstream file;
	file.open(filePath);
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	stageObjectStream_ << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

/// <summary>
/// 敵スクリプトの更新処理
/// </summary>
void Stage::EnemyScriptUpdate()
{
	if (isWaitEnemyScript_)
	{
		waitEnemyScriptTimer_ -= (1.0f / 60.0f) * (*gameFrame_);

		if (waitEnemyScriptTimer_ <= 0.0f)
		{
			// 待機をやめる
			isWaitEnemyScript_ = false;
		}
		
		return;
	}


	// 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (std::getline(enemyStream_, line))
	{
		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		// 文字
		std::string word;

		// カンマ区切りで行の先頭文字列を取得する
		std::getline(line_stream, word , ',');

		// "//"から始まるのはコメントのため無視する
		if (word.find("//") == 0)
		{
			continue;
		}


		// 敵
		if (word.find("ENEMY") == 0)
		{
			// 敵の種類
			std::getline(line_stream, word, ',');
			std::string enemyType = word;

			// 敵の体力
			int32_t hp = 0;
			std::getline(line_stream, word, '(');
			if (word.find("Hp") == 0)
			{
				std::getline(line_stream, word, ')');
				hp = (int32_t)std::atoi(word.c_str());

				std::getline(line_stream, word, ',');
			}

			// 敵の位置
			Vector3 pos = Vector3(0.0f, 0.0f, 0.0f);
			std::getline(line_stream, word, '(');
			if (word.find("Position") == 0)
			{
				// x座標
				std::getline(line_stream, word, ',');
				pos.x = (float)std::atof(word.c_str());

				// y座標
				std::getline(line_stream, word, ',');
				pos.y = (float)std::atof(word.c_str());

				// z座標
				std::getline(line_stream, word, ')');
				pos.z = (float)std::atof(word.c_str());

				std::getline(line_stream, word, ',');
			}

			// 接近
			Vector3 approachingDirection = Vector3(0.0f, 0.0f, 0.0f);
			float approachingTime = 0.0f;
			float approachingSpeed = 0.0f;

			std::getline(line_stream, word, '(');
			if (word.find("Approaching") == 0)
			{
				std::getline(line_stream, word, '(');
				if (word.find("Direction") == 0)
				{
					// x座標
					std::getline(line_stream, word, ',');
					approachingDirection.x = (float)std::atof(word.c_str());

					// y座標
					std::getline(line_stream, word, ',');
					approachingDirection.y = (float)std::atof(word.c_str());

					// z座標
					std::getline(line_stream, word, ')');
					approachingDirection.z = (float)std::atof(word.c_str());

					// 正規化
					approachingDirection = Normalize(approachingDirection);

					std::getline(line_stream, word, ',');
				}

				std::getline(line_stream, word, '(');
				if (word.find("Speed") == 0)
				{
					std::getline(line_stream, word, ')');
					approachingSpeed = (float)std::atof(word.c_str());

					std::getline(line_stream, word, ',');
				}

				std::getline(line_stream, word, '(');
				if (word.find("Time") == 0)
				{
					std::getline(line_stream, word, ')');
					approachingTime = (float)std::atof(word.c_str());
				}

				std::getline(line_stream, word, ')');
				std::getline(line_stream, word, ',');
			}
			

			// 離脱
			Vector3 AwayDirection = Vector3(0.0f, 0.0f, 0.0f);
			float AwayTime = 0.0f;
			float awaySpeed = 0.0f;

			std::getline(line_stream, word, '(');
			if (word.find("Away") == 0)
			{
				std::getline(line_stream, word, '(');
				if (word.find("Direction") == 0)
				{
					// x座標
					std::getline(line_stream, word, ',');
					AwayDirection.x = (float)std::atof(word.c_str());

					// y座標
					std::getline(line_stream, word, ',');
					AwayDirection.y = (float)std::atof(word.c_str());

					// z座標
					std::getline(line_stream, word, ')');
					AwayDirection.z = (float)std::atof(word.c_str());

					// 正規化
					AwayDirection = Normalize(AwayDirection);

					std::getline(line_stream, word, ',');
				}

				std::getline(line_stream, word, '(');
				if (word.find("Speed") == 0)
				{
					std::getline(line_stream, word, ')');
					awaySpeed = (float)std::atof(word.c_str());

					std::getline(line_stream, word, ',');
				}

				std::getline(line_stream, word, '(');
				if (word.find("Time") == 0)
				{
					std::getline(line_stream, word, ')');
					AwayTime = (float)std::atof(word.c_str());
				}

				std::getline(line_stream, word, ')');
				std::getline(line_stream, word, ',');
			}


			std::unique_ptr<BaseEnemy> enemy = std::move(SummonEnemy(enemyType));

			// 初期化と設定
			enemy->Initialize(engine_, camera3d_,textureHandleStore_, modelHandleStore_, pos, hp);
			enemy->SetGameScene(gameScene_);
			enemy->SetParent(centerAxis_->GetWorldTransform());
			enemy->SetGameTimer(gameFrame_);
			enemy->SetTarget(target_);
			enemy->SetApproachingDirection(approachingDirection);
			enemy->SetApproachingTimer(approachingTime);
			enemy->SetApproachingSpeed(approachingSpeed);
			enemy->SetAwayDirection(AwayDirection);
			enemy->SetAwayTimer(AwayTime);
			enemy->SetAwaySpeed(awaySpeed);
			gameScene_->EnemySummon(std::move(enemy));
		}

		if (word.find("WAIT") == 0)
		{
			std::getline(line_stream, word, ',');

			// 待ち時間
			float waitTime = (float)std::atof(word.c_str());

			isWaitEnemyScript_ = true;
			waitEnemyScriptTimer_ = waitTime;

			break;
		}
	}
}

/// <summary>
/// ステージオブジェクトスクリプトの更新処理
/// </summary>
void Stage::StageObjectScriptUpdate()
{
	if (isWaitStageObjectScript_)
	{
		waitStageObjectScriptTimer_ -= (1.0f / 60.0f) * (*gameFrame_);

		if (waitStageObjectScriptTimer_ <= 0.0f)
		{
			// 待機をやめる
			isWaitStageObjectScript_ = false;
		}

		return;
	}


	// 1行分の文字列を入れる変数
	std::string line;

	// コマンド実行ループ
	while (std::getline(stageObjectStream_, line))
	{
		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		// 文字
		std::string word;

		// カンマ区切りで行の先頭文字列を取得する
		std::getline(line_stream, word, ',');

		// "//"から始まるのはコメントのため無視する
		if (word.find("//") == 0)
		{
			continue;
		}


		if (word.find("OBJECT") == 0)
		{
			// オブジェクトの種類
			std::getline(line_stream, word, ',');
			std::string stageObjectType = word;

			Vector3 position = Vector3(0.0f, 0.0f, 0.0f);
			Vector3 rotation = Vector3(0.0f, 0.0f, 0.0f);

			std::getline(line_stream, word, '(');
			if (word.find("Position") == 0)
			{
				// x座標
				std::getline(line_stream, word, ',');
				position.x = (float)std::atof(word.c_str());

				// y座標
				std::getline(line_stream, word, ',');
				position.y = (float)std::atof(word.c_str());

				// z座標
				std::getline(line_stream, word, ')');
				position.z = (float)std::atof(word.c_str());

				std::getline(line_stream, word, ',');
			}

			std::getline(line_stream, word, '(');
			if (word.find("Rotation") == 0)
			{
				// x座標
				std::getline(line_stream, word, ',');
				rotation.x = (float)std::atof(word.c_str());

				// y座標
				std::getline(line_stream, word, ',');
				rotation.y = (float)std::atof(word.c_str());

				// z座標
				std::getline(line_stream, word, ')');
				rotation.z = (float)std::atof(word.c_str());
			}


			std::unique_ptr<BaseStageObject> stageObject = std::move(SummonStageObject(stageObjectType));
			stageObject->Initialize(engine_, camera3d_, modelHandleStore_, gameFrame_, 0.0f, position, rotation);
			stageObjects_.push_back(std::move(stageObject));
		}
	}
}


/// <summary>
/// 敵を生成する
/// </summary>
std::unique_ptr<BaseEnemy> Stage::SummonEnemy(std::string& enemyType)
{
	if (strcmp(enemyType.c_str(), "Butterfly") == 0)
	{
		std::unique_ptr<EnemyButterfly> enemy = std::make_unique<EnemyButterfly>();
		return enemy;
	}
	else if (strcmp(enemyType.c_str(), "Bird") == 0)
	{
		std::unique_ptr<EnemyBird> enemy = std::make_unique<EnemyBird>();
		return enemy;
	}
	else if (strcmp(enemyType.c_str(), "Stingray") == 0)
	{
		std::unique_ptr<EnemyStingray> enemy = std::make_unique<EnemyStingray>();
		return enemy;
	}
	else if (strcmp(enemyType.c_str(), "Jet") == 0)
	{
		std::unique_ptr<EnemyJet> enemy = std::make_unique<EnemyJet>();
		return enemy;
	}
	else if (strcmp(enemyType.c_str(), "Fairy") == 0)
	{
		std::unique_ptr<EnemyFairy> enemy = std::make_unique<EnemyFairy>();
		return enemy;
	}
	else if (strcmp(enemyType.c_str(), "Devil") == 0)
	{
		std::unique_ptr<EnemyDevil> enemy = std::make_unique<EnemyDevil>();
		return enemy;
	}

	assert(false);
	return nullptr;
}

/// <summary>
/// ステージオブジェクトを生成する
/// </summary>
std::unique_ptr<BaseStageObject> Stage::SummonStageObject(std::string& stageObjectType)
{
	if (strcmp(stageObjectType.c_str(), "BuildingA") == 0)
	{
		std::unique_ptr<StageObjectBuildingA> stageObject = std::make_unique<StageObjectBuildingA>();
		return stageObject;
	}
	else if(strcmp(stageObjectType.c_str(), "BuildingB") == 0)
	{
		std::unique_ptr<StageObjectBuildingB> stageObject = std::make_unique<StageObjectBuildingB>();
		return stageObject;
	}
	else if (strcmp(stageObjectType.c_str(), "BuildingC") == 0)
	{
		std::unique_ptr<StageObjectBuildingC> stageObject = std::make_unique<StageObjectBuildingC>();
		return stageObject;
	}
	else if (strcmp(stageObjectType.c_str(), "BuildingD") == 0)
	{
		std::unique_ptr<StageObjectBuildingD> stageObject = std::make_unique<StageObjectBuildingD>();
		return stageObject;
	}
	else if (strcmp(stageObjectType.c_str(), "Goal") == 0)
	{
		std::unique_ptr<StageObjectGoal> stageObject = std::make_unique<StageObjectGoal>();
		return stageObject;
	}
	else if (strcmp(stageObjectType.c_str(), "Tree") == 0)
	{
		std::unique_ptr<StageObjectTree> stageObject = std::make_unique<StageObjectTree>();
		return stageObject;
	}
	else if (strcmp(stageObjectType.c_str(), "Rainbow") == 0)
	{
		std::unique_ptr<StageObjectRainbow> stageObject = std::make_unique<StageObjectRainbow>();
		return stageObject;
	}
	else if (strcmp(stageObjectType.c_str(), "House") == 0)
	{
		std::unique_ptr<StageObjectHouse> stageObject = std::make_unique<StageObjectHouse>();
		return stageObject;
	}

	assert(false);
	return nullptr;
}