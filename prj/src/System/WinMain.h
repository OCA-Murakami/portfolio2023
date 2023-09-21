#pragma once

#include <string>
#include "DxLib.h"
#include "EffekseerForDXLib.h"
#include "Input.h"

#define PI			3.141592653589793		//!< 円周率

#define WINDOW_W	1280			//!< ウィンドウの横幅
#define WINDOW_H	720				//!< ウィンドウの縦幅
#define WINDOW_HALF_W  WINDOW_W / 2		//!< ウィンドウの横幅の中央値
#define WINDOW_HALF_H  WINDOW_H / 2		//!< ウィンドウの立幅の中央値

//---------------------------------------------------------------
// 構造体
//---------------------------------------------------------------
//! @brief 平面の座標用(整数型)
struct VectorInt2
{
	int x, y;
};

struct Vector2
{
	float x;
	float y;
};

struct BoxInt
{
	int xTop, xUnder;
	int yTop, yUnder;
};

namespace
{
	int shadowHandle = -1;
}

//---------------------------------------------------------------
// 関数
//---------------------------------------------------------------
float TO_RADIAN(float _degree);
float TO_DEGREE(float _radian);

int handleHandle();