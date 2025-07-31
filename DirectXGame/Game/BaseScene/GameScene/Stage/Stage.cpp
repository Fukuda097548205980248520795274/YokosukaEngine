#include "Stage.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
void Stage::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d, const ModelHandleStore* modelHandleStore, const float* gameFrame)
{
	// nullptrチェック
	assert(engine);
	assert(camera3d);
	assert(modelHandleStore);
	assert(gameFrame);

	// 引数を受け取る
	engine_ = engine;
	camera3d_ = camera3d;
	modelHandleStore_ = modelHandleStore;
	gameFrame_ = gameFrame;

	
	saturnPlanet_ = std::make_unique<StageObjectSaturnPlanet>();
	saturnPlanet_->Initialize(engine_, camera3d_, modelHandleStore_, gameFrame_, 1.0f, Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f));
	saturnPlanet_->InfiniteTimer();
}

/// <summary>
/// 更新処理
/// </summary>
void Stage::Update()
{
	saturnPlanet_->Update();
}

/// <summary>
/// 描画処理
/// </summary>
void Stage::Draw()
{
	saturnPlanet_->Draw();
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