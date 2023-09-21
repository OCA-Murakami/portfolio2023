#pragma once
//------------------------------------------------
// 当たり判定に使う関数
//------------------------------------------------
#include "../System/Cast.h"
#include "../Game/Class/Boss.h"
#include "../Game/Class/Player.h"

// ---------------------------------------------------------------------------
// オブジェクトの当たり判定
// ---------------------------------------------------------------------------

//! 球同士の当たり判定
//! @param &_p0 １つ目の球の座標
//! @param  _r0 １つ目の球の半径
//! @param &_p1 ２つ目の球の座標
//! @param  _r1 ２つ目の球の半径
bool CheckBallHit(const Vector3& _p0, const float _r0,
				  const Vector3& _p1, const float _r1);

//! @brief トレーニングステージのワープゾーンとプレイヤーの当たり判定
//! @param _playerPos		プレイヤーの座標
//! @param _warpCenterPos	ワープゾーンの中心座標
//! @return true : ゾーンに触れている			false : ゾーンに触れていない
bool CheckWarpZoneHit(const Vector3 _playerPos, const Vector3 _warpCenterPos, const float _warpRange);

// ---------------------------------------------------------------------------
// 攻撃関連の当たり判定
// ---------------------------------------------------------------------------

//! @brief プレイヤーの攻撃判定の関数
//! @param _attacker		プレイヤーのオブジェクト
//! @param _diffenPos		敵の座標
//! @param _isDebug			当たり判定のデバッグ描画
//! @return					true : 当たってる	false : 当たってない
bool CheckAttackHit(Player& _attacker, Boss& _diffender, bool _isDebug);

//! @brief ボスの弱攻撃判定の関数
//! @param _attacker		ボスのオブジェクト
//! @param _diffender		プレイヤーのオブジェクト
//! @param _isDebug			デバッグ描画をするかどうか
//! @return					true : 当たっている		false : 当たっていない
bool CheckBossFistAttack(Boss& _attacker, CharacterBase& _diffender, bool _isDebug);

//! @brief ボスの強攻撃の当たり判定関数
//! @param _attacker	ボスのオブジェクト
//! @param _diffender	プレイヤーのオブジェクト
//! @param _isDebug		デバッグ描画について
//! @return				true : 当たっている			false : 当たっていない
bool CheckBossClawAttack(Boss& _attacker, CharacterBase& _diffender, bool _isDebug);

//! @brief ボスのコンボ攻撃の当たり判定関数
//! @param _attacker	ボスのオブジェクト
//! @param _diffender	プレイヤーのオブジェクト
//! @param _isDebug		デバッグ描画について
//! @return				true : 当たっている			false : 当たっていない
bool CheckBossComboAttack(Boss& _attacker, CharacterBase& _diffender, bool _isDebug);

bool CheckBossJumpAttack(Boss& _attacker, CharacterBase& _diffender, float _impactSize);