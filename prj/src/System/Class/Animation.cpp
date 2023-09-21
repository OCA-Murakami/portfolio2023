#include "../Cast.h"
#include <vector>
#include <string>
#include "../WinMain.h"
#include "Vector3.h"
#include "Animation.h"

//Context& Animation::operator=(const Context& _assignment)
//{
//		配列で確保している場合はどうするのがいいか聞く
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
	// ここエラー表記ほしい

	// まず現在のアニメーションをデタッチ
	Detach(Now);
	Detach(Before);

	// ブレンド用にアニメーションの情報を移す
	CopyContext();

	// 各クラスから取得したステート番号を格納
	mContexts[Now].animState = _stateIdx;

	// 新しいアニメーションをアタッチ
	for (int i = 0; i < OrderMax; ++i)
	{
		AttachAnimation(i);
	}

	// アニメーションの総再生時間を取得する (CopyContext関数でアタッチ番号以外をコピーしているため、Nowだけ行う)
	mContexts[Now].totalTime = MV1GetAttachAnimTotalTime(mModelData,mContexts[Now].animAttachIdx);

	// 現再生時間のリセット
	mContexts[Now].nowTime = 0.0f;
	mContexts[Before].nowTime = mContexts[Before].totalTime;

	// Before内のanimStateが-1でなければ、ブレンドのセット
	if (mContexts[Before].animState != -1)
	{
		mBlendRatio = 1.0f;
	}
}

void Animation::AttachAnimation(const int _orderIdx)
{
	// animStateが-1ということは正常に値が取得できていないか、
	// １番初めの処理のため、BeforeのanimStateに値が入っていない時
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

	// 再生時間を進める
	mContexts[Now].nowTime += _playSpeed;

	for (int i = 0; i < OrderMax; ++i)
	{

		// 更新した再生時間をセットする
		MV1SetAttachAnimTime(mModelData, mContexts[i].animAttachIdx, mContexts[i].nowTime);
	}

	// アニメーションのブレンド
	mBlendRatio = std::fmax(0.0f, mBlendRatio - 0.05f);

	// Easeカーブによる補間を行う
	float newRatio = EaseInOutCubic(mBlendRatio);

	// ブレンド比率のセット
	MV1SetAttachAnimBlendRate(mModelData, mContexts[Now].animAttachIdx, 1.0f - newRatio);
	MV1SetAttachAnimBlendRate(mModelData, mContexts[Before].animAttachIdx, newRatio);

	// 補間が完了したら補間元のアニメーションを消す
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
	// それぞれの足元のフレーム番号を取得
	auto leftBaseFrame  = MV1SearchFrame(mModelData, _leftBase.c_str());
	auto rightBaseFrame = MV1SearchFrame(mModelData, _rightBase.c_str());

	// フレーム番号からMATRIXを取得
	auto leftBaseMat  = MV1GetFrameLocalWorldMatrix(mModelData, leftBaseFrame);
	auto rigthBaseMat = MV1GetFrameLocalWorldMatrix(mModelData, rightBaseFrame);

	// MATRIXをfloat3に変換
	float3 leftBasePos  = mul(float4(0, 0, 0, 1), cast(leftBaseMat)).xyz;
	float3 rightBasePos = mul(float4(0, 0, 0, 1), cast(leftBaseMat)).xyz;

	// ２つの値から両足の間の座標を取得
	float3 centerPos = (leftBasePos + rightBasePos) / 2;

	// VECTORを経由してVector3に変換
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

	// フレーム番号からMATRIXを取得
	auto baseMat = MV1GetFrameLocalWorldMatrix(mModelData, baseFrame);

	// MATRIXをfloat3に変換
	float3 basePos = mul(float4(0, 0, 0, 1), cast(baseMat)).xyz;

	// VECTORを経由してVector3に変換
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

