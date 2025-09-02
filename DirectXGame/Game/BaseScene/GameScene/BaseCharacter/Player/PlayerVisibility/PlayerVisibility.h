#pragma once
#include "../../../YokosukaEngine/Include/YokosukaEngine.h"

class PlayerVisibility
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="worldTransform"></param>
	void Initialize(WorldTransform* worldTransform);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 後ろ側の平面のGetter
	/// </summary>
	/// <returns></returns>
	Plane GetBackPlane()const { return planeBack_; }

	/// <summary>
	/// 上側の平面のGetter
	/// </summary>
	/// <returns></returns>
	Plane GetTopPlane()const { return planeTop_; }

	/// <summary>
	/// 下側の平面のGetter
	/// </summary>
	/// <returns></returns>
	Plane GetBottomPlane()const { return planeBottom_; }


private:


	// ワールドトランスフォーム
	WorldTransform* worldTransform_ = nullptr;


	// 平面
	Plane planeBack_;



	// 上側
	std::unique_ptr<WorldTransform> topWorldTransform_ = nullptr;

	// 下側
	std::unique_ptr<WorldTransform> bottomWorldTransform_ = nullptr;

	Plane planeTop_;
	Plane planeBottom_;
};

