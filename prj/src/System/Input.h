#pragma once
//---------------------------------------------------------------------------
//! @file   Input.h
//! @brief  キーやマウスの処理
//---------------------------------------------------------------------------

// --------------------------------------------
// キーボード入力
// --------------------------------------------
//! @brief キーが押された瞬間を取得する
//! @param _keyNum	入力されたキーの番号
//! @return		true : 前フレームで入力がなく、現フレームで入力があった		false : それ以外
bool PushHitKey(int _keyNum);

//! @brief キーが離された瞬間を取得する
//! @param _keyNum	入力されたキーの番号
//! @return		true : 前フレームで入力があり、現フレームで入力がなかった	false : それ以外
bool ReleaseKey(int _keyNum);

// --------------------------------------------
// マウス入力
// --------------------------------------------
//! @brief マウスの入力をチェック（長押し）
//! @param _buttonNum	入力されたボタンの番号
//! @return		true : 押されている		false : 押されていない
bool CheckMouseInput(const int _buttonNum);

//! @brief マウスの入力をチェック（単押し）
//! @param _buttonNum	入力されたボタンの番号
//! @return		true : ボタンが押されていたフレーム数が１の時		false : それ以外の数値の時
bool PushMouseInput(int _buttonNum);

//! @brief マウスのX座標を取得する
//! @return マウスのX座標
int GetMouseX();

//! @brief マウスのY座標を取得する
//! @return	マウスのY座標
int GetMouseY();

//! @brief マウスのX座標の移動量を取得する
//! @return 前フレームと現フレームのX座標の差
int GetMouseMoveX();

//! @brief マウスのY座標の移動量を取得する
//! @return 前フレームと現フレームのY座標の差
int GetMouseMoveY();

// --------------------------------------------
// 共同関数
// --------------------------------------------
//! @brief 更新
void InputUpdate();

//! @brief 現フレームでの入力情報をコピーする
void MemorizeInputInfo();

int getframe();