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


// コンストラクタ
Player::Player() :
	mIsReact(false), mIsDeath(false), mAttackCombo(), mIsRoll(), mIsTargetLook(),mIsRepell(),mIsBlowOff(),mIsGetUp(),mIsWalk(),
	mIsInvincible(),mMoveDir(),mAtkReserveS(),mAtkReserveE(),mSwordSize(),mCameraHAngle(),
	mAnim{}
{
}

// デストラクタ
Player::~Player()
{
}

void Player::SetRangeOfMove(const float _rangeRadius)
{
	mRangeOfMove = _rangeRadius;
}

// キー入力による向きの変更
void Player::ChangeRot()
{
	if (mIsTargetLook) return;
	// 前後左右
	if (CheckHitKey(KEY_INPUT_W)) mRot.y = TO_RADIAN(180.0f);
	if (CheckHitKey(KEY_INPUT_A)) mRot.y = TO_RADIAN(90.0f);
	if (CheckHitKey(KEY_INPUT_S)) mRot.y = TO_RADIAN(0.0f);
	if (CheckHitKey(KEY_INPUT_D)) mRot.y = TO_RADIAN(270.0f);
	// 斜め左右前後
	if (CheckHitKey(KEY_INPUT_W) && CheckHitKey(KEY_INPUT_A)) mRot.y = TO_RADIAN(135.0f);
	if (CheckHitKey(KEY_INPUT_W) && CheckHitKey(KEY_INPUT_D)) mRot.y = TO_RADIAN(225.0f);
	if (CheckHitKey(KEY_INPUT_S) && CheckHitKey(KEY_INPUT_A)) mRot.y = TO_RADIAN(45.0f);
	if (CheckHitKey(KEY_INPUT_S) && CheckHitKey(KEY_INPUT_D)) mRot.y = TO_RADIAN(315.0f);
}

void Player::ChangeMoveDir()
{
	// 非ロックオン中は操作しない
	if (!IsTargetLook()) return;

	if (CheckHitKey(KEY_INPUT_W)) mMoveDir = mRot.y;
	if (CheckHitKey(KEY_INPUT_A)) mMoveDir = mRot.y + (TO_RADIAN(270.0f));
	if (CheckHitKey(KEY_INPUT_S)) mMoveDir = mRot.y + (TO_RADIAN(180.0f));
	if (CheckHitKey(KEY_INPUT_D)) mMoveDir = mRot.y + (TO_RADIAN(90.0f));
}

// 敵の方向を計算する
void Player::SetEnemyDir(const Vector3 _enemyPos)
{
	float x = _enemyPos.x - mPos.x;
	float z = _enemyPos.z - mPos.z;
	float dir = atan2f(-x, -z);
	mRot.y = dir;
}

// 被ダメージ関数
void Player::SetDamage(const float _damage)
{
	if (mTargetInfo == nullptr)
		return;


	//mHp -= _damage;
	mHpBar -= (int)mTargetInfo->GetPower();

	// 死亡判定
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

// 移動のキー入力がないかチェックする
bool Player::IsNotMove()
{
	return !CheckHitKey(KEY_INPUT_W) && !CheckHitKey(KEY_INPUT_A) &&
		!CheckHitKey(KEY_INPUT_S) && !CheckHitKey(KEY_INPUT_D);
}

// 他の操作を受け付けたくないフラグをすべてチェックする
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

// 吹き飛び状態かチェック
bool Player::IsBlowOff() const
{
	return mIsBlowOff;
}

// 弾かれ状態かチェック
bool Player::IsRepell() const
{
	return mIsRepell;
}

bool Player::IsReact() const
{
	return mIsReact;
}

// 死亡状態かチェック
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

// 死亡状態へ
void Player::SetDeath()
{
	mIsDeath = true;
}

// ロックオン状態かチェック
bool Player::IsTargetLook() const
{
	return mIsTargetLook;
}

// ロックオン状態へ
void Player::SetTargetLook()
{
	if (mIsTargetLook)return;
	mIsTargetLook = true;
}

// ロックオン状態を解除
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

// 吹き飛び状態にする
void Player::SetBlowOff()
{
	mIsBlowOff = true;
}

void Player::SetRepell()
{
	mIsRepell = true;
}

// 現在の攻撃回数を取得
int Player::GetAtkCombo() const
{
	return mAttackCombo;
}

// 攻撃状態にする
void Player::SetAttack()
{
	mIsAttack = true;
}

// 攻撃状態かチェック
bool Player::IsAttack() const
{
	return mIsAttack;
}

// 無敵状態かチェック
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
	// アニメーションを差し替えた直後にmAttackComboに加算が入るため、
	// Combo4＋１しておかなければ、３段目の攻撃からtrueが返ってくる
	return mAttackCombo == ATTACKTYPE::Combo4 + 1;
}

// 待機アニメーションをセット
void Player::SetStateIdle()
{
	if (mIsReact || mIsDeath) return;

	mAttackCombo = 0;
	ChangeState(Idle);
}

// 歩行アニメーションをセット
void Player::SetStateWalk()
{
	mSpeed = mWALKSPEED;
	ChangeState(Walk);
}

// 走行アニメーションをセット
void Player::SetStateRun()
{
	if (mIsReact || mIsDeath) return;
	mSpeed = mRUNSPEED;
	ChangeState(Run);
}

// 攻撃アニメーションをセット
void Player::SetStateAttack(int _nextAttack)
{
	SetAttack();
	mIsAtkSend = false;


	// コンボ数によって攻撃力を変える
	switch (mAttackCombo)
	{
		// -----------------------
		// 攻撃1段目
		// -----------------------
	case ATTACKTYPE::Combo1:
		// 攻撃判定が発生するタイミング
		mAtkJudgeStart = 5.5f;
		mAtkJudgeEnd = 12.0f;

		// 次の攻撃の入力受付時間
		SetDuringReserve(10.0f, 23.0f);

		// 攻撃力の設定
		SetPower(5.0f);
		break;

		// -----------------------
		// 攻撃2段目
		// -----------------------
	case ATTACKTYPE::Combo2:
		// 攻撃判定が発生するタイミング
		mAtkJudgeStart = 6.0f;
		mAtkJudgeEnd = 10.5f;

		// 次の攻撃の入力受付時間
		SetDuringReserve(19.0f, 28.0f);

		// 攻撃力の設定
		SetPower(3.0f);
		break;

		// -----------------------
		// 攻撃3段目
		// -----------------------
	case ATTACKTYPE::Combo3:
		// 攻撃判定が発生するタイミング
		mAtkJudgeStart = 22.0f;
		mAtkJudgeEnd = 30.0f;

		// 入力受付時間を使えないようにする
		SetDuringReserve(27.0f, 44.0f);

		// 攻撃力の設定
		SetPower(7.0f);
		break;

		// -----------------------
		// 攻撃4段目
		// -----------------------
	case ATTACKTYPE::Combo4:
		// 攻撃判定が発生するタイミング
		mAtkJudgeStart = 43.0f;
		mAtkJudgeEnd = 57.5f;

		// 入力受付時間を使えないようにする
		InitDuringReserve();

		// 攻撃力の設定
		SetPower(10.0f);
		break;
	}

	if (mAttackCombo < mCONTATKMAX) ++mAttackCombo;

	ChangeState(_nextAttack);
}

// 弾かれアニメーションをセット
void Player::SetStateRepell()
{
	// コンボ回数をリセットしておく
	mAttackCombo = 0;

	mIsRepell = true;
	ChangeState(Repell);
}

// ダメージアニメーションをセット
void Player::SetStateReact()
{
	if (IsAttack()) ReleaseAttackInfo();
	if (IsRoll())	ReleaseRoll();
	
	mIsReact = true;
	ChangeState(React);
}

// 吹き飛びアニメーションをセット
void Player::SetStateBlowOff()
{
	if (IsAttack()) ReleaseAttackInfo();
	if (IsRoll())	ReleaseRoll();

	SetBlowOff();
	ChangeState(BlowOff);
}

// 前転アニメーションをセット
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

// 死亡アニメーションをセット
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
	//この関数に入ったとき、プレイヤーが攻撃中かダメージ中かで挿すエフェクトハンドル変えるよう直す
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

	// 渡されたエフェクトのハンドルに合った座標をセットする
	if (_effectHandle == GetSlashEffectHandle())
	{
		tmpPos.Set(mTargetInfo->GetPos());
		tmpPos.y = 15.0f;
	}
	if (_effectHandle == GetBloodEffectHandle()) tmpPos.Set(mPos.x,mPos.y + 10.0f,mPos.z);

	return tmpPos;
}

// 初期化
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

	// アニメーションの読み込み
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

	// 最初は待機状態をアタッチしておく
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

	// 最初は待機状態をアタッチしておく
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

// 更新
void Player::Update(float _delta)
{
	if (PushHitKey(KEY_INPUT_Y)) SetDamage(1080);
	// 毎フレーム、移動のフラグとベクトルを初期化
	mIsWalk = false;
	mMoveVector.Clear();

	// ---------------------------------------------------------
	// アニメーション再生処理
	// ---------------------------------------------------------
	// 再生速度の調整
	mAnimPlaySpeed *= _delta;

	// アニメーションの再生
	mAnimationPlayer.Play(mAnimPlaySpeed, mIsAnimLoop);
	
	// ---------------------------------------------------------
	// ステージ上の移動できる範囲の制限
	// ---------------------------------------------------------
	// ステージの中心から今の座標までの長さ
	float centerToPosLength = mPos.GetLength();
	// ステージの中心から座標の長さがステージの範囲より長ければ
	// ステージの範囲内の長さに戻す
	if(centerToPosLength > mRangeOfMove) mPos.SetLength(mRangeOfMove);

	// ---------------------------------------------------------
	// キー入力の処理
	// ---------------------------------------------------------
	// 2段目以降のコンボ攻撃の処理
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

	// 他のキー入力を受け付けている状態であれば
	if (!IsStopKeyInput())
	{
		// 前転
		if (PushHitKey(KEY_INPUT_SPACE))	SetStateRoll();
		// 攻撃
		// コンボ数の判定の都合で2，3段目の下に書いておく
		if (PushMouseInput(MOUSE_INPUT_LEFT))	SetStateAttack(Attack1);
	}

	// ロックオン
	if (CheckHitKey(KEY_INPUT_K))
	{
		// 敵がいる方向を取得
		SetEnemyDir(mTargetInfo->GetPos());
		// 敵のほうを向く
		SetTargetLook();
	}
	else
	{
		// ロックオン状態を解除
		ReleaseTargetLook();
	}

	// ---------------------------------------------------------
	// 行動パターン別の処理
	// ---------------------------------------------------------
	switch (mState)
	{
		// ---------------------------------------------
		// 待機
		// ---------------------------------------------
	case Idle:
		// 移動へ遷移
		if (CheckHitKey(KEY_INPUT_W) || CheckHitKey(KEY_INPUT_A) || CheckHitKey(KEY_INPUT_S) || CheckHitKey(KEY_INPUT_D))
		{
			ChangeState(Run);
		}

		// 徐々に減速して止まる
		mSpeed *= 0.9f;
		if (mSpeed < 0.001f) mSpeed = 0.0f;

		mAnimPlaySpeed = 0.3f;
		mIsAnimLoop = true;

		break;

		// ---------------------------------------------
		// 歩行
		// ---------------------------------------------
	case Walk:
		mAnimPlaySpeed = 0.3f;
		mIsAnimLoop = true;

		// 向きの変更
		//ChangeRot();
		// 移動方向の変更（ロックオン中のみ）
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

			// カメラの水平角度を考慮した方向に移動する
			if (mIsWalk)
			{
				// 計算して座標に渡すように
				Vector3 tmpMoveVector;

				// カメラの角度に併せて移動ベクトルを回転してから計算
				float sinParam = sinf(TO_RADIAN(mCameraHAngle));
				float cosParam = cosf(TO_RADIAN(mCameraHAngle));
				tmpMoveVector.x = mMoveVector.x * cosParam - mMoveVector.z * sinParam;
				tmpMoveVector.y = 0.0f;
				tmpMoveVector.z = mMoveVector.x * sinParam + mMoveVector.z * cosParam;

				// 正規化して速度を一定に
				tmpMoveVector.Normalize();

				// 計算できた移動ベクトルを座標に足しこむ
				mPos += tmpMoveVector;
			}
		}
		// ロックオン中は敵を軸に座標移動
		if (IsTargetLook())
		{
			mPos.x -= mWALKSPEED * sinf(mMoveDir);
			mPos.z -= mWALKSPEED * cosf(mMoveDir);
		}

		// 座標移動
		// ロックオン中は走らない
		if (CheckHitKey(KEY_INPUT_LSHIFT) && !IsTargetLook()) ChangeState(Run);
		// キー入力がない場合は待機状態へ
		if (IsNotMove()) ChangeState(Idle);
		
		break;

		// ---------------------------------------------
		// 走行
		// ---------------------------------------------
	case Run:
		mAnimPlaySpeed = 0.3f;
		mIsAnimLoop = true;

		// 向きの変更
		ChangeRot();

		// 座標移動
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

		// カメラの水平角度を考慮した方向に移動する
		if (mIsWalk)
		{
			// 徐々に速度をあげるための倍率変数の操作
			if (mSpeedScale < 1.8f)
			{
				mSpeedScale += 0.015f;
			}

			// 計算して座標に渡すように
			Vector3 tmpMoveVector;

			// カメラの角度に併せて移動ベクトルを回転してから計算
			float sinParam = sinf(TO_RADIAN(mCameraHAngle));
			float cosParam = cosf(TO_RADIAN(mCameraHAngle));
			tmpMoveVector.x = mMoveVector.x * cosParam - mMoveVector.z * sinParam;
			tmpMoveVector.y = 0.0f;
			tmpMoveVector.z = mMoveVector.x * sinParam + mMoveVector.z * cosParam;

			// 正規化して速度を一定に
			tmpMoveVector.Normalize();

			tmpMoveVector *= mSpeedScale;

			// 計算できた移動ベクトルを座標に足しこむ
			mPos += tmpMoveVector;
		}

		// 待機、歩行への遷移
		if (ReleaseKey(KEY_INPUT_LSHIFT)) ChangeState(Idle);
		// キー入力がない場合は待機状態へ
		if (IsNotMove())
		{
			mSpeedScale = 1.0f;
			ChangeState(Idle);
		}
		break;

		// ---------------------------------------------
		// 前転
		// ---------------------------------------------
	case Roll:
		mIsAnimLoop = false;
		// 前転開始時と立ち上がる時でスピードを変える
		if (mAnimationPlayer.GetAnimNowTime() < 20.0f)
		{
			mSpeed = 0.8f;
			mAnimPlaySpeed = 1.0f;

			mPos.x -= mSpeed * sinf(mRot.y);
			mPos.z -= mSpeed * cosf(mRot.y);
		}
		else mAnimPlaySpeed = 0.5f;


		// 無敵時間の設定
		// 
		// 
		// 
		// ここ関数にしたらelse消せる
		if (mAnimationPlayer.GetAnimNowTime() > 5.0f && mAnimationPlayer.GetAnimNowTime() < 30.0f)	mIsInvincible = true;
		else mIsInvincible = false;

		// 再生が終了したら待機状態へ
		if (!mAnimationPlayer.IsPlay())
		{
			ReleaseRoll();
			SetStateIdle();
		}
		break;

		// ---------------------------------------------
		// 起き上がり
		// ---------------------------------------------
	case GetUp:

		// アニメーションが終了したら待機状態へ
		if (!mAnimationPlayer.IsPlay())
		{
			ReleaseGetUp();
			SetStateIdle();
		}
		break;

		// ---------------------------------------------
		// 攻撃１段目
		// ---------------------------------------------
	case Attack1:
		mAnimPlaySpeed = 0.6f;
		mIsAnimLoop = false;

		// アニメーションが終了したら待機状態へ
		if (!mAnimationPlayer.IsPlay()) 
		{
			ReleaseAttack();
			SetStateIdle();
		}

		break;

		// ---------------------------------------------
		// 攻撃２段目
		// ---------------------------------------------
	case Attack2:
		mAnimPlaySpeed = 0.5f;
		mIsAnimLoop = false;

		// アニメーションが終了したら待機状態へ
		if (!mAnimationPlayer.IsPlay())
		{
			ReleaseAttack();
			SetStateIdle();
		}
		
		break;

		// ---------------------------------------------
		// 攻撃３段目
		// ---------------------------------------------
	case Attack3:
		// アニメーションの状況によって再生速度を変える
		if (mAnimationPlayer.GetAnimNowTime() < 56.5f) mAnimPlaySpeed = 0.9f;
		else mAnimPlaySpeed = 0.5f;

		mIsAnimLoop = false;

		// アニメーションが終了したら待機状態へ
		if (!mAnimationPlayer.IsPlay())
		{
			ReleaseAttack();
			SetStateIdle();
		}

		break;

		// ---------------------------------------------
		// 攻撃4段目
		// ---------------------------------------------
	case Attack4:
		// アニメーションの状況によって再生速度を変える
		if (mAnimationPlayer.GetAnimNowTime() < 56.5f) mAnimPlaySpeed = 0.9f;
		else mAnimPlaySpeed = 0.5f;
		mIsAnimLoop = false;

		// アニメーションが終了したら待機状態へ
		if (!mAnimationPlayer.IsPlay())
		{
			ReleaseAttack();
			SetStateIdle();
		}

		break;

		// ---------------------------------------------
		// 怯み
		// ---------------------------------------------
	case React:
		mAnimPlaySpeed = 0.3f;
		mIsAnimLoop = false;

		// アニメーションが終了したら待機状態へ
		if (!mAnimationPlayer.IsPlay())
		{
			ReleaseReact();
			SetStateIdle();
		}

		break;

		// ---------------------------------------------
		// 弾かれ
		// ---------------------------------------------
	case Repell:
		mAnimPlaySpeed = 0.2f;
		mIsAnimLoop = false;

		// アニメーションが終了したら待機状態へ
		if (!mAnimationPlayer.IsPlay())
		{
			ReleaseRepell();
			SetStateIdle();
		}

		break;

		// ---------------------------------------------
		// 吹き飛び
		// ---------------------------------------------
	case BlowOff:
		mAnimPlaySpeed = 0.5f;
		mIsAnimLoop = false;

		// モデルが浮いている間は後ろ方向へ座標を移動させる
		if (mAnimationPlayer.GetAnimNowTime() < 22.0f)
		{
			mPos.x += 1.0f * sinf(mRot.y);
			mPos.z += 1.0f * cosf(mRot.y);
		}

		// アニメーションが終了したら起き上がり状態へ
		if (!mAnimationPlayer.IsPlay())
		{
			ReleaseBlowOff();
			SetStateGetUp();
		}
		break;
	}

	// エフェクトの処理
	// エフェクト関連
	// エフェクトの再生フラグがセットされていたらリリースする
	if (mEffectPlayer.CheckPlayEnd()) mEffectPlayer.ReleasePlay();
}

// 描画
void Player::PostDraw()
{
	// HPバーの描画
	DrawLineBox(4, 2, 306, 31, GetColor(255, 255, 255));
	DrawBox(5, 3, mHpBar, 30, GetColor(0, 255, 0), TRUE);
}
