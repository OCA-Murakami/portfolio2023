#pragma once
//------------------------------------------------
// インクルード
//------------------------------------------------
#include "../../System/Class/Effect.h"
#include "../../System/Class/Animation.h"
#include "../../System/Class/Model.h"

//------------------------------------------------
//!@class CharacterBaseクラス
//!@brief キャラクターの共通個所をまとめた基底クラス 
//------------------------------------------------
class CharacterBase
{
protected:
	Model	mModelData;								//!< モデル
	int		mState;									//!< 行動状態
	Vector3 mPos;									//!< 座標
	Vector3 mRot;									//!< 向き
	Vector3 mSize;									//!< サイズ
	const CharacterBase* mTargetInfo = nullptr;		//!< プレイヤーまたは敵の情報を入れる

	//---------------------------------------
	// ステータスに関わる変数
	//---------------------------------------
	float	mHp;		//!< 残りHP
	float   mSpeed;		//!< 移動速度
	float mAtkPower;	//!< 攻撃力
	float mCollSize;	//!< ボディの当たり判定のサイズ

	//-----------------------------
	// 描画関連に使用する変数
	//-----------------------------
	int		mHpBar;					//!< HPバー減少用
	COLOR_F mBaseMatDif;			//!< モデルが持つもともとのディフューズカラーの値

	//-----------------------------
	// 攻撃するときに使う変数
	//-----------------------------
	float mAtkJudgeStart;	//!< 攻撃判定が発生するタイミング
	float mAtkJudgeEnd;		//!< 攻撃判定が終了するタイミング
	bool  mIsAtkSend;		//!< ダメージが１度だけ送られるようにするフラグ
	bool  mIsAttack;		//!< 攻撃フラグ

	//-----------------------------
	// アニメーションに使う変数
	//-----------------------------
	Animation mAnimationPlayer;		//!< アニメーションクラスのオブジェクト
	float	  mAnimPlaySpeed;		//!< アニメーションの再生速度
	bool	  mIsAnimLoop;			//!< アニメーション毎にループ設定を切り替えるためのフラグ

	//-----------------------------
	// エフェクトに使う変数
	//-----------------------------
	Effect mEffectPlayer;		//!< エフェクトの再生に使用するオブジェクト			

public:
	CharacterBase();	//!< コンストラクタ
	~CharacterBase();	//!< デストラクタ

	//---------------------------------------------------------------
	// 値の取得、セット関数
	//---------------------------------------------------------------
	//! @fn SetPos(Vector3)
	//! @brief Vector3型の値で一括セット
	void SetPos(const Vector3 _pos);

	//! @fn SetPos(float)
	//! @brief x,y,zそれぞれに値をセット
	void SetPos(const float _x, const float _y, const float _z);	

	//! @fn GetPos
	//! @brief 座標を取得する(Vector3型)
	//! @return 今の座標(Vector3型)
	Vector3 GetPos() const;

	//! @brief Vector3をVECTOR型に変換して取得する
	//! @return VECTOR型に変換した mPos
	VECTOR  GetVECTORPos() const;	

	//! @brief 向きを取得する
	//! @return mRot.y の値
	float	GetRot() const;				
	int		GetModel();					

	//! @brief ターゲット相手の情報をポインタで受け取る
	//! @param _target	ターゲット相手のオブジェクトをセットする（ターゲット相手：　Player => Boss		Boss, Camera => Player）
	void SetTargetInfo(const CharacterBase* _target);		

	//---------------------------------------------------------------
	// ディフューズカラー操作の関数
	//---------------------------------------------------------------
	//! @brief モデルのディフューズカラーをダメージ時の値に変更する
	void SetMatDifDamage();			

	//! @brief モデルの元々のディフューズカラーをセットする
	void SetMatDifBase();			

	//---------------------------------------------------------------
	// 攻撃関連の関数
	//---------------------------------------------------------------
	//! @brief 攻撃フラグのリリース
	void  ReleaseAttack();		

	//! @brief 攻撃判定の重複防止フラグをセット
	void  SetAtkSend();

	//! @brief 攻撃力のセット
	//! @param _power	攻撃の種類によって変化する攻撃力
	void  SetPower(const float _power);

	//! @brief 攻撃中かチェックする
	//! @return	ture : 攻撃中		false : 非攻撃中
	virtual bool  IsAttack() const;

	//! @brief 攻撃判定の重複防止フラグのチェック
	//! @return true : 既に攻撃判定が成立した			false : 攻撃判定が未成立
	bool  IsAtkSend()  const;

	//! @brief アニメーションのフレーム数から攻撃判定を発生させるフラグをチェック
	//! @return true : 攻撃判定を発生させる		false : 攻撃判定を発生させない
	bool  IsAtkJudge() const;

	//! @brief 攻撃力を取得する
	//! @return 現在の攻撃力（攻撃の種類によって値は変化する）
	float GetPower() const;

	//! @brief 攻撃を受ける時の判定に使用するコリジョンのサイズを取得する
	//! @return コリジョンのサイズ
	virtual float GetCollSize() const;	


	//! @brief ステージに描画されているスフィア内にいるかチェックする
	//! @param _stageLength		スフィアの半径
	//! @return true : スフィア内		false : スフィア外
	bool IsInTheStage(float _stageLength) const;

	//! @brief 
	//! @param _nextState 
	void ChangeState(const int _nextState);

	virtual void Init(std::string _filePath);		//!< 初期化
	virtual void Update(float _delta) = 0;	//!< 更新
	virtual void Draw();				//!< 描画
	virtual void PostDraw();			//!< 最前面に描画したいものをまとめる
	virtual void Exit();				//!< 終了
};

