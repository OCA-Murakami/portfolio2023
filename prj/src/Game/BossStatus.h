#pragma once
//---------------------------------------------------------------------------
//!	@file	BossStatud.h
//! @brief	CSVファイルからの値を受け取るための構造体と列挙体
//---------------------------------------------------------------------------
#include "../System/WinMain.h"
#include "../System/Class/Vector3.h"

//! @brief attackPowerを見やすくするための列挙体
enum BOSSATTACKTYPE
{
	Light,		//!< 弱攻撃
	Heavy,		//!< 強攻撃
	Combo,		//!< コンボ攻撃
	Jump,		//!< ジャンプ攻撃
	AttackMax,	//!< 最大数
};

//! @brief 攻撃用のカプセル座標の先端と根本用
enum BOSSATTACKCOLLTYPE
{
	Top,		//!< 先端
	Under,		//!< 根本
	AttackCollMax,	//!< 最大数
};

enum BOSSCOLLTYPE
{
	Body,			//!< ボディ
	FistTopX,		//!< 拳の先端X座標
	FistTopY,		//!< 拳の先端Y座標
	FistTopZ,		//!< 拳の先端Z座標
	FistUnderX,		//!< 拳の根本X座標
	FistUnderY,		//!< 拳の根本Y座標
	FistUnderZ,		//!< 拳の根本Z座標
	FistSize,		//!< 拳サイズ
	ClawTopX,		//!< 爪の先端X座標
	ClawTopY,		//!< 爪の先端Y座標
	ClawTopZ,		//!< 爪の先端Z座標
	ClawUnderX,		//!< 爪の根本X座標
	ClawUnderY,		//!< 爪の根本Y座標
	ClawUnderZ,		//!< 爪の根本Z座標
	ClawSize,		//!< 爪サイズ
	CollMax,		//!< 最大数
};

//! @brief [,]区切りで取得した各文字列の参照用
enum BOSSSTATUSTYPE
{
	HpMax,			//!< HP
	LightPower,		//!< 弱攻撃力
	HeavyPower,		//!< 強攻撃力
	ComboPower,		//!< コンボ攻撃力
	JumpPower,		//!< ジャンプ攻撃力
	MoveSpeed,		//!< 移動速度
	CollSize,		//!< 被攻撃時の当たり判定サイズ
	StatusMax,		//!< 最大数
};

//! @brief  CSVファイルから値を受け取る変数群
struct BossStatus
{
	float hpMax									 = 0.0f;					//!< HP
	float attackPower[BOSSATTACKTYPE::AttackMax] = {0.0f,0.0f,0.0f,0.0f};	//!< 攻撃種類
	float moveSpeed								 = 0.0f;					//!< 移動速度
	float collSize								 = 0.0f;					//!< 被攻撃時の当たり判定サイズ
};

//! @brief 当たり判定に使用する各変数を受け取る
struct BossCollInfo
{
	float   bodySize = 0.0f;								//!< 体のカプセルサイズ
	Vector3 fistPos[BOSSATTACKCOLLTYPE::AttackCollMax];		//!< 拳部分カプセルの座標
	float   fistSize = 0.0f;								//!< 拳部分カプセルのサイズ
	Vector3 clawPos[BOSSATTACKCOLLTYPE::AttackCollMax];		//!< 爪部分カプセルの座標
	float   clawSize = 0.0f;								//!< 爪部分カプセルのサイズ
};
