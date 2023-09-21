#include "../../System/WinMain.h"
#include "../../System/Class/Vector3.h"
#include "CharacterBase.h"


// �R���X�g���N�^
CharacterBase::CharacterBase():
	mState(-1),mHp(),mSpeed(-1.0f),
	mHpBar(-1),mBaseMatDif{-1.0f,-1.0f,-1.0f},
	mAtkPower(-1),mIsAtkSend(false),mAtkJudgeStart(-1.0f),mAtkJudgeEnd(-1.0f),mIsAttack(false), mCollSize(-1.0f),
	mAnimPlaySpeed(-1.0f),mIsAnimLoop(true)
{
}

// �f�X�g���N�^
CharacterBase::~CharacterBase()
{
}

// Vector3�^�̒l�ňꊇ�Z�b�g
void CharacterBase::SetPos(const Vector3 _pos)
{
	mPos.Set(_pos);
}

// x,y,z���ꂼ��ɒl���Z�b�g
void CharacterBase::SetPos(const float _x, const float _y, const float _z)
{
	mPos.Set(_x, _y, _z);
}

// �^�[�Q�b�g����̏����|�C���^�Ŏ擾����
void CharacterBase::SetTargetInfo(const CharacterBase* _target)
{
	mTargetInfo = _target;
}

// �U������̏d���h�~�t���O���Z�b�g
void CharacterBase::SetAtkSend()
{
	mIsAtkSend = true;
}

// �^�[�Q�b�g����̏����|�C���^�Ŏ󂯎��
bool CharacterBase::IsInTheStage(float _stageLength) const
{
	// �X�e�[�W�̒��S�i0,0,0�j������W�܂ł̒��������߂�
	float stageToPosLength = mPos.GetLength();

	if (_stageLength > stageToPosLength) return true;

	return false;
}

void CharacterBase::ChangeState(const int _nextState)
{
	mState = _nextState;
	mAnimationPlayer.Attach(_nextState);
}

// ���W���擾����
Vector3 CharacterBase::GetPos() const
{
	return mPos;
}

// VECTOR�^�ɕϊ����č��W���擾����
VECTOR CharacterBase::GetVECTORPos() const
{
	return 	mPos.GetVECTOR();
}

// �������擾����
float CharacterBase::GetRot() const
{
	return mRot.y;
}

// ���f���f�[�^���擾����
int CharacterBase::GetModel()
{
	return mModelData.GetModel();
}

// �U���͂��擾����
float CharacterBase::GetPower() const
{
	return mAtkPower;
}

// �U������鎞�Ɏg�p����R���W�����̃T�C�Y���擾����
float CharacterBase::GetCollSize() const
{
	return mCollSize;
}

// �U���͂��Z�b�g����
void CharacterBase::SetPower(const float _power)
{
	mAtkPower = _power;
}

// �U�������`�F�b�N����
bool CharacterBase::IsAttack() const
{
	return mIsAttack;
}

// ���f���̃f�B�t���[�Y�J���[��ԐF�ɕύX����
void CharacterBase::SetMatDifDamage()
{
	MV1SetMaterialAmbColor(GetModel(), 0, GetColorF(255.0f, 0.0f, 0.0f, 1.0f));
}

// ���f���̌��X�̃f�B�t���[�Y�J���[���Z�b�g����
void CharacterBase::SetMatDifBase()
{
	MV1SetMaterialDifColor(GetModel(), 0, mBaseMatDif);
}

// �U����Ԃ������[�X����
void CharacterBase::ReleaseAttack()
{
	mIsAttack	   = false;
	mAtkJudgeStart = -1.0f;
	mAtkJudgeEnd   = -1.0f;
}

// �U������̏d���h�~�t���O�̃`�F�b�N
bool CharacterBase::IsAtkSend() const
{
	return mIsAtkSend;
}

//�A�j���[�V�����̃t���[��������U������𔭐�������t���O���`�F�b�N
bool CharacterBase::IsAtkJudge() const
{
	return mAnimationPlayer.GetAnimNowTime() > mAtkJudgeStart && mAnimationPlayer.GetAnimNowTime() < mAtkJudgeEnd;
}

// �������֐�
void CharacterBase::Init(std::string _filePath)
{
	mModelData.Load(_filePath);
	mState = -1;
	mPos.Clear();
	mRot.Clear();
	mSize.Set(1.0f,1.0f,1.0f);

	mHp	   = 0.0f;
	mSpeed = 0.0f;
	mHpBar = 0;

	mAtkPower   = 0;
	mIsAtkSend  = false;

	mAnimPlaySpeed = 0.0f;
}

// �`��֐�
void CharacterBase::Draw()
{
	// ���f���̕`��
	MV1SetPosition(GetModel(), mPos.GetVECTOR());
	MV1SetRotationXYZ(GetModel(), mRot.GetVECTOR());
	MV1SetScale(GetModel(), mSize.GetVECTOR());
	mModelData.Draw();
}

// �I���֐�
void CharacterBase::Exit()
{
	mModelData.Exit();
	mAnimationPlayer.Exit();
}

// �őO�ʂɕ`�悵�������̂��܂Ƃ߂�
void CharacterBase::PostDraw()
{
}
