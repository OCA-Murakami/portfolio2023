#pragma once
//---------------------------------------------------------------------------
//!	@file	3DGame.h
//! @brief	�Q�[���S�̂̑���
//---------------------------------------------------------------------------

bool CheckModeStringRange(Vector2 _modePos, VectorInt2 _mousePos);

// -------------------------------------------------
// �A���t�@�l��K�p�����`��֐�
// -------------------------------------------------
//! @brief �A���t�@�l��K�p�����{�b�N�X�`��
//! @param _pos ���W
//! @param _alphaValue	�A���t�@�l 
//! @param _colorValue  �J���[�l
void DrawFillBoxA(BoxInt _pos, int _alphaValue, unsigned int _colorValue);

//! @brief �A���t�@�l��K�p����������`��
//! @param _pos ���W
//! @param _string �`�悵����������
//! @param _alphaValue	�A���t�@�l 
//! @param _colorValue	�J���[�l
void DrawStringA(VectorInt2 _pos, const TCHAR* _string, int _alphaValue, unsigned int _colorValue);

// -------------------------------------------------
// �^�C�g����ʂ̃e���b�v�֐�
// -------------------------------------------------
void TitleTelopUpdate();		//!< �^�C�g����ʂ̃e���b�v�X�V�֐�
void TitleTelopDraw();			//!< �^�C�g����ʂ̃e���b�v�`��֐�

// -------------------------------------------------
// ��ʈړ����̃t�F�[�h�����֐�
// -------------------------------------------------
void FadeUpdate();			//!< �t�F�[�h�C���A�t�F�[�h�A�E�g�̍X�V����
// --------------------------------
// ��ʈړ����̃t�F�[�h�C�������֐�
// --------------------------------
void FadeInUpdate();		//!< �t�F�[�h�C���̂ݍX�V
void SetFadeIn();			//!< �t�F�[�h�C���J�n
void ReleaseFadeIn();		//!< �t�F�[�h�C������
bool IsFadeIn();			//!< �t�F�[�h�C���`�F�b�N
bool IsfinishFadeIn();		//!< �t�F�[�h�C���I���`�F�b�N
// --------------------------------
// ��ʈړ����̃t�F�[�h�A�E�g�����֐�
// --------------------------------
void FadeOutUpdate();		//!< �t�F�[�h�A�E�g�̂ݍX�V
void SetFadeOut();			//!< �t�F�[�h�A�E�g�J�n
void ReleaseFadeOut();		//!< �t�F�[�h�A�E�g����
bool IsFadeOut();			//!< �t�F�[�h�A�E�g�`�F�b�N
bool IsFinishFadeOut();		//!< �t�F�[�h�A�E�g�I���`�F�b�N

// -------------------------------------------------
// �e�L�����N�^�[�̍U���������܂Ƃ߂��֐�
// -------------------------------------------------
void PlayerAttack();
void BossAttack();

// -------------------------------------------------
// �Q�[������֐�
// -------------------------------------------------
void GameInit();		//!< ������
void GameUpdate();		//!< �X�V
void GameDraw();		//!< �`��
void GamePostDraw();	//!< �őO�ʂɕ`�悵�������̂�����
void GameExit();		//!< �I��