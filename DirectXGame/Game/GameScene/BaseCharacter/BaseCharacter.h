#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"
#include "../PlaneProjectionShadow/PlaneProjectionShadow.h"

class BaseCharacter
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine"></param>
	/// <param name="camera3d"></param>
	virtual void Initialize(const YokosukaEngine* engine, const Camera3D* camera3d);

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// ワールドトランスフォームのGetter
	/// </summary>
	/// <returns></returns>
	WorldTransform* GetWorldTransform() { return worldTransform_.get(); }


protected:

	// エンジン
	const YokosukaEngine* engine_ = nullptr;

	// カメラ
	const Camera3D* camera3d_ = nullptr;


	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;

	// UVトランスフォーム
	std::unique_ptr<UvTransform> uvTransform_ = nullptr;


	// モデル
	struct ModelStruct
	{
		// ワールドトランスフォーム
		std::unique_ptr<WorldTransform> worldTransform = nullptr;

		// UVトランスフォーム
		std::unique_ptr<UvTransform> uvTransform = nullptr;

		// モデルハンドル
		uint32_t modelHandle = 0;

		// 色
		Vector4 color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

		// 影
		std::unique_ptr<PlaneProjectionShadow> planeProjectionShadow = nullptr;
	};
};

