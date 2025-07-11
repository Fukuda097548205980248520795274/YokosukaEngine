#pragma once
#include "../../Math/Vector4/Vector4.h"

// GPUに送る輝度
struct GPUforLuminance
{
    // 輝度の閾値
    float threshold;
};

// GPUに送るラスタースクロール
struct GPUforRasterScroll
{
    // 振幅
    float amplitude;

    // 周波数
    float frequency;

    // 位相オフセット
    float phaseOffset;

    // スクロール速度
    float scrollSpeed;

    // 時間
    float time;
};