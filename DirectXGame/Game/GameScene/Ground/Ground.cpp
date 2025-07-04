#include "Ground.h"

/// <summary>
/// Initialize
/// </summary>
/// <param name="engine"></param>
/// <param name="camera3d"></param>
void Ground::Initialize(const YokosukaEngine* engine, const Camera3D* camera3d)
{
	// Check for nullptr
	assert(engine);
	assert(camera3d);

	// Receive arguments
	engine_ = engine;
	camera3d_ = camera3d;


	// Create and initialize worldTransform
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();

	// Create and initialize uvTransform
	uvTransform_ = std::make_unique<UvTransform>();
	uvTransform_->Initialize();

	// Load the modelData
	modelHandle_ = engine_->LoadModelData("./Resources/Models/Ground", "Ground.obj");
}

/// <summary>
/// Update
/// </summary>
void Ground::Update()
{
	// Update worldTransform
	worldTransform_->UpdateWorldMatrix();

	// Update uvTransform
	uvTransform_->UpdateWorldMatrix();
}

/// <summary>
/// Draw
/// </summary>
void Ground::Draw()
{
	// Draw the model
	engine_->DrawModel(worldTransform_.get(), uvTransform_.get(), camera3d_, modelHandle_, Vector4(1.0f, 1.0f, 1.0f, 1.0f), true);
}