#include "TextureHandleStore.h"

/// <summary>
/// 初期化
/// </summary>
/// <param name="engine"></param>
void TextureHandleStore::Initialize(const YokosukaEngine* engine)
{
	// nullptrチェック
	assert(engine);

	// 引数を受け取る
	engine_ = engine;


	// 画像 : 白
	textureHandles_[kWhite] = std::make_unique<WhiteTextureHandle>();
	textureHandles_[kWhite]->Initialize(engine_);

	// 画像 : レティクル
	textureHandles_[kReticle] = std::make_unique<ReticleTextureHandle>();
	textureHandles_[kReticle]->Initialize(engine_);

	// 画像 : タイトル
	textureHandles_[kTitle] = std::make_unique<TitleTextureHandle>();
	textureHandles_[kTitle]->Initialize(engine_);


	// ポーズ :
	textureHandles_[kPose] = std::make_unique<PoseTextureHandle>();
	textureHandles_[kPose]->Initialize(engine_);

	// ポーズ :
	textureHandles_[kPoseReturnGame] = std::make_unique<PoseReturnGameTextureHandle>();
	textureHandles_[kPoseReturnGame]->Initialize(engine_);

	// ポーズ :
	textureHandles_[kPoseEndGame] = std::make_unique<PoseEndGameTextureHandle>();
	textureHandles_[kPoseEndGame]->Initialize(engine_);


	// ステージ1
	textureHandles_[kStage1] = std::make_unique<Stage1TextureHandle>();
	textureHandles_[kStage1]->Initialize(engine_);

	// ステージ2
	textureHandles_[kStage2] = std::make_unique<Stage2TextureHandle>();
	textureHandles_[kStage2]->Initialize(engine_);

	// ステージ3
	textureHandles_[kStage3] = std::make_unique<Stage3TextureHandle>();
	textureHandles_[kStage3]->Initialize(engine_);


	// ゲームパッド操作 : ステージセレクト
	textureHandles_[kGamepadStageSelect] = std::make_unique<TextGamepadStageSelectTextureHandle>();
	textureHandles_[kGamepadStageSelect]->Initialize(engine_);

	// ゲームパッド操作 : ゲーム開始
	textureHandles_[kGamepadStart] = std::make_unique<TextGamepadStartTextureHandle>();
	textureHandles_[kGamepadStart]->Initialize(engine_);

	// ゲームパッド操作 : 移動
	textureHandles_[kGamepadMove] = std::make_unique<TextGamepadMoveTextureHandle>();
	textureHandles_[kGamepadMove]->Initialize(engine_);

	// ゲームパッド操作 : 発射
	textureHandles_[kGamepadShot] = std::make_unique<TextGamepadShotTextureHandle>();
	textureHandles_[kGamepadShot]->Initialize(engine_);

	// ゲームパッド操作 : 時止め
	textureHandles_[kGamepadTimeStop] = std::make_unique<TextGamepadTimeStopTextureHandle>();
	textureHandles_[kGamepadTimeStop]->Initialize(engine_);

	// ゲームパッド操作 : ポーズ
	textureHandles_[kGamepadPose] = std::make_unique<TextGamepadPoseTextureHandle>();
	textureHandles_[kGamepadPose]->Initialize(engine_);


	// キーボード操作 : ステージセレクト
	textureHandles_[kKeyboardStageSelect] = std::make_unique<TextKeyboardStageSelectTextureHandle>();
	textureHandles_[kKeyboardStageSelect]->Initialize(engine_);

	// キーボード操作 : ゲーム開始
	textureHandles_[kKeyboardStart] = std::make_unique<TextkeyboardStartTextureHandle>();
	textureHandles_[kKeyboardStart]->Initialize(engine_);

	// キーボード操作 : 移動
	textureHandles_[kKeyboardMove] = std::make_unique<TextKeyboardMoveTextureHandle>();
	textureHandles_[kKeyboardMove]->Initialize(engine_);

	// キーボード操作 : 発射
	textureHandles_[kKeyboardShot] = std::make_unique<TextKeyboardShotTextureHandle>();
	textureHandles_[kKeyboardShot]->Initialize(engine_);

	// キーボード操作 : 時止め
	textureHandles_[kKeyboardTimeStop] = std::make_unique<TextKeyboardTimeStopTextureHandle>();
	textureHandles_[kKeyboardTimeStop]->Initialize(engine_);

	// キーボード操作 : ポーズ
	textureHandles_[kKeyboardPose] = std::make_unique<TextKeyboardPoseTextureHandle>();
	textureHandles_[kKeyboardPose]->Initialize(engine_);


	// ステージ : サービス軌道
	textureHandles_[kStageServiceRailway] = std::make_unique<TextStageServiceRailwayTextureHandle>();
	textureHandles_[kStageServiceRailway]->Initialize(engine_);

	// ステージ : 鍛錬の橋頭堡
	textureHandles_[kStageEfforTown] = std::make_unique<TextStageEfforTownTextureHandle>();
	textureHandles_[kStageEfforTown]->Initialize(engine_);

	// ステージ : 手腕の稟性
	textureHandles_[kStageTechnoRoot] = std::make_unique<TextStageTechnoRootTextureHandle>();
	textureHandles_[kStageTechnoRoot]->Initialize(engine_);

	// ステージ : 架空の手のひら
	textureHandles_[kStageImaginaryPlam] = std::make_unique<TextStageImaginaryPlamTextureHandle>();
	textureHandles_[kStageImaginaryPlam]->Initialize(engine_);


	// ゲームクリア
	textureHandles_[kGameClear] = std::make_unique<GameClearTextureHandle>();
	textureHandles_[kGameClear]->Initialize(engine_);

	// ゲームオーバー
	textureHandles_[kGameOver] = std::make_unique<GameOverTextureHandle>();
	textureHandles_[kGameOver]->Initialize(engine_);
}