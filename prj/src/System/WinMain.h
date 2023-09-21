#pragma once

#include <string>
#include "DxLib.h"
#include "EffekseerForDXLib.h"
#include "Input.h"

#define PI			3.141592653589793		//!< �~����

#define WINDOW_W	1280			//!< �E�B���h�E�̉���
#define WINDOW_H	720				//!< �E�B���h�E�̏c��
#define WINDOW_HALF_W  WINDOW_W / 2		//!< �E�B���h�E�̉����̒����l
#define WINDOW_HALF_H  WINDOW_H / 2		//!< �E�B���h�E�̗����̒����l

//---------------------------------------------------------------
// �\����
//---------------------------------------------------------------
//! @brief ���ʂ̍��W�p(�����^)
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
// �֐�
//---------------------------------------------------------------
float TO_RADIAN(float _degree);
float TO_DEGREE(float _radian);

int handleHandle();