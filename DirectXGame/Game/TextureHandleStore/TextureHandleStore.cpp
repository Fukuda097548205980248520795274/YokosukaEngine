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


	// スタートのテクスチャ
	textureHandles_[kStart] = std::make_unique<TextStartTextureHandle>();
	textureHandles_[kStart]->Initialize(engine_);

	// Aボタンのテクスチャ
	textureHandles_[kAButton] = std::make_unique<TextAButtonTextureHandle>();
	textureHandles_[kAButton]->Initialize(engine_);

	// Lスティックのテクスチャ
	textureHandles_[kLStick] = std::make_unique<TextLStickTextureHandle>();
	textureHandles_[kLStick]->Initialize(engine_);

	// Lスティック押し込みのテクスチャ
	textureHandles_[kLStickThumb] = std::make_unique<TextLStrickThumbTextureHandle>();
	textureHandles_[kLStickThumb]->Initialize(engine_);

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
}