#include "../../System/WinMain.h"
#include "../../System/Random.h"
#include "../../System/Class/Vector3.h"
#include "CharacterBase.h"
#include "Player.h"
#include "../../System/Input.h"
#include "Camera.h"

#include <cmath>

void Camera::AngleUpdate()
{
	// �}�E�X�̈ړ��ʂ��璍���_����]������
	Vector2 mousePos;
	mousePos.x = (float)GetMouseX();
	mousePos.y = (float)GetMouseY();

	// ��ʒ��S����ړ��ʂ��擾
	Vector2 mouseMoveValue;
	mouseMoveValue.x = WINDOW_HALF_W - mousePos.x;
	mouseMoveValue.y = WINDOW_HALF_H - mousePos.y;

	// ���̈ړ��ʂ��A���O���l�ɓ����
	mAngleH += (mouseMoveValue.x * mANGLEMOVERATIO);
	mAngleV += (mouseMoveValue.y * mANGLEMOVERATIO);

	// �A���O���l�̃��Z�b�g
	if (mAngleH <= -180.0f) mAngleH += 360.0f;
	if (mAngleH >=  180.0f) mAngleH -= 360.0f;
	if (mAngleV >=   10.0f) mAngleV  =  10.0f;
	if (mAngleV <=  -40.0f) mAngleV  = -40.0f;

	// �}�E�X�̈ʒu����ʒ����ɖ߂�
	SetMousePoint(WINDOW_W / 2, WINDOW_H / 2);
}

// �R���X�g���N�^
Camera::Camera():
	mAngleH(),mAngleV(),
	mIsShakeMovie(false),mShakeTimer(0),misFinishMovie(false)
{
}

// �f�X�g���N�^
Camera::~Camera()
{
}

void Camera::SetFinishMovie()
{
	misFinishMovie = true;
}

bool Camera::IsFinishMovie() const
{
	return misFinishMovie;
}

void Camera::SetMovieShakeTime()
{
	if (!IsMovieShake())
	{
		SetMovieShake();
		mShakeTimer = mMOVIESHAKETIME;
	}
}

void Camera::SetMovieShake()
{
	mIsShakeMovie = true;
}

bool Camera::IsMovieShake() const
{
	return mIsShakeMovie;
}

void Camera::ReleaseLockOn()
{
	// ----------------
	// �����p�x�̌v�Z
	// ----------------
	// ���W���璍���_�̃x�N�g�����g���č��̐����p�x���v�Z����
	Vector3 vecPosToLook  = mLookPos - mPos;
	float	lockOnHorizon = atan2f(vecPosToLook.x, vecPosToLook.z);

	lockOnHorizon *= -1;
	// �p�x�̓f�O���[�p�ň����Ă��邩��ϊ����đ������
	mAngleH = TO_DEGREE(lockOnHorizon);

	// ----------------
	// �����p�x�̌v�Z
	// ----------------
	// �킩��񂩂������񎿖⎆�ɍs���Ȃ�
	mAngleV = 0.0f;
}

// ���̐����p�x���擾����i�v���C���[�̍��W�ړ��Ɏg�p����j
float Camera::GetAngleHorizontal() const
{
	return mAngleH;
}

// �J�����̊p�x�����W�ɔ��f������
Vector3 Camera::ReflectionAngle() const
{
	float   sinParam;		// �����A�����p�x��sinf�֐��Ōv�Z�����l������
	float   cosParam;		// �����A�����p�x��cosf�֐��Ōv�Z�����l������
	Vector3 tmpPosV;		// ���f���ꂽ�����p�x�����Ă���
	Vector3 tmpPosH;		// ���f���ꂽ�����p�x�����Ă���

	//	�����p�x�𔽉f
	sinParam = sinf(TO_RADIAN(mAngleV));
	cosParam = cosf(TO_RADIAN(mAngleV));
	tmpPosV.x = 0.0f;
	tmpPosV.y =  sinParam * mLOOKDISTANCE;
	tmpPosV.z = -cosParam * mLOOKDISTANCE;

	// �����p�x�𔽉f
	sinParam = sin(TO_RADIAN(mAngleH));
	cosParam = cos(TO_RADIAN(mAngleH));
	tmpPosH.x = cosParam * tmpPosV.x - sinParam * tmpPosV.z;
	tmpPosH.y = tmpPosV.y;
	tmpPosH.z = sinParam * tmpPosV.x + cosParam * tmpPosV.z;

	// �Z�o�������W�ɒ����_�����Z�������̂��J�����̈ʒu
	return tmpPosH + mLookPos;
}

void Camera::ShakeUpdate()
{
	if (mShakeTimer > 0)
	{
		--mShakeTimer;
		mPos.x = GetRandomF(-0.5f, 0.5f);
	}

	if (mShakeTimer < 0)
	{
		mShakeTimer = 0;
		mIsShakeMovie = false;
	}
}

// �G�N���X���̏����Z�b�g����
void Camera::SetTargetInfo(const CharacterBase* _enemy)
{
	mTargetEnemy = _enemy;
}

void Camera::SetPlayerInfo(const Player* _player)
{
	mPlayerInfo = _player;
}

// �^�[�Q�b�g�ɃJ������������
void Camera::SetTargetPos( Vector3 _targetPos) 
{
	// �����_����^�[�Q�b�g�̃x�N�g��
	Vector3 moveVector = _targetPos - mLookPos;
	// �ɒ[�ɒl�����������邱�Ƃŏ��X�ɒ����_���ړ�������
	moveVector.Set(moveVector.x / 100.0f, 0.0f, moveVector.z / 100.0f);

	// �J�N�c�L��h�~���邽�߂ɂ�����x�A�����_���߂Â����瑫�����݂��~�߂�
	float distance = moveVector.GetLength();

	mLookPos += moveVector;

	if (distance < 0.01f)
	{
		mLookPos = _targetPos;
		mLookPos.y = 15.0f;
	}
}

void Camera::SetBehindPlayer(const Vector3& _playerPos, const float& _playerRot)
{
	// �v���C���[�̐^���ɂ��悤�ɂȂ��Ă�

	mPos.x = _playerPos.x - 35.0f * sinf(_playerRot + TO_RADIAN(180.0f));
	mPos.y = _playerPos.y + 20.0f;
	mPos.z = _playerPos.z - 35.0f * cosf(_playerRot + TO_RADIAN(180.0f));

}

// ������
void Camera::Init()
{
	Vector3 tmpPlayerPos = mPlayerInfo->GetPos();

	mAngleV	= 0.0f;
	mAngleH	= 0.0f;
	mPos.x	= tmpPlayerPos.x - 25.0f * sinf(TO_RADIAN(0.0f));
	mPos.y	= tmpPlayerPos.y + 20.0f;
	mPos.z	= tmpPlayerPos.z - 25.0f * cosf(TO_RADIAN(0.0f));

	mLookPos.x = tmpPlayerPos.x;
	mLookPos.y = 15.0f;
	mLookPos.z = tmpPlayerPos.z + 50.0f;

	mBasePos.x	   = tmpPlayerPos.x - 25.0f * sinf(TO_RADIAN(0.0f));
	mBasePos.y	   = tmpPlayerPos.y + 20.0f;
	mBasePos.z	   = tmpPlayerPos.z - 25.0f * cosf(TO_RADIAN(0.0f));
	mBaseLookPos   = tmpPlayerPos;
	mBaseLookPos.y = 15.0f;
}

void Camera::MovieInit()
{
	Vector3 tmpPlayerPos = mPlayerInfo->GetPos();
	Vector3 tmpBossPos   = mTargetEnemy->GetPos();

	mAngleV = 0.0f;
	mAngleH = 0.0f;
	mPos.x = tmpPlayerPos.x + 35.0f * sinf(TO_RADIAN(0.0f));
	mPos.y = tmpPlayerPos.y + 25.0f;
	mPos.z = tmpPlayerPos.z + 35.0f * cosf(TO_RADIAN(0.0f));

	mLookPos = tmpBossPos;
	mLookPos.y = 30.0f;

	mShakeTimer    = 0;
	misFinishMovie = false;
	mIsShakeMovie  = false;

}

void Camera::MovieUpdate()
{
	// ���[�r�[�����o���̃A���O������
	if (mBaseLookPos.y < mLookPos.y) mLookPos.y -= 0.13f;
	else
	{
		// �J�����������I������A�Q�[�����̒�ʒu�܂Ō�シ��
		Vector3 moveVector = mBasePos - mPos;
		// ���X�ɐi�ނ悤�ړ��x�N�g��������
		const float DIVISIONVALUE = 100.0f;
		moveVector.Set(moveVector / 100.0f);

		// ���̃x�N�g���̒��������߂āA�ړ��̔���Ɏg�p����
		float length = moveVector.GetLength();

		// �J�����̍��W�ɓ���Ă���
		mPos += moveVector;

		// ���[�r�[�����o�̏I������
		if (length < 0.01f)
		{
			mPos = mBasePos;
			mLookPos = mBaseLookPos;
			SetFinishMovie();
		}
	}

	// �J������h�炷����
	//SetMovieShakeTime();
	if(IsMovieShake())	ShakeUpdate();
}

// �X�V
void Camera::Update(bool _isDebug)
{
	// �v���C���[�̍��W�����Ă���
	Vector3 playerPos = mPlayerInfo->GetPos();

	if (mPlayerInfo->IsTargetLook())
	{
		// ���b�N�I���Ώۂ𒍎����A�v���C���[�̔w��Ɉړ�����
		SetTargetPos(mTargetEnemy->GetPos());
		SetBehindPlayer(mPlayerInfo->GetPos(), mPlayerInfo->GetRot());
	}

	// ���b�N�I����������u�Ԃ̊p�x������
	if (ReleaseKey(KEY_INPUT_K))
	{
		ReleaseLockOn();
	}

	// �񃍃b�N�I�����̑���
	if (!mPlayerInfo->IsTargetLook())
	{
		// �����_�̉�]����
		if (!_isDebug)
		{
			AngleUpdate();
		}

		// �����_�̓v���C���[���班�������ʒu
		mLookPos = playerPos;
		mLookPos.y += 15.0f;

		// �J�����̊p�x������W��ݒ肷��
		mPos = ReflectionAngle();
	}
}

// �`��
void Camera::Draw()
{
	// ���W�ƒ����_���Z�b�g����
	SetCameraPositionAndTarget_UpVecY(mPos.GetVECTOR(),mLookPos.GetVECTOR());
}

// �I��
void Camera::Exit()
{

}
