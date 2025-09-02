#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"

#include "../../../ModelHandleStore/ModelHandleStore.h"
#include "../../../TextureHandleStore/TextureHandleStore.h"

class BaseCharacter
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera3d"></param>
	/// <param name="position"></param>
	virtual void Initialize(const YokosukaEngine* engine, const ModelHandleStore* modelHandleStore , const TextureHandleStore* textureHandleStore,
		const Camera3D* camera3d , const Vector3& position);

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// ワールド座標のGetter
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition() const;



protected:


	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// 3Dカメラ
	const Camera3D* camera3d_ = nullptr;

	// モデルハンドル格納場所
	const ModelHandleStore* modelHandleStore_ = nullptr;

	// テクスチャハンドル格納場所
	const TextureHandleStore* textureHandleStore_ = nullptr;


	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;
};

