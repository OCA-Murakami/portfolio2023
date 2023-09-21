#include "../Cast.h"
#include <vector>
#include <string>
#include "../WinMain.h"
#include "Vector3.h"
#include "Animation.h"

//Context& Animation::operator=(const Context& _assignment)
//{
//		�z��Ŋm�ۂ��Ă���ꍇ�͂ǂ�����̂�����������
//}

void Animation::Loop()
{
	if (IsPlay()) return;
	mContexts[Now].nowTime -= GetAnimTotalTime();
}

Animation::Animation():
	mBeforeAnimState(-1),mModelData(-1),mBlendRatio(-1.0f)
{
}

Animation::~Animation()
{
}

int Animation::SetModelData(const int& _modelData)
{
	return mModelData = _modelData;
}

void Animation::Load(std::string _filePath)
{
	mAnimPool.emplace_back(MV1LoadModel(_filePath.c_str()));
}

void Animation::Attach(const int _stateIdx)
{
	// �����G���[�\�L�ق���

	// �܂����݂̃A�j���[�V�������f�^�b�`
	Detach(Now);
	Detach(Before);

	// �u�����h�p�ɃA�j���[�V�����̏����ڂ�
	CopyContext();

	// �e�N���X����擾�����X�e�[�g�ԍ����i�[
	mContexts[Now].animState = _stateIdx;

	// �V�����A�j���[�V�������A�^�b�`
	for (int i = 0; i < OrderMax; ++i)
	{
		AttachAnimation(i);
	}

	// �A�j���[�V�����̑��Đ����Ԃ��擾���� (CopyContext�֐��ŃA�^�b�`�ԍ��ȊO���R�s�[���Ă��邽�߁ANow�����s��)
	mContexts[Now].totalTime = MV1GetAttachAnimTotalTime(mModelData,mContexts[Now].animAttachIdx);

	// ���Đ����Ԃ̃��Z�b�g
	mContexts[Now].nowTime = 0.0f;
	mContexts[Before].nowTime = mContexts[Before].totalTime;

	// Before����animState��-1�łȂ���΁A�u�����h�̃Z�b�g
	if (mContexts[Before].animState != -1)
	{
		mBlendRatio = 1.0f;
	}
}

void Animation::AttachAnimation(const int _orderIdx)
{
	// animState��-1�Ƃ������Ƃ͐���ɒl���擾�ł��Ă��Ȃ����A
	// �P�ԏ��߂̏����̂��߁ABefore��animState�ɒl�������Ă��Ȃ���
	if (mContexts[_orderIdx].animState == -1)
		return;

	const int searchHandle = mAnimPool[mContexts[_orderIdx].animState];

	mContexts[_orderIdx].animAttachIdx = MV1AttachAnim(mModelData, mANIMIDX, searchHandle, true);
}

void Animation::Detach( const int _orderIdx)
{
	MV1DetachAnim(mModelData, mContexts[_orderIdx].animAttachIdx);
	mContexts[_orderIdx].animAttachIdx = -1;
}

void Animation::Play(float _playSpeed, bool _isLoop)
{
	if (_isLoop)
	{
		Loop();
	}

	// �Đ����Ԃ�i�߂�
	mContexts[Now].nowTime += _playSpeed;

	for (int i = 0; i < OrderMax; ++i)
	{

		// �X�V�����Đ����Ԃ��Z�b�g����
		MV1SetAttachAnimTime(mModelData, mContexts[i].animAttachIdx, mContexts[i].nowTime);
	}

	// �A�j���[�V�����̃u�����h
	mBlendRatio = std::fmax(0.0f, mBlendRatio - 0.05f);

	// Ease�J�[�u�ɂ���Ԃ��s��
	float newRatio = EaseInOutCubic(mBlendRatio);

	// �u�����h�䗦�̃Z�b�g
	MV1SetAttachAnimBlendRate(mModelData, mContexts[Now].animAttachIdx, 1.0f - newRatio);
	MV1SetAttachAnimBlendRate(mModelData, mContexts[Before].animAttachIdx, newRatio);

	// ��Ԃ������������Ԍ��̃A�j���[�V����������
	if (mBlendRatio == 0.0f)
	{
		Detach(Before);
		mContexts[Before].animState = -1;
	}
}

bool Animation::IsPlay() const
{
	return mContexts[Now].nowTime < mContexts[Now].totalTime;
}

void Animation::SetAnimTime(const float _setTime)
{
	mContexts[Now].nowTime = _setTime;
}

float Animation::GetAnimTotalTime() const
{
	return mContexts[Now].totalTime;
}

float Animation::GetAnimNowTime() const
{
	return mContexts[Now].nowTime;
}

Vector3 Animation::FixedOffset(std::string _leftBase, std::string _rightBase)
{
	// ���ꂼ��̑����̃t���[���ԍ����擾
	auto leftBaseFrame  = MV1SearchFrame(mModelData, _leftBase.c_str());
	auto rightBaseFrame = MV1SearchFrame(mModelData, _rightBase.c_str());

	// �t���[���ԍ�����MATRIX���擾
	auto leftBaseMat  = MV1GetFrameLocalWorldMatrix(mModelData, leftBaseFrame);
	auto rigthBaseMat = MV1GetFrameLocalWorldMatrix(mModelData, rightBaseFrame);

	// MATRIX��float3�ɕϊ�
	float3 leftBasePos  = mul(float4(0, 0, 0, 1), cast(leftBaseMat)).xyz;
	float3 rightBasePos = mul(float4(0, 0, 0, 1), cast(leftBaseMat)).xyz;

	// �Q�̒l���痼���̊Ԃ̍��W���擾
	float3 centerPos = (leftBasePos + rightBasePos) / 2;

	// VECTOR���o�R����Vector3�ɕϊ�
	VECTOR tmpVECTOR = cast(centerPos);
	Vector3 returnPos;
	returnPos.SetVECTOR(tmpVECTOR);

	return returnPos;
}

size_t Animation::GetAnimPoolSize() const
{
	return mAnimPool.size();
}

int Animation::GetAttachIdx() const
{
	return mContexts[Now].animAttachIdx;
}

float Animation::EaseInOutCubic(float _blendRatio)
{
	return _blendRatio < 0.5f ? 2.0f * _blendRatio * _blendRatio : _blendRatio * (4.0f - 2.0f * _blendRatio) - 1.0f;
}

void Animation::Exit()
{
	for (size_t i = 0; i < mAnimPool.size(); ++i)
	{
		MV1DeleteModel(mAnimPool[i]);
	}
}

Vector3 Animation::GetBasePos(std::string _baseFrameName)
{
	auto baseFrame = MV1SearchFrame(mModelData, _baseFrameName.c_str());

	// �t���[���ԍ�����MATRIX���擾
	auto baseMat = MV1GetFrameLocalWorldMatrix(mModelData, baseFrame);

	// MATRIX��float3�ɕϊ�
	float3 basePos = mul(float4(0, 0, 0, 1), cast(baseMat)).xyz;

	// VECTOR���o�R����Vector3�ɕϊ�
	VECTOR tmpVECTOR = cast(basePos);
	Vector3 returnPos;
	returnPos.SetVECTOR(tmpVECTOR);

	return returnPos;
}

void Animation::CopyContext()
{
	mContexts[Before].animAttachIdx = mContexts[Now].animAttachIdx;
	mContexts[Before].animState		= mContexts[Now].animState;
	mContexts[Before].nowTime		= mContexts[Now].nowTime;
	mContexts[Before].totalTime		= mContexts[Now].totalTime;
}

void Animation::InitContexts()
{
	for (int i = 0; i < OrderMax; ++i)
	{
		Detach(i);
		mContexts[i].animState	   = -1;
		mContexts[i].nowTime	   = 0.0f;
		mContexts[i].totalTime	   = 0.0f;
	}
}

