#pragma once
//------------------------------------------------
// �C���N���[�h
//------------------------------------------------
#include "../../System/Class/Effect.h"
#include "../../System/Class/Animation.h"
#include "../../System/Class/Model.h"

//------------------------------------------------
//!@class CharacterBase�N���X
//!@brief �L�����N�^�[�̋��ʌ����܂Ƃ߂����N���X 
//------------------------------------------------
class CharacterBase
{
protected:
	Model	mModelData;								//!< ���f��
	int		mState;									//!< �s�����
	Vector3 mPos;									//!< ���W
	Vector3 mRot;									//!< ����
	Vector3 mSize;									//!< �T�C�Y
	const CharacterBase* mTargetInfo = nullptr;		//!< �v���C���[�܂��͓G�̏�������

	//---------------------------------------
	// �X�e�[�^�X�Ɋւ��ϐ�
	//---------------------------------------
	float	mHp;		//!< �c��HP
	float   mSpeed;		//!< �ړ����x
	float mAtkPower;	//!< �U����
	float mCollSize;	//!< �{�f�B�̓����蔻��̃T�C�Y

	//-----------------------------
	// �`��֘A�Ɏg�p����ϐ�
	//-----------------------------
	int		mHpBar;					//!< HP�o�[�����p
	COLOR_F mBaseMatDif;			//!< ���f���������Ƃ��Ƃ̃f�B�t���[�Y�J���[�̒l

	//-----------------------------
	// �U������Ƃ��Ɏg���ϐ�
	//-----------------------------
	float mAtkJudgeStart;	//!< �U�����肪��������^�C�~���O
	float mAtkJudgeEnd;		//!< �U�����肪�I������^�C�~���O
	bool  mIsAtkSend;		//!< �_���[�W���P�x����������悤�ɂ���t���O
	bool  mIsAttack;		//!< �U���t���O

	//-----------------------------
	// �A�j���[�V�����Ɏg���ϐ�
	//-----------------------------
	Animation mAnimationPlayer;		//!< �A�j���[�V�����N���X�̃I�u�W�F�N�g
	float	  mAnimPlaySpeed;		//!< �A�j���[�V�����̍Đ����x
	bool	  mIsAnimLoop;			//!< �A�j���[�V�������Ƀ��[�v�ݒ��؂�ւ��邽�߂̃t���O

	//-----------------------------
	// �G�t�F�N�g�Ɏg���ϐ�
	//-----------------------------
	Effect mEffectPlayer;		//!< �G�t�F�N�g�̍Đ��Ɏg�p����I�u�W�F�N�g			

public:
	CharacterBase();	//!< �R���X�g���N�^
	~CharacterBase();	//!< �f�X�g���N�^

	//---------------------------------------------------------------
	// �l�̎擾�A�Z�b�g�֐�
	//---------------------------------------------------------------
	//! @fn SetPos(Vector3)
	//! @brief Vector3�^�̒l�ňꊇ�Z�b�g
	void SetPos(const Vector3 _pos);

	//! @fn SetPos(float)
	//! @brief x,y,z���ꂼ��ɒl���Z�b�g
	void SetPos(const float _x, const float _y, const float _z);	

	//! @fn GetPos
	//! @brief ���W���擾����(Vector3�^)
	//! @return ���̍��W(Vector3�^)
	Vector3 GetPos() const;

	//! @brief Vector3��VECTOR�^�ɕϊ����Ď擾����
	//! @return VECTOR�^�ɕϊ����� mPos
	VECTOR  GetVECTORPos() const;	

	//! @brief �������擾����
	//! @return mRot.y �̒l
	float	GetRot() const;				
	int		GetModel();					

	//! @brief �^�[�Q�b�g����̏����|�C���^�Ŏ󂯎��
	//! @param _target	�^�[�Q�b�g����̃I�u�W�F�N�g���Z�b�g����i�^�[�Q�b�g����F�@Player => Boss		Boss, Camera => Player�j
	void SetTargetInfo(const CharacterBase* _target);		

	//---------------------------------------------------------------
	// �f�B�t���[�Y�J���[����̊֐�
	//---------------------------------------------------------------
	//! @brief ���f���̃f�B�t���[�Y�J���[���_���[�W���̒l�ɕύX����
	void SetMatDifDamage();			

	//! @brief ���f���̌��X�̃f�B�t���[�Y�J���[���Z�b�g����
	void SetMatDifBase();			

	//---------------------------------------------------------------
	// �U���֘A�̊֐�
	//---------------------------------------------------------------
	//! @brief �U���t���O�̃����[�X
	void  ReleaseAttack();		

	//! @brief �U������̏d���h�~�t���O���Z�b�g
	void  SetAtkSend();

	//! @brief �U���͂̃Z�b�g
	//! @param _power	�U���̎�ނɂ���ĕω�����U����
	void  SetPower(const float _power);

	//! @brief �U�������`�F�b�N����
	//! @return	ture : �U����		false : ��U����
	virtual bool  IsAttack() const;

	//! @brief �U������̏d���h�~�t���O�̃`�F�b�N
	//! @return true : ���ɍU�����肪��������			false : �U�����肪������
	bool  IsAtkSend()  const;

	//! @brief �A�j���[�V�����̃t���[��������U������𔭐�������t���O���`�F�b�N
	//! @return true : �U������𔭐�������		false : �U������𔭐������Ȃ�
	bool  IsAtkJudge() const;

	//! @brief �U���͂��擾����
	//! @return ���݂̍U���́i�U���̎�ނɂ���Ēl�͕ω�����j
	float GetPower() const;

	//! @brief �U�����󂯂鎞�̔���Ɏg�p����R���W�����̃T�C�Y���擾����
	//! @return �R���W�����̃T�C�Y
	virtual float GetCollSize() const;	


	//! @brief �X�e�[�W�ɕ`�悳��Ă���X�t�B�A���ɂ��邩�`�F�b�N����
	//! @param _stageLength		�X�t�B�A�̔��a
	//! @return true : �X�t�B�A��		false : �X�t�B�A�O
	bool IsInTheStage(float _stageLength) const;

	//! @brief 
	//! @param _nextState 
	void ChangeState(const int _nextState);

	virtual void Init(std::string _filePath);		//!< ������
	virtual void Update(float _delta) = 0;	//!< �X�V
	virtual void Draw();				//!< �`��
	virtual void PostDraw();			//!< �őO�ʂɕ`�悵�������̂��܂Ƃ߂�
	virtual void Exit();				//!< �I��
};

