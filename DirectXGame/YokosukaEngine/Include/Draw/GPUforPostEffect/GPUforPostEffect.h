#pragma once
#include "../../Math/Vector4/Vector4.h"
#include "../../Math/Vector2/Vector2.h"

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

// GPUに送る走査線
struct GPUforScanline
{
    // 画面サイズ
    Vector2 screenSize;

    // 走査線の太さ
    float thickness;

    // 走査線の濃さ (0.0f ~ 1.0f)
    float intensity;

    // 走査線の間隔
    float spacing;

    // 時間
    float time;

    // 移動速度
    float speed;
};