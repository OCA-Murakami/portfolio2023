#include "WinMain.h"
#include "Input.h"

namespace
{
	// --------------------------------------------
	// �L�[�{�[�h����
	// --------------------------------------------
	char keyBuffer[256];	//!< ���̃t���[���œ��͂��ꂽ�L�[������
	char keyBefore[256];	//!< �P�O�̃t���[���œ��͂��ꂽ�L�[������

	// --------------------------------------------
	// �}�E�X����
	// --------------------------------------------
	//! �}�E�X�{�^������Ɏg�p����
	enum MOUSEBUTTON
	{
		Right,		//!< �E�{�^���p
		Left,		//!< ���{�^���p
		ButtonMax,	//!< �ő吔
	};

	//! �}�E�X�̈ړ��ʌv���p
	enum MOUSEMEMTYPE
	{
		Now,		//!< ���t���[��
		Before,		//!< �O�t���[��
		TypeMax,	//!< �ő吔
	};

	int mousePosX[MOUSEMEMTYPE::TypeMax]       = { 0, 0 };				//!< �O�t���[���ƌ��t���[���̃}�E�X�̍��W
	int mousePosY[MOUSEMEMTYPE::TypeMax]	   = { 0, 0 };				//!< �O�t���[���ƌ��t���[���̃}�E�X�̍��W
	bool mouseInputMemL[MOUSEMEMTYPE::TypeMax] = { false, false };		//!< ���{�^���̃N���b�N�L�^
	bool mouseInputMemR[MOUSEMEMTYPE::TypeMax] = { false, false };		//!< �E�{�^���̃N���b�N�L�^
}


bool PushHitKey(int _keyNum)
{
	// ���݂̓��͏�������
	int buffer = keyBuffer[_keyNum];
	// �P�O�̃t���[���̓��͏�������
	int before = keyBefore[_keyNum];

	// ���݁A���͂������ĂP�O�̃t���[���̂ł͓��͂��Ȃ��Ȃ�true��Ԃ�
	if (buffer == 1 && before == 0)	return true;
	return false;
}

bool ReleaseKey(int _keyNum)
{
	// ���݂̓��͏�������
	int buffer = keyBuffer[_keyNum];
	// �P�O�̃t���[���̓��͏�������
	int before = keyBefore[_keyNum];

	// 1�t���[���O�͓��͂������č��̃t���[���ł͓��͂��Ȃ�����
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
	// ���{�^���̔���
	if (_buttonNum & MOUSE_INPUT_LEFT)
	{
		// �{�^���̒P�����t���O
		const bool ISPUSH = mouseInputMemL[MOUSEMEMTYPE::Now] && !mouseInputMemL[MOUSEMEMTYPE::Before];
		if (ISPUSH)
		{
			return true;
		}
	}

	// �E�{�^���̔���
	if (_buttonNum & MOUSE_INPUT_RIGHT)
	{
		// �{�^���̒P�����t���O
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
	// �S�ẴL�[���͏����擾����
	GetHitKeyStateAll(keyBuffer);

	// �}�E�X�N���b�N�L�^�����Z�b�g����
	mouseInputMemL[MOUSEMEMTYPE::Now] = false;
	mouseInputMemR[MOUSEMEMTYPE::Now] = false;

	// �N���b�N������΋L�^���X�V����
	if (CheckMouseInput(MOUSE_INPUT_LEFT))
	{
		mouseInputMemL[MOUSEMEMTYPE::Now] = true;
	}

	if (CheckMouseInput(MOUSE_INPUT_RIGHT))
	{
		mouseInputMemR[MOUSEMEMTYPE::Now] = true;
	}

	// ���̃}�E�X���W���擾
	mousePosX[MOUSEMEMTYPE::Now] = GetMouseX();
	mousePosY[MOUSEMEMTYPE::Now] = GetMouseY();

}

void MemorizeInputInfo()
{
	// ���̃L�[���͏����P�t���[���O�̓��͏��ɂ���
	memcpy(keyBefore, keyBuffer, sizeof(char) * 256);

	// ���̃}�E�X���W���P�O�̓��͏��ɂ���
	mousePosX[MOUSEMEMTYPE::Before] = mousePosX[MOUSEMEMTYPE::Now];
	mousePosY[MOUSEMEMTYPE::Before] = mousePosY[MOUSEMEMTYPE::Now];

	// �}�E�X�̃N���b�N�����ڂ�
	mouseInputMemL[MOUSEMEMTYPE::Before] = mouseInputMemL[MOUSEMEMTYPE::Now];
	mouseInputMemR[MOUSEMEMTYPE::Before] = mouseInputMemR[MOUSEMEMTYPE::Now];
}
