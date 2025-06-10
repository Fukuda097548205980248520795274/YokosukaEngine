#pragma once
#include "../BaseLine3d.h"

class Line3dBlendScreen : public BaseLine3d
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Line3dBlendScreen();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="log">ログ出力</param>
	/// <param name="dxc">シェーダコンパイラ</param>
	void Initialize(OutputLog* log, DirectXShaderCompiler* dxc, Microsoft::WRL::ComPtr<ID3D12Device> device) override;
};

