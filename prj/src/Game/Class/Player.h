#pragma once
#include "../../System/Class/Effect.h"
#include "../../System/Class/Animation.h"
#include "../../System/Class/Model.h"


//------------------------------------------------
//!@class Player�N���X
//!@brief �v���C���[�L�����N�^�[���� 
//------------------------------------------------
class Player : public CharacterBase
{
	//------------------------------------------------
	// �񋓑�
	//------------------------------------------------
	//! �X�e�[�g�̗񋓑�
	enum PLAYERSTATE
	{
		Idle,		//!< �ҋ@
		Attack1,	//!< �U��
		Attack2,
		Attack3,
		Attack4,
		Walk,		//!< ���s
		Run,		//!< ���s
		Roll,		//!< �O�]
		GetUp,		//!< �N���オ��
		React,		//!< ����
		Repell,		//!< �e����
		BlowOff,	//!< �������
		Death,		//!< ���S
		StateMax,	//!< �v�f�̍ő吔��p�i�K�����̗v�f���Ō�ɗ��邱�Ɓj
	};

	//! �G�t�F�N�g�n���h���̗񋓑�
	enum EFFECTHANDLE
	{
		Slash,		//!< �a��
		Blood,		//!< �_���[�W
		HandleMax,	//!< �v�f�̍ő吔��p�i�K�����̗v�f���Ō�ɗ��邱�Ɓj
	};

	//! �U���񐔂��J�E���g����p
	enum ATTACKTYPE
	{
		Combo1,		//!< 1�i��
		Combo2,		//!< �Q�i��
		Combo3,		//!< �R�i��
		Combo4,		//!< �S�i��
		ComboMax,	//!< �ő吔
	};

	//-----------------------------
	// �萔
	//-----------------------------
	const float mWALKSPEED = 0.8f;		//!< ���s���x
	const float mRUNSPEED = 1.4f;		//!< ���s���x
	const int	mCONTATKMAX = 4;		//!< �A���U���̍ő吔

	//-----------------------------
	// �s�����Ɏg���t���O
	//-----------------------------
	bool mIsRoll;			//!< �O�]
	bool mIsGetUp;			//!< �N���オ��
	bool mIsReact;			//!< ����
	bool mIsBlowOff;		//!< �������
	bool mIsRepell;			//!< �e����
	bool mIsDeath;			//!< ���S
	bool mIsInvincible;		//!< ���G
	bool mIsTargetLook;		//!< ���b�N�I��

	//-----------------------------
	// �U������Ƃ��Ɏg���ϐ�
	//-----------------------------
	int	  mAttackCombo;		//!< �U���񐔂̃J�E���g 
	float mSwordSize;		//!< �����蔻��Ɏg�p���錕�̃T�C�Y
	float mAtkReserveS;		//!< �R���{�̓��͎�t�J�n����
	float mAtkReserveE;		//!< �R���{�̓��͎�t�I������

	//-----------------------------
	// �A�j���[�V�����Ɏg���ϐ�
	//-----------------------------
	int   mAnim[StateMax];	//!< �A�j���[�V�����̃��f���f�[�^

	//-----------------------------
	// ���̑��Ɏg���ϐ�
	//-----------------------------
	float mMoveDir;			//!< �ړ������i�L�����N�^�[�̌����ł͂Ȃ��̒��Ӂj



	// �V����������ϐ��i�蒅���悵�����胊�t�@�N�^�����O�j
	bool mIsWalk;
	Vector3 mMoveVector;
	float mCameraHAngle;	//!< �J�����N���X����R�s�[���Ă��������p�x
	float mRangeOfMove;		//!< �X�e�[�W���̈ړ��͈�
	float mSpeedScale;		//!< �ړ����x�̔{��
	


	//-----------------------------
	// �G�t�F�N�g�Ɏg���ϐ�
	//-----------------------------
	int mEffectHandles[HandleMax];	//!< �G�t�F�N�g�̃n���h��������
	Effect mEffectPlayer;

public:
	Player();	//!< �R���X�g���N�^
	~Player();	//!< �f�X�g���N�^	

	void SetRangeOfMove(const float _rangeRadius);

	//------------------------------------
	// �l�̃Z�b�g�A�����[�X�̊֐�
	//------------------------------------
	void ChangeRot();											//!< �L�[���͂ɂ������̕ύX
	void ChangeMoveDir();										//!< ���b�N�I�����̈ړ����������ݒ肷��
	void SetEnemyDir(const Vector3 _enemyPos);					//!< �G�̍��W����G�̕������Z�b�g����
	void SetDamage(const float _damage);	//!< ��_���[�W�֐�
	void CopyCameraHAngle(const float _cameraHAngle);

	//------------------------------------
	// �t���O�ϐ��̃Z�b�g�ƃ����[�X�̊֐�
	//------------------------------------
	void SetRoll();					//!< �O�]��Ԃɂ���
	void SetGetUp();				//!< �N���オ���Ԃɂ���
	void SetDeath();				//!< ���S��Ԃɂ���
	void SetTargetLook();			//!< ���b�N�I����Ԃɂ���
	void SetBlowOff();				//!< ������я�Ԃɂ���
	void SetRepell();				//!< �e�����Ԃɂ���
	void ReleaseTargetLook();		//!< ���b�N�I����Ԃ�����
	void ReleaseRoll();				//!< �O�]��Ԃ̉���
	void ReleaseReact();			//!< ���ݏ�Ԃ̉���
	void ReleaseGetUp();			//!< �N���オ���Ԃ̉���
	void ReleaseBlowOff();			//!< ������я�Ԃ̉���
	void ReleaseDeath();			//!< ���S��Ԃ̉���
	void ReleaseRepell();			//!< �e�����Ԃ̉���

	//------------------------------------
	// �t���O�`�F�b�N�̊֐�
	//------------------------------------
	bool IsNotMove();				//!< �ړ��̃L�[���͂��Ȃ����`�F�b�N
	bool IsStopKeyInput() const;	//!< �L�[���͂��󂯕t�����Ă��Ȃ����`�F�b�N
	bool IsRoll()		  const;	//!< �O�]�`�F�b�N
	bool IsGetUp()		  const;	//!< �N���オ��`�F�b�N
	bool IsBlowOff()	  const;	//!< ������΂���Ă����Ԃ��`�F�b�N
	bool IsRepell()		  const;	//!< �U�����e���ꂽ���`�F�b�N
	bool IsReact()		  const;	//!< �_���[�W��Ԃ��`�F�b�N
	bool IsDeath()		  const;	//!< ���S���Ă��邩�`�F�b�N
	bool IsTargetLook()	  const;	//!< ���b�N�I�������`�F�b�N

	//------------------------------------
	// �U���֘A�̊֐�
	//------------------------------------
	int  GetAtkCombo() const;		//!< �R���{���̎擾
	void SetAttack();				//!< �U����ԂɃZ�b�g
	bool IsAttack() const;			//!< �U�������`�F�b�N
	bool IsInvincible();			//!< ���G�����`�F�b�N
	float GetSwordSize() const;		//!< �����蔻��Ɏg�p���錕�̃J�v�Z���̃T�C�Y
	void ReleaseAttackInfo();		//!< �U���Ɏg�p����ϐ������ׂČ��ɖ߂�
	void SetDuringReserve(const float _startFraem, const float _endFrame);	//!< ���͊��Ԃ̐ݒ�
	void InitDuringReserve();		//!< ���͊��Ԃ̏�����
	bool IsDuringReserve() const;	//!< �R���{�U���ɂ�����A���̍U���ւ̓��͎�t�t���[���̃`�F�b�N
	bool CheckNextAttackCondition(const int _nowAttack) const;	//!< �R���{�U���ɂ�����A���̍U���֑J�ڂł����Ԃł��邩�`�F�b�N
	bool IsLastComboAttack() const;	//!< �R���{�U���̍ŏI�i���`�F�b�N

	//------------------------------------
	// �A�j���[�V�����̐؂�ւ��֐�
	//------------------------------------
	void SetStateIdle();						//!< �ҋ@��Ԃ�
	void SetStateWalk();						//!< ���s��Ԃ�
	void SetStateRun();							//!< ���s��Ԃ�
	void SetStateAttack(int _nextAttack);		//!< �U����Ԃ�
	void SetStateRoll();						//!< �O�]��Ԃ�
	void SetStateGetUp();						//!< �N���オ���Ԃ�
	void SetStateReact();						//!< �_���[�W��Ԃ�
	void SetStateBlowOff();						//!< ������я�Ԃ�
	void SetStateRepell();						//!< �e�����Ԃ�
	void SetStateDeath();						//!< ���S��Ԃ�

	//------------------------------------
	// �G�t�F�N�g�̊֐�
	//------------------------------------
	int GetSlashEffectHandle() const;		//!< �a���G�t�F�N�g�̃n���h�����擾����
	int GetBloodEffectHandle() const;		//!< �_���[�W�G�t�F�N�g�̃n���h�����擾����	

	void SetEffect();
	Vector3 GetEffectPos(const int _effectHandle) const;			//!< �G�t�F�N�g�̍��W��ݒ肷��

	void Init(std::string _filePath, const float _rangeMoveRad);
	void ReInit();													//!< �Q�[�����ēx�J�n����Ƃ��ɏ��������K�v�ȕϐ���������������
	void EditInit();												//!< �G�f�B�b�g���ɍ��킹��������
	void Update(float _delta);
	void PostDraw();												//!< ��ԏ�ɕ`�悵�������̂��������ŕ`�悷��
};