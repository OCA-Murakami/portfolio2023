#pragma once
#include "../../System/Class/Effect.h"
#include "../../System/Class/Animation.h"
#include "../../System/Class/Model.h"


//------------------------------------------------
//!@class Playerクラス
//!@brief プレイヤーキャラクター操作 
//------------------------------------------------
class Player : public CharacterBase
{
	//------------------------------------------------
	// 列挙体
	//------------------------------------------------
	//! ステートの列挙体
	enum PLAYERSTATE
	{
		Idle,		//!< 待機
		Attack1,	//!< 攻撃
		Attack2,
		Attack3,
		Attack4,
		Walk,		//!< 歩行
		Run,		//!< 走行
		Roll,		//!< 前転
		GetUp,		//!< 起き上がり
		React,		//!< 怯み
		Repell,		//!< 弾かれ
		BlowOff,	//!< 吹き飛び
		Death,		//!< 死亡
		StateMax,	//!< 要素の最大数専用（必ずこの要素が最後に来ること）
	};

	//! エフェクトハンドルの列挙体
	enum EFFECTHANDLE
	{
		Slash,		//!< 斬撃
		Blood,		//!< ダメージ
		HandleMax,	//!< 要素の最大数専用（必ずこの要素が最後に来ること）
	};

	//! 攻撃回数をカウントする用
	enum ATTACKTYPE
	{
		Combo1,		//!< 1段目
		Combo2,		//!< ２段目
		Combo3,		//!< ３段目
		Combo4,		//!< ４段目
		ComboMax,	//!< 最大数
	};

	//-----------------------------
	// 定数
	//-----------------------------
	const float mWALKSPEED = 0.8f;		//!< 歩行速度
	const float mRUNSPEED = 1.4f;		//!< 走行速度
	const int	mCONTATKMAX = 4;		//!< 連続攻撃の最大数

	//-----------------------------
	// 行動時に使うフラグ
	//-----------------------------
	bool mIsRoll;			//!< 前転
	bool mIsGetUp;			//!< 起き上がり
	bool mIsReact;			//!< 怯み
	bool mIsBlowOff;		//!< 吹き飛び
	bool mIsRepell;			//!< 弾かれ
	bool mIsDeath;			//!< 死亡
	bool mIsInvincible;		//!< 無敵
	bool mIsTargetLook;		//!< ロックオン

	//-----------------------------
	// 攻撃するときに使う変数
	//-----------------------------
	int	  mAttackCombo;		//!< 攻撃回数のカウント 
	float mSwordSize;		//!< 当たり判定に使用する剣のサイズ
	float mAtkReserveS;		//!< コンボの入力受付開始時間
	float mAtkReserveE;		//!< コンボの入力受付終了時間

	//-----------------------------
	// アニメーションに使う変数
	//-----------------------------
	int   mAnim[StateMax];	//!< アニメーションのモデルデータ

	//-----------------------------
	// その他に使う変数
	//-----------------------------
	float mMoveDir;			//!< 移動方向（キャラクターの向きではないの注意）



	// 新しく作った変数（定着次第しっかりリファクタリング）
	bool mIsWalk;
	Vector3 mMoveVector;
	float mCameraHAngle;	//!< カメラクラスからコピーしてきた水平角度
	float mRangeOfMove;		//!< ステージ内の移動範囲
	float mSpeedScale;		//!< 移動速度の倍率
	


	//-----------------------------
	// エフェクトに使う変数
	//-----------------------------
	int mEffectHandles[HandleMax];	//!< エフェクトのハンドルを入れる
	Effect mEffectPlayer;

public:
	Player();	//!< コンストラクタ
	~Player();	//!< デストラクタ	

	void SetRangeOfMove(const float _rangeRadius);

	//------------------------------------
	// 値のセット、リリースの関数
	//------------------------------------
	void ChangeRot();											//!< キー入力による向きの変更
	void ChangeMoveDir();										//!< ロックオン中の移動する向きを設定する
	void SetEnemyDir(const Vector3 _enemyPos);					//!< 敵の座標から敵の方向をセットする
	void SetDamage(const float _damage);	//!< 被ダメージ関数
	void CopyCameraHAngle(const float _cameraHAngle);

	//------------------------------------
	// フラグ変数のセットとリリースの関数
	//------------------------------------
	void SetRoll();					//!< 前転状態にする
	void SetGetUp();				//!< 起き上がり状態にする
	void SetDeath();				//!< 死亡状態にする
	void SetTargetLook();			//!< ロックオン状態にする
	void SetBlowOff();				//!< 吹き飛び状態にする
	void SetRepell();				//!< 弾かれ状態にする
	void ReleaseTargetLook();		//!< ロックオン状態を解除
	void ReleaseRoll();				//!< 前転状態の解除
	void ReleaseReact();			//!< 怯み状態の解除
	void ReleaseGetUp();			//!< 起き上がり状態の解除
	void ReleaseBlowOff();			//!< 吹き飛び状態の解除
	void ReleaseDeath();			//!< 死亡状態の解除
	void ReleaseRepell();			//!< 弾かれ状態の解除

	//------------------------------------
	// フラグチェックの関数
	//------------------------------------
	bool IsNotMove();				//!< 移動のキー入力がないかチェック
	bool IsStopKeyInput() const;	//!< キー入力が受け付けられていないかチェック
	bool IsRoll()		  const;	//!< 前転チェック
	bool IsGetUp()		  const;	//!< 起き上がりチェック
	bool IsBlowOff()	  const;	//!< 吹き飛ばされている状態かチェック
	bool IsRepell()		  const;	//!< 攻撃が弾かれたかチェック
	bool IsReact()		  const;	//!< ダメージ状態かチェック
	bool IsDeath()		  const;	//!< 死亡しているかチェック
	bool IsTargetLook()	  const;	//!< ロックオン中かチェック

	//------------------------------------
	// 攻撃関連の関数
	//------------------------------------
	int  GetAtkCombo() const;		//!< コンボ数の取得
	void SetAttack();				//!< 攻撃状態にセット
	bool IsAttack() const;			//!< 攻撃中かチェック
	bool IsInvincible();			//!< 無敵中かチェック
	float GetSwordSize() const;		//!< 当たり判定に使用する剣のカプセルのサイズ
	void ReleaseAttackInfo();		//!< 攻撃に使用する変数をすべて元に戻す
	void SetDuringReserve(const float _startFraem, const float _endFrame);	//!< 入力期間の設定
	void InitDuringReserve();		//!< 入力期間の初期化
	bool IsDuringReserve() const;	//!< コンボ攻撃における、次の攻撃への入力受付フレームのチェック
	bool CheckNextAttackCondition(const int _nowAttack) const;	//!< コンボ攻撃における、次の攻撃へ遷移できる状態であるかチェック
	bool IsLastComboAttack() const;	//!< コンボ攻撃の最終段かチェック

	//------------------------------------
	// アニメーションの切り替え関数
	//------------------------------------
	void SetStateIdle();						//!< 待機状態へ
	void SetStateWalk();						//!< 歩行状態へ
	void SetStateRun();							//!< 走行状態へ
	void SetStateAttack(int _nextAttack);		//!< 攻撃状態へ
	void SetStateRoll();						//!< 前転状態へ
	void SetStateGetUp();						//!< 起き上がり状態へ
	void SetStateReact();						//!< ダメージ状態へ
	void SetStateBlowOff();						//!< 吹き飛び状態へ
	void SetStateRepell();						//!< 弾かれ状態へ
	void SetStateDeath();						//!< 死亡状態へ

	//------------------------------------
	// エフェクトの関数
	//------------------------------------
	int GetSlashEffectHandle() const;		//!< 斬撃エフェクトのハンドルを取得する
	int GetBloodEffectHandle() const;		//!< ダメージエフェクトのハンドルを取得する	

	void SetEffect();
	Vector3 GetEffectPos(const int _effectHandle) const;			//!< エフェクトの座標を設定する

	void Init(std::string _filePath, const float _rangeMoveRad);
	void ReInit();													//!< ゲームを再度開始するときに初期化が必要な変数だけ初期化する
	void EditInit();												//!< エディット時に合わせた初期化
	void Update(float _delta);
	void PostDraw();												//!< 一番上に描画したいものをこっちで描画する
};