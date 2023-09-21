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
	// マウスの移動量から注視点を回転させる
	Vector2 mousePos;
	mousePos.x = (float)GetMouseX();
	mousePos.y = (float)GetMouseY();

	// 画面中心から移動量を取得
	Vector2 mouseMoveValue;
	mouseMoveValue.x = WINDOW_HALF_W - mousePos.x;
	mouseMoveValue.y = WINDOW_HALF_H - mousePos.y;

	// その移動量をアングル値に入れる
	mAngleH += (mouseMoveValue.x * mANGLEMOVERATIO);
	mAngleV += (mouseMoveValue.y * mANGLEMOVERATIO);

	// アングル値のリセット
	if (mAngleH <= -180.0f) mAngleH += 360.0f;
	if (mAngleH >=  180.0f) mAngleH -= 360.0f;
	if (mAngleV >=   10.0f) mAngleV  =  10.0f;
	if (mAngleV <=  -40.0f) mAngleV  = -40.0f;

	// マウスの位置を画面中央に戻す
	SetMousePoint(WINDOW_W / 2, WINDOW_H / 2);
}

// コンストラクタ
Camera::Camera():
	mAngleH(),mAngleV(),
	mIsShakeMovie(false),mShakeTimer(0),misFinishMovie(false)
{
}

// デストラクタ
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
	// 水平角度の計算
	// ----------------
	// 座標から注視点のベクトルを使って今の水平角度を計算する
	Vector3 vecPosToLook  = mLookPos - mPos;
	float	lockOnHorizon = atan2f(vecPosToLook.x, vecPosToLook.z);

	lockOnHorizon *= -1;
	// 角度はデグリー角で扱っているから変換して代入する
	mAngleH = TO_DEGREE(lockOnHorizon);

	// ----------------
	// 垂直角度の計算
	// ----------------
	// わからんからもう一回質問紙に行きなね
	mAngleV = 0.0f;
}

// 今の水平角度を取得する（プレイヤーの座標移動に使用する）
float Camera::GetAngleHorizontal() const
{
	return mAngleH;
}

// カメラの角度を座標に反映させる
Vector3 Camera::ReflectionAngle() const
{
	float   sinParam;		// 垂直、水平角度をsinf関数で計算した値を入れる
	float   cosParam;		// 垂直、水平角度をcosf関数で計算した値を入れる
	Vector3 tmpPosV;		// 反映された垂直角度を入れておく
	Vector3 tmpPosH;		// 反映された水平角度を入れておく

	//	垂直角度を反映
	sinParam = sinf(TO_RADIAN(mAngleV));
	cosParam = cosf(TO_RADIAN(mAngleV));
	tmpPosV.x = 0.0f;
	tmpPosV.y =  sinParam * mLOOKDISTANCE;
	tmpPosV.z = -cosParam * mLOOKDISTANCE;

	// 水平角度を反映
	sinParam = sin(TO_RADIAN(mAngleH));
	cosParam = cos(TO_RADIAN(mAngleH));
	tmpPosH.x = cosParam * tmpPosV.x - sinParam * tmpPosV.z;
	tmpPosH.y = tmpPosV.y;
	tmpPosH.z = sinParam * tmpPosV.x + cosParam * tmpPosV.z;

	// 算出した座標に注視点を加算したものがカメラの位置
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

// 敵クラスをの情報をセットする
void Camera::SetTargetInfo(const CharacterBase* _enemy)
{
	mTargetEnemy = _enemy;
}

void Camera::SetPlayerInfo(const Player* _player)
{
	mPlayerInfo = _player;
}

// ターゲットにカメラを向ける
void Camera::SetTargetPos( Vector3 _targetPos) 
{
	// 注視点からターゲットのベクトル
	Vector3 moveVector = _targetPos - mLookPos;
	// 極端に値を小さくすることで徐々に注視点を移動させる
	moveVector.Set(moveVector.x / 100.0f, 0.0f, moveVector.z / 100.0f);

	// カクツキを防止するためにある程度、注視点が近づいたら足しこみを止める
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
	// プレイヤーの真後ろにつくようになってる

	mPos.x = _playerPos.x - 35.0f * sinf(_playerRot + TO_RADIAN(180.0f));
	mPos.y = _playerPos.y + 20.0f;
	mPos.z = _playerPos.z - 35.0f * cosf(_playerRot + TO_RADIAN(180.0f));

}

// 初期化
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
	// ムービー風演出時のアングル操作
	if (mBaseLookPos.y < mLookPos.y) mLookPos.y -= 0.13f;
	else
	{
		// カメラを下げ終えたら、ゲーム時の定位置まで交代する
		Vector3 moveVector = mBasePos - mPos;
		// 徐々に進むよう移動ベクトルを割る
		const float DIVISIONVALUE = 100.0f;
		moveVector.Set(moveVector / 100.0f);

		// このベクトルの長さを求めて、移動の判定に使用する
		float length = moveVector.GetLength();

		// カメラの座標に入れていく
		mPos += moveVector;

		// ムービー風演出の終了する
		if (length < 0.01f)
		{
			mPos = mBasePos;
			mLookPos = mBaseLookPos;
			SetFinishMovie();
		}
	}

	// カメラを揺らす処理
	//SetMovieShakeTime();
	if(IsMovieShake())	ShakeUpdate();
}

// 更新
void Camera::Update(bool _isDebug)
{
	// プレイヤーの座標を入れておく
	Vector3 playerPos = mPlayerInfo->GetPos();

	if (mPlayerInfo->IsTargetLook())
	{
		// ロックオン対象を注視し、プレイヤーの背後に移動する
		SetTargetPos(mTargetEnemy->GetPos());
		SetBehindPlayer(mPlayerInfo->GetPos(), mPlayerInfo->GetRot());
	}

	// ロックオン解除する瞬間の角度を入れる
	if (ReleaseKey(KEY_INPUT_K))
	{
		ReleaseLockOn();
	}

	// 非ロックオン時の操作
	if (!mPlayerInfo->IsTargetLook())
	{
		// 注視点の回転処理
		if (!_isDebug)
		{
			AngleUpdate();
		}

		// 注視点はプレイヤーから少し高い位置
		mLookPos = playerPos;
		mLookPos.y += 15.0f;

		// カメラの角度から座標を設定する
		mPos = ReflectionAngle();
	}
}

// 描画
void Camera::Draw()
{
	// 座標と注視点をセットする
	SetCameraPositionAndTarget_UpVecY(mPos.GetVECTOR(),mLookPos.GetVECTOR());
}

// 終了
void Camera::Exit()
{

}
