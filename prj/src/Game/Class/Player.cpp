#include "../../System/WinMain.h"
#include "../../external/imgui/imgui.h"
#include "../../external/imgui/imgui_impl_win32.h"
#include "../../external/imgui/imgui_impl_dx11.h"
#include "../../System/Class/Vector3.h"
#include "../../System/Class/Effect.h"
#include "CharacterBase.h"
#include "../../System/Input.h"
#include "Player.h"
#include <math.h>


// �R���X�g���N�^
Player::Player() :
	mIsReact(false), mIsDeath(false), mAttackCombo(), mIsRoll(), mIsTargetLook(),mIsRepell(),mIsBlowOff(),mIsGetUp(),mIsWalk(),
	mIsInvincible(),mMoveDir(),mAtkReserveS(),mAtkReserveE(),mSwordSize(),mCameraHAngle(),
	mAnim{}
{
}

// �f�X�g���N�^
Player::~Player()
{
}

void Player::SetRangeOfMove(const float _rangeRadius)
{
	mRangeOfMove = _rangeRadius;
}

// �L�[���͂ɂ������̕ύX
void Player::ChangeRot()
{
	if (mIsTargetLook) return;
	// �O�㍶�E
	if (CheckHitKey(KEY_INPUT_W)) mRot.y = TO_RADIAN(180.0f);
	if (CheckHitKey(KEY_INPUT_A)) mRot.y = TO_RADIAN(90.0f);
	if (CheckHitKey(KEY_INPUT_S)) mRot.y = TO_RADIAN(0.0f);
	if (CheckHitKey(KEY_INPUT_D)) mRot.y = TO_RADIAN(270.0f);
	// �΂ߍ��E�O��
	if (CheckHitKey(KEY_INPUT_W) && CheckHitKey(KEY_INPUT_A)) mRot.y = TO_RADIAN(135.0f);
	if (CheckHitKey(KEY_INPUT_W) && CheckHitKey(KEY_INPUT_D)) mRot.y = TO_RADIAN(225.0f);
	if (CheckHitKey(KEY_INPUT_S) && CheckHitKey(KEY_INPUT_A)) mRot.y = TO_RADIAN(45.0f);
	if (CheckHitKey(KEY_INPUT_S) && CheckHitKey(KEY_INPUT_D)) mRot.y = TO_RADIAN(315.0f);
}

void Player::ChangeMoveDir()
{
	// �񃍃b�N�I�����͑��삵�Ȃ�
	if (!IsTargetLook()) return;

	if (CheckHitKey(KEY_INPUT_W)) mMoveDir = mRot.y;
	if (CheckHitKey(KEY_INPUT_A)) mMoveDir = mRot.y + (TO_RADIAN(270.0f));
	if (CheckHitKey(KEY_INPUT_S)) mMoveDir = mRot.y + (TO_RADIAN(180.0f));
	if (CheckHitKey(KEY_INPUT_D)) mMoveDir = mRot.y + (TO_RADIAN(90.0f));
}

// �G�̕������v�Z����
void Player::SetEnemyDir(const Vector3 _enemyPos)
{
	float x = _enemyPos.x - mPos.x;
	float z = _enemyPos.z - mPos.z;
	float dir = atan2f(-x, -z);
	mRot.y = dir;
}

// ��_���[�W�֐�
void Player::SetDamage(const float _damage)
{
	if (mTargetInfo == nullptr)
		return;


	//mHp -= _damage;
	mHpBar -= (int)mTargetInfo->GetPower();

	// ���S����
	if (mHpBar <= 5)
	{
		mHp = 5;
		SetDeath();
		ChangeState(Death);
	}
}

void Player::CopyCameraHAngle(const float _cameraHAngle)
{
	mCameraHAngle = _cameraHAngle;
}

// �ړ��̃L�[���͂��Ȃ����`�F�b�N����
bool Player::IsNotMove()
{
	return !CheckHitKey(KEY_INPUT_W) && !CheckHitKey(KEY_INPUT_A) &&
		!CheckHitKey(KEY_INPUT_S) && !CheckHitKey(KEY_INPUT_D);
}

// ���̑�����󂯕t�������Ȃ��t���O�����ׂă`�F�b�N����
bool Player::IsStopKeyInput() const
{
	return (IsReact()  || IsDeath() || IsRoll() || IsAttack() || IsRepell() ||
			IsBlowOff() || IsGetUp());
}

bool Player::IsRoll() const
{
	return mIsRoll;
}

bool Player::IsGetUp() const
{
	return mIsGetUp;
}

// ������я�Ԃ��`�F�b�N
bool Player::IsBlowOff() const
{
	return mIsBlowOff;
}

// �e�����Ԃ��`�F�b�N
bool Player::IsRepell() const
{
	return mIsRepell;
}

bool Player::IsReact() const
{
	return mIsReact;
}

// ���S��Ԃ��`�F�b�N
bool Player::IsDeath() const
{
	return mIsDeath;
}

void Player::SetRoll()
{
	mIsRoll = true;
}

void Player::SetGetUp()
{
	mIsGetUp = true;
}

// ���S��Ԃ�
void Player::SetDeath()
{
	mIsDeath = true;
}

// ���b�N�I����Ԃ��`�F�b�N
bool Player::IsTargetLook() const
{
	return mIsTargetLook;
}

// ���b�N�I����Ԃ�
void Player::SetTargetLook()
{
	if (mIsTargetLook)return;
	mIsTargetLook = true;
}

// ���b�N�I����Ԃ�����
void Player::ReleaseTargetLook()
{
	if (!mIsTargetLook)return;
	mIsTargetLook = false;
}

void Player::ReleaseRoll()
{
	mIsRoll = false;
	mIsInvincible = false;
}

void Player::ReleaseReact()
{
	mIsReact = false;
}

void Player::ReleaseGetUp()
{
	mIsGetUp = false;
}

void Player::ReleaseBlowOff()
{
	mIsBlowOff = false;
}

void Player::ReleaseDeath()
{
	mIsDeath = false;
}

void Player::ReleaseRepell()
{
	mIsRepell = false;
}

// ������я�Ԃɂ���
void Player::SetBlowOff()
{
	mIsBlowOff = true;
}

void Player::SetRepell()
{
	mIsRepell = true;
}

// ���݂̍U���񐔂��擾
int Player::GetAtkCombo() const
{
	return mAttackCombo;
}

// �U����Ԃɂ���
void Player::SetAttack()
{
	mIsAttack = true;
}

// �U����Ԃ��`�F�b�N
bool Player::IsAttack() const
{
	return mIsAttack;
}

// ���G��Ԃ��`�F�b�N
bool Player::IsInvincible()
{
	return (mIsInvincible || IsGetUp());
}

float Player::GetSwordSize() const
{
	return mSwordSize;
}

void Player::ReleaseAttackInfo()
{
	if (!IsAttack()) return;
	ReleaseAttack();
	mAttackCombo = 0;
}

void Player::SetDuringReserve(const float _startFraem, const float _endFrame)
{
	mAtkReserveS = _startFraem;
	mAtkReserveE = _endFrame;
}

void Player::InitDuringReserve()
{
	mAtkReserveS = -1.0f;
	mAtkReserveE = -1.0f;
}

bool Player::IsDuringReserve() const
{
	return mAnimationPlayer.GetAnimNowTime() > mAtkReserveS && mAnimationPlayer.GetAnimNowTime() < mAtkReserveE;
}

bool Player::CheckNextAttackCondition(const int _nowAttack) const
{
	return GetAtkCombo() == _nowAttack && IsDuringReserve();
}

bool Player::IsLastComboAttack() const
{
	// �A�j���[�V�����������ւ��������mAttackCombo�ɉ��Z�����邽�߁A
	// Combo4�{�P���Ă����Ȃ���΁A�R�i�ڂ̍U������true���Ԃ��Ă���
	return mAttackCombo == ATTACKTYPE::Combo4 + 1;
}

// �ҋ@�A�j���[�V�������Z�b�g
void Player::SetStateIdle()
{
	if (mIsReact || mIsDeath) return;

	mAttackCombo = 0;
	ChangeState(Idle);
}

// ���s�A�j���[�V�������Z�b�g
void Player::SetStateWalk()
{
	mSpeed = mWALKSPEED;
	ChangeState(Walk);
}

// ���s�A�j���[�V�������Z�b�g
void Player::SetStateRun()
{
	if (mIsReact || mIsDeath) return;
	mSpeed = mRUNSPEED;
	ChangeState(Run);
}

// �U���A�j���[�V�������Z�b�g
void Player::SetStateAttack(int _nextAttack)
{
	SetAttack();
	mIsAtkSend = false;


	// �R���{���ɂ���čU���͂�ς���
	switch (mAttackCombo)
	{
		// -----------------------
		// �U��1�i��
		// -----------------------
	case ATTACKTYPE::Combo1:
		// �U�����肪��������^�C�~���O
		mAtkJudgeStart = 5.5f;
		mAtkJudgeEnd = 12.0f;

		// ���̍U���̓��͎�t����
		SetDuringReserve(10.0f, 23.0f);

		// �U���͂̐ݒ�
		SetPower(5.0f);
		break;

		// -----------------------
		// �U��2�i��
		// -----------------------
	case ATTACKTYPE::Combo2:
		// �U�����肪��������^�C�~���O
		mAtkJudgeStart = 6.0f;
		mAtkJudgeEnd = 10.5f;

		// ���̍U���̓��͎�t����
		SetDuringReserve(19.0f, 28.0f);

		// �U���͂̐ݒ�
		SetPower(3.0f);
		break;

		// -----------------------
		// �U��3�i��
		// -----------------------
	case ATTACKTYPE::Combo3:
		// �U�����肪��������^�C�~���O
		mAtkJudgeStart = 22.0f;
		mAtkJudgeEnd = 30.0f;

		// ���͎�t���Ԃ��g���Ȃ��悤�ɂ���
		SetDuringReserve(27.0f, 44.0f);

		// �U���͂̐ݒ�
		SetPower(7.0f);
		break;

		// -----------------------
		// �U��4�i��
		// -----------------------
	case ATTACKTYPE::Combo4:
		// �U�����肪��������^�C�~���O
		mAtkJudgeStart = 43.0f;
		mAtkJudgeEnd = 57.5f;

		// ���͎�t���Ԃ��g���Ȃ��悤�ɂ���
		InitDuringReserve();

		// �U���͂̐ݒ�
		SetPower(10.0f);
		break;
	}

	if (mAttackCombo < mCONTATKMAX) ++mAttackCombo;

	ChangeState(_nextAttack);
}

// �e����A�j���[�V�������Z�b�g
void Player::SetStateRepell()
{
	// �R���{�񐔂����Z�b�g���Ă���
	mAttackCombo = 0;

	mIsRepell = true;
	ChangeState(Repell);
}

// �_���[�W�A�j���[�V�������Z�b�g
void Player::SetStateReact()
{
	if (IsAttack()) ReleaseAttackInfo();
	if (IsRoll())	ReleaseRoll();
	
	mIsReact = true;
	ChangeState(React);
}

// ������уA�j���[�V�������Z�b�g
void Player::SetStateBlowOff()
{
	if (IsAttack()) ReleaseAttackInfo();
	if (IsRoll())	ReleaseRoll();

	SetBlowOff();
	ChangeState(BlowOff);
}

// �O�]�A�j���[�V�������Z�b�g
void Player::SetStateRoll()
{
	mIsRoll = true;
	ChangeState(Roll);
}

void Player::SetStateGetUp()
{
	mIsGetUp = true;
	ChangeState(GetUp);
}

// ���S�A�j���[�V�������Z�b�g
void Player::SetStateDeath()
{
	mSpeed		  = 0.0f;
	mIsReact = false;
	mIsDeath	  = true;
	ChangeState(Death);
}

int Player::GetSlashEffectHandle() const
{
	return mEffectHandles[Slash];
}

int Player::GetBloodEffectHandle() const
{
	return mEffectHandles[Blood];
}

void Player::SetEffect()
{
	//���̊֐��ɓ������Ƃ��A�v���C���[���U�������_���[�W�����ő}���G�t�F�N�g�n���h���ς���悤����
	int effectHandle = -1;

	if (IsReact() || IsBlowOff())
	{
		effectHandle = GetBloodEffectHandle();
	}
	else if (IsAttack())
	{
		effectHandle = GetSlashEffectHandle();
	}

	Vector3 effectPos = GetEffectPos(effectHandle);
	mEffectPlayer.SetPlay(effectHandle,effectPos);
}

Vector3 Player::GetEffectPos(const int _effectHandle) const
{
	Vector3 tmpPos;

	// �n���ꂽ�G�t�F�N�g�̃n���h���ɍ��������W���Z�b�g����
	if (_effectHandle == GetSlashEffectHandle())
	{
		tmpPos.Set(mTargetInfo->GetPos());
		tmpPos.y = 15.0f;
	}
	if (_effectHandle == GetBloodEffectHandle()) tmpPos.Set(mPos.x,mPos.y + 10.0f,mPos.z);

	return tmpPos;
}

// ������
void Player::Init(std::string _filePath, const float _rangeMoveRad)
{
	mState = Idle;
	mPos.Set(0.0f, 0.0f, -80.0f);
	mRot.Set(0.0f, TO_RADIAN(180.0f), 0.0f);
	mSize.Set(0.1f, 0.1f, 0.1f);
	mHp				= 50;
	mHpBar			= 305;
	mSpeed		    = 0.0f;
	mAttackCombo	= 0;
	mSwordSize		= 3.0f;
	mAtkJudgeStart  = 0.0f;
	mAtkJudgeEnd	= 0.0f;
	mCollSize		= 7.0f;
	mMoveDir		= TO_RADIAN(0.0f);
	mRangeOfMove	= _rangeMoveRad;

	mIsAttack		= false;
	mIsRoll			= false;
	mIsReact		= false;
	mIsRepell		= false;
	mIsDeath		= false;
	mIsInvincible	= false;
	mIsAtkSend		= false;
	mIsTargetLook	= false;

	// �A�j���[�V�����̓ǂݍ���
	mAnimationPlayer.Load("data/model/Player/Animation/Player_Idle.mv1");
	mAnimationPlayer.Load("data/model/Player/Animation/Player_Attack1.mv1");
	mAnimationPlayer.Load("data/model/Player/Animation/Player_Attack2.mv1");
	mAnimationPlayer.Load("data/model/Player/Animation/Player_Attack_Trun.mv1");
	mAnimationPlayer.Load("data/model/Player/Animation/Player_Attack3.mv1");
	mAnimationPlayer.Load("data/model/Player/Animation/Player_Walk.mv1");
	mAnimationPlayer.Load("data/model/Player/Animation/Player_Run.mv1");
	mAnimationPlayer.Load("data/model/Player/Animation/Player_Roll.mv1");
	mAnimationPlayer.Load("data/model/Player/Animation/Player_GettingUp.mv1");
	mAnimationPlayer.Load("data/model/Player/Animation/Player_React.mv1");
	mAnimationPlayer.Load("data/model/Player/Animation/Player_Repell.mv1");
	mAnimationPlayer.Load("data/model/Player/Animation/Player_BlowOff.mv1");
	mAnimationPlayer.Load("data/model/Player/Animation/Player_Death.mv1");

	mModelData.Load(_filePath);
	mAnimationPlayer.SetModelData(mModelData.GetModel());
	mAnimationPlayer.InitContexts();

	// �ŏ��͑ҋ@��Ԃ��A�^�b�`���Ă���
	ChangeState(Idle);
	mAnimPlaySpeed = 0.3f;

	mEffectPlayer.ReleasePlay();
	mEffectHandles[Slash] = LoadEffekseerEffect("data/effect/player/Slash.efkefc", 10.0f);
	mEffectHandles[Blood] = LoadEffekseerEffect("data/effect/player/Blood2.efkefc", 0.5f);

	mSpeedScale = 1.0f;





}

void Player::ReInit()
{
	mPos.Set(0.0f, 0.0f, -80.0f);
	mRot.Set(0.0f, TO_RADIAN(180.0f), 0.0f);
	mHp            = 50;
	mHpBar         = 305;
	mAttackCombo   = 0;
	mAtkJudgeStart = 0.0f;
	mAtkJudgeEnd   = 0.0f;
	mCollSize	   = 7.0f;
	mMoveDir	   = TO_RADIAN(0.0f);

	mIsAttack	  = false;
	mIsRoll		  = false;
	mIsReact	  = false;
	mIsRepell	  = false;
	mIsDeath	  = false;
	mIsInvincible = false;
	mIsAtkSend    = false;
	mIsTargetLook = false;

	mAnimationPlayer.InitContexts();

	// �ŏ��͑ҋ@��Ԃ��A�^�b�`���Ă���
	ChangeState(Idle);
	mAnimPlaySpeed = 0.3f;

	mEffectPlayer.ReleasePlay();

	mSpeedScale = 1.0f;
}

void Player::EditInit()
{
	mPos.Set(0.0f, 0.0f, -80.0f);
	mRot.Set(0.0f, TO_RADIAN(180.0f), 0.0f);
	mIsReact	  = false;
	mIsRepell	  = false;
	mIsDeath	  = false;
	mIsInvincible = false;

	mAnimationPlayer.InitContexts();
	ChangeState(Idle);
	mAnimPlaySpeed = 0.3f;

}

// �X�V
void Player::Update(float _delta)
{
	if (PushHitKey(KEY_INPUT_Y)) SetDamage(1080);
	// ���t���[���A�ړ��̃t���O�ƃx�N�g����������
	mIsWalk = false;
	mMoveVector.Clear();

	// ---------------------------------------------------------
	// �A�j���[�V�����Đ�����
	// ---------------------------------------------------------
	// �Đ����x�̒���
	mAnimPlaySpeed *= _delta;

	// �A�j���[�V�����̍Đ�
	mAnimationPlayer.Play(mAnimPlaySpeed, mIsAnimLoop);
	
	// ---------------------------------------------------------
	// �X�e�[�W��̈ړ��ł���͈͂̐���
	// ---------------------------------------------------------
	// �X�e�[�W�̒��S���獡�̍��W�܂ł̒���
	float centerToPosLength = mPos.GetLength();
	// �X�e�[�W�̒��S������W�̒������X�e�[�W�͈̔͂�蒷�����
	// �X�e�[�W�͈͓̔��̒����ɖ߂�
	if(centerToPosLength > mRangeOfMove) mPos.SetLength(mRangeOfMove);

	// ---------------------------------------------------------
	// �L�[���͂̏���
	// ---------------------------------------------------------
	// 2�i�ڈȍ~�̃R���{�U���̏���
	if (PushMouseInput(MOUSE_INPUT_LEFT))
	{
		if (CheckNextAttackCondition(Attack3))
		{
			SetStateAttack(Attack4);
		}
		else if (CheckNextAttackCondition(Attack2))
		{
			SetStateAttack(Attack3);
		}
		else if (CheckNextAttackCondition(Attack1))
		{
			SetStateAttack(Attack2);
		}
	}

	// ���̃L�[���͂��󂯕t���Ă����Ԃł����
	if (!IsStopKeyInput())
	{
		// �O�]
		if (PushHitKey(KEY_INPUT_SPACE))	SetStateRoll();
		// �U��
		// �R���{���̔���̓s����2�C3�i�ڂ̉��ɏ����Ă���
		if (PushMouseInput(MOUSE_INPUT_LEFT))	SetStateAttack(Attack1);
	}

	// ���b�N�I��
	if (CheckHitKey(KEY_INPUT_K))
	{
		// �G������������擾
		SetEnemyDir(mTargetInfo->GetPos());
		// �G�̂ق�������
		SetTargetLook();
	}
	else
	{
		// ���b�N�I����Ԃ�����
		ReleaseTargetLook();
	}

	// ---------------------------------------------------------
	// �s���p�^�[���ʂ̏���
	// ---------------------------------------------------------
	switch (mState)
	{
		// ---------------------------------------------
		// �ҋ@
		// ---------------------------------------------
	case Idle:
		// �ړ��֑J��
		if (CheckHitKey(KEY_INPUT_W) || CheckHitKey(KEY_INPUT_A) || CheckHitKey(KEY_INPUT_S) || CheckHitKey(KEY_INPUT_D))
		{
			ChangeState(Run);
		}

		// ���X�Ɍ������Ď~�܂�
		mSpeed *= 0.9f;
		if (mSpeed < 0.001f) mSpeed = 0.0f;

		mAnimPlaySpeed = 0.3f;
		mIsAnimLoop = true;

		break;

		// ---------------------------------------------
		// ���s
		// ---------------------------------------------
	case Walk:
		mAnimPlaySpeed = 0.3f;
		mIsAnimLoop = true;

		// �����̕ύX
		//ChangeRot();
		// �ړ������̕ύX�i���b�N�I�����̂݁j
		ChangeMoveDir();

		if (!IsTargetLook())
		{
			if (CheckHitKey(KEY_INPUT_W))
			{
				mRot.y = TO_RADIAN(180.0f - mCameraHAngle);
				mIsWalk = true;
				mMoveVector.z = mWALKSPEED;
			}
			if (CheckHitKey(KEY_INPUT_A))
			{
				mRot.y = TO_RADIAN(90.0f - mCameraHAngle);
				mIsWalk = true;
				mMoveVector.x = -mWALKSPEED;
			}
			if (CheckHitKey(KEY_INPUT_S))
			{
				mRot.y = TO_RADIAN(0.0f - mCameraHAngle);
				mIsWalk = true;
				mMoveVector.z = -mWALKSPEED;
			}
			if (CheckHitKey(KEY_INPUT_D))
			{
				mRot.y = TO_RADIAN(-90.0f - mCameraHAngle);
				mIsWalk = true;
				mMoveVector.x = mWALKSPEED;
			}

			if (CheckHitKey(KEY_INPUT_W) && CheckHitKey(KEY_INPUT_D))
			{
				mRot.y = TO_RADIAN(-135.0f - mCameraHAngle);
			}
			if (CheckHitKey(KEY_INPUT_W) && CheckHitKey(KEY_INPUT_A))
			{
				mRot.y = TO_RADIAN(135.0f - mCameraHAngle);
			}
			if (CheckHitKey(KEY_INPUT_S) && CheckHitKey(KEY_INPUT_D))
			{
				mRot.y = TO_RADIAN(-45.0f - mCameraHAngle);
			}
			if (CheckHitKey(KEY_INPUT_S) && CheckHitKey(KEY_INPUT_A))
			{
				mRot.y = TO_RADIAN(45.0f - mCameraHAngle);
			}

			// �J�����̐����p�x���l�����������Ɉړ�����
			if (mIsWalk)
			{
				// �v�Z���č��W�ɓn���悤��
				Vector3 tmpMoveVector;

				// �J�����̊p�x�ɕ����Ĉړ��x�N�g������]���Ă���v�Z
				float sinParam = sinf(TO_RADIAN(mCameraHAngle));
				float cosParam = cosf(TO_RADIAN(mCameraHAngle));
				tmpMoveVector.x = mMoveVector.x * cosParam - mMoveVector.z * sinParam;
				tmpMoveVector.y = 0.0f;
				tmpMoveVector.z = mMoveVector.x * sinParam + mMoveVector.z * cosParam;

				// ���K�����đ��x������
				tmpMoveVector.Normalize();

				// �v�Z�ł����ړ��x�N�g�������W�ɑ�������
				mPos += tmpMoveVector;
			}
		}
		// ���b�N�I�����͓G�����ɍ��W�ړ�
		if (IsTargetLook())
		{
			mPos.x -= mWALKSPEED * sinf(mMoveDir);
			mPos.z -= mWALKSPEED * cosf(mMoveDir);
		}

		// ���W�ړ�
		// ���b�N�I�����͑���Ȃ�
		if (CheckHitKey(KEY_INPUT_LSHIFT) && !IsTargetLook()) ChangeState(Run);
		// �L�[���͂��Ȃ��ꍇ�͑ҋ@��Ԃ�
		if (IsNotMove()) ChangeState(Idle);
		
		break;

		// ---------------------------------------------
		// ���s
		// ---------------------------------------------
	case Run:
		mAnimPlaySpeed = 0.3f;
		mIsAnimLoop = true;

		// �����̕ύX
		ChangeRot();

		// ���W�ړ�
		if (CheckHitKey(KEY_INPUT_W))
		{
			mRot.y = TO_RADIAN(180.0f - mCameraHAngle);
			mIsWalk = true;
			mMoveVector.z = mWALKSPEED;
		}
		if (CheckHitKey(KEY_INPUT_A))
		{
			mRot.y = TO_RADIAN(90.0f - mCameraHAngle);
			mIsWalk = true;
			mMoveVector.x = -mWALKSPEED;
		}
		if (CheckHitKey(KEY_INPUT_S))
		{
			mRot.y = TO_RADIAN(0.0f - mCameraHAngle);
			mIsWalk = true;
			mMoveVector.z = -mWALKSPEED;
		}
		if (CheckHitKey(KEY_INPUT_D))
		{
			mRot.y = TO_RADIAN(-90.0f - mCameraHAngle);
			mIsWalk = true;
			mMoveVector.x = mWALKSPEED;
		}

		if (CheckHitKey(KEY_INPUT_W) && CheckHitKey(KEY_INPUT_D))
		{
			mRot.y = TO_RADIAN(-135.0f - mCameraHAngle);
		}
		if (CheckHitKey(KEY_INPUT_W) && CheckHitKey(KEY_INPUT_A))
		{
			mRot.y = TO_RADIAN(135.0f - mCameraHAngle);
		}
		if (CheckHitKey(KEY_INPUT_S) && CheckHitKey(KEY_INPUT_D))
		{
			mRot.y = TO_RADIAN(-45.0f - mCameraHAngle);
		}
		if (CheckHitKey(KEY_INPUT_S) && CheckHitKey(KEY_INPUT_A))
		{
			mRot.y = TO_RADIAN(45.0f - mCameraHAngle);
		}

		// �J�����̐����p�x���l�����������Ɉړ�����
		if (mIsWalk)
		{
			// ���X�ɑ��x�������邽�߂̔{���ϐ��̑���
			if (mSpeedScale < 1.8f)
			{
				mSpeedScale += 0.015f;
			}

			// �v�Z���č��W�ɓn���悤��
			Vector3 tmpMoveVector;

			// �J�����̊p�x�ɕ����Ĉړ��x�N�g������]���Ă���v�Z
			float sinParam = sinf(TO_RADIAN(mCameraHAngle));
			float cosParam = cosf(TO_RADIAN(mCameraHAngle));
			tmpMoveVector.x = mMoveVector.x * cosParam - mMoveVector.z * sinParam;
			tmpMoveVector.y = 0.0f;
			tmpMoveVector.z = mMoveVector.x * sinParam + mMoveVector.z * cosParam;

			// ���K�����đ��x������
			tmpMoveVector.Normalize();

			tmpMoveVector *= mSpeedScale;

			// �v�Z�ł����ړ��x�N�g�������W�ɑ�������
			mPos += tmpMoveVector;
		}

		// �ҋ@�A���s�ւ̑J��
		if (ReleaseKey(KEY_INPUT_LSHIFT)) ChangeState(Idle);
		// �L�[���͂��Ȃ��ꍇ�͑ҋ@��Ԃ�
		if (IsNotMove())
		{
			mSpeedScale = 1.0f;
			ChangeState(Idle);
		}
		break;

		// ---------------------------------------------
		// �O�]
		// ---------------------------------------------
	case Roll:
		mIsAnimLoop = false;
		// �O�]�J�n���Ɨ����オ�鎞�ŃX�s�[�h��ς���
		if (mAnimationPlayer.GetAnimNowTime() < 20.0f)
		{
			mSpeed = 0.8f;
			mAnimPlaySpeed = 1.0f;

			mPos.x -= mSpeed * sinf(mRot.y);
			mPos.z -= mSpeed * cosf(mRot.y);
		}
		else mAnimPlaySpeed = 0.5f;


		// ���G���Ԃ̐ݒ�
		// 
		// 
		// 
		// �����֐��ɂ�����else������
		if (mAnimationPlayer.GetAnimNowTime() > 5.0f && mAnimationPlayer.GetAnimNowTime() < 30.0f)	mIsInvincible = true;
		else mIsInvincible = false;

		// �Đ����I��������ҋ@��Ԃ�
		if (!mAnimationPlayer.IsPlay())
		{
			ReleaseRoll();
			SetStateIdle();
		}
		break;

		// ---------------------------------------------
		// �N���オ��
		// ---------------------------------------------
	case GetUp:

		// �A�j���[�V�������I��������ҋ@��Ԃ�
		if (!mAnimationPlayer.IsPlay())
		{
			ReleaseGetUp();
			SetStateIdle();
		}
		break;

		// ---------------------------------------------
		// �U���P�i��
		// ---------------------------------------------
	case Attack1:
		mAnimPlaySpeed = 0.6f;
		mIsAnimLoop = false;

		// �A�j���[�V�������I��������ҋ@��Ԃ�
		if (!mAnimationPlayer.IsPlay()) 
		{
			ReleaseAttack();
			SetStateIdle();
		}

		break;

		// ---------------------------------------------
		// �U���Q�i��
		// ---------------------------------------------
	case Attack2:
		mAnimPlaySpeed = 0.5f;
		mIsAnimLoop = false;

		// �A�j���[�V�������I��������ҋ@��Ԃ�
		if (!mAnimationPlayer.IsPlay())
		{
			ReleaseAttack();
			SetStateIdle();
		}
		
		break;

		// ---------------------------------------------
		// �U���R�i��
		// ---------------------------------------------
	case Attack3:
		// �A�j���[�V�����̏󋵂ɂ���čĐ����x��ς���
		if (mAnimationPlayer.GetAnimNowTime() < 56.5f) mAnimPlaySpeed = 0.9f;
		else mAnimPlaySpeed = 0.5f;

		mIsAnimLoop = false;

		// �A�j���[�V�������I��������ҋ@��Ԃ�
		if (!mAnimationPlayer.IsPlay())
		{
			ReleaseAttack();
			SetStateIdle();
		}

		break;

		// ---------------------------------------------
		// �U��4�i��
		// ---------------------------------------------
	case Attack4:
		// �A�j���[�V�����̏󋵂ɂ���čĐ����x��ς���
		if (mAnimationPlayer.GetAnimNowTime() < 56.5f) mAnimPlaySpeed = 0.9f;
		else mAnimPlaySpeed = 0.5f;
		mIsAnimLoop = false;

		// �A�j���[�V�������I��������ҋ@��Ԃ�
		if (!mAnimationPlayer.IsPlay())
		{
			ReleaseAttack();
			SetStateIdle();
		}

		break;

		// ---------------------------------------------
		// ����
		// ---------------------------------------------
	case React:
		mAnimPlaySpeed = 0.3f;
		mIsAnimLoop = false;

		// �A�j���[�V�������I��������ҋ@��Ԃ�
		if (!mAnimationPlayer.IsPlay())
		{
			ReleaseReact();
			SetStateIdle();
		}

		break;

		// ---------------------------------------------
		// �e����
		// ---------------------------------------------
	case Repell:
		mAnimPlaySpeed = 0.2f;
		mIsAnimLoop = false;

		// �A�j���[�V�������I��������ҋ@��Ԃ�
		if (!mAnimationPlayer.IsPlay())
		{
			ReleaseRepell();
			SetStateIdle();
		}

		break;

		// ---------------------------------------------
		// �������
		// ---------------------------------------------
	case BlowOff:
		mAnimPlaySpeed = 0.5f;
		mIsAnimLoop = false;

		// ���f���������Ă���Ԃ͌������֍��W���ړ�������
		if (mAnimationPlayer.GetAnimNowTime() < 22.0f)
		{
			mPos.x += 1.0f * sinf(mRot.y);
			mPos.z += 1.0f * cosf(mRot.y);
		}

		// �A�j���[�V�������I��������N���オ���Ԃ�
		if (!mAnimationPlayer.IsPlay())
		{
			ReleaseBlowOff();
			SetStateGetUp();
		}
		break;
	}

	// �G�t�F�N�g�̏���
	// �G�t�F�N�g�֘A
	// �G�t�F�N�g�̍Đ��t���O���Z�b�g����Ă����烊���[�X����
	if (mEffectPlayer.CheckPlayEnd()) mEffectPlayer.ReleasePlay();
}

// �`��
void Player::PostDraw()
{
	// HP�o�[�̕`��
	DrawLineBox(4, 2, 306, 31, GetColor(255, 255, 255));
	DrawBox(5, 3, mHpBar, 30, GetColor(0, 255, 0), TRUE);
}
