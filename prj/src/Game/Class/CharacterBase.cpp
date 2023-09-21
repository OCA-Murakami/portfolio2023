#include "../../System/WinMain.h"
#include "../../System/Class/Vector3.h"
#include "CharacterBase.h"


// コンストラクタ
CharacterBase::CharacterBase():
	mState(-1),mHp(),mSpeed(-1.0f),
	mHpBar(-1),mBaseMatDif{-1.0f,-1.0f,-1.0f},
	mAtkPower(-1),mIsAtkSend(false),mAtkJudgeStart(-1.0f),mAtkJudgeEnd(-1.0f),mIsAttack(false), mCollSize(-1.0f),
	mAnimPlaySpeed(-1.0f),mIsAnimLoop(true)
{
}

// デストラクタ
CharacterBase::~CharacterBase()
{
}

// Vector3型の値で一括セット
void CharacterBase::SetPos(const Vector3 _pos)
{
	mPos.Set(_pos);
}

// x,y,zそれぞれに値をセット
void CharacterBase::SetPos(const float _x, const float _y, const float _z)
{
	mPos.Set(_x, _y, _z);
}

// ターゲット相手の情報をポインタで取得する
void CharacterBase::SetTargetInfo(const CharacterBase* _target)
{
	mTargetInfo = _target;
}

// 攻撃判定の重複防止フラグをセット
void CharacterBase::SetAtkSend()
{
	mIsAtkSend = true;
}

// ターゲット相手の情報をポインタで受け取る
bool CharacterBase::IsInTheStage(float _stageLength) const
{
	// ステージの中心（0,0,0）から座標までの長さを求める
	float stageToPosLength = mPos.GetLength();

	if (_stageLength > stageToPosLength) return true;

	return false;
}

void CharacterBase::ChangeState(const int _nextState)
{
	mState = _nextState;
	mAnimationPlayer.Attach(_nextState);
}

// 座標を取得する
Vector3 CharacterBase::GetPos() const
{
	return mPos;
}

// VECTOR型に変換して座標を取得する
VECTOR CharacterBase::GetVECTORPos() const
{
	return 	mPos.GetVECTOR();
}

// 向きを取得する
float CharacterBase::GetRot() const
{
	return mRot.y;
}

// モデルデータを取得する
int CharacterBase::GetModel()
{
	return mModelData.GetModel();
}

// 攻撃力を取得する
float CharacterBase::GetPower() const
{
	return mAtkPower;
}

// 攻撃される時に使用するコリジョンのサイズを取得する
float CharacterBase::GetCollSize() const
{
	return mCollSize;
}

// 攻撃力をセットする
void CharacterBase::SetPower(const float _power)
{
	mAtkPower = _power;
}

// 攻撃中かチェックする
bool CharacterBase::IsAttack() const
{
	return mIsAttack;
}

// モデルのディフューズカラーを赤色に変更する
void CharacterBase::SetMatDifDamage()
{
	MV1SetMaterialAmbColor(GetModel(), 0, GetColorF(255.0f, 0.0f, 0.0f, 1.0f));
}

// モデルの元々のディフューズカラーをセットする
void CharacterBase::SetMatDifBase()
{
	MV1SetMaterialDifColor(GetModel(), 0, mBaseMatDif);
}

// 攻撃状態をリリースする
void CharacterBase::ReleaseAttack()
{
	mIsAttack	   = false;
	mAtkJudgeStart = -1.0f;
	mAtkJudgeEnd   = -1.0f;
}

// 攻撃判定の重複防止フラグのチェック
bool CharacterBase::IsAtkSend() const
{
	return mIsAtkSend;
}

//アニメーションのフレーム数から攻撃判定を発生させるフラグをチェック
bool CharacterBase::IsAtkJudge() const
{
	return mAnimationPlayer.GetAnimNowTime() > mAtkJudgeStart && mAnimationPlayer.GetAnimNowTime() < mAtkJudgeEnd;
}

// 初期化関数
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

// 描画関数
void CharacterBase::Draw()
{
	// モデルの描画
	MV1SetPosition(GetModel(), mPos.GetVECTOR());
	MV1SetRotationXYZ(GetModel(), mRot.GetVECTOR());
	MV1SetScale(GetModel(), mSize.GetVECTOR());
	mModelData.Draw();
}

// 終了関数
void CharacterBase::Exit()
{
	mModelData.Exit();
	mAnimationPlayer.Exit();
}

// 最前面に描画したいものをまとめる
void CharacterBase::PostDraw()
{
}
