#pragma once
#include "../../YokosukaEngine/Include/YokosukaEngine.h"
#include "Player/Player.h"
#include "Skydome/Skydome.h"

#include "StageObject/StageObject.h"
#include "StageObject/Tutorial/Ground/TutorialGroundEmpty/TutorialGroundEmpty.h"

#include "BaseBoss/BaseBoss.h"
#include "BaseBoss/BossBenkei/BossBenkei.h"

#include "BaseEnemy/BaseEnemy.h"
#include "BaseEnemy/EnemyButterfly/EnemyButterfly.h"


class GameScene : public Scene
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="engine">エンジン</param>
	void Initialize(const YokosukaEngine* engine) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw()override;


private:


	
};
