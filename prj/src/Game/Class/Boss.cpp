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

//プレイヤーと接していたら動作する攻撃選定関数が呼ばれ続けてるからそれの修正

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

// コンストラクタ
Boss::Boss() : mAnim{-1},mMoveEndPos{0.0f,0.0f,0.0f}, mDownValue(-1), mIsRightDirection(false), mReactValue(-1), mDownTimer(0),mOutOfSightTimer(0),
			   mIsFinishMovie(false),mIsGroundMovie(false),
			   mIsLightAttack(false), mIsHeavyAttack(false), mIsDown(false), mIsDeath(false), mIsIdle(false), mIsGuard(false), mIsJump(false),mIsComboAttack(false),
			   mIsBlowOff(false),mIsReact(false),
			   mThresholdGurad(-1),mstoredDamage(-1),
			   mEffectWaveHandle(-1),mEffectDangerHandle(-1),mEffectRoaringHandle(-1)
{
}

// デストラクタ
Boss::~Boss()
{
}

// プレイヤーとの座標から角度を計算する
void Boss::GetPlayerPos(Vector3 _playerPos) 
{
	mMoveEndPos = _playerPos;
}

// プレイヤーの方向を計算する
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

// ダメージを受け取る
void Boss::SetDamage(const float _damage)
{
	if ((mHpBar) <= 100) return;

	float damegeRatio = _damage / mHp;

	mHp -= _damage;
	mHpBar -= (int)(mHpBar * damegeRatio);

	// 蓄ダメージによる怯み処理
	CountReactValue(_damage);
	
	// ダウン判定
	if (mDownValue >= mDOWNMAX)
	{
		mDownValue = 0;
		ReleaseReact();
		ReleaseAllAttack();
		SetStateDown();
	}

	// 死亡判定
	if ((mHpBar) <= 100 && !mIsDeath)
	{
		mHpBar = 100;

		// 攻撃中にこの関数に入ると攻撃フラグがセットされたままになるので
		// ここでチェックしてリリースする
		if (IsAttack()) ReleaseAttack();
		if (IsJump()) ReleaseJump();

		mHp = 0;
		//mIsDeath = true;
		ChangeState(Death);
	}
}

// 怯み値の増加
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
	// 残りHPの割合から次の閾値を決定する
	// １０割～７割
	if		(mHp > (mHPMAX * 0.7f))	mThresholdGurad = GetRand(8) + 4;
	// ７割～４割
	else if (mHp > (mHPMAX * 0.4f)) mThresholdGurad = GetRand(5) + 3;
	// ４割以下
	else							mThresholdGurad = GetRand(3) + 2;
}

float Boss::GetCollSize() const
{
	return mStatus.collSize;
}

bool Boss::IsInvincible() const
{
	// 吹き飛び中、地面につくまでは無敵状態にする
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

// 攻撃状態へ
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

// ガード状態へ
void Boss::SetGuard()
{
	if (IsGuard())  return;
	mIsGuard = true;
}

// カウンター攻撃
void Boss::SetGuardToAttack()
{
	mIsGuard = false;
	// プレイヤーが視野内にいるかどうかで繰り出す攻撃を変える
	if (IsSeePlayer()) SetStateLightAttack();
	else			   SetStateJump();
}

// ジャンプ状態へ
void Boss::SetJump()
{
	mIsAtkSend = false;
	mIsJump = true;
}

void Boss::SetBlowOff()
{
	mIsBlowOff = true;
}

// 怯み状態へ
void Boss::SetReact()
{
	mIsReact = true;
}

void Boss::SetDown()
{
	mIsDown = true;
}

// 死亡状態へ
void Boss::SetDeath()
{
	mIsDeath = true;
}

// ジャンプ状態のリリース
void Boss::ReleaseJump()
{
	mIsJump = false;
}

// 弱攻撃状態のリリース
void Boss::ReleaseLightAttack()
{
	mIsLightAttack = false;
	ReleaseAttack();
}

// 強攻撃状態のリリース
void Boss::ReleaseHeavyAttack()
{
	mIsHeavyAttack = false;
	ReleaseAttack();
}

// コンボ攻撃状態のリリース
void Boss::ReleaseComboAttack()
{
	mIsComboAttack = false;
	ReleaseAttack();
}

// 怯み状態のリリース
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

// 全ての攻撃フラグをチェックしてそれぞれリリースする
void Boss::ReleaseAllAttack()
{
	if (IsLightAttack()) mIsLightAttack = false;
	if (IsHeavyAttack()) mIsHeavyAttack = false;
	if (IsComboAttack()) mIsComboAttack = false;
	if (IsJump())		 mIsJump		= false;
	if (IsAttack())		 mIsAttack		= false;
}


// 移動関数
void Boss::Move(float _moveSpeed)
{
	mPos.x -= _moveSpeed * sinf(mRot.y);
	mPos.z -= _moveSpeed * cosf(mRot.y);
}

// プレイヤーが視野内にいるか判定
bool Boss::IsSeePlayer()
{
	// 自分からプレイヤーへのベクトルと長さを計算
	Vector3 vecBtoP    = mTargetInfo->GetPos() - GetPos();

	// 正面方向のベクトルを設定
	float directionRad = GetRot();
	Vector3 vecFront;
	vecFront.Set( -1.0f * sinf(directionRad), 
				  0.0f,
				  -1.0f * cosf(directionRad));

	// 自分からプレイヤーへのベクトルを正規化
	vecBtoP.Normalize();

	// 正面ベクトルと正規化したベクトルで内積を計算する
	float dot = GetVector3Dot(vecBtoP, vecFront);

	// 視野の範囲をcosにする
	float fanCos = cosf(TO_RADIAN(mVIEWANGLE) / 2.0f);

	// この範囲にプレイヤーがいるかどうかチェック
	if (fanCos > dot) return false;

	return true;
}

// プレイヤーが左右どちら側にいるか計算
bool Boss::IsRightDirection()
{
	// 自分の正面方向のベクトルを用意して
	Vector3 vecFront;
	vecFront.Set(-1.0f * sinf(GetRot()),
		0.0f,
		-1.0f * cosf(GetRot()));
	// 自分からプレイヤーへのベクトルとの外積を計算する
	Vector3 cross = GetVector3Cross((mTargetInfo->GetPos() - mPos), vecFront);

	// 外積が正の数なら左側、負の数なら右側にいる
	if (cross.y > 0.0f)
	{
		// 左回りに旋回しながら歩行する
		return false;
	}
	else
	{
		// 右回りに旋回しながら歩行する
		return true;
	}
}

void Boss::MoveDirection(const bool _isRightDir)
{
	// 右回り
	if (_isRightDir)
	{
		mRot.y += 0.03f;
	}
	// 左回り
	else
	{
		mRot.y -= 0.03f;
	}
}

// 繰り出す攻撃を残りHPを基準にランダムで選ぶ
void Boss::ChoiseAttackType()
{
	// 怯んでいたり、ガード中はこの処理を抜ける
	if (IsLimitActions())	
		return;

	SetAttack();

	// 数値をランダムで１つ取得する
	const int VALUEMAX = 9;
	int randValue = GetRand(VALUEMAX);

	// 残りのHPによって判定の区分が変わる
	// １０割～７割
	if (mHp > (mHPMAX * 0.7f))
	{
		// 弱攻撃:強攻撃		6:4
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
	// ７割～４割
	else if (mHp > (mHPMAX * 0.4f))
	{
		// 弱攻撃：強攻撃：コンボ攻撃		4:4:2
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
	// ４割以下
	else
	{
		// 弱攻撃：強攻撃：コンボ攻撃		2:4:4
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


// 歩行状態へ
void Boss::SetStateWalk()
{
	// 重複セット防止
	if (IsWalk()) return;

	// フラグのリリース忘れを防止するために一斉解除
	ReleaseAllFlag();

	// 旋回の方向をセットしてアニメーションの切り替え
	mIsRightDirection = IsRightDirection();
	ChangeState(Walk);
}

// 走行状態へ
void Boss::SetStateRun()
{
	ChangeState(Run);
}

// 弱攻撃状態へ
void Boss::SetStateLightAttack()
{
	if (IsJump()) return;

	// 攻撃判定の発生と終了のタイミング
	mAtkJudgeStart = 2.0f;
	mAtkJudgeEnd = 10.5f;
	// この攻撃の威力を設定
	SetPower(mStatus.attackPower[BOSSATTACKTYPE::Light]);

	// 攻撃の種類を判別するためにフラグを２つ立てる
	SetLightAttack();
	// ステートを変更
	ChangeState(LightAttack);
}

// 強攻撃状態へ
void Boss::SetStateHeavyAttack()
{
	if (IsJump()) return;

	// 攻撃判定の開始と終了のフレームを設定
	mAtkJudgeStart = 25.0f;
	mAtkJudgeEnd = 38.0f;
	// この攻撃の威力を設定
	SetPower(mStatus.attackPower[BOSSATTACKTYPE::Heavy]);

	// 当たり判定の処理で攻撃の種類を判別するためにもう１つフラグを立てておく
	SetHeavyAttack();
	ChangeState(HeavyAttack);
}

// コンボ攻撃状態へ
void Boss::SetStateComboAttack()
{
	mEffectPlayer.SetPlay(mEffectDangerHandle,mPos);

	// 攻撃判定の開始と終了のフレームを設定
	mAtkJudgeStart = 50.0f;
	mAtkJudgeEnd   = 98.0f;
	// この攻撃の威力を設定
	SetPower(mStatus.attackPower[BOSSATTACKTYPE::Combo]);

	// 攻撃の種類を判定するために２つフラグを立てる
	SetComboAttack();
	ChangeState(ComboAttack);
}

// 攻撃状態へ
void Boss::SetStateGuard()
{
	// 攻撃行動中にガードしないためにフラグを立てる
	if (IsAttack() || IsJump()) return;
	SetGuard();
	ChangeState(Guard);
}

// ジャンプ状態へ
void Boss::SetStateJump()
{
	if (IsJump() && IsAttack()) return;

	// 当たり判定を発生させるアニメーションの時間を設定する
	mAtkJudgeStart = 62.0f;
	mAtkJudgeEnd = 72.0f;
	// この攻撃の威力を設定
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

// 怯み状態へ
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

	// 攻撃判定の発生フレームの設定
	mAtkJudgeStart = 20.0f;
	mAtkJudgeEnd = 40.0f;

	// 他の攻撃と発生条件が違うため、ChoiseAttackType関数を使用しない
	SetAttack();
	SetPower(10.0f);
	ChangeState(TurnAttack);
}

// 待機状態かチェック
bool Boss::IsIdle() const
{
	return mState == Idle;
}

// 歩行状態かチェック
bool Boss::IsWalk() const
{
	return mState == Walk;
}

// ガード状態かチェック
bool Boss::IsGuard() const
{
	return mIsGuard;
}

// ジャンプ状態かチェック
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

// 爪のカプセルのサイズ
float Boss::GetClawSize() const
{
	return mCollInfo.clawSize;
}

// 拳のカプセルのサイズ
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

// ジャンプ時の衝撃波のエフェクトを取得
int Boss::GetWaveEffectHandle() const
{
	return mEffectWaveHandle;
}

// コンボ攻撃時の予告エフェクトを取得
int Boss::GetDangerEffectHandle() const
{
	return mEffectDangerHandle;
}

// 咆哮時のエフェクトを取得
int Boss::GetRoaringEffectHandle() const
{
	return mEffectRoaringHandle;
}

// エフェクトプレイヤーにハンドルと座標をセットする
void Boss::SetEffect(const int _effectHandle)
{
	Vector3 effectPos = GetEffectPos(_effectHandle);
	mEffectPlayer.SetPlay(_effectHandle, effectPos);
}

// 再生するエフェクトの座標をセットする（再生するエフェクトによって座標が変わる）
Vector3 Boss::GetEffectPos(const int _effectHandle) const
{
	Vector3 tmpPos = GetPos();

	// 咆哮エフェクトはY座標を少し上に
	if (_effectHandle == GetRoaringEffectHandle()) tmpPos.y = 18.0f;

	return tmpPos;
}

// 初期化
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
	mHpBar = 1180;		// ＋１００はHPバーの左上のX座標

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

	// アニメーションをロード
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
	mEffectRoaringHandle = LoadEffekseerEffect("data/effect/boss/Roaring - コピー.efkefc", 15.0f);
}

void Boss::ReInit()
{
	mPos.Set(0.0f, 50.0f, 80.0f);
	mRot.Clear();

	LoadStatus("data/csv/BossStatus.csv", mStatus);
	mHp = mStatus.hpMax;
	mHpBar = 1180;		// ＋１００はHPバーの左上のX座標

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
	// アニメーションの再生
	mAnimationPlayer.Play(mAnimPlaySpeed,mIsAnimLoop);

	// アニメーションの遷移
	switch (mState)
	{
		// ---------------------------------------------
		// 登場
		// ---------------------------------------------
	case Entry:
		mIsAnimLoop = false;
		// 着地まで座標を下げる（アニメーションと座標をリンクさせるやつができればこれもいらない？）
		if (mPos.y > 0) --mPos.y;
		else	   mPos.y = 0.0f;

		// アニメーションが着地したタイミングで着地フラグを立てる
		if (mAnimationPlayer.GetAnimNowTime() > 15.0f) SetIsGroundMovie();

		// アニメーションが終了したら咆哮する
		if (!mAnimationPlayer.IsPlay())
		{
			ChangeState(Roaring);
		}

		break;

		// ---------------------------------------------
		// 咆哮
		// ---------------------------------------------
	case Roaring:
		mIsAnimLoop = false;
		// 再生速度を速くするか、エフェクトの再生時間を伸ばせるか
		mAnimPlaySpeed = 0.5f;

		// エフェクトをセットする(重複再生防止のために再生チェックをする)
		if (!mEffectPlayer.IsPlay() && mAnimationPlayer.GetAnimNowTime() > 20.0f) SetEffect(GetRoaringEffectHandle());

		// アニメーションの再生が終了したら、エフェクトのフラグをリリースし、歩行状態へ
		if (!mAnimationPlayer.IsPlay())
		{
			mEffectPlayer.ReleasePlay();
			ChangeState(Idle);
		}
		break;

		// ---------------------------------------------
		// 待機
		// ---------------------------------------------
	case Idle:
		mIsAnimLoop	   = true;
		mAnimPlaySpeed = 0.5f;
		break;
	}
}

// 更新
void Boss::Update(float _delta)
{

	DamageUpdate();

	// ---------------------------------------------------------
	// アニメーション再生処理
	// ---------------------------------------------------------
	mAnimationPlayer.Play(mAnimPlaySpeed,mIsAnimLoop);

	// 再生速度の調整
	mAnimPlaySpeed *= _delta;

	// 移動終点との距離を計算
	Vector3 v = mMoveEndPos - mPos;
	// 移動地点までの距離
	float destiDistance = v.GetLength();

	
	// 足元の座標をもとに実際の座標との差分を計算
	Vector3 tmpPos = mAnimationPlayer.FixedOffset("mixamorig:LeftToeBase", "mixamorig:RightToeBase");
	Vector3 subPosToBase = tmpPos - mPos;
	
	// 腰の座標をもとに実際の座標との差分を計算
	Vector3 hipPos = mAnimationPlayer.GetBasePos("mixamorig:Hips");
	Vector3 subPosToHips = hipPos - mPos;
	

	// ---------------------------------------------------------------------------------
	// 行動パターン別の処理
	// ---------------------------------------------------------------------------------
	switch (mState)
	{	
		// ---------------------------------------------
		// 待機
		// ---------------------------------------------
	case Idle:
		// 画面に映っているモデルの座標と実際の座標の誤差を修正する
		mPos += subPosToBase;

		// ムービーから遷移してきたらそのまま歩行状態へ
		SetStateWalk();

		break;

		// ---------------------------------------------
		// 歩行
		// ---------------------------------------------
	case Walk:
		mAnimPlaySpeed = 0.2f;

		// 移動速度の設定と座標移動関数
		Move(mStatus.moveSpeed);

		// プレイヤーが視野の外に居たら旋回しながら移動する
		if (!IsSeePlayer())
		{
			MoveDirection(mIsRightDirection);

			// プレイヤーが視野外にいる時間を計測する
			++mOutOfSightTimer;
			// 規定数に達した時に範囲内にプレイヤーがいればジャンプ攻撃をする
			if (mOutOfSightTimer > mOUTOFSIGHTMAX)
			{
				mOutOfSightTimer = 0;
				//SetStateJump();
			}
		}
		else 
		{
			// 視野内にいるときはプレイヤーを追尾する
			SetPlayerDir(mTargetInfo->GetPos());
		}

		mIsAnimLoop = true;

		break;

		// ---------------------------------------------
		// 走行
		// ---------------------------------------------
	case Run:
		mAnimPlaySpeed = 0.3f;
		Move(mSpeed * 2.0f);

		// 到達地まで来たら待機状態へ
		if (destiDistance < mPos.GetLength())
		{
			ChangeState(Idle);
		}
		break;

		// ---------------------------------------------
		// 弱攻撃待機
		// ---------------------------------------------
	case LightAttackIdle:

		mAnimPlaySpeed = 0.6f;

		// アニメーションが一周したら歩行状態へ
		if (!mAnimationPlayer.IsPlay())
		{
			SetPower(mStatus.attackPower[BOSSATTACKTYPE::Light]);
			SetStateLightAttack();
		}
		break;

		// ---------------------------------------------
		// 弱攻撃
		// ---------------------------------------------
	case LightAttack:
		mAnimPlaySpeed = 0.25f;
		mAnimPlaySpeed *= _delta;

		// アニメーションが一周したら歩行状態へ
		if (!mAnimationPlayer.IsPlay())
		{
			ReleaseLightAttack();
			SetStateWalk();
		}
		break;

		// ---------------------------------------------
		// 強攻撃待機
		// ---------------------------------------------
	case HeavyAttackIdle:

		mAnimPlaySpeed = 0.6f;

		// アニメーション終了後は歩行状態へ
		if (!mAnimationPlayer.IsPlay())
		{
			SetPower(mStatus.attackPower[BOSSATTACKTYPE::Heavy]);
			SetStateHeavyAttack();
		}

		break;

		// ---------------------------------------------
		// 強攻撃
		// ---------------------------------------------
	case HeavyAttack:
		// アニメーションの時間によって再生時間を変える
		if		(mAnimationPlayer.GetAnimNowTime() < 25.0f) mAnimPlaySpeed = 0.25f;		// 振りはじめ
		else if (mAnimationPlayer.GetAnimNowTime() < 38.0f)	mAnimPlaySpeed = 0.55f;		// 振り抜き中（この区間だけ当たり判定を出す）
		else												mAnimPlaySpeed = 0.25f;		// 振り切り後

		// アニメーション終了後は歩行状態へ
		if (!mAnimationPlayer.IsPlay())
		{
			ReleaseHeavyAttack();
			SetStateWalk();
		}

		break;

		// ---------------------------------------------
		// コンボ攻撃
		// ---------------------------------------------
	case ComboAttack:
		// アニメーションに合わせて再生速度を変える
		// 振りかぶり
		if (mAnimationPlayer.GetAnimNowTime() < 50.0f)
		{
			mAnimPlaySpeed = 0.25f;		
		}
		// 攻撃中
		else if (mAnimationPlayer.GetAnimNowTime() < 98.0f)
		{
			mAnimPlaySpeed = 0.45f;
		}
		// 攻撃終了後
		else
		{
			mAnimPlaySpeed = 0.3f;		
		}

		// アニメーション終了後は歩行状態へ
		if (!mAnimationPlayer.IsPlay())
		{
			mEffectPlayer.ReleasePlay();
			ReleaseComboAttack();
			ChangeState(Idle);
		}

		break;

		// ---------------------------------------------
		// ジャンプ
		// ---------------------------------------------
	case Jump:
		// アニメーションにに合わせて再生速度を変える
		if		(mAnimationPlayer.GetAnimNowTime() < 34.0f)	mAnimPlaySpeed = 0.35f;
		else if (mAnimationPlayer.GetAnimNowTime() < 57.0f) mAnimPlaySpeed = 0.5f;
		else								mAnimPlaySpeed = 0.25f;

		// アニメーションに合わせてY座標を昇降させる
		if (mAnimationPlayer.GetAnimNowTime() > 35.0f) mPos.y += 0.1f;
		if (mAnimationPlayer.GetAnimNowTime() > 50.0f) mPos.y -= 0.2f;
		if (mAnimationPlayer.GetAnimNowTime() > 62.0f)
		{
			mPos.y = 0.0f;
			// 衝撃波のエフェクトをセットする(重複再生を防止するために再生チェックをする)
			if (!mEffectPlayer.IsPlay())mEffectPlayer.SetPlay(mEffectWaveHandle,GetPos());
		}

		// アニメーションが終了したら歩行状態へ
		if (!mAnimationPlayer.IsPlay())
		{
			// カウンターでジャンプ攻撃をしたときは視野外のタイマーに
			// 値が入ったままなので０にする
			if (mOutOfSightTimer != 0) mOutOfSightTimer = 0;

			// エフェクトプレイヤーのフラグをリリースする
			mEffectPlayer.ReleasePlay();

			ReleaseJump();
			SetStateWalk();
		}

		break;

		// ---------------------------------------------
		// 吹き飛び
		// ---------------------------------------------
	case BlowOff:
		mAnimPlaySpeed = 0.35f;

		// 起き上がる
		if (!mAnimationPlayer.IsPlay())
		{
			ChangeState(GetUp);
		}
		break;

		// ---------------------------------------------
		// 起き上がり
		// ---------------------------------------------
	case GetUp:
		mAnimPlaySpeed = 0.6f;

		// 起き上がる
		if (!mAnimationPlayer.IsPlay())
		{
			ReleaseBlowOff();
			ChangeState(Idle);
		}
		break;

		// ---------------------------------------------
		// 怯み
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
		// ガード
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
		// ダウン
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
			// 画面に映っているモデルの位置と実際の座標の誤差を修正する
			mPos += subPosToHips;
			mPos.y = 0.0f;
			ChangeState(DownGetUp);
		}

		break;

		// ---------------------------------------------
		// ダウンからの起き上がり
		// ---------------------------------------------
	case DownGetUp:
		mAnimPlaySpeed = 0.4f;

		if (mAnimationPlayer.GetAnimNowTime() > 100.0f)
		{
			SetStateTurnAttack();
		}

		break;

		// ---------------------------------------------
		// ダウンからの起き上がり時の回転攻撃
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
		// 死亡
		// ---------------------------------------------
	case Death:
		mAnimPlaySpeed = 0.3f;
		// 倒れ切ったあたりで死亡判定にする
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

// 最前列描画
void Boss::PostDraw()
{
	// HPバーの描画
	DrawLineBox(99, 699, WINDOW_W - 99, 716, GetColor(255, 255, 255));
	DrawBox(100, 700, mHpBar, 715, GetColor(255, 128, 64), TRUE);
}
