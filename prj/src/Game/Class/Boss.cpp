#include <math.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "../../external/imgui/imgui.h"
#include "../../external/imgui/imgui_impl_win32.h"
#include "../../external/imgui/imgui_impl_dx11.h"

#include "../../System/Cast.h"
#include "../../System/WinMain.h"
#include "../../System/Random.h"
#include "../../System/Class/Vector3.h"
#include "../../System/Class/Effect.h"
#include "../../System/Class/Animation.h"
#include "CharacterBase.h"
#include "Boss.h"

//�v���C���[�Ɛڂ��Ă����瓮�삷��U���I��֐����Ă΂ꑱ���Ă邩�炻��̏C��

bool Boss::IsLimitActions() const
{
	if (IsAttack() || IsReact() || IsBlowOff() || IsDown() || IsDeath())
	{
		return true;
	}

	return false;
}

void Boss::ReleaseAllFlag()
{
	ReleaseReact();
	ReleaseBlowOff();
	ReleaseDown();
	ReleaseAllAttack();
}

// �R���X�g���N�^
Boss::Boss() : mAnim{-1},mMoveEndPos{0.0f,0.0f,0.0f}, mDownValue(-1), mIsRightDirection(false), mReactValue(-1), mDownTimer(0),mOutOfSightTimer(0),
			   mIsFinishMovie(false),mIsGroundMovie(false),
			   mIsLightAttack(false), mIsHeavyAttack(false), mIsDown(false), mIsDeath(false), mIsIdle(false), mIsGuard(false), mIsJump(false),mIsComboAttack(false),
			   mIsBlowOff(false),mIsReact(false),
			   mThresholdGurad(-1),mstoredDamage(-1),
			   mEffectWaveHandle(-1),mEffectDangerHandle(-1),mEffectRoaringHandle(-1)
{
}

// �f�X�g���N�^
Boss::~Boss()
{
}

// �v���C���[�Ƃ̍��W����p�x���v�Z����
void Boss::GetPlayerPos(Vector3 _playerPos) 
{
	mMoveEndPos = _playerPos;
}

// �v���C���[�̕������v�Z����
void Boss::SetPlayerDir(Vector3 _playerPos)
{
	float x = _playerPos.x - mPos.x;
	float z = _playerPos.z - mPos.z;
	float dir = atan2f(-x, -z);
	mRot.y = dir;
}

bool Boss::IsGroundMovie() const
{
	return mIsGroundMovie;
}

bool Boss::IsFinishMoive() const
{
	return mIsFinishMovie;
}

bool Boss::IsAttack() const
{
	if (CheckAllAttack())
		return true;

	return false;
}

bool Boss::IsLightAttack() const
{
	return mIsLightAttack;
}

bool Boss::IsHeavyAttack() const
{
	return mIsHeavyAttack;
}

bool Boss::IsComboAttack() const
{
	return mIsComboAttack;
}

bool Boss::IsReact() const
{
	return mIsReact;
}

bool Boss::IsDeath() const
{
	return mIsDeath;
}

// �_���[�W���󂯎��
void Boss::SetDamage(const float _damage)
{
	if ((mHpBar) <= 100) return;

	float damegeRatio = _damage / mHp;

	mHp -= _damage;
	mHpBar -= (int)(mHpBar * damegeRatio);

	// �~�_���[�W�ɂ�鋯�ݏ���
	CountReactValue(_damage);
	
	// �_�E������
	if (mDownValue >= mDOWNMAX)
	{
		mDownValue = 0;
		ReleaseReact();
		ReleaseAllAttack();
		SetStateDown();
	}

	// ���S����
	if ((mHpBar) <= 100 && !mIsDeath)
	{
		mHpBar = 100;

		// �U�����ɂ��̊֐��ɓ���ƍU���t���O���Z�b�g���ꂽ�܂܂ɂȂ�̂�
		// �����Ń`�F�b�N���ă����[�X����
		if (IsAttack()) ReleaseAttack();
		if (IsJump()) ReleaseJump();

		mHp = 0;
		//mIsDeath = true;
		ChangeState(Death);
	}
}

// ���ݒl�̑���
void Boss::CountReactValue(const float _damage)
{

	if (IsBlowOff() || IsReact() || mIsDown) return;

	mReactValue += static_cast<int>(_damage);

	if (mReactValue >= mREACTMAX)
	{
		mReactValue = 0;
		++mDownValue;
		ReleaseAllAttack();
		SetStateReact();
	}
}

void Boss::SetThresholdGurad()
{
	// �c��HP�̊������玟��臒l�����肷��
	// �P�O���`�V��
	if		(mHp > (mHPMAX * 0.7f))	mThresholdGurad = GetRand(8) + 4;
	// �V���`�S��
	else if (mHp > (mHPMAX * 0.4f)) mThresholdGurad = GetRand(5) + 3;
	// �S���ȉ�
	else							mThresholdGurad = GetRand(3) + 2;
}

float Boss::GetCollSize() const
{
	return mStatus.collSize;
}

bool Boss::IsInvincible() const
{
	// ������ђ��A�n�ʂɂ��܂ł͖��G��Ԃɂ���
	if (!IsBlowOff()) return false;

	if (mAnimationPlayer.GetAnimNowTime() < 33.0f)
	{
		return true;
	}

	return false;
}

bool Boss::CheckAllAttack() const
{
	if (IsLightAttack())
		return true;

	if (IsHeavyAttack())
		return true;

	if (IsComboAttack())
		return true;

	return false;
}

bool Boss::IsSeriousDamage() const
{
	if (IsBlowOff())
		return true;

	if (IsDown())
		return true;

	return false;
}

void Boss::SetIsGroundMovie()
{
	if (IsGroundMovie()) return;
	mIsGroundMovie = true;
}

void Boss::SetFinishMovie()
{
	mIsFinishMovie = true;
}

// �U����Ԃ�
void Boss::SetAttack()
{
	mIsAtkSend = false;
	mIsAttack  = true;
}

void Boss::SetLightAttack()
{
	mIsLightAttack = true;
}

void Boss::SetHeavyAttack()
{
	mIsHeavyAttack = true;
}

void Boss::SetComboAttack()
{
	mIsComboAttack = true;
}

// �K�[�h��Ԃ�
void Boss::SetGuard()
{
	if (IsGuard())  return;
	mIsGuard = true;
}

// �J�E���^�[�U��
void Boss::SetGuardToAttack()
{
	mIsGuard = false;
	// �v���C���[��������ɂ��邩�ǂ����ŌJ��o���U����ς���
	if (IsSeePlayer()) SetStateLightAttack();
	else			   SetStateJump();
}

// �W�����v��Ԃ�
void Boss::SetJump()
{
	mIsAtkSend = false;
	mIsJump = true;
}

void Boss::SetBlowOff()
{
	mIsBlowOff = true;
}

// ���ݏ�Ԃ�
void Boss::SetReact()
{
	mIsReact = true;
}

void Boss::SetDown()
{
	mIsDown = true;
}

// ���S��Ԃ�
void Boss::SetDeath()
{
	mIsDeath = true;
}

// �W�����v��Ԃ̃����[�X
void Boss::ReleaseJump()
{
	mIsJump = false;
}

// ��U����Ԃ̃����[�X
void Boss::ReleaseLightAttack()
{
	mIsLightAttack = false;
	ReleaseAttack();
}

// ���U����Ԃ̃����[�X
void Boss::ReleaseHeavyAttack()
{
	mIsHeavyAttack = false;
	ReleaseAttack();
}

// �R���{�U����Ԃ̃����[�X
void Boss::ReleaseComboAttack()
{
	mIsComboAttack = false;
	ReleaseAttack();
}

// ���ݏ�Ԃ̃����[�X
void Boss::ReleaseReact()
{
	mIsReact = false;
}

void Boss::ReleaseBlowOff()
{
	mIsBlowOff = false;
}

void Boss::ReleaseDown()
{
	mIsDown = false;
}

// �S�Ă̍U���t���O���`�F�b�N���Ă��ꂼ�ꃊ���[�X����
void Boss::ReleaseAllAttack()
{
	if (IsLightAttack()) mIsLightAttack = false;
	if (IsHeavyAttack()) mIsHeavyAttack = false;
	if (IsComboAttack()) mIsComboAttack = false;
	if (IsJump())		 mIsJump		= false;
	if (IsAttack())		 mIsAttack		= false;
}


// �ړ��֐�
void Boss::Move(float _moveSpeed)
{
	mPos.x -= _moveSpeed * sinf(mRot.y);
	mPos.z -= _moveSpeed * cosf(mRot.y);
}

// �v���C���[��������ɂ��邩����
bool Boss::IsSeePlayer()
{
	// ��������v���C���[�ւ̃x�N�g���ƒ������v�Z
	Vector3 vecBtoP    = mTargetInfo->GetPos() - GetPos();

	// ���ʕ����̃x�N�g����ݒ�
	float directionRad = GetRot();
	Vector3 vecFront;
	vecFront.Set( -1.0f * sinf(directionRad), 
				  0.0f,
				  -1.0f * cosf(directionRad));

	// ��������v���C���[�ւ̃x�N�g���𐳋K��
	vecBtoP.Normalize();

	// ���ʃx�N�g���Ɛ��K�������x�N�g���œ��ς��v�Z����
	float dot = GetVector3Dot(vecBtoP, vecFront);

	// ����͈̔͂�cos�ɂ���
	float fanCos = cosf(TO_RADIAN(mVIEWANGLE) / 2.0f);

	// ���͈̔͂Ƀv���C���[�����邩�ǂ����`�F�b�N
	if (fanCos > dot) return false;

	return true;
}

// �v���C���[�����E�ǂ��瑤�ɂ��邩�v�Z
bool Boss::IsRightDirection()
{
	// �����̐��ʕ����̃x�N�g����p�ӂ���
	Vector3 vecFront;
	vecFront.Set(-1.0f * sinf(GetRot()),
		0.0f,
		-1.0f * cosf(GetRot()));
	// ��������v���C���[�ւ̃x�N�g���Ƃ̊O�ς��v�Z����
	Vector3 cross = GetVector3Cross((mTargetInfo->GetPos() - mPos), vecFront);

	// �O�ς����̐��Ȃ獶���A���̐��Ȃ�E���ɂ���
	if (cross.y > 0.0f)
	{
		// �����ɐ��񂵂Ȃ�����s����
		return false;
	}
	else
	{
		// �E���ɐ��񂵂Ȃ�����s����
		return true;
	}
}

void Boss::MoveDirection(const bool _isRightDir)
{
	// �E���
	if (_isRightDir)
	{
		mRot.y += 0.03f;
	}
	// �����
	else
	{
		mRot.y -= 0.03f;
	}
}

// �J��o���U�����c��HP����Ƀ����_���őI��
void Boss::ChoiseAttackType()
{
	// ����ł�����A�K�[�h���͂��̏����𔲂���
	if (IsLimitActions())	
		return;

	SetAttack();

	// ���l�������_���łP�擾����
	const int VALUEMAX = 9;
	int randValue = GetRand(VALUEMAX);

	// �c���HP�ɂ���Ĕ���̋敪���ς��
	// �P�O���`�V��
	if (mHp > (mHPMAX * 0.7f))
	{
		// ��U��:���U��		6:4
		if (randValue <= 5)
		{
			SetLightAttack();
			ChangeState(LightAttackIdle);
		}
		else
		{
			SetHeavyAttack();
			ChangeState(HeavyAttackIdle);
		}
	}
	// �V���`�S��
	else if (mHp > (mHPMAX * 0.4f))
	{
		// ��U���F���U���F�R���{�U��		4:4:2
		if (randValue <= 3)
		{
			SetLightAttack();
			ChangeState(LightAttackIdle);
		}
		else if (randValue <= 7)
		{
			SetHeavyAttack();
			ChangeState(HeavyAttackIdle);
		}
		else
		{
			SetStateComboAttack();
		}
	}
	// �S���ȉ�
	else
	{
		// ��U���F���U���F�R���{�U��		2:4:4
		if (randValue <= 1)
		{
			ChangeState(LightAttackIdle);
		}
		else if (randValue <= 5)
		{
			ChangeState(HeavyAttackIdle);
		}
		else
		{
			SetStateComboAttack();
		}
	}
}


// ���s��Ԃ�
void Boss::SetStateWalk()
{
	// �d���Z�b�g�h�~
	if (IsWalk()) return;

	// �t���O�̃����[�X�Y���h�~���邽�߂Ɉ�ĉ���
	ReleaseAllFlag();

	// ����̕������Z�b�g���ăA�j���[�V�����̐؂�ւ�
	mIsRightDirection = IsRightDirection();
	ChangeState(Walk);
}

// ���s��Ԃ�
void Boss::SetStateRun()
{
	ChangeState(Run);
}

// ��U����Ԃ�
void Boss::SetStateLightAttack()
{
	if (IsJump()) return;

	// �U������̔����ƏI���̃^�C�~���O
	mAtkJudgeStart = 2.0f;
	mAtkJudgeEnd = 10.5f;
	// ���̍U���̈З͂�ݒ�
	SetPower(mStatus.attackPower[BOSSATTACKTYPE::Light]);

	// �U���̎�ނ𔻕ʂ��邽�߂Ƀt���O���Q���Ă�
	SetLightAttack();
	// �X�e�[�g��ύX
	ChangeState(LightAttack);
}

// ���U����Ԃ�
void Boss::SetStateHeavyAttack()
{
	if (IsJump()) return;

	// �U������̊J�n�ƏI���̃t���[����ݒ�
	mAtkJudgeStart = 25.0f;
	mAtkJudgeEnd = 38.0f;
	// ���̍U���̈З͂�ݒ�
	SetPower(mStatus.attackPower[BOSSATTACKTYPE::Heavy]);

	// �����蔻��̏����ōU���̎�ނ𔻕ʂ��邽�߂ɂ����P�t���O�𗧂ĂĂ���
	SetHeavyAttack();
	ChangeState(HeavyAttack);
}

// �R���{�U����Ԃ�
void Boss::SetStateComboAttack()
{
	mEffectPlayer.SetPlay(mEffectDangerHandle,mPos);

	// �U������̊J�n�ƏI���̃t���[����ݒ�
	mAtkJudgeStart = 50.0f;
	mAtkJudgeEnd   = 98.0f;
	// ���̍U���̈З͂�ݒ�
	SetPower(mStatus.attackPower[BOSSATTACKTYPE::Combo]);

	// �U���̎�ނ𔻒肷�邽�߂ɂQ�t���O�𗧂Ă�
	SetComboAttack();
	ChangeState(ComboAttack);
}

// �U����Ԃ�
void Boss::SetStateGuard()
{
	// �U���s�����ɃK�[�h���Ȃ����߂Ƀt���O�𗧂Ă�
	if (IsAttack() || IsJump()) return;
	SetGuard();
	ChangeState(Guard);
}

// �W�����v��Ԃ�
void Boss::SetStateJump()
{
	if (IsJump() && IsAttack()) return;

	// �����蔻��𔭐�������A�j���[�V�����̎��Ԃ�ݒ肷��
	mAtkJudgeStart = 62.0f;
	mAtkJudgeEnd = 72.0f;
	// ���̍U���̈З͂�ݒ�
	SetPower(mStatus.attackPower[BOSSATTACKTYPE::Jump]);

	SetJump();
	ChangeState(Jump);
}

void Boss::SetStateBlowOff()
{
	if (IsSeriousDamage()) return;

	ReleaseAllAttack();
	SetBlowOff();
	mReactValue = 0;
	ChangeState(BlowOff);
}

// ���ݏ�Ԃ�
void Boss::SetStateReact()
{
	SetReact();
	ChangeState(React);
}

void Boss::SetStateDown()
{
	if (IsSeriousDamage()) return;
	ReleaseAllAttack();
	SetDown();
	ChangeState(Down);
}

void Boss::SetStateTurnAttack()
{
	if (!IsDown()) return;

	// �U������̔����t���[���̐ݒ�
	mAtkJudgeStart = 20.0f;
	mAtkJudgeEnd = 40.0f;

	// ���̍U���Ɣ����������Ⴄ���߁AChoiseAttackType�֐����g�p���Ȃ�
	SetAttack();
	SetPower(10.0f);
	ChangeState(TurnAttack);
}

// �ҋ@��Ԃ��`�F�b�N
bool Boss::IsIdle() const
{
	return mState == Idle;
}

// ���s��Ԃ��`�F�b�N
bool Boss::IsWalk() const
{
	return mState == Walk;
}

// �K�[�h��Ԃ��`�F�b�N
bool Boss::IsGuard() const
{
	return mIsGuard;
}

// �W�����v��Ԃ��`�F�b�N
bool Boss::IsJump() const
{
	return mIsJump;
}

bool Boss::IsBlowOff() const
{
	return mIsBlowOff;
}

bool Boss::IsDown() const
{
	return mIsDown;
}

// �܂̃J�v�Z���̃T�C�Y
float Boss::GetClawSize() const
{
	return mCollInfo.clawSize;
}

// ���̃J�v�Z���̃T�C�Y
float Boss::GetFistSize() const
{
	return mCollInfo.fistSize;
}

Vector3 Boss::GetFistTopPos() const
{
	return mCollInfo.fistPos[BOSSATTACKCOLLTYPE::Top];
}

Vector3 Boss::GetFistUnderPos() const
{
	return mCollInfo.fistPos[BOSSATTACKCOLLTYPE::Under];
}

Vector3 Boss::GetClawTopPos() const
{
	return mCollInfo.clawPos[BOSSATTACKCOLLTYPE::Top];
}

Vector3 Boss::GetClawUnderPos() const
{
	return mCollInfo.clawPos[BOSSATTACKCOLLTYPE::Under];
}

// �W�����v���̏Ռ��g�̃G�t�F�N�g���擾
int Boss::GetWaveEffectHandle() const
{
	return mEffectWaveHandle;
}

// �R���{�U�����̗\���G�t�F�N�g���擾
int Boss::GetDangerEffectHandle() const
{
	return mEffectDangerHandle;
}

// ���K���̃G�t�F�N�g���擾
int Boss::GetRoaringEffectHandle() const
{
	return mEffectRoaringHandle;
}

// �G�t�F�N�g�v���C���[�Ƀn���h���ƍ��W���Z�b�g����
void Boss::SetEffect(const int _effectHandle)
{
	Vector3 effectPos = GetEffectPos(_effectHandle);
	mEffectPlayer.SetPlay(_effectHandle, effectPos);
}

// �Đ�����G�t�F�N�g�̍��W���Z�b�g����i�Đ�����G�t�F�N�g�ɂ���č��W���ς��j
Vector3 Boss::GetEffectPos(const int _effectHandle) const
{
	Vector3 tmpPos = GetPos();

	// ���K�G�t�F�N�g��Y���W���������
	if (_effectHandle == GetRoaringEffectHandle()) tmpPos.y = 18.0f;

	return tmpPos;
}

// ������
void Boss::Init(std::string _filePath)
{
	mModelData.Load(_filePath);
	mPos.Set(0.0f, 50.0f, 80.0f);
	mRot.Clear();
	mSize.Set(0.3f, 0.3f, 0.3f);
	mBaseMatDif = MV1GetMaterialDifColor(mModelData.GetModel(), 0);

	LoadStatus("data/csv/BossStatus.csv", mStatus);
	LoadCollData("data/csv/BossColl.csv", mCollInfo);
	mHp = mStatus.hpMax;
	mHpBar = 1180;		// �{�P�O�O��HP�o�[�̍����X���W

	mDownTimer	 = 0;	
	mOutOfSightTimer = 0;
	mReactValue		 = 0;
	mDownValue		 = 0;
	mstoredDamage	 = 0;
	mThresholdGurad  = GetRand(3) + 3;

	mAtkJudgeStart = 0.0f;
	mAtkJudgeEnd   = 0.0f;

	mIsGroundMovie    = false;
	mIsFinishMovie	  = false;
	mIsAttack		  = false;
	mIsIdle			  = false;
	mIsDown			  = false;
	mIsJump			  = false;
	mIsGuard		  = false;
	mIsDeath		  = false;
	mIsRightDirection = false;

	// �A�j���[�V���������[�h
	mAnimationPlayer.Load("data/model/Boss/Animation/Boss_Entry_jump.mv1");
	mAnimationPlayer.Load("data/model/Boss/Animation/Boss_Roaring.mv1");
	mAnimationPlayer.Load("data/model/Boss/Animation/Boss_Idle.mv1");
	mAnimationPlayer.Load("data/model/Boss/Animation/Boss_Walking.mv1");
	mAnimationPlayer.Load("data/model/Boss/Animation/Boss_Run.mv1");
	mAnimationPlayer.Load("data/model/Boss/Animation/Boss_LightAttack_Idle.mv1");
	mAnimationPlayer.Load("data/model/Boss/Animation/Boss_LightAttack.mv1");
	mAnimationPlayer.Load("data/model/Boss/Animation/Boss_HeavyAttack_Idle.mv1");
	mAnimationPlayer.Load("data/model/Boss/Animation/Boss_HeavyAttack.mv1");
	mAnimationPlayer.Load("data/model/Boss/Animation/Boss_ComboAttack.mv1");
	mAnimationPlayer.Load("data/model/Boss/Animation/Boss_TurnAttack.mv1");
	mAnimationPlayer.Load("data/model/Boss/Animation/Boss_Jump.mv1");
	mAnimationPlayer.Load("data/model/Boss/Animation/Boss_Guard.mv1");
	mAnimationPlayer.Load("data/model/Boss/Animation/Boss_BlowOff.mv1");
	mAnimationPlayer.Load("data/model/Boss/Animation/Boss_GettingUp.mv1");
	mAnimationPlayer.Load("data/model/Boss/Animation/Boss_React.mv1");
	mAnimationPlayer.Load("data/model/Boss/Animation/Boss_Down.mv1");
	mAnimationPlayer.Load("data/model/Boss/Animation/Boss_Down_GettingUp.mv1");
	mAnimationPlayer.Load("data/model/Boss/Animation/Boss_Death.mv1");
	mAnimationPlayer.SetModelData(GetModel());
	
	mAnimationPlayer.InitContexts();

	ChangeState(Entry);
	mAnimPlaySpeed = 0.3f;

	mEffectPlayer.ReleasePlay();
	mEffectWaveHandle	 = LoadEffekseerEffect("data/effect/boss/Wave.efkefc", 12.0f);
	mEffectDangerHandle  = LoadEffekseerEffect("data/effect/boss/Danger.efkefc", 10.0f);
	mEffectRoaringHandle = LoadEffekseerEffect("data/effect/boss/Roaring - �R�s�[.efkefc", 15.0f);
}

void Boss::ReInit()
{
	mPos.Set(0.0f, 50.0f, 80.0f);
	mRot.Clear();

	LoadStatus("data/csv/BossStatus.csv", mStatus);
	mHp = mStatus.hpMax;
	mHpBar = 1180;		// �{�P�O�O��HP�o�[�̍����X���W

	mDownTimer = 0;
	mOutOfSightTimer = 0;
	mReactValue = 0;
	mDownValue = 0;
	mstoredDamage = 0;
	mThresholdGurad = GetRand(3) + 3;

	mAtkJudgeStart = 0.0f;
	mAtkJudgeEnd = 0.0f;

	mIsGroundMovie = false;
	mIsFinishMovie = false;
	mIsAttack = false;
	mIsIdle = false;
	mIsDown = false;
	mIsJump = false;
	mIsGuard = false;
	mIsDeath = false;
	mIsRightDirection = false;

	mAnimationPlayer.InitContexts();

	ChangeState(Entry);
	mAnimPlaySpeed = 0.3f;

	mEffectPlayer.ReleasePlay();
}

void Boss::EditInit()
{
	mPos.Set(0.0f, 0.0f, 80.0f);
	mRot.Clear();
	LoadStatus(csvPaths.statusPath.c_str(), mStatus);
	LoadCollData(csvPaths.collPath.c_str(), mCollInfo);
	mAnimationPlayer.InitContexts();
	ChangeState(Idle);
	mHp			   = mStatus.hpMax;
	mAnimPlaySpeed = 0.5f;
}

void Boss::MovieUpdate()
{
	// �A�j���[�V�����̍Đ�
	mAnimationPlayer.Play(mAnimPlaySpeed,mIsAnimLoop);

	// �A�j���[�V�����̑J��
	switch (mState)
	{
		// ---------------------------------------------
		// �o��
		// ---------------------------------------------
	case Entry:
		mIsAnimLoop = false;
		// ���n�܂ō��W��������i�A�j���[�V�����ƍ��W�������N���������ł���΂��������Ȃ��H�j
		if (mPos.y > 0) --mPos.y;
		else	   mPos.y = 0.0f;

		// �A�j���[�V���������n�����^�C�~���O�Œ��n�t���O�𗧂Ă�
		if (mAnimationPlayer.GetAnimNowTime() > 15.0f) SetIsGroundMovie();

		// �A�j���[�V�������I����������K����
		if (!mAnimationPlayer.IsPlay())
		{
			ChangeState(Roaring);
		}

		break;

		// ---------------------------------------------
		// ���K
		// ---------------------------------------------
	case Roaring:
		mIsAnimLoop = false;
		// �Đ����x�𑬂����邩�A�G�t�F�N�g�̍Đ����Ԃ�L�΂��邩
		mAnimPlaySpeed = 0.5f;

		// �G�t�F�N�g���Z�b�g����(�d���Đ��h�~�̂��߂ɍĐ��`�F�b�N������)
		if (!mEffectPlayer.IsPlay() && mAnimationPlayer.GetAnimNowTime() > 20.0f) SetEffect(GetRoaringEffectHandle());

		// �A�j���[�V�����̍Đ����I��������A�G�t�F�N�g�̃t���O�������[�X���A���s��Ԃ�
		if (!mAnimationPlayer.IsPlay())
		{
			mEffectPlayer.ReleasePlay();
			ChangeState(Idle);
		}
		break;

		// ---------------------------------------------
		// �ҋ@
		// ---------------------------------------------
	case Idle:
		mIsAnimLoop	   = true;
		mAnimPlaySpeed = 0.5f;
		break;
	}
}

// �X�V
void Boss::Update(float _delta)
{

	DamageUpdate();

	// ---------------------------------------------------------
	// �A�j���[�V�����Đ�����
	// ---------------------------------------------------------
	mAnimationPlayer.Play(mAnimPlaySpeed,mIsAnimLoop);

	// �Đ����x�̒���
	mAnimPlaySpeed *= _delta;

	// �ړ��I�_�Ƃ̋������v�Z
	Vector3 v = mMoveEndPos - mPos;
	// �ړ��n�_�܂ł̋���
	float destiDistance = v.GetLength();

	
	// �����̍��W�����ƂɎ��ۂ̍��W�Ƃ̍������v�Z
	Vector3 tmpPos = mAnimationPlayer.FixedOffset("mixamorig:LeftToeBase", "mixamorig:RightToeBase");
	Vector3 subPosToBase = tmpPos - mPos;
	
	// ���̍��W�����ƂɎ��ۂ̍��W�Ƃ̍������v�Z
	Vector3 hipPos = mAnimationPlayer.GetBasePos("mixamorig:Hips");
	Vector3 subPosToHips = hipPos - mPos;
	

	// ---------------------------------------------------------------------------------
	// �s���p�^�[���ʂ̏���
	// ---------------------------------------------------------------------------------
	switch (mState)
	{	
		// ---------------------------------------------
		// �ҋ@
		// ---------------------------------------------
	case Idle:
		// ��ʂɉf���Ă��郂�f���̍��W�Ǝ��ۂ̍��W�̌덷���C������
		mPos += subPosToBase;

		// ���[�r�[����J�ڂ��Ă����炻�̂܂ܕ��s��Ԃ�
		SetStateWalk();

		break;

		// ---------------------------------------------
		// ���s
		// ---------------------------------------------
	case Walk:
		mAnimPlaySpeed = 0.2f;

		// �ړ����x�̐ݒ�ƍ��W�ړ��֐�
		Move(mStatus.moveSpeed);

		// �v���C���[������̊O�ɋ�������񂵂Ȃ���ړ�����
		if (!IsSeePlayer())
		{
			MoveDirection(mIsRightDirection);

			// �v���C���[������O�ɂ��鎞�Ԃ��v������
			++mOutOfSightTimer;
			// �K�萔�ɒB�������ɔ͈͓��Ƀv���C���[������΃W�����v�U��������
			if (mOutOfSightTimer > mOUTOFSIGHTMAX)
			{
				mOutOfSightTimer = 0;
				//SetStateJump();
			}
		}
		else 
		{
			// ������ɂ���Ƃ��̓v���C���[��ǔ�����
			SetPlayerDir(mTargetInfo->GetPos());
		}

		mIsAnimLoop = true;

		break;

		// ---------------------------------------------
		// ���s
		// ---------------------------------------------
	case Run:
		mAnimPlaySpeed = 0.3f;
		Move(mSpeed * 2.0f);

		// ���B�n�܂ŗ�����ҋ@��Ԃ�
		if (destiDistance < mPos.GetLength())
		{
			ChangeState(Idle);
		}
		break;

		// ---------------------------------------------
		// ��U���ҋ@
		// ---------------------------------------------
	case LightAttackIdle:

		mAnimPlaySpeed = 0.6f;

		// �A�j���[�V�����������������s��Ԃ�
		if (!mAnimationPlayer.IsPlay())
		{
			SetPower(mStatus.attackPower[BOSSATTACKTYPE::Light]);
			SetStateLightAttack();
		}
		break;

		// ---------------------------------------------
		// ��U��
		// ---------------------------------------------
	case LightAttack:
		mAnimPlaySpeed = 0.25f;
		mAnimPlaySpeed *= _delta;

		// �A�j���[�V�����������������s��Ԃ�
		if (!mAnimationPlayer.IsPlay())
		{
			ReleaseLightAttack();
			SetStateWalk();
		}
		break;

		// ---------------------------------------------
		// ���U���ҋ@
		// ---------------------------------------------
	case HeavyAttackIdle:

		mAnimPlaySpeed = 0.6f;

		// �A�j���[�V�����I����͕��s��Ԃ�
		if (!mAnimationPlayer.IsPlay())
		{
			SetPower(mStatus.attackPower[BOSSATTACKTYPE::Heavy]);
			SetStateHeavyAttack();
		}

		break;

		// ---------------------------------------------
		// ���U��
		// ---------------------------------------------
	case HeavyAttack:
		// �A�j���[�V�����̎��Ԃɂ���čĐ����Ԃ�ς���
		if		(mAnimationPlayer.GetAnimNowTime() < 25.0f) mAnimPlaySpeed = 0.25f;		// �U��͂���
		else if (mAnimationPlayer.GetAnimNowTime() < 38.0f)	mAnimPlaySpeed = 0.55f;		// �U�蔲�����i���̋�Ԃ��������蔻����o���j
		else												mAnimPlaySpeed = 0.25f;		// �U��؂��

		// �A�j���[�V�����I����͕��s��Ԃ�
		if (!mAnimationPlayer.IsPlay())
		{
			ReleaseHeavyAttack();
			SetStateWalk();
		}

		break;

		// ---------------------------------------------
		// �R���{�U��
		// ---------------------------------------------
	case ComboAttack:
		// �A�j���[�V�����ɍ��킹�čĐ����x��ς���
		// �U�肩�Ԃ�
		if (mAnimationPlayer.GetAnimNowTime() < 50.0f)
		{
			mAnimPlaySpeed = 0.25f;		
		}
		// �U����
		else if (mAnimationPlayer.GetAnimNowTime() < 98.0f)
		{
			mAnimPlaySpeed = 0.45f;
		}
		// �U���I����
		else
		{
			mAnimPlaySpeed = 0.3f;		
		}

		// �A�j���[�V�����I����͕��s��Ԃ�
		if (!mAnimationPlayer.IsPlay())
		{
			mEffectPlayer.ReleasePlay();
			ReleaseComboAttack();
			ChangeState(Idle);
		}

		break;

		// ---------------------------------------------
		// �W�����v
		// ---------------------------------------------
	case Jump:
		// �A�j���[�V�����ɂɍ��킹�čĐ����x��ς���
		if		(mAnimationPlayer.GetAnimNowTime() < 34.0f)	mAnimPlaySpeed = 0.35f;
		else if (mAnimationPlayer.GetAnimNowTime() < 57.0f) mAnimPlaySpeed = 0.5f;
		else								mAnimPlaySpeed = 0.25f;

		// �A�j���[�V�����ɍ��킹��Y���W�����~������
		if (mAnimationPlayer.GetAnimNowTime() > 35.0f) mPos.y += 0.1f;
		if (mAnimationPlayer.GetAnimNowTime() > 50.0f) mPos.y -= 0.2f;
		if (mAnimationPlayer.GetAnimNowTime() > 62.0f)
		{
			mPos.y = 0.0f;
			// �Ռ��g�̃G�t�F�N�g���Z�b�g����(�d���Đ���h�~���邽�߂ɍĐ��`�F�b�N������)
			if (!mEffectPlayer.IsPlay())mEffectPlayer.SetPlay(mEffectWaveHandle,GetPos());
		}

		// �A�j���[�V�������I����������s��Ԃ�
		if (!mAnimationPlayer.IsPlay())
		{
			// �J�E���^�[�ŃW�����v�U���������Ƃ��͎���O�̃^�C�}�[��
			// �l���������܂܂Ȃ̂łO�ɂ���
			if (mOutOfSightTimer != 0) mOutOfSightTimer = 0;

			// �G�t�F�N�g�v���C���[�̃t���O�������[�X����
			mEffectPlayer.ReleasePlay();

			ReleaseJump();
			SetStateWalk();
		}

		break;

		// ---------------------------------------------
		// �������
		// ---------------------------------------------
	case BlowOff:
		mAnimPlaySpeed = 0.35f;

		// �N���オ��
		if (!mAnimationPlayer.IsPlay())
		{
			ChangeState(GetUp);
		}
		break;

		// ---------------------------------------------
		// �N���オ��
		// ---------------------------------------------
	case GetUp:
		mAnimPlaySpeed = 0.6f;

		// �N���オ��
		if (!mAnimationPlayer.IsPlay())
		{
			ReleaseBlowOff();
			ChangeState(Idle);
		}
		break;

		// ---------------------------------------------
		// ����
		// ---------------------------------------------
	case React:
		mAnimPlaySpeed = 0.2f;
		if (!mAnimationPlayer.IsPlay())
		{
			ReleaseReact();
			SetStateWalk();
		}
		break;

		// ---------------------------------------------
		// �K�[�h
		// ---------------------------------------------
	case Guard:
		mAnimPlaySpeed = 0.45f;
		if (!mAnimationPlayer.IsPlay())
		{
			mIsGuard = false;
			SetStateWalk();
		}
		break;

		// ---------------------------------------------
		// �_�E��
		// ---------------------------------------------
	case Down:
		mAnimPlaySpeed = 0.3f;
		mIsAnimLoop = false;

		if (mAnimationPlayer.GetAnimNowTime() > 42.0f)
		{
			++mDownTimer;
		}

		if (mDownTimer >= mDOWNTIMEMAX)
		{
			// ��ʂɉf���Ă��郂�f���̈ʒu�Ǝ��ۂ̍��W�̌덷���C������
			mPos += subPosToHips;
			mPos.y = 0.0f;
			ChangeState(DownGetUp);
		}

		break;

		// ---------------------------------------------
		// �_�E������̋N���オ��
		// ---------------------------------------------
	case DownGetUp:
		mAnimPlaySpeed = 0.4f;

		if (mAnimationPlayer.GetAnimNowTime() > 100.0f)
		{
			SetStateTurnAttack();
		}

		break;

		// ---------------------------------------------
		// �_�E������̋N���オ�莞�̉�]�U��
		// ---------------------------------------------
	case TurnAttack:
		mAnimPlaySpeed = 0.4f;
		
		if (!mAnimationPlayer.IsPlay())
		{
			ReleaseDown();
			ReleaseAttack();
			SetStateWalk();
		}

		break;

		// ---------------------------------------------
		// ���S
		// ---------------------------------------------
	case Death:
		mAnimPlaySpeed = 0.3f;
		// �|��؂���������Ŏ��S����ɂ���
		if (mAnimationPlayer.GetAnimNowTime() > 60.0f) SetDeath();

		mIsAnimLoop = false;
		break;
	}
}

void Boss::DamageDraw()
{
	if (mDamageFrame % 10 != 0)
	{
		CharacterBase::SetMatDifDamage();
	}
	else
	{
		CharacterBase::SetMatDifBase();
	}
}

// �őO��`��
void Boss::PostDraw()
{
	// HP�o�[�̕`��
	DrawLineBox(99, 699, WINDOW_W - 99, 716, GetColor(255, 255, 255));
	DrawBox(100, 700, mHpBar, 715, GetColor(255, 128, 64), TRUE);
}
