#pragma once
//---------------------------------------------------------------------------
//!	@file	3DGame.h
//! @brief	ゲーム全体の操作
//---------------------------------------------------------------------------

bool CheckModeStringRange(Vector2 _modePos, VectorInt2 _mousePos);

// -------------------------------------------------
// アルファ値を適用した描画関数
// -------------------------------------------------
//! @brief アルファ値を適用したボックス描画
//! @param _pos 座標
//! @param _alphaValue	アルファ値 
//! @param _colorValue  カラー値
void DrawFillBoxA(BoxInt _pos, int _alphaValue, unsigned int _colorValue);

//! @brief アルファ値を適用した文字列描画
//! @param _pos 座標
//! @param _string 描画したい文字列
//! @param _alphaValue	アルファ値 
//! @param _colorValue	カラー値
void DrawStringA(VectorInt2 _pos, const TCHAR* _string, int _alphaValue, unsigned int _colorValue);

// -------------------------------------------------
// タイトル画面のテロップ関数
// -------------------------------------------------
void TitleTelopUpdate();		//!< タイトル画面のテロップ更新関数
void TitleTelopDraw();			//!< タイトル画面のテロップ描画関数

// -------------------------------------------------
// 画面移動時のフェード処理関数
// -------------------------------------------------
void FadeUpdate();			//!< フェードイン、フェードアウトの更新処理
// --------------------------------
// 画面移動時のフェードイン処理関数
// --------------------------------
void FadeInUpdate();		//!< フェードインのみ更新
void SetFadeIn();			//!< フェードイン開始
void ReleaseFadeIn();		//!< フェードイン解除
bool IsFadeIn();			//!< フェードインチェック
bool IsfinishFadeIn();		//!< フェードイン終了チェック
// --------------------------------
// 画面移動時のフェードアウト処理関数
// --------------------------------
void FadeOutUpdate();		//!< フェードアウトのみ更新
void SetFadeOut();			//!< フェードアウト開始
void ReleaseFadeOut();		//!< フェードアウト解除
bool IsFadeOut();			//!< フェードアウトチェック
bool IsFinishFadeOut();		//!< フェードアウト終了チェック

// -------------------------------------------------
// 各キャラクターの攻撃処理をまとめた関数
// -------------------------------------------------
void PlayerAttack();
void BossAttack();

// -------------------------------------------------
// ゲーム操作関数
// -------------------------------------------------
void GameInit();		//!< 初期化
void GameUpdate();		//!< 更新
void GameDraw();		//!< 描画
void GamePostDraw();	//!< 最前面に描画したいものを入れる
void GameExit();		//!< 終了