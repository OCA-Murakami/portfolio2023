#pragma once
//---------------------------------------------------------------------------
//! @file   Input.h
//! @brief  �L�[��}�E�X�̏���
//---------------------------------------------------------------------------

// --------------------------------------------
// �L�[�{�[�h����
// --------------------------------------------
//! @brief �L�[�������ꂽ�u�Ԃ��擾����
//! @param _keyNum	���͂��ꂽ�L�[�̔ԍ�
//! @return		true : �O�t���[���œ��͂��Ȃ��A���t���[���œ��͂�������		false : ����ȊO
bool PushHitKey(int _keyNum);

//! @brief �L�[�������ꂽ�u�Ԃ��擾����
//! @param _keyNum	���͂��ꂽ�L�[�̔ԍ�
//! @return		true : �O�t���[���œ��͂�����A���t���[���œ��͂��Ȃ�����	false : ����ȊO
bool ReleaseKey(int _keyNum);

// --------------------------------------------
// �}�E�X����
// --------------------------------------------
//! @brief �}�E�X�̓��͂��`�F�b�N�i�������j
//! @param _buttonNum	���͂��ꂽ�{�^���̔ԍ�
//! @return		true : ������Ă���		false : ������Ă��Ȃ�
bool CheckMouseInput(const int _buttonNum);

//! @brief �}�E�X�̓��͂��`�F�b�N�i�P�����j
//! @param _buttonNum	���͂��ꂽ�{�^���̔ԍ�
//! @return		true : �{�^����������Ă����t���[�������P�̎�		false : ����ȊO�̐��l�̎�
bool PushMouseInput(int _buttonNum);

//! @brief �}�E�X��X���W���擾����
//! @return �}�E�X��X���W
int GetMouseX();

//! @brief �}�E�X��Y���W���擾����
//! @return	�}�E�X��Y���W
int GetMouseY();

//! @brief �}�E�X��X���W�̈ړ��ʂ��擾����
//! @return �O�t���[���ƌ��t���[����X���W�̍�
int GetMouseMoveX();

//! @brief �}�E�X��Y���W�̈ړ��ʂ��擾����
//! @return �O�t���[���ƌ��t���[����Y���W�̍�
int GetMouseMoveY();

// --------------------------------------------
// �����֐�
// --------------------------------------------
//! @brief �X�V
void InputUpdate();

//! @brief ���t���[���ł̓��͏����R�s�[����
void MemorizeInputInfo();

int getframe();