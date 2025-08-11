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
void Stage::LoadStageScript(const char* filePath)
{
	// nullptrチェック
	assert(filePath);

	// ファイルを開く
	std::ifstream file;
	file.open(filePath);
	assert(file.is_open());

	// ファイルの内容を文字列ストリームにコピー
	stageStream_ << file.rdbuf();

	// ファイルを閉じる
	file.close();
}


/// <summary>
/// ステージスクリプトの更新処理
/// </summary>
void Stage::StageScriptUpdate()
{

}


/// <summary>
/// 敵を生成する
/// </summary>
void Stage::SummonEnemy()
{
	std::unique_ptr<EnemyDevil> enemy = std::make_unique<EnemyDevil>();
	enemy->Initialize(engine_, camera3d_, modelHandleStore_, Vector3(0.0f, 0.0f, 15.0f), 50);
	enemy->SetGameScene(gameScene_);
	enemy->SetParent(centerAxis_->GetWorldTransform());
	enemy->SetGameTimer(gameFrame_);
	enemy->SetTarget(target_);
	gameScene_->EnemySummon(std::move(enemy));
}