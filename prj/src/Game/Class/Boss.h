#pragma once
//------------------------------------------------
// インクルード
//------------------------------------------------
#include "../../System/Class/Effect.h"
#include "../../System/Class/Animation.h"
#include "../../System/FileLoader/CsvLoading.h"
#include "../BossStatus.h"

//------------------------------------------------
//!@class Bossクラス
//!@brief ボスキャラクター操作 
//------------------------------------------------
class Boss : public CharacterBase
{
	//------------------------------------------------
	// 列挙体
	//------------------------------------------------
	//! アニメーションの要素数にアクセスするための列挙体
	enum BOSSSTATE
	{
		Entry,				//!< 登場
		Roaring,			//!< 咆哮
		Idle,				//!< 待機
		Walk,				//!< 歩行
		Run,				//!< 走行
		LightAttackIdle,	//!< 弱攻撃前の溜め
		LightAttack,		//!< 弱攻撃
		HeavyAttackIdle,	//!< 強攻撃前の溜め
		HeavyAttack,		//!< 強攻撃
		ComboAttack,		//!< コンボ攻撃
		TurnAttack,			//!< 360度攻撃
		Jump,				//!< ジャンプ
		Guard,				//!< 防御
		BlowOff,			//!< 吹き飛び
		GetUp,				//!< 起き上がり
		React,				//!< 怯み
		Down,				//!< ダウン
		DownGetUp,			//!< ダウンからの起き上がり
		Death,				//!< 死亡
		StateMax,			//!< 列挙体の最大値（これが最後に来ること）
	};

	struct CSVFilePath
	{
		std::string statusPath = "data/csv/BossStatus.csv";
		std::string collPath   = "data/csv/BossColl.csv";
	};

	//-----------------------------
	// 定数
	//-----------------------------
	const float mNEXTACTIONTIME = 10.0f;	//!< 待機状態での最大待ち時間
	const float mBLOCKTIMERMAX  = 60.0f;	//!< 防御体制の最大時間
	const float mVIEWANGLE		= 35.0f;	//!< 視野角の半値
	const float mHPMAX			= 100.0f;
	const int	mREACTMAX		= 30;		//!< 怯み値の最大
	const int	mDOWNMAX		= 2;		//!< ダウン値の最大
	const int   mDOWNTIMEMAX	= 3 * 60;	//!< ダウンの最大フレーム数
	const int   mOUTOFSIGHTMAX  = 2 * 60;	//!< プレイヤーが視野外にいる時間の最大計測時間

	//------------------------------------
	// CSVファイルから数値を受け取る変数　　
	//------------------------------------
	CSVFilePath csvPaths;		//!< 各CSVファイルのパス
	BossStatus	 mStatus;		//!< ステータス
	BossCollInfo mCollInfo;		//!< コリジョン設定

	//-----------------------------
	// 各行動へ遷移するときの閾値
	//-----------------------------
	int mReactValue;		//!< 怯み値
	int mDownValue;			//!< ダウン値
	int mstoredDamage;		//!< 蓄積ダメージ値
	int mThresholdGurad;	//!< ガードする閾値

	//-----------------------------
	// 行動時に使うフラグ
	//-----------------------------
	bool mIsGroundMovie;			//!< ムービー風演出内での着地フラグ
	bool mIsFinishMovie;		//!< ムービー風演出の終了フラグ
	bool mIsIdle;				//!< 待機
	bool mIsDown;				//!< ダウン
	bool mIsReact;				//!< 怯み
	bool mIsLightAttack;		//!< 弱攻撃
	bool mIsHeavyAttack;		//!< 強攻撃
	bool mIsComboAttack;		//!< コンボ攻撃
	bool mIsJump;				//!< ジャンプ
	bool mIsGuard;				//!< ガード
	bool mIsBlowOff;			//!< 吹き飛ぶ
	bool mIsDeath;				//!< 死亡
	bool mIsRightDirection;		//!< プレイヤーが自分の右側にいるか左側にいるか

	//-----------------------------
	// 行動時に使う変数
	//-----------------------------
	Vector3 mMoveEndPos;			//!< 移動時の終点（プレイヤーの座標）
	int	mDownTimer;				//!< ダウン状態のタイマー
	int     mOutOfSightTimer;		//!< プレイヤーが視野外にいる時間を計測する

	//-----------------------------
	// アニメーションに使う変数
	//-----------------------------
	int   mAnim[StateMax];	//!< アニメーションのモデルデータ	

	//-----------------------------
	// エフェクトに使う変数
	//-----------------------------
	int mEffectWaveHandle;		//!< ジャンプ攻撃の衝撃波エフェクトのハンドルを入れる
	int mEffectDangerHandle;	//!< 大きい攻撃を繰り出す前の予告に使う
	int mEffectRoaringHandle;	//!< 咆哮時に使用するエフェクト
	Effect mEffectPlayer;


	int mDamageFrame = 0;
	Vector3 mFixOffset;

private:
	bool IsLimitActions() const;		//!< 行動を遮られたく無い時に使用する行動チェック関数
	void ReleaseAllFlag();				//!< 全てのフラグを一斉リリース

public:
	Boss();		//!< コンストラクタ
	~Boss();	//!< デストラクタ

	void Move(float _moveSpeed);	//!< 座標移動をまとめた関数
	bool IsSeePlayer();				//!< プレイヤーが視野内にいるかチェック
	bool IsRightDirection();		//!< プレイヤーが右側にいるかチェック
	void MoveDirection(const bool _isRightDir);	//!< 移動中の旋回処理
	void ChoiseAttackType();		//!< 攻撃パターンを決定する

	// -----------------------------------------
	// 各変数の取得、セット関数
	// -----------------------------------------
	// ２つの関数はなくてもいい気がする
	void  GetPlayerPos(Vector3 _playerPos);		//!< プレイヤーの座標を取得する
	void  SetPlayerDir(Vector3 _playerPos);		//!< プレイヤーがどの向きにいるかを計算する
	void  SetThresholdGurad();					//!< ガードする閾値をHPを基準にランダムで決定する
	float GetCollSize() const override;

	// -----------------------------------------
	// 各フラグ変数のセット関数
	// -----------------------------------------
	void SetIsGroundMovie();	//!< ムービー風演出内での着地フラグ
	void SetFinishMovie();		//!< ムービー風演出
	void SetAttack();			//!< 攻撃行動
	void SetLightAttack();		//!< 弱攻撃
	void SetHeavyAttack();		//!< 強攻撃
	void SetComboAttack();		//!< コンボ攻撃
	void SetGuard();			//!< 防御
	void SetGuardToAttack();	//!< 防御から攻撃へ（カウンター）
	void SetJump();				//!< ジャンプ
	void SetBlowOff();			//!< 吹き飛ぶ
	void SetReact();			//!< 怯み
	void SetDown();				//!< ダウン
	void SetDeath();			//!< 死亡

	// -----------------------------------------
	// 各フラグ変数のリリース関数
	// -----------------------------------------
	void ReleaseJump();				//!< ジャンプ
	void ReleaseLightAttack();		//!< 弱攻撃
	void ReleaseHeavyAttack();		//!< 強攻撃
	void ReleaseComboAttack();		//!< コンボ攻撃
	void ReleaseReact();			//!< 怯み
	void ReleaseBlowOff();			//!< 吹き飛び
	void ReleaseDown();				//!< ダウン
	void ReleaseAllAttack();		//!< 攻撃フラグを全てチェックしてリリースする

	// -----------------------------------------
	// 各フラグ変数のチェック関数
	// -----------------------------------------
	bool IsGroundMovie()   const;				//!< ムービー風演出内での着地フラグ
	bool IsFinishMoive()   const;				//!< ムービー風演出
	bool IsAttack()		   const override;		//!< 攻撃
	bool IsLightAttack()   const;				//!< 弱攻撃
	bool IsHeavyAttack()   const;				//!< 強攻撃
	bool IsComboAttack()   const;				//!< コンボ攻撃
	bool IsReact()		   const;				//!< 怯み
	bool IsDeath()		   const;				//!< 死亡
	bool IsIdle()		   const;				//!< 待機
	bool IsWalk()		   const;				//!< 歩行
	bool IsGuard()		   const;				//!< 防御
	bool IsJump()		   const;				//!< ジャンプ
	bool IsBlowOff()	   const;				//!< 吹き飛ぶ
	bool IsDown()		   const;				//!< ダウン
	bool IsInvincible()    const;				//!< 無敵状態
	bool CheckAllAttack()  const;				//!< 全ての攻撃フラグ
	bool IsSeriousDamage() const;				//!< 怯み又はダウン状態

	//------------------------------------
	// 当たり判定関連関数
	//------------------------------------
	float GetClawSize()		  const;		//!< 爪部分の当たり判定のサイズ
	float GetFistSize()		  const;		//!< 拳部分の当たり判定のサイズ
	Vector3 GetFistTopPos()   const;		//!< 拳部分の先端座標
	Vector3 GetFistUnderPos() const;		//!< 拳部分の根本座標
	Vector3 GetClawTopPos()   const;		//!< 爪部分の先端座標
	Vector3 GetClawUnderPos() const;		//!< 爪部分の根本座標

	//------------------------------------
	// ダメージ関連の関数
	//------------------------------------
	void SetDamage(const float _damage);		//!< HP減少
	void CountReactValue(const float _damage);	//!< 怯み値の更新
	void SetDamage()
	{
		mDamageFrame = 60;
	}
	void DamageUpdate()
	{
		--mDamageFrame;
		if (mDamageFrame < 0)
		{
			mDamageFrame = 0;
		}
	}

	// -----------------------------------------
	// ステート、アニメーションの切り替え関数
	// -----------------------------------------
	void SetStateWalk();				//!< 歩行状態にする
	void SetStateRun();					//!< 走行状態にする
	void SetStateLightAttack();			//!< 弱攻撃状態にする
	void SetStateHeavyAttack();			//!< 強攻撃状態にする
	void SetStateComboAttack();			//!< コンボ攻撃状態にする
	void SetStateGuard();				//!< 防御状態にする
	void SetStateJump();				//!< ジャンプ状態にする
	void SetStateBlowOff();				//!< 吹き飛び状態にする
	void SetStateReact();				//!< 怯み状態にする
	void SetStateDown();				//!< ダウン状態にする
	void SetStateTurnAttack();			//!< 回転攻撃状態にする

	// -----------------------------------------
	// エフェクトの関数
	// -----------------------------------------
	int		GetWaveEffectHandle()				  const;		//!< ジャンプ時の衝撃波のエフェクトを取得する
	int		GetDangerEffectHandle()				  const;		//!< コンボ攻撃時の予告エフェクトを取得する
	int		GetRoaringEffectHandle()			  const;		//!< 咆哮時のエフェクトを取得する
	void	SetEffect(const int _effectHandle);					//!< エフェクトプレイヤーに再生するハンドルと座標をセットする
	Vector3 GetEffectPos(const int _effectHandle) const;		//!< エフェクトの座標を設定する


	void Init(std::string _filePath);		//!< 初期化
	void ReInit();							//!< 再初期化（ハンドル格納変数を除く）
	void EditInit();						//!< エディット時に合わせた初期化
	void MovieUpdate();						//!< ムービー風演出時の更新
	void Update(float _delta) override;		//!< 更新
	void DamageDraw();
	void PostDraw();						//!< 最前面に描画したいものをまとめる
};

