#pragma once
//------------------------------------------------
// �C���N���[�h
//------------------------------------------------
#include "../../System/Class/Effect.h"
#include "../../System/Class/Animation.h"
#include "../../System/FileLoader/CsvLoading.h"
#include "../BossStatus.h"

//------------------------------------------------
//!@class Boss�N���X
//!@brief �{�X�L�����N�^�[���� 
//------------------------------------------------
class Boss : public CharacterBase
{
	//------------------------------------------------
	// �񋓑�
	//------------------------------------------------
	//! �A�j���[�V�����̗v�f���ɃA�N�Z�X���邽�߂̗񋓑�
	enum BOSSSTATE
	{
		Entry,				//!< �o��
		Roaring,			//!< ���K
		Idle,				//!< �ҋ@
		Walk,				//!< ���s
		Run,				//!< ���s
		LightAttackIdle,	//!< ��U���O�̗���
		LightAttack,		//!< ��U��
		HeavyAttackIdle,	//!< ���U���O�̗���
		HeavyAttack,		//!< ���U��
		ComboAttack,		//!< �R���{�U��
		TurnAttack,			//!< 360�x�U��
		Jump,				//!< �W�����v
		Guard,				//!< �h��
		BlowOff,			//!< �������
		GetUp,				//!< �N���オ��
		React,				//!< ����
		Down,				//!< �_�E��
		DownGetUp,			//!< �_�E������̋N���オ��
		Death,				//!< ���S
		StateMax,			//!< �񋓑̂̍ő�l�i���ꂪ�Ō�ɗ��邱�Ɓj
	};

	struct CSVFilePath
	{
		std::string statusPath = "data/csv/BossStatus.csv";
		std::string collPath   = "data/csv/BossColl.csv";
	};

	//-----------------------------
	// �萔
	//-----------------------------
	const float mNEXTACTIONTIME = 10.0f;	//!< �ҋ@��Ԃł̍ő�҂�����
	const float mBLOCKTIMERMAX  = 60.0f;	//!< �h��̐��̍ő厞��
	const float mVIEWANGLE		= 35.0f;	//!< ����p�̔��l
	const float mHPMAX			= 100.0f;
	const int	mREACTMAX		= 30;		//!< ���ݒl�̍ő�
	const int	mDOWNMAX		= 2;		//!< �_�E���l�̍ő�
	const int   mDOWNTIMEMAX	= 3 * 60;	//!< �_�E���̍ő�t���[����
	const int   mOUTOFSIGHTMAX  = 2 * 60;	//!< �v���C���[������O�ɂ��鎞�Ԃ̍ő�v������

	//------------------------------------
	// CSV�t�@�C�����琔�l���󂯎��ϐ��@�@
	//------------------------------------
	CSVFilePath csvPaths;		//!< �eCSV�t�@�C���̃p�X
	BossStatus	 mStatus;		//!< �X�e�[�^�X
	BossCollInfo mCollInfo;		//!< �R���W�����ݒ�

	//-----------------------------
	// �e�s���֑J�ڂ���Ƃ���臒l
	//-----------------------------
	int mReactValue;		//!< ���ݒl
	int mDownValue;			//!< �_�E���l
	int mstoredDamage;		//!< �~�σ_���[�W�l
	int mThresholdGurad;	//!< �K�[�h����臒l

	//-----------------------------
	// �s�����Ɏg���t���O
	//-----------------------------
	bool mIsGroundMovie;			//!< ���[�r�[�����o���ł̒��n�t���O
	bool mIsFinishMovie;		//!< ���[�r�[�����o�̏I���t���O
	bool mIsIdle;				//!< �ҋ@
	bool mIsDown;				//!< �_�E��
	bool mIsReact;				//!< ����
	bool mIsLightAttack;		//!< ��U��
	bool mIsHeavyAttack;		//!< ���U��
	bool mIsComboAttack;		//!< �R���{�U��
	bool mIsJump;				//!< �W�����v
	bool mIsGuard;				//!< �K�[�h
	bool mIsBlowOff;			//!< �������
	bool mIsDeath;				//!< ���S
	bool mIsRightDirection;		//!< �v���C���[�������̉E���ɂ��邩�����ɂ��邩

	//-----------------------------
	// �s�����Ɏg���ϐ�
	//-----------------------------
	Vector3 mMoveEndPos;			//!< �ړ����̏I�_�i�v���C���[�̍��W�j
	int	mDownTimer;				//!< �_�E����Ԃ̃^�C�}�[
	int     mOutOfSightTimer;		//!< �v���C���[������O�ɂ��鎞�Ԃ��v������

	//-----------------------------
	// �A�j���[�V�����Ɏg���ϐ�
	//-----------------------------
	int   mAnim[StateMax];	//!< �A�j���[�V�����̃��f���f�[�^	

	//-----------------------------
	// �G�t�F�N�g�Ɏg���ϐ�
	//-----------------------------
	int mEffectWaveHandle;		//!< �W�����v�U���̏Ռ��g�G�t�F�N�g�̃n���h��������
	int mEffectDangerHandle;	//!< �傫���U�����J��o���O�̗\���Ɏg��
	int mEffectRoaringHandle;	//!< ���K���Ɏg�p����G�t�F�N�g
	Effect mEffectPlayer;


	int mDamageFrame = 0;
	Vector3 mFixOffset;

private:
	bool IsLimitActions() const;		//!< �s�����Ղ�ꂽ���������Ɏg�p����s���`�F�b�N�֐�
	void ReleaseAllFlag();				//!< �S�Ẵt���O����ă����[�X

public:
	Boss();		//!< �R���X�g���N�^
	~Boss();	//!< �f�X�g���N�^

	void Move(float _moveSpeed);	//!< ���W�ړ����܂Ƃ߂��֐�
	bool IsSeePlayer();				//!< �v���C���[��������ɂ��邩�`�F�b�N
	bool IsRightDirection();		//!< �v���C���[���E���ɂ��邩�`�F�b�N
	void MoveDirection(const bool _isRightDir);	//!< �ړ����̐��񏈗�
	void ChoiseAttackType();		//!< �U���p�^�[�������肷��

	// -----------------------------------------
	// �e�ϐ��̎擾�A�Z�b�g�֐�
	// -----------------------------------------
	// �Q�̊֐��͂Ȃ��Ă������C������
	void  GetPlayerPos(Vector3 _playerPos);		//!< �v���C���[�̍��W���擾����
	void  SetPlayerDir(Vector3 _playerPos);		//!< �v���C���[���ǂ̌����ɂ��邩���v�Z����
	void  SetThresholdGurad();					//!< �K�[�h����臒l��HP����Ƀ����_���Ō��肷��
	float GetCollSize() const override;

	// -----------------------------------------
	// �e�t���O�ϐ��̃Z�b�g�֐�
	// -----------------------------------------
	void SetIsGroundMovie();	//!< ���[�r�[�����o���ł̒��n�t���O
	void SetFinishMovie();		//!< ���[�r�[�����o
	void SetAttack();			//!< �U���s��
	void SetLightAttack();		//!< ��U��
	void SetHeavyAttack();		//!< ���U��
	void SetComboAttack();		//!< �R���{�U��
	void SetGuard();			//!< �h��
	void SetGuardToAttack();	//!< �h�䂩��U���ցi�J�E���^�[�j
	void SetJump();				//!< �W�����v
	void SetBlowOff();			//!< �������
	void SetReact();			//!< ����
	void SetDown();				//!< �_�E��
	void SetDeath();			//!< ���S

	// -----------------------------------------
	// �e�t���O�ϐ��̃����[�X�֐�
	// -----------------------------------------
	void ReleaseJump();				//!< �W�����v
	void ReleaseLightAttack();		//!< ��U��
	void ReleaseHeavyAttack();		//!< ���U��
	void ReleaseComboAttack();		//!< �R���{�U��
	void ReleaseReact();			//!< ����
	void ReleaseBlowOff();			//!< �������
	void ReleaseDown();				//!< �_�E��
	void ReleaseAllAttack();		//!< �U���t���O��S�ă`�F�b�N���ă����[�X����

	// -----------------------------------------
	// �e�t���O�ϐ��̃`�F�b�N�֐�
	// -----------------------------------------
	bool IsGroundMovie()   const;				//!< ���[�r�[�����o���ł̒��n�t���O
	bool IsFinishMoive()   const;				//!< ���[�r�[�����o
	bool IsAttack()		   const override;		//!< �U��
	bool IsLightAttack()   const;				//!< ��U��
	bool IsHeavyAttack()   const;				//!< ���U��
	bool IsComboAttack()   const;				//!< �R���{�U��
	bool IsReact()		   const;				//!< ����
	bool IsDeath()		   const;				//!< ���S
	bool IsIdle()		   const;				//!< �ҋ@
	bool IsWalk()		   const;				//!< ���s
	bool IsGuard()		   const;				//!< �h��
	bool IsJump()		   const;				//!< �W�����v
	bool IsBlowOff()	   const;				//!< �������
	bool IsDown()		   const;				//!< �_�E��
	bool IsInvincible()    const;				//!< ���G���
	bool CheckAllAttack()  const;				//!< �S�Ă̍U���t���O
	bool IsSeriousDamage() const;				//!< ���ݖ��̓_�E�����

	//------------------------------------
	// �����蔻��֘A�֐�
	//------------------------------------
	float GetClawSize()		  const;		//!< �ܕ����̓����蔻��̃T�C�Y
	float GetFistSize()		  const;		//!< �������̓����蔻��̃T�C�Y
	Vector3 GetFistTopPos()   const;		//!< �������̐�[���W
	Vector3 GetFistUnderPos() const;		//!< �������̍��{���W
	Vector3 GetClawTopPos()   const;		//!< �ܕ����̐�[���W
	Vector3 GetClawUnderPos() const;		//!< �ܕ����̍��{���W

	//------------------------------------
	// �_���[�W�֘A�̊֐�
	//------------------------------------
	void SetDamage(const float _damage);		//!< HP����
	void CountReactValue(const float _damage);	//!< ���ݒl�̍X�V
	void SetDamage()
	{
		mDamageFrame = 60;
	}
	void DamageUpdate()
	{
		--mDamageFrame;
		if (mDamageFrame < 0)
		{
			mDamageFrame = 0;
		}
	}

	// -----------------------------------------
	// �X�e�[�g�A�A�j���[�V�����̐؂�ւ��֐�
	// -----------------------------------------
	void SetStateWalk();				//!< ���s��Ԃɂ���
	void SetStateRun();					//!< ���s��Ԃɂ���
	void SetStateLightAttack();			//!< ��U����Ԃɂ���
	void SetStateHeavyAttack();			//!< ���U����Ԃɂ���
	void SetStateComboAttack();			//!< �R���{�U����Ԃɂ���
	void SetStateGuard();				//!< �h���Ԃɂ���
	void SetStateJump();				//!< �W�����v��Ԃɂ���
	void SetStateBlowOff();				//!< ������я�Ԃɂ���
	void SetStateReact();				//!< ���ݏ�Ԃɂ���
	void SetStateDown();				//!< �_�E����Ԃɂ���
	void SetStateTurnAttack();			//!< ��]�U����Ԃɂ���

	// -----------------------------------------
	// �G�t�F�N�g�̊֐�
	// -----------------------------------------
	int		GetWaveEffectHandle()				  const;		//!< �W�����v���̏Ռ��g�̃G�t�F�N�g���擾����
	int		GetDangerEffectHandle()				  const;		//!< �R���{�U�����̗\���G�t�F�N�g���擾����
	int		GetRoaringEffectHandle()			  const;		//!< ���K���̃G�t�F�N�g���擾����
	void	SetEffect(const int _effectHandle);					//!< �G�t�F�N�g�v���C���[�ɍĐ�����n���h���ƍ��W���Z�b�g����
	Vector3 GetEffectPos(const int _effectHandle) const;		//!< �G�t�F�N�g�̍��W��ݒ肷��


	void Init(std::string _filePath);		//!< ������
	void ReInit();							//!< �ď������i�n���h���i�[�ϐ��������j
	void EditInit();						//!< �G�f�B�b�g���ɍ��킹��������
	void MovieUpdate();						//!< ���[�r�[�����o���̍X�V
	void Update(float _delta) override;		//!< �X�V
	void DamageDraw();
	void PostDraw();						//!< �őO�ʂɕ`�悵�������̂��܂Ƃ߂�
};

