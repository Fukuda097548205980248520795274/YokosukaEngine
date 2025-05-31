#include "DebugCamera.h"
#include "../../YokosukaEngine.h"

/// <summary>
/// 初期化
/// </summary>
void DebugCamera::Initialize(YokosukaEngine* engine)
{
	// nullptrチェック
	assert(engine);

	// 引数を受け取る
	engine_ = engine;

	// ローカル拡縮
	scale_ = { 1.0f , 1.0f , 1.0f };

	// X,Y,Z軸回りのローカル回転角
	rotation_ = { 0.0f , 0.0f , 0.0f };

	// ローカル座標
	translation_ = { 0.0f , 0.0f , -50.0f };
}

/// <summary>
/// 更新処理
/// </summary>
void DebugCamera::Update()
{
	ImGui::Begin("DebugCamera");
	ImGui::DragFloat3("translation", &translation_.x, 0.1f);
	ImGui::DragFloat3("rotation", &rotation_.x, 0.01f);
	ImGui::DragFloat3("scale", &scale_.x, 0.01f);
	ImGui::End();
}