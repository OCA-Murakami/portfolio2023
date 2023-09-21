#pragma once
//---------------------------------------------------------------------------
//! @file   Editor.h
//! @brief  エディタークラス
//---------------------------------------------------------------------------
#include "../../Game/BossStatus.h"
#include "../FileLoader/CsvLoading.h"

#include "../Random.h"
#include "Vector3.h"
#include "Effect.h"
#include "Animation.h"
#include "../../Game/Class/CharacterBase.h"
#include "../../Game/Class/Player.h"
#include "../../Game/Class/Boss.h"

class Editor
{
	BossStatus	 mBossStatus;		//! CSVファイルからステータスを受け取る変数
	BossCollInfo mBossCollInfo;		//!< CSVファイルから当たり判定情報を受け取る
	Boss*	   mBossInfo;			//!< ボスクラスの情報を入れる
	Player*	   mPlayerInfo;			//!< プレイヤークラスの情報を入れる

	bool	   mIsTestMode;		//!< テストモードフラグ

	VectorInt2  mcursorPos;		//!< 今のカーソル位置を入れる用
	VectorInt2	mMousePos;		//!< マウスの座標

	//! @note	Cameraクラスにはこのクラスに必要ない処理が多いため使わないでおく
	Vector3		mCameraPos;			//!< カメラの座標
	Vector3		mCameraLookPos;		//!< カメラの注視点

	MATRIX mMatCameraWorld;		//!< ワールド行列
	MATRIX mMatView;			//!< ビュー行列
	VECTOR mWorldUp;			//!< 世界の方向用

	// ----------------------------------------------------------
	// Private関数
	// ----------------------------------------------------------
	//! @brief ImGuiのメインの操作
	void GUIDraw();

	//! @brief ボスのコリジョン描画
	void CollDraw();

	//! @brief MATRIX型変数の初期化関数
	//! @param _initMat		初期化したいMATRIX型変数
	void SetMatIdentity(MATRIX& _initMat);

	//! @brief カメラの操作関数
	void CameraUpdate();

	//! @brief テストモード中の更新
	void TestModeUpdate();

	//! @brief テストモード終了時、各キャラクターをリセットする
	void ResetCharacter();

	//! @brief テストモードボタンのラベルを変更する
	//! @param _changeStr		変更後の文字列を入れる変数
	void ChangeButtonLabel(std::string& _changeStr);

	//! @brief 位置をわかりやすくするためのワイヤー描画
	void DrawWire();

public:
	Editor();		//!< コンストラクタ
	~Editor();		//!< デストラクタ

	//! @brief テストモードフラグをセットする
	void SetTestMode();	

	//! @brief テストモードフラグをチェックする
	//! @return		true : テストモード		false : エディットモード
	bool IsTestMode() const; 

	//! @brief ボスクラスのオブジェクトをポインタで取得する
	//! @param _bossObj		ボスクラスのオブジェクト
	void SetBossInfo(Boss* _bossObj);

	void SetPlayerInfo(Player* _playerObj);

	//! @brief  初期化
	void Init();

	//! @brief 更新
	void Update();

	//! @brief 描画
	void Draw();
};

//! @brief float型からstring型に変換する
//! @param nums_	変換したいfloat型の値
//! @return			変換したstring型の値
std::string ChengeNumFloatToString(float nums_);
