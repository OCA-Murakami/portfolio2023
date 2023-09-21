//---------------------------------------------------------------------------
//! @file   Editor.cpp
//! @brief  エディター
//---------------------------------------------------------------------------
#include "../Cast.h"
#include "../../external/imgui/imgui.h"
#include "../../external/imgui/imgui_impl_win32.h"
#include "../../external/imgui/imgui_impl_dx11.h"
#include <math.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "../WinMain.h"
#include "../Input.h"
#include "../FileLoader/CsvLoading.h"
#include "../Input.h"
#include "Editor.h"

// ------------------------------------------------------------------------------
// private関数
// ------------------------------------------------------------------------------
// ImGuiのメイン処理
void Editor::GUIDraw()
{
	// ImGuiのウィンドウの座標を設定
	int windowX = 0;
	int windowY = 0;
	GetWindowPosition(&windowX, &windowY);
	windowX += WINDOW_W;

	// ウィンドウの作成
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
	ImGui::SetNextWindowPos(ImVec2(static_cast<float>(windowX), static_cast<float>(windowY)));
	ImGui::SetNextWindowSize(ImVec2(500, 800), ImGuiCond_Once);

	// ImGuiの開始
	ImGui::Begin("Edit Mode");

	// -------------------------------------------------------------
	// データの書き込みの処理
	// -------------------------------------------------------------
	if (ImGui::Button("Save")) {

		// CSVファイルの行数
		const int LINEMAX = 2;

		// -------------------------------------------
		// ステータスの保存
		// -------------------------------------------
		std::ofstream statusFile("data/csv/BossStatus.csv");

		for (int i = 0; i < LINEMAX; ++i)
		{
			std::string str;
			if (i == 0)
			{
				// 各ラベルを１行目に書き込む
				str = "HP,弱攻撃力,強攻撃力,コンボ攻撃力,ジャンプ攻撃力,移動速度,当たり判定のサイズ";
				statusFile << str << std::endl;
			}

			if (i != 0)
			{
				// それぞれの値を２行目に書き込む
				str = ChengeNumFloatToString(mBossStatus.hpMax) + ',' + ChengeNumFloatToString(mBossStatus.attackPower[BOSSATTACKTYPE::Light]) + ',' +
					ChengeNumFloatToString(mBossStatus.attackPower[BOSSATTACKTYPE::Heavy]) + ',' + ChengeNumFloatToString(mBossStatus.attackPower[BOSSATTACKTYPE::Combo]) + ',' +
					ChengeNumFloatToString(mBossStatus.attackPower[BOSSATTACKTYPE::Jump]) + ',' + ChengeNumFloatToString(mBossStatus.moveSpeed) + ',' + ChengeNumFloatToString(mBossStatus.collSize);
				statusFile << str << std::endl;
			}
		}

		statusFile.close();

		// -------------------------------------------
		// コリジョンの保存
		// -------------------------------------------
		std::ofstream collFile("data/csv/BossColl.csv");

		for (int i = 0; i < LINEMAX; ++i)
		{
			std::string str;
			if (i == 0)
			{
				// 各ラベルを１行目に書き込む
				str = "ボディのサイズ,拳の先端X,拳の先端Y,拳の先端Z,拳の根本X,拳の根本Y,拳の根本Z,拳のサイズ,爪の先端X,爪の先端Y,爪の先端Z,爪の根本X,爪の根本Y,爪の根本Z,爪のサイズ,";
				collFile << str << std::endl;
			}

			if (i != 0)
			{
				// それぞれの値を２行目に書き込む
				str = ChengeNumFloatToString(mBossCollInfo.bodySize) + ',' +
					ChengeNumFloatToString(mBossCollInfo.fistPos[BOSSATTACKCOLLTYPE::Top].x)   + ',' + ChengeNumFloatToString(mBossCollInfo.fistPos[BOSSATTACKCOLLTYPE::Top].y)   + ',' + ChengeNumFloatToString(mBossCollInfo.fistPos[BOSSATTACKCOLLTYPE::Top].z)   + ',' +
					ChengeNumFloatToString(mBossCollInfo.fistPos[BOSSATTACKCOLLTYPE::Under].x) + ',' + ChengeNumFloatToString(mBossCollInfo.fistPos[BOSSATTACKCOLLTYPE::Under].y) + ',' + ChengeNumFloatToString(mBossCollInfo.fistPos[BOSSATTACKCOLLTYPE::Under].z) + ',' +
					ChengeNumFloatToString(mBossCollInfo.fistSize) + ',' +
					ChengeNumFloatToString(mBossCollInfo.clawPos[BOSSATTACKCOLLTYPE::Top].x)   + ',' + ChengeNumFloatToString(mBossCollInfo.clawPos[BOSSATTACKCOLLTYPE::Top].y)   + ',' + ChengeNumFloatToString(mBossCollInfo.clawPos[BOSSATTACKCOLLTYPE::Top].z) +   ',' +
					ChengeNumFloatToString(mBossCollInfo.clawPos[BOSSATTACKCOLLTYPE::Under].x) + ',' + ChengeNumFloatToString(mBossCollInfo.clawPos[BOSSATTACKCOLLTYPE::Under].y) + ',' + ChengeNumFloatToString(mBossCollInfo.clawPos[BOSSATTACKCOLLTYPE::Under].z) + ',' +
					ChengeNumFloatToString(mBossCollInfo.clawSize);
					collFile << str << std::endl;
			}
		}

		collFile.close();

	}

	// -------------------------------------------------------------
	// テストモードの処理
	// -------------------------------------------------------------
	// テストモード中かどうかでボタンに描画する文字列を変更する
	std::string buttonStr;
	ChangeButtonLabel(buttonStr);

	if (ImGui::Button(buttonStr.c_str()))
	{
		mIsTestMode = !mIsTestMode;

		// テストモード終了時はキャラクターのリセット
		if (!mIsTestMode)
		{
			ResetCharacter();
		}
	}

	// -------------------------------------------------------------
	// ステータス操作
	// -------------------------------------------------------------
	// ツリーの作成
	if (ImGui::TreeNode("Status"))
	{
		ImGui::SliderFloat("Hp", &mBossStatus.hpMax, 50.0f, 300.0f, "%.0f");
		ImGui::SliderFloat("LightAttackPower", &mBossStatus.attackPower[BOSSATTACKTYPE::Light], 1.0f, 50.0f, "%.0f");
		ImGui::SliderFloat("HeavyAttackPower", &mBossStatus.attackPower[BOSSATTACKTYPE::Heavy], 1.0f, 50.0f, "%.0f");
		ImGui::SliderFloat("ComboAttackPower", &mBossStatus.attackPower[BOSSATTACKTYPE::Combo], 1.0f, 50.0f, "%.0f");
		ImGui::SliderFloat("JumpAttackPower", &mBossStatus.attackPower[BOSSATTACKTYPE::Jump], 0.0f, 30.0f, "%.0f");
		ImGui::SliderFloat("MoveSpeed", &mBossStatus.moveSpeed, 0.0f, 3.0f, "%.2f");
		ImGui::SliderFloat("CollSize", &mBossStatus.collSize, 0.0f, 50.0f, "%.0f");

		// このツリーをポップする
		ImGui::TreePop();
	}

	// -------------------------------------------------------------
	// 当たり判定操作
	// -------------------------------------------------------------
	if (ImGui::TreeNode("Collision"))
	{
			ImGui::SliderFloat("Body", &mBossCollInfo.bodySize, 1.0f, 30.0f, "%.0f");

			if (ImGui::TreeNode("Fist"))
			{
				ImGui::SliderFloat("TopX",   &mBossCollInfo.fistPos[BOSSATTACKCOLLTYPE::Top].x, -100.0f, 100.0f, "%.0f");
				ImGui::SliderFloat("TopY",   &mBossCollInfo.fistPos[BOSSATTACKCOLLTYPE::Top].y, -100.0f, 100.0f, "%.0f");
				ImGui::SliderFloat("TopZ",   &mBossCollInfo.fistPos[BOSSATTACKCOLLTYPE::Top].z, -100.0f, 100.0f, "%.0f");
				ImGui::SliderFloat("UnderX", &mBossCollInfo.fistPos[BOSSATTACKCOLLTYPE::Under].x, -100.0f, 100.0f, "%.0f");
				ImGui::SliderFloat("UnderY", &mBossCollInfo.fistPos[BOSSATTACKCOLLTYPE::Under].y, -100.0f, 100.0f, "%.0f");
				ImGui::SliderFloat("UnderZ", &mBossCollInfo.fistPos[BOSSATTACKCOLLTYPE::Under].z, -100.0f, 100.0f, "%.0f");
				ImGui::SliderFloat("Size",   &mBossCollInfo.fistSize, 1.0f, 50.0f, "%.0f");
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Claw"))
			{
				ImGui::SliderFloat("TopX",   &mBossCollInfo.clawPos[BOSSATTACKCOLLTYPE::Top].x, -100.0f, 100.0f, "%.0f");
				ImGui::SliderFloat("TopY",   &mBossCollInfo.clawPos[BOSSATTACKCOLLTYPE::Top].y, -100.0f, 100.0f, "%.0f");
				ImGui::SliderFloat("TopZ",   &mBossCollInfo.clawPos[BOSSATTACKCOLLTYPE::Top].z, -100.0f, 100.0f, "%.0f");
				ImGui::SliderFloat("UnderX", &mBossCollInfo.clawPos[BOSSATTACKCOLLTYPE::Under].x, -100.0f, 100.0f, "%.0f");
				ImGui::SliderFloat("UnderY", &mBossCollInfo.clawPos[BOSSATTACKCOLLTYPE::Under].y, -100.0f, 100.0f, "%.0f");
				ImGui::SliderFloat("UnderZ", &mBossCollInfo.clawPos[BOSSATTACKCOLLTYPE::Under].z, -100.0f, 100.0f, "%.0f");
				ImGui::SliderFloat("Size",   &mBossCollInfo.clawSize, 1.0f, 50.0f, "%.0f");
				ImGui::TreePop();
			}

			ImGui::TreePop();
	}

	ImGui::End();
	ImGui::PopStyleColor();

}

void Editor::CollDraw()
{
	// --------------------------------------------
	// 拳部分のカプセル描画
	// --------------------------------------------
	{
		VECTOR fistPos[2];
		fistPos[0] = mBossCollInfo.fistPos[BOSSATTACKCOLLTYPE::Top].GetVECTOR();
		fistPos[1] = mBossCollInfo.fistPos[BOSSATTACKCOLLTYPE::Under].GetVECTOR();

		// 拳のマトリックスを取得
		int    attachFrame = MV1SearchFrame(mBossInfo->GetModel(), "mixamorig:RightHand");
		MATRIX rightArmMat = MV1GetFrameLocalWorldMatrix(mBossInfo->GetModel(), attachFrame);

		// 根本座標とマトリックスを乗算し、最終的な根本座標を取得
		float3 newFistPos = mul(float4(fistPos[1].x, fistPos[1].y, fistPos[1].z, 1), cast(rightArmMat)).xyz;

		// マトリックスを使用した座標変換を行い、最終的な先端座標を取得する
		VECTOR newDir = VTransformSR(fistPos[0], rightArmMat);

		DrawCapsule3D(cast(newFistPos), cast(newFistPos + float3(newDir.x, newDir.y, newDir.z)), mBossCollInfo.fistSize, 32, GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);
	}

	// --------------------------------------------
	// 爪部分のカプセル描画
	// --------------------------------------------
	{
		VECTOR clawPos[2];
		clawPos[0] = mBossCollInfo.clawPos[BOSSATTACKCOLLTYPE::Top].GetVECTOR();
		clawPos[1] = mBossCollInfo.clawPos[BOSSATTACKCOLLTYPE::Under].GetVECTOR();

		// 拳のマトリックスを取得
		int    attachFrame = MV1SearchFrame(mBossInfo->GetModel(), "mixamorig:LeftHand");
		MATRIX leftArmMat = MV1GetFrameLocalWorldMatrix(mBossInfo->GetModel(), attachFrame);

		// 根本座標とマトリックスを乗算し、最終的な根本座標を取得
		float3 newClawPos = mul(float4(clawPos[1].x, clawPos[1].y, clawPos[1].z, 1), cast(leftArmMat)).xyz;

		// マトリックスを使用した座標変換を行い、最終的な先端座標を取得する
		VECTOR newDir = VTransformSR(clawPos[0], leftArmMat);

		DrawCapsule3D(cast(newClawPos), cast(newClawPos + float3(newDir.x, newDir.y, newDir.z)), mBossCollInfo.clawSize, 32, GetColor(0, 255, 0), GetColor(0, 255 , 0), FALSE);
	}
}

// MATRIX型変数の初期化
void Editor::SetMatIdentity(MATRIX& _initMat)
{
	_initMat.m[0][0] = 1.0f;
	_initMat.m[1][1] = 1.0f;
	_initMat.m[2][2] = 1.0f;
	_initMat.m[3][3] = 1.0f;
}

// カメラの移動
void Editor::CameraUpdate()
{
	// ビュー行列からワールド行列を取得する
	mMatView = GetCameraViewMatrix();
	mMatCameraWorld = MInverse(mMatView);

	// 3方向の移動ベクトル
	const auto& copy = mMatCameraWorld;
	Vector3 right = { copy.m[0][0],copy.m[0][1],copy.m[0][2] };
	Vector3 top = { copy.m[1][0],copy.m[1][1],copy.m[1][2] };
	Vector3 front = { copy.m[2][0],copy.m[2][1],copy.m[2][2] };
	front *= -1;

	// マウスの移動量
	VectorInt2 cursorPos;
	GetMousePoint(&cursorPos.x, &cursorPos.y);

	// 今のカーソル座標を入れておく
	mcursorPos = cursorPos;

	// 移動差分
	float dx = static_cast<float>(cursorPos.x - mMousePos.x);
	float dy = static_cast<float>(cursorPos.y - mMousePos.y);

	// ウィンドウの情報を取得
	int windowX = 0;
	int windowY = 0;
	GetWindowPosition(&windowX, &windowY);

	// .exeの左上
	mcursorPos.x -= windowX;
	mcursorPos.y -= windowY;

	// 次の移動差分計算用に代入
	mMousePos = cursorPos;

	// 移動量
	Vector3 movDir = { 0.0f,0.0f,0.0f };

	// WASDで移動量を加減する
	if (CheckHitKey(KEY_INPUT_W))
	{
		movDir -= front;
	}

	if (CheckHitKey(KEY_INPUT_A))
	{
		movDir -= right;
	}

	if (CheckHitKey(KEY_INPUT_S))
	{
		movDir += front;
	}

	if (CheckHitKey(KEY_INPUT_D))
	{
		movDir += right;
	}

	// 座標と注視点を進める
	mCameraPos += movDir * 0.8f;
	mCameraLookPos += movDir * 0.8f;

	// マウスからの回転
	if (CheckMouseInput(MOUSE_INPUT_LEFT))
	{
		// 座標から注視点へのベクトル
		Vector3 dir = mCameraLookPos - mCameraPos;

		// 左右回転
		{
			// Y軸回転し、関数に渡せるようにキャストする
			MATRIX rotY = MGetRotY(dx * 0.005f);
			float4x4 rotY2 = cast(rotY);

			// ベクトルをfloat4型に変換
			VECTOR toDir = dir.GetVECTOR();
			float4 dir2 = cast(toDir);

			// 乗算してVector3型に戻す
			float3 vec3Dir;
			vec3Dir = mul(rotY2, float4(dir2.xyz, 0.0f)).xyz;
			dir.x = vec3Dir.x;
			dir.y = vec3Dir.y;
			dir.z = vec3Dir.z;
		}

		// 上下回転
		{
			// 角度回転し、あとで関数に渡すためにfloat4x4型に変換
			MATRIX   rotX = MGetRotAxis(right.GetVECTOR(), dy * 0.005f);
			float4x4 rotX2 = cast(rotX);

			// 座標から注視点のベクトルをfloat4型に変換
			VECTOR toDir2 = dir.GetVECTOR();
			float4 dir2 = cast(toDir2);

			// float3型で計算し、Vector3型に戻す
			float3 vec3Dir;
			vec3Dir = mul(rotX2, float4(dir2.xyz, 0.0f)).xyz;
			dir.x = vec3Dir.x;
			dir.y = vec3Dir.y;
			dir.z = vec3Dir.z;
		}

		// 注視点
		mCameraLookPos = mCameraPos + dir;
	}

	// 必要な値をVECTOR型に変換
	VECTOR vecFrom = mCameraPos.GetVECTOR();			// カメラの位置
	VECTOR vecLookat = mCameraLookPos.GetVECTOR();		// カメラの注視点
	VECTOR vecUp = VGet(0, 0, 0);					// カメラの上方向

	// 右手座標系のビュー行列を作成する
	CreateLookAtMatrixRH(&mMatView, &vecFrom, &vecLookat, &vecUp);
}

// テストモード中の更新
void Editor::TestModeUpdate()
{
	mBossInfo->Update(1.0f);

	// ボスの攻撃開始判定
	Vector3 vecBossToPlayer = mPlayerInfo->GetPos() - mBossInfo->GetPos();
	float length = vecBossToPlayer.GetLength();
	const float ATTACKRANGE = 15.0f;
	if (length < ATTACKRANGE)
	{
		mBossInfo->ChoiseAttackType();
	}

	// -------------------------------------------------------------
	// 各攻撃命令ボタン
	// -------------------------------------------------------------
	ImGui::SetNextWindowSize(ImVec2(200, 140), ImGuiCond_Once);
	ImGui::Begin("AttackButton");
	if (ImGui::Button("Light", ImVec2(180, 20)))
	{
		mBossInfo->SetStateLightAttack();
	}
	if (ImGui::Button("Heavy", ImVec2(180, 20)))
	{
		mBossInfo->SetStateHeavyAttack();
	}
	if (ImGui::Button("Combo", ImVec2(180, 20)))
	{
		mBossInfo->SetStateComboAttack();
	}
	if (ImGui::Button("Turn", ImVec2(180, 20)))
	{
		mBossInfo->SetStateTurnAttack();
	}
	ImGui::End();

}

void Editor::ResetCharacter()
{
	mPlayerInfo->EditInit();
	mBossInfo->EditInit();
}

// 舞台のワイヤー描画
void Editor::DrawWire()
{
	// 位置が分かりやすいように格子を描画
	const float LINEAREASIZE = 10000.0f;
	const int LINEMAX = 50;

	VECTOR Pos1;
	VECTOR Pos2;

	// z軸のライン
	Pos1 = VGet(-LINEAREASIZE / 2.0f, 0.0f, -LINEAREASIZE / 2.0f);
	Pos2 = VGet(-LINEAREASIZE / 2.0f, 0.0f, LINEAREASIZE / 2.0f);
	for (int i = 0; i <= LINEMAX; i++)
	{
		DrawLine3D(Pos1, Pos2, GetColor(255, 255, 255));
		Pos1.x += LINEAREASIZE / LINEMAX;
		Pos2.x += LINEAREASIZE / LINEMAX;
	}

	// x軸のライン
	Pos1 = VGet(-LINEAREASIZE / 2.0f, 0.0f, -LINEAREASIZE / 2.0f);
	Pos2 = VGet(LINEAREASIZE / 2.0f, 0.0f, -LINEAREASIZE / 2.0f);
	for (int i = 0; i < LINEMAX; i++)
	{
		DrawLine3D(Pos1, Pos2, GetColor(255, 255, 255));
		Pos1.z += LINEAREASIZE / LINEMAX;
		Pos2.z += LINEAREASIZE / LINEMAX;
	}

	// X軸、Y軸、Z軸の中心のライン
	DrawLine3D(VGet(500.0f, 3.0f, 0.0f), VGet(-500.0f, 0.0f, 0.0f), GetColor(255, 0, 0));
	DrawLine3D(VGet(0.0f, 500.0f, 0.0f), VGet(0.0f, 0.0f, 0.0f), GetColor(0, 255, 0));
	DrawLine3D(VGet(0.0f, 3.0f, 500.0f), VGet(0.0f, 0.0f, -500.0f), GetColor(0, 0, 255));
}

// テストモード中かどうかでラベルの文字列を変更する
void Editor::ChangeButtonLabel(std::string& _changeStr)
{
	// ボタンに描画する文字列の変更
	if (IsTestMode())
	{
		_changeStr = "FinishTest";
	}
	else
	{
		_changeStr = "StartTest";
	}
}

// ------------------------------------------------------------------------------
// public関数
// ------------------------------------------------------------------------------
// コンストラクタ
Editor::Editor() :
	mBossStatus{ -1.0f }, mBossCollInfo{ -1.0f }, mBossInfo(nullptr), mPlayerInfo(nullptr), mIsTestMode(false),
	mMousePos{ 0,0 }
{
}

// デストラクタ
Editor::~Editor()
{
}

// テストモードの開始
void Editor::SetTestMode()
{
	mIsTestMode = true;
}

// テストモードフラグのチェック
bool Editor::IsTestMode() const
{
	return mIsTestMode;
}

// ボスクラスの情報をセットする
void Editor::SetBossInfo(Boss* _bossObj)
{
	mBossInfo = _bossObj;
}

void Editor::SetPlayerInfo(Player* _playerObj)
{
	mPlayerInfo = _playerObj;
}

// 初期化
void Editor::Init()
{
	LoadStatus("data/csv/BossStatus.csv", mBossStatus);
	LoadCollData("data/csv/BossColl.csv", mBossCollInfo);

	mIsTestMode = false;
	mBossInfo->EditInit();
	mPlayerInfo->EditInit();

	mCameraPos.Set(-100.0f,50.0f,-100.0f);
	mCameraLookPos.Set(0.0f, 0.0f, 0.0f);

	SetMatIdentity(mMatCameraWorld);
	SetMatIdentity(mMatView);
}

// 更新
void Editor::Update()
{
	// テストモードの更新
	if (IsTestMode())
	{
		TestModeUpdate();
	}
	
	// カメラの移動処理
	CameraUpdate();
}

// 描画
void Editor::Draw()
{
	// カメラのセット
	SetCameraPositionAndTarget_UpVecY(mCameraPos.GetVECTOR(), mCameraLookPos.GetVECTOR());

	// ステージワイヤー描画
	DrawWire();

	// 各キャラクターの描画
	mBossInfo->Draw();
	mPlayerInfo->Draw();

	// コリジョンの描画
	CollDraw();

	// ImGuiの描画
	GUIDraw();
}

// float型の数値をstring型に変更する
std::string ChengeNumFloatToString(float nums_)
{
	char tmpStr[256];
	sprintf_s(tmpStr, "%g", nums_);
	std::string afterChangeStr = tmpStr;

	return afterChangeStr;
}
