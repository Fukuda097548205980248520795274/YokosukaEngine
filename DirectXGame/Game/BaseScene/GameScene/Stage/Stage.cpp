#include "Stage.h"
#include "../GameScene.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
void Stage::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, 
	const ModelHandleStore* modelHandleStore, const float* gameFrame, GameScene* gameScene)
{
	// nullptrチェック
	assert(engine);
	assert(camera3d);
	assert(modelHandleStore);
	assert(gameFrame);
	assert(gameScene);

	// 引数を受け取る
	engine_ = engine;
	camera3d_ = camera3d;
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
		waitEnemyScriptTimer_ -= 1.0f / 60.0f;

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
				if (word.find("Time") == 0)
				{
					std::getline(line_stream, word, ')');
					approachingTime = (float)std::atoi(word.c_str());
				}

				std::getline(line_stream, word, ')');
				std::getline(line_stream, word, ',');
			}
			

			// 離脱
			Vector3 AwayDirection = Vector3(0.0f, 0.0f, 0.0f);
			float AwayTime = 0.0f;

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
				if (word.find("Time") == 0)
				{
					std::getline(line_stream, word, ')');
					AwayTime = (float)std::atoi(word.c_str());
				}

				std::getline(line_stream, word, ')');
				std::getline(line_stream, word, ',');
			}


			SummonEnemy(enemyType, hp, pos , approachingDirection , approachingTime , AwayDirection , AwayTime);
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
		waitStageObjectScriptTimer_ -= 1.0f / 60.0f;

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


		if (word.find("STAGE_OBJECT") == 0)
		{
			// オブジェクトの種類
			std::getline(line_stream, word, ',');
			std::string stageObjectType = word;

			// x座標
			std::getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			std::getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			std::getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());


			// x座標
			std::getline(line_stream, word, ',');
			float rx = (float)std::atof(word.c_str());

			// y座標
			std::getline(line_stream, word, ',');
			float ry = (float)std::atof(word.c_str());

			// z座標
			std::getline(line_stream, word, ',');
			float rz = (float)std::atof(word.c_str());
		}

		if (word.find("WAIT") == 0)
		{
			std::getline(line_stream, word, ',');

			// 待ち時間
			float waitTime = (float)std::atof(word.c_str());

			isWaitStageObjectScript_ = true;
			waitStageObjectScriptTimer_ = waitTime;

			break;
		}
	}
}


/// <summary>
/// 敵を生成する
/// </summary>
void Stage::SummonEnemy(std::string& enemyType, int32_t hp, const Vector3& position ,
	const Vector3 approachingDirection , float approachingTime , const Vector3 awayDirection, float awayTime)
{
	if (strcmp(enemyType.c_str(), "Butterfly") == 0)
	{
		std::unique_ptr<EnemyButterfly> enemy = std::make_unique<EnemyButterfly>();
		enemy->Initialize(engine_, camera3d_, modelHandleStore_, position, hp);
		enemy->SetGameScene(gameScene_);
		enemy->SetParent(centerAxis_->GetWorldTransform());
		enemy->SetGameTimer(gameFrame_);
		enemy->SetTarget(target_);
		gameScene_->EnemySummon(std::move(enemy));
	}
	else if (strcmp(enemyType.c_str(), "Bird") == 0)
	{
		std::unique_ptr<EnemyBird> enemy = std::make_unique<EnemyBird>();
		enemy->Initialize(engine_, camera3d_, modelHandleStore_, position, hp);
		enemy->SetGameScene(gameScene_);
		enemy->SetParent(centerAxis_->GetWorldTransform());
		enemy->SetGameTimer(gameFrame_);
		enemy->SetTarget(target_);
		gameScene_->EnemySummon(std::move(enemy));
	}
	else if (strcmp(enemyType.c_str(), "Stingray") == 0)
	{
		std::unique_ptr<EnemyStingray> enemy = std::make_unique<EnemyStingray>();
		enemy->Initialize(engine_, camera3d_, modelHandleStore_, position, hp);
		enemy->SetGameScene(gameScene_);
		enemy->SetParent(centerAxis_->GetWorldTransform());
		enemy->SetGameTimer(gameFrame_);
		enemy->SetTarget(target_);
		gameScene_->EnemySummon(std::move(enemy));
	}
	else if (strcmp(enemyType.c_str(), "Jet") == 0)
	{
		std::unique_ptr<EnemyJet> enemy = std::make_unique<EnemyJet>();
		enemy->Initialize(engine_, camera3d_, modelHandleStore_, position, hp);
		enemy->SetGameScene(gameScene_);
		enemy->SetParent(centerAxis_->GetWorldTransform());
		enemy->SetGameTimer(gameFrame_);
		enemy->SetTarget(target_);
		gameScene_->EnemySummon(std::move(enemy));
	}
	else if (strcmp(enemyType.c_str(), "Fairy") == 0)
	{
		std::unique_ptr<EnemyFairy> enemy = std::make_unique<EnemyFairy>();
		enemy->Initialize(engine_, camera3d_, modelHandleStore_, position, hp);
		enemy->SetGameScene(gameScene_);
		enemy->SetParent(centerAxis_->GetWorldTransform());
		enemy->SetGameTimer(gameFrame_);
		enemy->SetTarget(target_);
		gameScene_->EnemySummon(std::move(enemy));
	}
	else if (strcmp(enemyType.c_str(), "Devil") == 0)
	{
		std::unique_ptr<EnemyDevil> enemy = std::make_unique<EnemyDevil>();
		enemy->Initialize(engine_, camera3d_, modelHandleStore_, position, hp);
		enemy->SetGameScene(gameScene_);
		enemy->SetParent(centerAxis_->GetWorldTransform());
		enemy->SetGameTimer(gameFrame_);
		enemy->SetTarget(target_);
		gameScene_->EnemySummon(std::move(enemy));
	}
}

/// <summary>
/// ステージオブジェクトを生成する
/// </summary>
void Stage::SummonStageObject(std::string& enemyType, const Vector3& position, const Vector3& rotation)
{

}