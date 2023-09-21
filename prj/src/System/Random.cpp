#include "WinMain.h"
#include "Random.h"

int GetRandomI(const int _maxValue)
{
	return rand() % (_maxValue + 1);
}

int GetRandomI(const int _minValue, const int _maxValue)
{
	int diffValue = _maxValue - _minValue;
	int randomValue = GetRandomI(diffValue);
	return _minValue + randomValue;
}

// 0.0f ~ 1.0f�܂ł̗�����Ԃ�
float GetRandomF()
{
	float randomValue = (float)rand();
	return randomValue / RAND_MAX;
}

// 0.0f ~ _maxValue�܂ł̗�����Ԃ�
float GetRandomF(const float _maxValue)
{
	float randomValue = GetRandomF();
	return randomValue * _maxValue;
}

// _minValue ~ _maxValue�܂ł̗�����Ԃ�
float GetRandomF(const float _minValue, const float _maxValue)
{
	float diffValue = _maxValue - _minValue;
	float randomValue = GetRandomF(diffValue);
	return _minValue + randomValue;
}
