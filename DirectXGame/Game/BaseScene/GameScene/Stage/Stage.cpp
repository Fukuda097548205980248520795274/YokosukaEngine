#include "Stage.h"
#include "../GameScene.h"
#include "../BaseCharacter/BaseEnemy/EnemyButterfly/EnemyButterfly.h"
#include "../BaseCharacter/BaseEnemy/EnemyBird/EnemyBird.h"
#include "../BaseCharacter/Player/Player.h"

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

	
	
	std::unique_ptr<StageObjectBuildingA> stageObject0 = std::make_unique<StageObjectBuildingA>();
	stageObject0->Initialize(engine_, camera3d_, modelHandleStore_, gameFrame_, 1.0f, Vector3(50.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f));
	stageObject0->InfiniteTimer();
	stageObjects_.push_back(std::move(stageObject0));

	std::unique_ptr<StageObjectBuildingB> stageObject1 = std::make_unique<StageObjectBuildingB>();
	stageObject1->Initialize(engine_, camera3d_, modelHandleStore_, gameFrame_, 1.0f, Vector3(-50.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f));
	stageObject1->InfiniteTimer();
	stageObjects_.push_back(std::move(stageObject1));

	std::unique_ptr<StageObjectBuildingC> stageObject2 = std::make_unique<StageObjectBuildingC>();
	stageObject2->Initialize(engine_, camera3d_, modelHandleStore_, gameFrame_, 1.0f, Vector3(50.0f, 0.0f, 100.0f), Vector3(0.0f, 0.0f, 0.0f));
	stageObject2->InfiniteTimer();
	stageObjects_.push_back(std::move(stageObject2));

	std::unique_ptr<StageObjectBuildingD> stageObject3 = std::make_unique<StageObjectBuildingD>();
	stageObject3->Initialize(engine_, camera3d_, modelHandleStore_, gameFrame_, 1.0f, Vector3(-50.0f, 0.0f, 100.0f), Vector3(0.0f, 0.0f, 0.0f));
	stageObject3->InfiniteTimer();
	stageObjects_.push_back(std::move(stageObject3));



	std::unique_ptr<StageObjectBuildingD> stageObject4 = std::make_unique<StageObjectBuildingD>();
	stageObject4->Initialize(engine_, camera3d_, modelHandleStore_, gameFrame_, 1.0f, Vector3(50.0f, 0.0f, 200.0f), Vector3(0.0f, 0.0f, 0.0f));
	stageObject4->InfiniteTimer();
	stageObjects_.push_back(std::move(stageObject4));

	std::unique_ptr<StageObjectBuildingB> stageObject5 = std::make_unique<StageObjectBuildingB>();
	stageObject5->Initialize(engine_, camera3d_, modelHandleStore_, gameFrame_, 1.0f, Vector3(-50.0f, 0.0f, 200.0f), Vector3(0.0f, 0.0f, 0.0f));
	stageObject5->InfiniteTimer();
	stageObjects_.push_back(std::move(stageObject5));

	std::unique_ptr<StageObjectBuildingC> stageObject6 = std::make_unique<StageObjectBuildingC>();
	stageObject6->Initialize(engine_, camera3d_, modelHandleStore_, gameFrame_, 1.0f, Vector3(50.0f, 0.0f, 300.0f), Vector3(0.0f, 0.0f, 0.0f));
	stageObject6->InfiniteTimer();
	stageObjects_.push_back(std::move(stageObject6));

	std::unique_ptr<StageObjectBuildingA> stageObject7 = std::make_unique<StageObjectBuildingA>();
	stageObject7->Initialize(engine_, camera3d_, modelHandleStore_, gameFrame_, 1.0f, Vector3(-50.0f, 0.0f, 300.0f), Vector3(0.0f, 0.0f, 0.0f));
	stageObject7->InfiniteTimer();
	stageObjects_.push_back(std::move(stageObject7));


	std::unique_ptr<StageObjectBuildingD> stageObject8 = std::make_unique<StageObjectBuildingD>();
	stageObject8->Initialize(engine_, camera3d_, modelHandleStore_, gameFrame_, 1.0f, Vector3(50.0f, 0.0f, 400.0f), Vector3(0.0f, 0.0f, 0.0f));
	stageObject8->InfiniteTimer();
	stageObjects_.push_back(std::move(stageObject8));

	std::unique_ptr<StageObjectBuildingB> stageObject9 = std::make_unique<StageObjectBuildingB>();
	stageObject9->Initialize(engine_, camera3d_, modelHandleStore_, gameFrame_, 1.0f, Vector3(-50.0f, 0.0f, 400.0f), Vector3(0.0f, 0.0f, 0.0f));
	stageObject9->InfiniteTimer();
	stageObjects_.push_back(std::move(stageObject9));

	std::unique_ptr<StageObjectBuildingC> stageObject10 = std::make_unique<StageObjectBuildingC>();
	stageObject10->Initialize(engine_, camera3d_, modelHandleStore_, gameFrame_, 1.0f, Vector3(80.0f, 0.0f, 500.0f), Vector3(0.0f, 0.0f, 0.0f));
	stageObject10->InfiniteTimer();
	stageObjects_.push_back(std::move(stageObject10));

	std::unique_ptr<StageObjectBuildingA> stageObject11 = std::make_unique<StageObjectBuildingA>();
	stageObject11->Initialize(engine_, camera3d_, modelHandleStore_, gameFrame_, 1.0f, Vector3(-20.0f, 0.0f, 500.0f), Vector3(0.0f, 0.0f, 0.0f));
	stageObject11->InfiniteTimer();
	stageObjects_.push_back(std::move(stageObject11));


	std::unique_ptr<StageObjectBuildingD> stageObject12 = std::make_unique<StageObjectBuildingD>();
	stageObject12->Initialize(engine_, camera3d_, modelHandleStore_, gameFrame_, 1.0f, Vector3(200.0f, 0.0f, 600.0f), Vector3(0.0f, 1.0f, 0.0f));
	stageObject12->InfiniteTimer();
	stageObjects_.push_back(std::move(stageObject12));

	std::unique_ptr<StageObjectBuildingB> stageObject13 = std::make_unique<StageObjectBuildingB>();
	stageObject13->Initialize(engine_, camera3d_, modelHandleStore_, gameFrame_, 1.0f, Vector3(10.0f, 0.0f, 600.0f), Vector3(0.0f, 0.0f, 0.0f));
	stageObject13->InfiniteTimer();
	stageObjects_.push_back(std::move(stageObject13));

	std::unique_ptr<StageObjectBuildingC> stageObject14 = std::make_unique<StageObjectBuildingC>();
	stageObject14->Initialize(engine_, camera3d_, modelHandleStore_, gameFrame_, 1.0f, Vector3(160.0f, 0.0f, 700.0f), Vector3(0.0f, 0.0f, 0.0f));
	stageObject14->InfiniteTimer();
	stageObjects_.push_back(std::move(stageObject14));

	std::unique_ptr<StageObjectBuildingA> stageObject15 = std::make_unique<StageObjectBuildingA>();
	stageObject15->Initialize(engine_, camera3d_, modelHandleStore_, gameFrame_, 1.0f, Vector3(100.0f, 0.0f, 650.0f), Vector3(0.0f, 0.0f, 0.0f));
	stageObject15->InfiniteTimer();
	stageObjects_.push_back(std::move(stageObject15));


	std::unique_ptr<StageObjectBuildingB> stageObject16 = std::make_unique<StageObjectBuildingB>();
	stageObject16->Initialize(engine_, camera3d_, modelHandleStore_, gameFrame_, 1.0f, Vector3(200.0f, 0.0f, 800.0f), Vector3(0.0f, 1.0f, 0.0f));
	stageObject16->InfiniteTimer();
	stageObjects_.push_back(std::move(stageObject16));

	std::unique_ptr<StageObjectBuildingD> stageObject17 = std::make_unique<StageObjectBuildingD>();
	stageObject17->Initialize(engine_, camera3d_, modelHandleStore_, gameFrame_, 1.0f, Vector3(10.0f, 0.0f, 800.0f), Vector3(0.0f, 0.0f, 0.0f));
	stageObject17->InfiniteTimer();
	stageObjects_.push_back(std::move(stageObject17));

	std::unique_ptr<StageObjectBuildingA> stageObject18 = std::make_unique<StageObjectBuildingA>();
	stageObject18->Initialize(engine_, camera3d_, modelHandleStore_, gameFrame_, 1.0f, Vector3(250.0f, 0.0f, 900.0f), Vector3(0.0f, 0.0f, 0.0f));
	stageObject18->InfiniteTimer();
	stageObjects_.push_back(std::move(stageObject18));

	std::unique_ptr<StageObjectBuildingC> stageObject19 = std::make_unique<StageObjectBuildingC>();
	stageObject19->Initialize(engine_, camera3d_, modelHandleStore_, gameFrame_, 1.0f, Vector3(200.0f, 0.0f, 900.0f), Vector3(0.0f, 0.0f, 0.0f));
	stageObject19->InfiniteTimer();
	stageObjects_.push_back(std::move(stageObject19));


	std::unique_ptr<StageObjectBuildingA> stageObject20 = std::make_unique<StageObjectBuildingA>();
	stageObject20->Initialize(engine_, camera3d_, modelHandleStore_, gameFrame_, 1.0f, Vector3(300.0f, 0.0f, 800.0f), Vector3(0.0f, 1.0f, 0.0f));
	stageObject20->InfiniteTimer();
	stageObjects_.push_back(std::move(stageObject20));

	std::unique_ptr<StageObjectBuildingC> stageObject21 = std::make_unique<StageObjectBuildingC>();
	stageObject21->Initialize(engine_, camera3d_, modelHandleStore_, gameFrame_, 1.0f, Vector3(410.0f, 0.0f, 800.0f), Vector3(0.0f, 0.0f, 0.0f));
	stageObject21->InfiniteTimer();
	stageObjects_.push_back(std::move(stageObject21));

	std::unique_ptr<StageObjectBuildingB> stageObject22 = std::make_unique<StageObjectBuildingB>();
	stageObject22->Initialize(engine_, camera3d_, modelHandleStore_, gameFrame_, 1.0f, Vector3(350.0f, 0.0f, 900.0f), Vector3(0.0f, 0.0f, 0.0f));
	stageObject22->InfiniteTimer();
	stageObjects_.push_back(std::move(stageObject22));

	std::unique_ptr<StageObjectBuildingD> stageObject23 = std::make_unique<StageObjectBuildingD>();
	stageObject23->Initialize(engine_, camera3d_, modelHandleStore_, gameFrame_, 1.0f, Vector3(300.0f, 0.0f, 900.0f), Vector3(0.0f, 0.0f, 0.0f));
	stageObject23->InfiniteTimer();
	stageObjects_.push_back(std::move(stageObject23));



	std::unique_ptr<EnemyStingray> enemy = std::make_unique<EnemyStingray>();
	enemy->Initialize(engine_, camera3d_, modelHandleStore_, Vector3(0.0f, 0.0f, 15.0f), 50);
	enemy->SetGameScene(gameScene_);
	enemy->SetParent(centerAxis_->GetWorldTransform());
	enemy->SetGameTimer(gameFrame_);
	enemy->SetTarget(target_);
	gameScene_->EnemySummon(std::move(enemy));
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