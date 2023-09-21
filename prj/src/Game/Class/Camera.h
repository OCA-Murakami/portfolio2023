#pragma once
//------------------------------------------------
// インクルード
//------------------------------------------------

//------------------------------------------------
//!@class カメラクラス
//!@brief  
//------------------------------------------------
class Camera
{

	const float	  mANGLEMOVERATIO = 0.02f;		//!< カメラアングルの移動速度
	const float	  mLOOKDISTANCE   = 30.0f;		//!< 注視点までの距離
		  float	  mAngleH;						//!< 水平方向の角度
		  float	  mAngleV;						//!< 垂直方向の角度
		  Vector3 mPos;							//!< 座標
		  Vector3 mLookPos;						//!< 注視点
		  int	  mShakeTimer;					//!< カメラを揺らすタイマー

	//-----------------------------
	// ムービー風演出時に使う変数
	//-----------------------------
	const int mMOVIESHAKETIME = 30;	//!< 演出内でカメラを揺らす時間
	Vector3	  mBasePos;				//!< 演出終了時にいて欲しい定位置
	Vector3	  mBaseLookPos;			//!< 演出終了時に見て欲しい定位置
	bool	  mIsShakeMovie;		//!< 演出内でカメラを揺らすフラグ
	bool	  misFinishMovie;		//!< 演出の終了フラグ

	//-----------------------------
	// ロックオンの処理に使う変数
	//-----------------------------
	const CharacterBase* mTargetEnemy = nullptr;	//!< 注視する敵
	const Player*		 mPlayerInfo  = nullptr;	//!< 追尾するプレイヤーの情報

	void AngleUpdate();		//!< マウスで注視点の操作

public:
	Camera();		//!< コンストラクタ
	~Camera();		//!< デストラクタ

	float   GetAngleHorizontal() const;		//!< カメラの水平方向を取得する
	Vector3 ReflectionAngle()    const;		//!< カメラの角度を座標に反映させる
	void    ShakeUpdate();					//!< カメラを揺らす

	//-----------------------------
	// ムービー風演出時に使う関数
	//-----------------------------
	void SetFinishMovie();			//!< 演出の終了
	bool IsFinishMovie() const;		//!< 演出の終了チェック
	void SetMovieShakeTime();		//!< 演出の注視点を揺らす時間をセットする
	void SetMovieShake();			//!< カメラを揺らすフラグをセットする
	bool IsMovieShake() const;		//!< カメラを揺らすフラグをチェック

	//------------------------------------
	// ロックオン処理の関数
	//------------------------------------
	void SetTargetInfo(const CharacterBase* _enemy);							//!< ロックオン時、注視する対象をセット
	void SetPlayerInfo(const Player* _player);									//!< プレイヤーの情報を取得
	void SetTargetPos(Vector3 _targetPos);										//!< 敵にカメラを向ける（ターゲット機能）
	void SetBehindPlayer(const Vector3& _playerPos, const float& _playerRot);	//!< プレイヤーの後ろへ移動する
	void ReleaseLockOn();														//!< ロックオンを解除する瞬間の角度を保持させる

	void Init();					//!< 初期化
	void MovieInit();				//!< ムービーモード用の初期化
	void MovieUpdate();				//!< ムービー風演出時の処理
	void Update(bool _isDebug);		//!< 更新
	void Draw();					//!< 描画
	void Exit();					//!< 終了
};

