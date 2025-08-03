#pragma once
#include "../../YokosukaEngine/Include/YokosukaEngine.h"
#include "../ModelHandleStore/ModelHandleStore.h"
#include "../TextureHandleStore/TextureHandleStore.h"

class BaseScene
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	virtual void Initialize(const YokosukaEngine* engine , const ModelHandleStore* modelHandleStore);

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// 終了フラグのGetter
	/// </summary>
	bool IsFinished() { return isFinished_; }


protected:

	// モデルハンドル格納場所
	const ModelHandleStore* modelHandleStore_ = nullptr;

	// テクスチャハンドル格納場所
	const TextureHandleStore* textureHandleStore_ = nullptr;


	// シーン
	std::unique_ptr<Scene> scene_ = nullptr;

	// メインカメラ
	MainCamera* mainCamera_ = nullptr;

	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// 3Dカメラ
	const Camera3D* camera3d_ = nullptr;


	// 終了フラグ
	bool isFinished_ = false;
};

