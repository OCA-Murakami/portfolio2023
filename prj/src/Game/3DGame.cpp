//---------------------------------------------------------------
// インクルード
//---------------------------------------------------------------
#include "../external/hlslpp/include/hlsl++.h"
#include "../System/WinMain.h"
#include "../System/FileLoader/FileStream.h"
#include "../System/Input.h"
#include "../System/Class/Editor.h"
#include "../System/Class/Vector3.h"
#include "../System/Class/Effect.h"
#include "Class/CharacterBase.h"

#include "../external/imgui/imgui.h"
#include "../external/imgui/imgui_impl_win32.h"
#include "../external/imgui/imgui_impl_dx11.h"

#include "Class/Stage.h"
#include "Class/Player.h"
#include "Class/Boss.h"
#include "Class/Camera.h"

#include <fstream>
#include <string>
#include <vector>
#include "../System/Class/Tutorial.h"

#include "3DGame.h"
#include "../System/Hit.h"


namespace
{
	//---------------------------------------------------------------
	// 列挙体
	//---------------------------------------------------------------
	//! @brief ゲームの画面を操作する列挙体
	enum SCENE
	{
		Title,		//!< タイトル
		Edit,		//!< エディット
		Training,	//!< トレーニング
		Movie,		//!< 敵の登場ムービー風
		Game,		//!< ゲーム
		Finish,		//!< 終了
	};

	//! @brief モデルデータの参照用
	enum MODELTYPE
	{
		PlayerData,		//!< プレイヤー
		BossData,		//!< ボス
		StageData,		//!< ステージ
		TrainingData,	//!< トレーニングステージ
		TypeMax,
	};

	//! @brief タイトル画面のボタンの描画モード
	enum TITLETELOPTYPE	
	{
		PushButton,		//!< ボタン入力待ち
		SelectMode,		//!< モード選択待ち
		TutorialSwitch,	//!< チュートリアル有無の選択待ち
		VIEWTYPEMAX,	//!< 最大数
	};

	//! @brief チュートリアルの有無
	enum TUTORIALSWITCH
	{
		On,
		Off,
		SWITCHMAX,
	};

	//---------------------------------------------------------------
	// 変数
	//---------------------------------------------------------------
	//-------------------------------------
	// ゲームの操作に使用する変数
	//-------------------------------------
	const float STAGERADIUS = 210.0f;		//!< ステージの移動範囲の半径
	int		    nowScene;					//!< ゲームのシーン用の変数
	int		    lockOnMakerImg;				//!< ロックオン中のマーカーの画像データ
	bool	    isDebug;					//!< デバッグモードのフラグ
	bool		isTutorial;					//!< チュートリアルの有無

	//-------------------------------------
	// タイトル画面の操作で使用する変数
	//-------------------------------------
	const int	ALPHAHMOVESPEED = 8;	//!< 透明度の増減の速度
	const int	FADEVALUESPEED = 5;		//!< フェードアウトの速度
	int			titleTelopMode;			//!< テロップ表示モード
	int			screenFadeValue;		//!< 画面のフェードイン、フェードアウトに使用するボックスの透明値
	int			titleLogoImg;			//!< タイトルロゴの画像データを入れる
	int			alphaValue;				//!< テロップやカーソルの透明度
	int			selectCursor;			//!< タイトル画面で使用するカーソルの番号
	bool		isViewingMode;			//!< モード選択が表示されているか
	bool		isRisingAlphaValue;		//!< 透明度の増減フラグ
	bool		isFadeIn  = false;		//!< フェードインフラグ
	bool		isFadeOut = false;		//!< フェードアウトフラグ

	//-------------------------------------
	// シャドウマップに使う変数
	//-------------------------------------
	const VectorInt2 shadowSize = {1024,1024};		//!< シャドウマップのサイズ

	//-------------------------------------
	// マウスでの画面操作に使用する変数
	//-------------------------------------
	Vector2 gameModeRange;		//!< 「GameMode」文字列の選択範囲
	Vector2 editModeRange;		//!< 「EditMode」文字列の選択範囲

	//-------------------------------------
	// クラスのオブジェクト
	//-------------------------------------
	Camera    gameCamera;	//!< ゲームを映すカメラ
	Stage     gameStage;	//!< ステージクラス
	Player    player;		//!< プレイヤークラス
	Boss      boss;			//!< ボスクラス
	Editor    editor;		//!< エディター
	Tutorial  tutorial;		//!< チュートリアル

	//-------------------------------------
	// ファイルパス系
	//-------------------------------------
	//! 使用するモデルのファイルのパスを入れる
	const std::string modelFilePaths[MODELTYPE::TypeMax] =
	{
		"data/model/Player/Player.mv1",
		"data/model/Boss/Boss.mv1",
		"data/model/Stage/Stage.mv1",
		"data/model/Stage/TrainingStage.mv1",
	};

	const std::string fontPath			 = "data/font/phantom.ttf";			//!< フォントデータ
	const std::string fontName			 = "Phantom of the Opera";			//!< フォント名
	const std::string logoImgPath		 = "data/image/TitleLogo.png";		//!< タイトルロゴ画像データー
	const std::string targetMakerImgPath = "data/image/TargetMarker.png";	//!< ターゲットマーカーデータ
	const std::string tutorialTelopPath  = "data/text/TutorialTelops.txt";	//!< チュートリアルのテロップテキスト

	//! ステージクラスに渡しやすいよう別変数に格納
	const std::string stagePaths[2] = { modelFilePaths[MODELTYPE::StageData],
									    modelFilePaths[MODELTYPE::TrainingData] };

	//-------------------------------------
	// 攻撃の時に使う変数
	//-------------------------------------
	float hitStop;			//!< ヒットストップ倍率
	float hitStopTimer;		//!< ヒットストップが効いてる時間
}

//---------------------------------------------------------------
// 関数
//---------------------------------------------------------------
// 各キャラクターの攻撃判定の発生をチェックする関数
bool IsAttackSituation(CharacterBase& _attacker)
{
	return _attacker.IsAttack() && _attacker.IsAtkJudge() && !_attacker.IsAtkSend();
}

// キャラクター同士がぶつかったときに押し返す計算
// chara1が軽い方
void CharaRebound(CharacterBase& chara1, CharacterBase& chara2)
{
	Vector3 vecCenter = (chara1.GetPos() + chara2.GetPos()) / 2.0f;
	Vector3 vecDir = chara2.GetPos() - chara1.GetPos();
	vecDir.y = 0.0f;
	vecDir.SetLength((6.0f + 18.0f) / 2.0f);
	Vector3 newPos = vecCenter - vecDir;
	chara1.SetPos(newPos);
}

// タイトル画面でのカーソルの位置から次の画面を決定させる
void SelectNextScene(const int _cursorValue)
{
	if (_cursorValue == SCENE::Training)
	{
		nowScene = SCENE::Training;
	}

	else if (_cursorValue == SCENE::Movie)
	{
		nowScene = SCENE::Movie;
	}

	if (_cursorValue == SCENE::Edit)
	{
		nowScene = SCENE::Edit;
		// エディターにボスクラスの情報を渡す
		editor.Init();
	}
}

bool CheckBoxRange(Vector2 _modeBoxPos, VectorInt2 _mousePos, VectorInt2 _size)
{
	if (_mousePos.x <= _modeBoxPos.x)
		return false;

	if (_mousePos.x >= _modeBoxPos.x + _size.x)
		return false;

	if (_mousePos.y <= _modeBoxPos.y)
		return false;

	if (_mousePos.y >= _modeBoxPos.y + _size.y)
		return false;

	return true;
}

bool CheckModeStringRange(Vector2 _modePos, VectorInt2 _mousePos)
{

	if (CheckBoxRange(_modePos, _mousePos, { 220, 42 }))
	{
		return true;
	}

	return false;
}

void DrawFillBoxA(BoxInt _pos, int _alphaValue, unsigned int _colorValue)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _alphaValue);
	DrawFillBox(_pos.xTop, _pos.yTop, _pos.xTop + _pos.xUnder, _pos.yTop + _pos.yUnder, _colorValue);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

void DrawStringA(VectorInt2 _pos, const TCHAR* _string, int _alphaValue, unsigned int _colorValue)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _alphaValue);
	DrawString(_pos.x, _pos.y, _string, _colorValue);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

void TitleTelopUpdate()
{
	switch (titleTelopMode)
	{
		// -------------------------------------------
		// エンターキー入力待ち状態
		// -------------------------------------------
	case TITLETELOPTYPE::PushButton:
		// モード選択待ち状態へ
		if (PushHitKey(KEY_INPUT_RETURN) || PushMouseInput(MOUSE_INPUT_LEFT))
		{
			titleTelopMode = TITLETELOPTYPE::SelectMode;
		}

		break;

		// -------------------------------------------
		// モード選択待ち状態
		// -------------------------------------------
	case TITLETELOPTYPE::SelectMode:
		// 上下キーで選択カーソルの移動
		if (PushHitKey(KEY_INPUT_DOWN) || PushHitKey(KEY_INPUT_UP))
		{
			if (selectCursor == SCENE::Training)
			{
				selectCursor = SCENE::Edit;
			}
			else
			{
				selectCursor = SCENE::Training;
			}
		}

		if (PushHitKey(KEY_INPUT_RETURN))
		{
			if (selectCursor == SCENE::Training)
			{
				SetFadeOut();
				//titleTelopMode = TITLETELOPTYPE::TutorialSwitch;
			}
			// エディットモードへ遷移
			else if (selectCursor == SCENE::Edit)
			{
				SetFadeOut();
			}
		}

		break;

		// -------------------------------------------
		// チュートリアルの有無選択待ち状態
		// -------------------------------------------
	case TITLETELOPTYPE::TutorialSwitch:
		// 上下キーで選択カーソルの移動
		if (PushHitKey(KEY_INPUT_DOWN) || PushHitKey(KEY_INPUT_UP))
		{
			isTutorial = !isTutorial;
		}

		if (PushHitKey(KEY_INPUT_RETURN))
		{
			SetFadeOut();
		}

		if (PushHitKey(KEY_INPUT_ESCAPE))
		{
			titleTelopMode = TITLETELOPTYPE::SelectMode;
		}

		break;

	default:
		break;
	}
}

void TitleTelopDraw()
{
	int cursorPosY;

	switch (titleTelopMode)
	{
	case TITLETELOPTYPE::PushButton:

		DrawStringA({ 425,(WINDOW_H / 2) + 90 }, "Push Enter Key", alphaValue, GetColor(255, 255, 255));

		break;

	case TITLETELOPTYPE::SelectMode:
		// モード選択カーソルの移動判定
		cursorPosY = (selectCursor == SCENE::Training ? (WINDOW_H / 2) + 142 : (WINDOW_H / 2) + 228);

		// カーソル用ボックスの描画
		DrawFillBoxA({ 445,365, cursorPosY,62 }, alphaValue,GetColor(255,255,255));

		// モードの文字列
		DrawFormatString(448, (WINDOW_H / 2) + 150, GetColor(255, 255, 255), "Game Mode");
		DrawFormatString((WINDOW_W / 2) - 160, (WINDOW_H / 2) + 230, GetColor(255, 255, 255), "Edit Mode");

		break;

	case TITLETELOPTYPE::TutorialSwitch:
		cursorPosY = (isTutorial ? (WINDOW_H / 2) + 142 : (WINDOW_H / 2) + 228);

		// 選択カーソルの透明度と座標移動の処理
		DrawFillBoxA({ 445,365,cursorPosY,cursorPosY + 62 }, alphaValue,GetColor(255,255,255));

		DrawFormatString(448, (WINDOW_H / 2) + 150, GetColor(255, 255, 255), "Tutorial ON");
		DrawFormatString(448, (WINDOW_H / 2) + 230, GetColor(255, 255, 255), "Tutorial OFF");

		break;
	}
}

void FadeUpdate()
{
	FadeInUpdate();
	FadeOutUpdate();
}

void FadeInUpdate()
{
	if (!IsFadeIn()) 
		return;

	screenFadeValue -= 5;
	if (IsfinishFadeIn())
	{
		screenFadeValue = 0;
		isFadeIn = false;
	}
}

void FadeOutUpdate()
{
	if (!IsFadeOut())
		return;

	screenFadeValue += 5;
	if (IsFinishFadeOut())
	{
		screenFadeValue = 255;
		isFadeOut = false;
	}
}

void SetFadeOut()
{
	if (IsFadeIn()) return;
	isFadeOut = true;
}

void ReleaseFadeOut()
{
	isFadeOut = false;
}

void SetFadeIn()
{
	if (IsFadeOut()) return;
	isFadeIn = true;
}

void ReleaseFadeIn()
{
	isFadeIn = false;
}

bool IsFadeOut()
{
	return isFadeOut;
}

bool IsFadeIn()
{
	return isFadeIn;
}

bool IsFinishFadeOut()
{
	if (screenFadeValue >= 255)
	{
		return true;
	}

	return false;
}

bool IsfinishFadeIn()
{
	if (screenFadeValue <= 0)
	{
		return true;
	}

	return false;
}

// プレイヤーの攻撃行動の処理
void PlayerAttack()
{
	// 無敵時間中は何もしない
	if (boss.IsInvincible())
	{
		return;
	}

	// ディフューズカラーを赤く点滅させる
	boss.SetDamage();
	// ダメージ処理
	boss.SetDamage(player.GetPower());
	player.SetAtkSend();
	player.SetEffect();
	hitStopTimer = 10.0f;
	hitStop = 0.0f;

	// プレイヤーの攻撃がコンボの最終段であればボスを吹き飛ばす
	if (player.IsLastComboAttack())
	{
		boss.SetStateBlowOff();
		return;
	}

	// ボスが防御中であれば攻撃が弾かれる
	if (boss.IsGuard())
	{
		player.SetStateRepell();
		boss.SetGuardToAttack();
		return;
	}

}

void BossAttack()
{
	if (player.IsInvincible())
	{
		return;
	}

	player.SetDamage(boss.GetPower());
	player.SetStateBlowOff();
	player.SetEffect();
	boss.SetAtkSend();
}

//---------------------------------------------------------------
// ゲーム操作関数
//---------------------------------------------------------------
// 初期化
void GameInit()
{

	nowScene = SCENE::Title;
	lockOnMakerImg = LoadGraph(targetMakerImgPath.c_str());
	isDebug = false;
	titleLogoImg = LoadGraph(logoImgPath.c_str());
	AddFontResourceA(fontPath.c_str());
	ChangeFont(fontName.c_str());

	// 各オブジェクトの初期化
	player.Init(modelFilePaths[MODELTYPE::PlayerData], STAGERADIUS);
	gameStage.Init(stagePaths);
	boss.Init(modelFilePaths[MODELTYPE::BossData]);

	// 各々のクラスで使う別クラスの情報を渡す
	player.SetTargetInfo(&boss);
	boss.SetTargetInfo(&player);
	gameCamera.SetPlayerInfo(&player);
	gameCamera.SetTargetInfo(&boss);

	// カメラの初期化
	gameCamera.Init();

	// エディットモードで使用する情報を渡す
	editor.SetPlayerInfo(&player);
	editor.SetBossInfo(&boss);

	tutorial.Init(tutorialTelopPath);

	hitStop      = 1.0f;
	hitStopTimer = 0.0f;

	screenFadeValue	   = 0;
	alphaValue		   = 255;
	titleTelopMode	   = TITLETELOPTYPE::PushButton;
	selectCursor	   = SCENE::Training;
	isTutorial		   = true;
	isViewingMode	   = false;
	isRisingAlphaValue = false;

	// モード選択の文字列をマウスでいじれるようにしてる
	gameModeRange.x = (WINDOW_W / 2) - 120;
	gameModeRange.y = (WINDOW_H / 2) + 148;
	editModeRange.x = (WINDOW_W / 2) - 120;
	editModeRange.y = (WINDOW_H / 2) + 198;

}

// ゲームをもう1度プレイするために必要なものだけを再初期化
void ReInit()
{
	nowScene       = SCENE::Title;
	titleTelopMode = TITLETELOPTYPE::PushButton;
	selectCursor   = SCENE::Training;
	isTutorial	   = true;

	gameStage.Init(stagePaths);
	player.ReInit();
	boss.ReInit();
	gameCamera.Init();

	hitStop		 = 1.0f;
	hitStopTimer = 0.0f;
}

// 更新
void GameUpdate()
{	
	// シーンごとの更新処理
	switch (nowScene)
	{
		// ----------------------------------------------------
		// タイトルシーン
		// ----------------------------------------------------
	case SCENE::Title:
		// 「Push Enter Key」文字列の点滅処理
		if (isRisingAlphaValue)
		{
			alphaValue += ALPHAHMOVESPEED;
			if (alphaValue >= 255)
			{
				isRisingAlphaValue = !isRisingAlphaValue;
			}
		}
		else
		{
			alphaValue -= ALPHAHMOVESPEED;
			if (alphaValue < 0)
			{
				isRisingAlphaValue = !isRisingAlphaValue;
			}
		}

		// キー入力で表示するテロップのモード切替
		TitleTelopUpdate();

		// フェード値の更新
		FadeUpdate();

		// 画面がフェードアウトしたら選択した画面へ
		if (IsFinishFadeOut())
		{
			isFadeIn = true;
			SelectNextScene(selectCursor);
		}

		tutorial.Update();

		break;

		// ----------------------------------------------------
		// エディットシーン
		// ----------------------------------------------------
	case SCENE::Edit:

		// 画面移動時のフェード処理
		FadeUpdate();

		if (IsFinishFadeOut())
		{
			isFadeOut = false;
			isFadeIn = true;
			ReInit();
		}

		// 更新
		editor.Update();

		// フェードアウトスイッチをセット
		if (PushHitKey(KEY_INPUT_ESCAPE))
		{
			if (screenFadeValue <= 0)
			{
				isFadeOut = true;
			}

		}

		break;

		// ----------------------------------------------------
		// トレーニングシーン
		// ----------------------------------------------------
	case SCENE::Training:

		FadeInUpdate();

		gameCamera.Update(isDebug);

		// カメラの水平方向から進行方向を決める
		player.CopyCameraHAngle(gameCamera.GetAngleHorizontal());
		player.Update(hitStop);

		// ワープゾーンとプレイヤーの当たり判定
		if (CheckWarpZoneHit(player.GetPos(), gameStage.GetEffectWarpPos(), gameStage.GetWarpRange()))
		{
			player.ReInit();
			gameCamera.MovieInit();
			nowScene = SCENE::Movie;
		}

		break;

		// ----------------------------------------------------
		// ムービーシーン
		// ----------------------------------------------------
	case SCENE::Movie:
		if (isFadeIn)
		{
			screenFadeValue -= 5;
			if (screenFadeValue <= 0)
			{
				screenFadeValue = 0;
				isFadeIn = false;
			}
		}
	
		gameCamera.MovieUpdate();
		boss.MovieUpdate();

		// ボスが着地していたらカメラを揺らす
		if (boss.IsGroundMovie())
		{
			gameCamera.SetMovieShakeTime();
		}

		// カメラ内のムービー風演出が終了したら全てのムービー風演出が終了したことになる
		if (gameCamera.IsFinishMovie())
		{
			boss.SetFinishMovie();
			nowScene = SCENE::Game;
		}

		break;

		// ----------------------------------------------------
		// ゲームシーン
		// ----------------------------------------------------
	case SCENE::Game:

		// F1、F2キーでデバッグモードの切り替え
		if (PushHitKey(KEY_INPUT_F1)) isDebug = !isDebug;

		// --------------------------------------
		// カメラの処理
		// --------------------------------------
		// カメラの水平角度をプレイヤークラスにコピーする
		player.CopyCameraHAngle(gameCamera.GetAngleHorizontal());
		// 更新処理
		gameCamera.Update(isDebug);

		// --------------------------------------
		// ヒットストップのタイマーの処理
		// --------------------------------------
		if (hitStop < 1.0f)	hitStopTimer--;

		if (hitStopTimer < 0)
		{
			hitStopTimer = 0.0f;
			hitStop = 1.0f;
		}

		// --------------------------------------
		// プレイヤー関連の処理
		// --------------------------------------
		player.Update(hitStop);

		// 敗北判定
		if (player.IsDeath()) nowScene = SCENE::Finish;

		// --------------------------------------
		// ボス関連の処理
		// --------------------------------------
		boss.Update(hitStop);

		// 死亡判定
		if (boss.IsDeath()) nowScene = SCENE::Finish;

		// --------------------------------------
		// 当たり判定の処理
		// --------------------------------------
		// キャラクター同士のすり抜け防止
		if (CheckBallHit(player.GetPos(), 6.0f, boss.GetPos(), 18.0f))
		{
			CharaRebound(player, boss);
			// ボスを攻撃状態へ
			if (boss.IsSeePlayer() && !boss.IsGuard())
			{
				boss.ChoiseAttackType();
			}
		} 

		// ---------------------------------
		// プレイヤーからボスへの当たり判定
		// ---------------------------------
		if (CheckAttackHit(player, boss, isDebug))
		{
			if (IsAttackSituation(player))
			{
				PlayerAttack();
			}
		}

		// ----------------------------------
		// ボスからプレイヤーへの当たり判定
		// ---------------------------------
		// 右腕での攻撃
		if (CheckBossFistAttack(boss, player, isDebug))
		{
			if (IsAttackSituation(boss))
			{
				BossAttack();
			}
		}

		// 左腕での攻撃
		if (CheckBossClawAttack(boss, player, isDebug))
		{
			if (IsAttackSituation(boss))
			{
				BossAttack();
			}
		}

		// 両腕での攻撃
		if (CheckBossComboAttack(boss, player, isDebug))
		{
			if (IsAttackSituation(boss))
			{
				BossAttack();
			}
		}

		break;

		// ----------------------------------------------------
		// ゲーム終了シーン
		// ----------------------------------------------------
	case SCENE::Finish:

		// キー入力で各値を初期化してタイトルへ戻る
		if (PushHitKey(KEY_INPUT_RETURN) || PushMouseInput(MOUSE_INPUT_LEFT))
		{
			SetFadeOut();
		}

		FadeOutUpdate();
		
		if (screenFadeValue >= 255)
		{
			isFadeOut = false;
			isFadeIn = true;
			ReInit();
		}
		break;
	}

}

// 描画
void GameDraw()
{
	// ---------------------------------------
	// シャドウマップへの描画
	// ---------------------------------------
	// 描画準備
	ShadowMap_DrawSetup(handleHandle());

	// それぞれのモデルをシャドウマップへ描画
	gameStage.StageDraw();
	player.Draw();
	boss.Draw();

	// シャドウマップへの描画を終了
	ShadowMap_DrawEnd();

	// ---------------------------------------
	// switch内で使用する変数の宣言
	// ---------------------------------------
	const int LOGOFONTSIZE	  = 84;		//!< タイトルロゴ用のサイズ
	const int MODEFONTSIZE	  = 42;		//!< タイトルロゴ用のサイズ
	const int DEFAULTFONTSIZE = 14;		//!< DXライブラリの初期サイズ

	gameCamera.Draw();
	if (nowScene != Edit && nowScene != Training)
	{
		gameStage.StageDraw();
	}

	//Trainingステージが描画できるようになったから、Training　＝＞　ゲームステージに行けるようにする


	// シーンごとの描画処理
	switch (nowScene)
	{
		// ----------------------------------------------------
		// タイトルシーン
		// ----------------------------------------------------
	case SCENE::Title:
		SetFontSize(62);

		TitleTelopDraw();

		SetFontSize(DEFAULTFONTSIZE);

		if (isDebug)
		{
			DrawLine(WINDOW_W / 2, 0, WINDOW_W / 2, WINDOW_H, GetColor(0, 0, 255));
			DrawLine(0, WINDOW_H / 2, WINDOW_W, WINDOW_H / 2, GetColor(255, 0, 0));
		}

		DrawGraph((WINDOW_W / 2) - 450, (WINDOW_H / 2) - 140, titleLogoImg, TRUE);

		break;

		// ----------------------------------------------------
		// エディットシーン
		// ----------------------------------------------------
	case SCENE::Edit:
		editor.Draw();

		break;

		// ----------------------------------------------------
		// トレーニングシーン
		// ----------------------------------------------------
	case SCENE::Training:
		gameStage.TrainingDraw();

		player.Draw();
		player.PostDraw();

		break;

		// ----------------------------------------------------
		// ムービーシーン
		// ゲームシーン
		// ----------------------------------------------------
	case SCENE::Movie:
	case SCENE::Game:
		// 描画に使用するシャドウマップを設定
		SetUseShadowMap(0, handleHandle());

		boss.Draw();
		boss.DamageDraw();
		player.Draw();

		if (player.IsTargetLook())
		{
			// マーカーの描画
			Vector3 makerPos = boss.GetPos();
			makerPos.y += 30.0f;
			DrawGraph3D(makerPos.x, makerPos.y, makerPos.z, lockOnMakerImg, TRUE);
		}

		// シャドウマップの設定を解除
		SetUseShadowMap(0, -1);

		// ステージの描画
		DrawSphere3D(VGet(0.0f, 0.0, 0.0f), STAGERADIUS, 32, GetColor(128, 24, 24), GetColor(128, 24, 24),FALSE);

		// 一番前に描画したいもの（UIなど）
		player.PostDraw();
		boss.PostDraw();

		break;

		// ----------------------------------------------------
		// ゲーム終了シーン
		// ----------------------------------------------------
	case SCENE::Finish:

		SetFontSize(LOGOFONTSIZE);
		// 勝敗によって描画する文字列を変える
		if (player.IsDeath())
		{
			DrawFormatString((WINDOW_W / 2) - 200, (WINDOW_H / 2) - 100, GetColor(16, 16, 255), " L O S E ");
		}
		if (boss.IsDeath())
		{
			DrawFormatString((WINDOW_W / 2) - 150, (WINDOW_H / 2) - 100, GetColor(255, 16, 16), " W I N ");
		}
		SetFontSize(DEFAULTFONTSIZE);
		DrawFormatString((WINDOW_W / 2) - 52, (WINDOW_H / 2) + 70, GetColor(255, 255, 255), "Push Any Key");

		break;
	}

}

// 最前面に描画したいものを入れる
void GamePostDraw()
{

	// ムービー風演出時は画面の上下に黒い板を描画して
	// ユーザーにムービー中であることを分かりやすくする
	if (nowScene == Movie)
	{
		DrawFillBox(0,            0, WINDOW_W,       50, GetColor(0, 0, 0));
		DrawFillBox(0, WINDOW_H - 50, WINDOW_W, WINDOW_H, GetColor(0, 0, 0));
	}

	// 画面フェードアウト用のボックス
	DrawFillBoxA({ 0,WINDOW_W,0,WINDOW_H }, screenFadeValue, GetColor(0, 0, 0));
}

// 終了
void GameExit()
{
	gameCamera.Exit();
	gameStage.Exit();
	player.Exit();
	boss.Exit();
}