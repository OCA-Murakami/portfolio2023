#pragma once
//------------------------------------------------
// 乱数生成に使う関数
//------------------------------------------------
int GetRandomI(const int _maxValue);						//!< _maxValue までの乱数を返す
int GetRandomI(const int _minValue, const int _maxValue);	//!< _minValue ~ _maxValue までの乱数を返す

float GetRandomF();													//!< 0.0f ~ 1.0f までの乱数を返す
float GetRandomF(const float _maxValue);							//!< 0.0f ~ _maxValue までの乱数を返す
float GetRandomF(const float _minValue, const float _maxValue);		//!< _minValue ~ _maxValue までの乱数を返す