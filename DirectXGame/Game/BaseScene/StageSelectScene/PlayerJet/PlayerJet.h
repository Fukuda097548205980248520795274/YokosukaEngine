#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"
#include "../../../ModelHandleStore/ModelHandleStore.h"

class PlayerJet
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera3d"></param>
	/// <param name="modelHandleStore"></param>
	void Initialize(const YokosukaEngine* engine, const Camera3D* camera3d , const ModelHandleStore* modelHandleStore);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// ワールドトランスフォームのGetter
	/// </summary>
	/// <returns></returns>
	WorldTransform* GetWorldTransform() { return worldTransform_.get(); }

	/// <summary>
	/// ワールドトランスフォームの親を設定する
	/// </summary>
	/// <param name="worldTransform"></param>
	void SetParent(WorldTransform* worldTransform) { worldTransform_->SetParent(worldTransform); }


private:

	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// カメラ
	const Camera3D* camera3d_ = nullptr;

	// モデルハンドル格納場所
	const ModelHandleStore* modelHandleStore_ = nullptr;


	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;

	// モデルハンドル
	uint32_t modelHandle_ = 0;
};

