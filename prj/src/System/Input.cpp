#include "WinMain.h"
#include "Input.h"

namespace
{
	// --------------------------------------------
	// キーボード入力
	// --------------------------------------------
	char keyBuffer[256];	//!< 今のフレームで入力されたキーを入れる
	char keyBefore[256];	//!< １つ前のフレームで入力されたキーを入れる

	// --------------------------------------------
	// マウス入力
	// --------------------------------------------
	//! マウスボタン操作に使用する
	enum MOUSEBUTTON
	{
		Right,		//!< 右ボタン用
		Left,		//!< 左ボタン用
		ButtonMax,	//!< 最大数
	};

	//! マウスの移動量計測用
	enum MOUSEMEMTYPE
	{
		Now,		//!< 現フレーム
		Before,		//!< 前フレーム
		TypeMax,	//!< 最大数
	};

	int mousePosX[MOUSEMEMTYPE::TypeMax]       = { 0, 0 };				//!< 前フレームと現フレームのマウスの座標
	int mousePosY[MOUSEMEMTYPE::TypeMax]	   = { 0, 0 };				//!< 前フレームと現フレームのマウスの座標
	bool mouseInputMemL[MOUSEMEMTYPE::TypeMax] = { false, false };		//!< 左ボタンのクリック記録
	bool mouseInputMemR[MOUSEMEMTYPE::TypeMax] = { false, false };		//!< 右ボタンのクリック記録
}


bool PushHitKey(int _keyNum)
{
	// 現在の入力情報を入れる
	int buffer = keyBuffer[_keyNum];
	// １つ前のフレームの入力情報を入れる
	int before = keyBefore[_keyNum];

	// 現在、入力があって１つ前のフレームのでは入力がないならtrueを返す
	if (buffer == 1 && before == 0)	return true;
	return false;
}

bool ReleaseKey(int _keyNum)
{
	// 現在の入力情報を入れる
	int buffer = keyBuffer[_keyNum];
	// １つ前のフレームの入力情報を入れる
	int before = keyBefore[_keyNum];

	// 1フレーム前は入力があって今のフレームでは入力がない時に
	if (buffer == 0 && before == 1) return true;
	return false;
}

bool CheckMouseInput(const int _buttonNum)
{
	if (GetMouseInput() & _buttonNum)
	{
		return true;
	}

	return false;
}

bool PushMouseInput(int _buttonNum)
{
	// 左ボタンの判定
	if (_buttonNum & MOUSE_INPUT_LEFT)
	{
		// ボタンの単押しフラグ
		const bool ISPUSH = mouseInputMemL[MOUSEMEMTYPE::Now] && !mouseInputMemL[MOUSEMEMTYPE::Before];
		if (ISPUSH)
		{
			return true;
		}
	}

	// 右ボタンの判定
	if (_buttonNum & MOUSE_INPUT_RIGHT)
	{
		// ボタンの単押しフラグ
		const bool ISPUSH = mouseInputMemR[MOUSEMEMTYPE::Now] && !mouseInputMemR[MOUSEMEMTYPE::Before];
		if (ISPUSH)
		{
			return true;
		}
	}

	return false;
}

int GetMouseX()
{
	VectorInt2 mousePos;
	GetMousePoint(&mousePos.x, &mousePos.y);
	return mousePos.x;
}

int GetMouseY()
{
	VectorInt2 mousePos;
	GetMousePoint(&mousePos.x, &mousePos.y);
	return mousePos.y;
}

int GetMouseMoveX()
{
	return mousePosX[MOUSEMEMTYPE::Now] - mousePosX[MOUSEMEMTYPE::Before];
}

int GetMouseMoveY()
{
	return mousePosY[MOUSEMEMTYPE::Now] - mousePosY[MOUSEMEMTYPE::Before];
}

void InputUpdate()
{
	// 全てのキー入力情報を取得する
	GetHitKeyStateAll(keyBuffer);

	// マウスクリック記録をリセットする
	mouseInputMemL[MOUSEMEMTYPE::Now] = false;
	mouseInputMemR[MOUSEMEMTYPE::Now] = false;

	// クリックがあれば記録を更新する
	if (CheckMouseInput(MOUSE_INPUT_LEFT))
	{
		mouseInputMemL[MOUSEMEMTYPE::Now] = true;
	}

	if (CheckMouseInput(MOUSE_INPUT_RIGHT))
	{
		mouseInputMemR[MOUSEMEMTYPE::Now] = true;
	}

	// 今のマウス座標を取得
	mousePosX[MOUSEMEMTYPE::Now] = GetMouseX();
	mousePosY[MOUSEMEMTYPE::Now] = GetMouseY();

}

void MemorizeInputInfo()
{
	// 今のキー入力情報を１フレーム前の入力情報にする
	memcpy(keyBefore, keyBuffer, sizeof(char) * 256);

	// 今のマウス座標を１つ前の入力情報にする
	mousePosX[MOUSEMEMTYPE::Before] = mousePosX[MOUSEMEMTYPE::Now];
	mousePosY[MOUSEMEMTYPE::Before] = mousePosY[MOUSEMEMTYPE::Now];

	// マウスのクリック情報を移す
	mouseInputMemL[MOUSEMEMTYPE::Before] = mouseInputMemL[MOUSEMEMTYPE::Now];
	mouseInputMemR[MOUSEMEMTYPE::Before] = mouseInputMemR[MOUSEMEMTYPE::Now];
}
