#pragma once
//---------------------------------------------------------------------------
//! @file   Editor.h
//! @brief  �G�f�B�^�[�N���X
//---------------------------------------------------------------------------
#include "../../Game/BossStatus.h"
#include "../FileLoader/CsvLoading.h"

#include "../Random.h"
#include "Vector3.h"
#include "Effect.h"
#include "Animation.h"
#include "../../Game/Class/CharacterBase.h"
#include "../../Game/Class/Player.h"
#include "../../Game/Class/Boss.h"

class Editor
{
	BossStatus	 mBossStatus;		//! CSV�t�@�C������X�e�[�^�X���󂯎��ϐ�
	BossCollInfo mBossCollInfo;		//!< CSV�t�@�C�����瓖���蔻������󂯎��
	Boss*	   mBossInfo;			//!< �{�X�N���X�̏�������
	Player*	   mPlayerInfo;			//!< �v���C���[�N���X�̏�������

	bool	   mIsTestMode;		//!< �e�X�g���[�h�t���O

	VectorInt2  mcursorPos;		//!< ���̃J�[�\���ʒu������p
	VectorInt2	mMousePos;		//!< �}�E�X�̍��W

	//! @note	Camera�N���X�ɂ͂��̃N���X�ɕK�v�Ȃ��������������ߎg��Ȃ��ł���
	Vector3		mCameraPos;			//!< �J�����̍��W
	Vector3		mCameraLookPos;		//!< �J�����̒����_

	MATRIX mMatCameraWorld;		//!< ���[���h�s��
	MATRIX mMatView;			//!< �r���[�s��
	VECTOR mWorldUp;			//!< ���E�̕����p

	// ----------------------------------------------------------
	// Private�֐�
	// ----------------------------------------------------------
	//! @brief ImGui�̃��C���̑���
	void GUIDraw();

	//! @brief �{�X�̃R���W�����`��
	void CollDraw();

	//! @brief MATRIX�^�ϐ��̏������֐�
	//! @param _initMat		������������MATRIX�^�ϐ�
	void SetMatIdentity(MATRIX& _initMat);

	//! @brief �J�����̑���֐�
	void CameraUpdate();

	//! @brief �e�X�g���[�h���̍X�V
	void TestModeUpdate();

	//! @brief �e�X�g���[�h�I�����A�e�L�����N�^�[�����Z�b�g����
	void ResetCharacter();

	//! @brief �e�X�g���[�h�{�^���̃��x����ύX����
	//! @param _changeStr		�ύX��̕����������ϐ�
	void ChangeButtonLabel(std::string& _changeStr);

	//! @brief �ʒu���킩��₷�����邽�߂̃��C���[�`��
	void DrawWire();

public:
	Editor();		//!< �R���X�g���N�^
	~Editor();		//!< �f�X�g���N�^

	//! @brief �e�X�g���[�h�t���O���Z�b�g����
	void SetTestMode();	

	//! @brief �e�X�g���[�h�t���O���`�F�b�N����
	//! @return		true : �e�X�g���[�h		false : �G�f�B�b�g���[�h
	bool IsTestMode() const; 

	//! @brief �{�X�N���X�̃I�u�W�F�N�g���|�C���^�Ŏ擾����
	//! @param _bossObj		�{�X�N���X�̃I�u�W�F�N�g
	void SetBossInfo(Boss* _bossObj);

	void SetPlayerInfo(Player* _playerObj);

	//! @brief  ������
	void Init();

	//! @brief �X�V
	void Update();

	//! @brief �`��
	void Draw();
};

//! @brief float�^����string�^�ɕϊ�����
//! @param nums_	�ϊ�������float�^�̒l
//! @return			�ϊ�����string�^�̒l
std::string ChengeNumFloatToString(float nums_);
