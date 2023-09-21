//---------------------------------------------------------------------------
//! @file   Editor.cpp
//! @brief  �G�f�B�^�[
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
// private�֐�
// ------------------------------------------------------------------------------
// ImGui�̃��C������
void Editor::GUIDraw()
{
	// ImGui�̃E�B���h�E�̍��W��ݒ�
	int windowX = 0;
	int windowY = 0;
	GetWindowPosition(&windowX, &windowY);
	windowX += WINDOW_W;

	// �E�B���h�E�̍쐬
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
	ImGui::SetNextWindowPos(ImVec2(static_cast<float>(windowX), static_cast<float>(windowY)));
	ImGui::SetNextWindowSize(ImVec2(500, 800), ImGuiCond_Once);

	// ImGui�̊J�n
	ImGui::Begin("Edit Mode");

	// -------------------------------------------------------------
	// �f�[�^�̏������݂̏���
	// -------------------------------------------------------------
	if (ImGui::Button("Save")) {

		// CSV�t�@�C���̍s��
		const int LINEMAX = 2;

		// -------------------------------------------
		// �X�e�[�^�X�̕ۑ�
		// -------------------------------------------
		std::ofstream statusFile("data/csv/BossStatus.csv");

		for (int i = 0; i < LINEMAX; ++i)
		{
			std::string str;
			if (i == 0)
			{
				// �e���x�����P�s�ڂɏ�������
				str = "HP,��U����,���U����,�R���{�U����,�W�����v�U����,�ړ����x,�����蔻��̃T�C�Y";
				statusFile << str << std::endl;
			}

			if (i != 0)
			{
				// ���ꂼ��̒l���Q�s�ڂɏ�������
				str = ChengeNumFloatToString(mBossStatus.hpMax) + ',' + ChengeNumFloatToString(mBossStatus.attackPower[BOSSATTACKTYPE::Light]) + ',' +
					ChengeNumFloatToString(mBossStatus.attackPower[BOSSATTACKTYPE::Heavy]) + ',' + ChengeNumFloatToString(mBossStatus.attackPower[BOSSATTACKTYPE::Combo]) + ',' +
					ChengeNumFloatToString(mBossStatus.attackPower[BOSSATTACKTYPE::Jump]) + ',' + ChengeNumFloatToString(mBossStatus.moveSpeed) + ',' + ChengeNumFloatToString(mBossStatus.collSize);
				statusFile << str << std::endl;
			}
		}

		statusFile.close();

		// -------------------------------------------
		// �R���W�����̕ۑ�
		// -------------------------------------------
		std::ofstream collFile("data/csv/BossColl.csv");

		for (int i = 0; i < LINEMAX; ++i)
		{
			std::string str;
			if (i == 0)
			{
				// �e���x�����P�s�ڂɏ�������
				str = "�{�f�B�̃T�C�Y,���̐�[X,���̐�[Y,���̐�[Z,���̍��{X,���̍��{Y,���̍��{Z,���̃T�C�Y,�܂̐�[X,�܂̐�[Y,�܂̐�[Z,�܂̍��{X,�܂̍��{Y,�܂̍��{Z,�܂̃T�C�Y,";
				collFile << str << std::endl;
			}

			if (i != 0)
			{
				// ���ꂼ��̒l���Q�s�ڂɏ�������
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
	// �e�X�g���[�h�̏���
	// -------------------------------------------------------------
	// �e�X�g���[�h�����ǂ����Ń{�^���ɕ`�悷�镶�����ύX����
	std::string buttonStr;
	ChangeButtonLabel(buttonStr);

	if (ImGui::Button(buttonStr.c_str()))
	{
		mIsTestMode = !mIsTestMode;

		// �e�X�g���[�h�I�����̓L�����N�^�[�̃��Z�b�g
		if (!mIsTestMode)
		{
			ResetCharacter();
		}
	}

	// -------------------------------------------------------------
	// �X�e�[�^�X����
	// -------------------------------------------------------------
	// �c���[�̍쐬
	if (ImGui::TreeNode("Status"))
	{
		ImGui::SliderFloat("Hp", &mBossStatus.hpMax, 50.0f, 300.0f, "%.0f");
		ImGui::SliderFloat("LightAttackPower", &mBossStatus.attackPower[BOSSATTACKTYPE::Light], 1.0f, 50.0f, "%.0f");
		ImGui::SliderFloat("HeavyAttackPower", &mBossStatus.attackPower[BOSSATTACKTYPE::Heavy], 1.0f, 50.0f, "%.0f");
		ImGui::SliderFloat("ComboAttackPower", &mBossStatus.attackPower[BOSSATTACKTYPE::Combo], 1.0f, 50.0f, "%.0f");
		ImGui::SliderFloat("JumpAttackPower", &mBossStatus.attackPower[BOSSATTACKTYPE::Jump], 0.0f, 30.0f, "%.0f");
		ImGui::SliderFloat("MoveSpeed", &mBossStatus.moveSpeed, 0.0f, 3.0f, "%.2f");
		ImGui::SliderFloat("CollSize", &mBossStatus.collSize, 0.0f, 50.0f, "%.0f");

		// ���̃c���[���|�b�v����
		ImGui::TreePop();
	}

	// -------------------------------------------------------------
	// �����蔻�葀��
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
	// �������̃J�v�Z���`��
	// --------------------------------------------
	{
		VECTOR fistPos[2];
		fistPos[0] = mBossCollInfo.fistPos[BOSSATTACKCOLLTYPE::Top].GetVECTOR();
		fistPos[1] = mBossCollInfo.fistPos[BOSSATTACKCOLLTYPE::Under].GetVECTOR();

		// ���̃}�g���b�N�X���擾
		int    attachFrame = MV1SearchFrame(mBossInfo->GetModel(), "mixamorig:RightHand");
		MATRIX rightArmMat = MV1GetFrameLocalWorldMatrix(mBossInfo->GetModel(), attachFrame);

		// ���{���W�ƃ}�g���b�N�X����Z���A�ŏI�I�ȍ��{���W���擾
		float3 newFistPos = mul(float4(fistPos[1].x, fistPos[1].y, fistPos[1].z, 1), cast(rightArmMat)).xyz;

		// �}�g���b�N�X���g�p�������W�ϊ����s���A�ŏI�I�Ȑ�[���W���擾����
		VECTOR newDir = VTransformSR(fistPos[0], rightArmMat);

		DrawCapsule3D(cast(newFistPos), cast(newFistPos + float3(newDir.x, newDir.y, newDir.z)), mBossCollInfo.fistSize, 32, GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);
	}

	// --------------------------------------------
	// �ܕ����̃J�v�Z���`��
	// --------------------------------------------
	{
		VECTOR clawPos[2];
		clawPos[0] = mBossCollInfo.clawPos[BOSSATTACKCOLLTYPE::Top].GetVECTOR();
		clawPos[1] = mBossCollInfo.clawPos[BOSSATTACKCOLLTYPE::Under].GetVECTOR();

		// ���̃}�g���b�N�X���擾
		int    attachFrame = MV1SearchFrame(mBossInfo->GetModel(), "mixamorig:LeftHand");
		MATRIX leftArmMat = MV1GetFrameLocalWorldMatrix(mBossInfo->GetModel(), attachFrame);

		// ���{���W�ƃ}�g���b�N�X����Z���A�ŏI�I�ȍ��{���W���擾
		float3 newClawPos = mul(float4(clawPos[1].x, clawPos[1].y, clawPos[1].z, 1), cast(leftArmMat)).xyz;

		// �}�g���b�N�X���g�p�������W�ϊ����s���A�ŏI�I�Ȑ�[���W���擾����
		VECTOR newDir = VTransformSR(clawPos[0], leftArmMat);

		DrawCapsule3D(cast(newClawPos), cast(newClawPos + float3(newDir.x, newDir.y, newDir.z)), mBossCollInfo.clawSize, 32, GetColor(0, 255, 0), GetColor(0, 255 , 0), FALSE);
	}
}

// MATRIX�^�ϐ��̏�����
void Editor::SetMatIdentity(MATRIX& _initMat)
{
	_initMat.m[0][0] = 1.0f;
	_initMat.m[1][1] = 1.0f;
	_initMat.m[2][2] = 1.0f;
	_initMat.m[3][3] = 1.0f;
}

// �J�����̈ړ�
void Editor::CameraUpdate()
{
	// �r���[�s�񂩂烏�[���h�s����擾����
	mMatView = GetCameraViewMatrix();
	mMatCameraWorld = MInverse(mMatView);

	// 3�����̈ړ��x�N�g��
	const auto& copy = mMatCameraWorld;
	Vector3 right = { copy.m[0][0],copy.m[0][1],copy.m[0][2] };
	Vector3 top = { copy.m[1][0],copy.m[1][1],copy.m[1][2] };
	Vector3 front = { copy.m[2][0],copy.m[2][1],copy.m[2][2] };
	front *= -1;

	// �}�E�X�̈ړ���
	VectorInt2 cursorPos;
	GetMousePoint(&cursorPos.x, &cursorPos.y);

	// ���̃J�[�\�����W�����Ă���
	mcursorPos = cursorPos;

	// �ړ�����
	float dx = static_cast<float>(cursorPos.x - mMousePos.x);
	float dy = static_cast<float>(cursorPos.y - mMousePos.y);

	// �E�B���h�E�̏����擾
	int windowX = 0;
	int windowY = 0;
	GetWindowPosition(&windowX, &windowY);

	// .exe�̍���
	mcursorPos.x -= windowX;
	mcursorPos.y -= windowY;

	// ���̈ړ������v�Z�p�ɑ��
	mMousePos = cursorPos;

	// �ړ���
	Vector3 movDir = { 0.0f,0.0f,0.0f };

	// WASD�ňړ��ʂ���������
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

	// ���W�ƒ����_��i�߂�
	mCameraPos += movDir * 0.8f;
	mCameraLookPos += movDir * 0.8f;

	// �}�E�X����̉�]
	if (CheckMouseInput(MOUSE_INPUT_LEFT))
	{
		// ���W���璍���_�ւ̃x�N�g��
		Vector3 dir = mCameraLookPos - mCameraPos;

		// ���E��]
		{
			// Y����]���A�֐��ɓn����悤�ɃL���X�g����
			MATRIX rotY = MGetRotY(dx * 0.005f);
			float4x4 rotY2 = cast(rotY);

			// �x�N�g����float4�^�ɕϊ�
			VECTOR toDir = dir.GetVECTOR();
			float4 dir2 = cast(toDir);

			// ��Z����Vector3�^�ɖ߂�
			float3 vec3Dir;
			vec3Dir = mul(rotY2, float4(dir2.xyz, 0.0f)).xyz;
			dir.x = vec3Dir.x;
			dir.y = vec3Dir.y;
			dir.z = vec3Dir.z;
		}

		// �㉺��]
		{
			// �p�x��]���A���ƂŊ֐��ɓn�����߂�float4x4�^�ɕϊ�
			MATRIX   rotX = MGetRotAxis(right.GetVECTOR(), dy * 0.005f);
			float4x4 rotX2 = cast(rotX);

			// ���W���璍���_�̃x�N�g����float4�^�ɕϊ�
			VECTOR toDir2 = dir.GetVECTOR();
			float4 dir2 = cast(toDir2);

			// float3�^�Ōv�Z���AVector3�^�ɖ߂�
			float3 vec3Dir;
			vec3Dir = mul(rotX2, float4(dir2.xyz, 0.0f)).xyz;
			dir.x = vec3Dir.x;
			dir.y = vec3Dir.y;
			dir.z = vec3Dir.z;
		}

		// �����_
		mCameraLookPos = mCameraPos + dir;
	}

	// �K�v�Ȓl��VECTOR�^�ɕϊ�
	VECTOR vecFrom = mCameraPos.GetVECTOR();			// �J�����̈ʒu
	VECTOR vecLookat = mCameraLookPos.GetVECTOR();		// �J�����̒����_
	VECTOR vecUp = VGet(0, 0, 0);					// �J�����̏����

	// �E����W�n�̃r���[�s����쐬����
	CreateLookAtMatrixRH(&mMatView, &vecFrom, &vecLookat, &vecUp);
}

// �e�X�g���[�h���̍X�V
void Editor::TestModeUpdate()
{
	mBossInfo->Update(1.0f);

	// �{�X�̍U���J�n����
	Vector3 vecBossToPlayer = mPlayerInfo->GetPos() - mBossInfo->GetPos();
	float length = vecBossToPlayer.GetLength();
	const float ATTACKRANGE = 15.0f;
	if (length < ATTACKRANGE)
	{
		mBossInfo->ChoiseAttackType();
	}

	// -------------------------------------------------------------
	// �e�U�����߃{�^��
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

// ����̃��C���[�`��
void Editor::DrawWire()
{
	// �ʒu��������₷���悤�Ɋi�q��`��
	const float LINEAREASIZE = 10000.0f;
	const int LINEMAX = 50;

	VECTOR Pos1;
	VECTOR Pos2;

	// z���̃��C��
	Pos1 = VGet(-LINEAREASIZE / 2.0f, 0.0f, -LINEAREASIZE / 2.0f);
	Pos2 = VGet(-LINEAREASIZE / 2.0f, 0.0f, LINEAREASIZE / 2.0f);
	for (int i = 0; i <= LINEMAX; i++)
	{
		DrawLine3D(Pos1, Pos2, GetColor(255, 255, 255));
		Pos1.x += LINEAREASIZE / LINEMAX;
		Pos2.x += LINEAREASIZE / LINEMAX;
	}

	// x���̃��C��
	Pos1 = VGet(-LINEAREASIZE / 2.0f, 0.0f, -LINEAREASIZE / 2.0f);
	Pos2 = VGet(LINEAREASIZE / 2.0f, 0.0f, -LINEAREASIZE / 2.0f);
	for (int i = 0; i < LINEMAX; i++)
	{
		DrawLine3D(Pos1, Pos2, GetColor(255, 255, 255));
		Pos1.z += LINEAREASIZE / LINEMAX;
		Pos2.z += LINEAREASIZE / LINEMAX;
	}

	// X���AY���AZ���̒��S�̃��C��
	DrawLine3D(VGet(500.0f, 3.0f, 0.0f), VGet(-500.0f, 0.0f, 0.0f), GetColor(255, 0, 0));
	DrawLine3D(VGet(0.0f, 500.0f, 0.0f), VGet(0.0f, 0.0f, 0.0f), GetColor(0, 255, 0));
	DrawLine3D(VGet(0.0f, 3.0f, 500.0f), VGet(0.0f, 0.0f, -500.0f), GetColor(0, 0, 255));
}

// �e�X�g���[�h�����ǂ����Ń��x���̕������ύX����
void Editor::ChangeButtonLabel(std::string& _changeStr)
{
	// �{�^���ɕ`�悷�镶����̕ύX
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
// public�֐�
// ------------------------------------------------------------------------------
// �R���X�g���N�^
Editor::Editor() :
	mBossStatus{ -1.0f }, mBossCollInfo{ -1.0f }, mBossInfo(nullptr), mPlayerInfo(nullptr), mIsTestMode(false),
	mMousePos{ 0,0 }
{
}

// �f�X�g���N�^
Editor::~Editor()
{
}

// �e�X�g���[�h�̊J�n
void Editor::SetTestMode()
{
	mIsTestMode = true;
}

// �e�X�g���[�h�t���O�̃`�F�b�N
bool Editor::IsTestMode() const
{
	return mIsTestMode;
}

// �{�X�N���X�̏����Z�b�g����
void Editor::SetBossInfo(Boss* _bossObj)
{
	mBossInfo = _bossObj;
}

void Editor::SetPlayerInfo(Player* _playerObj)
{
	mPlayerInfo = _playerObj;
}

// ������
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

// �X�V
void Editor::Update()
{
	// �e�X�g���[�h�̍X�V
	if (IsTestMode())
	{
		TestModeUpdate();
	}
	
	// �J�����̈ړ�����
	CameraUpdate();
}

// �`��
void Editor::Draw()
{
	// �J�����̃Z�b�g
	SetCameraPositionAndTarget_UpVecY(mCameraPos.GetVECTOR(), mCameraLookPos.GetVECTOR());

	// �X�e�[�W���C���[�`��
	DrawWire();

	// �e�L�����N�^�[�̕`��
	mBossInfo->Draw();
	mPlayerInfo->Draw();

	// �R���W�����̕`��
	CollDraw();

	// ImGui�̕`��
	GUIDraw();
}

// float�^�̐��l��string�^�ɕύX����
std::string ChengeNumFloatToString(float nums_)
{
	char tmpStr[256];
	sprintf_s(tmpStr, "%g", nums_);
	std::string afterChangeStr = tmpStr;

	return afterChangeStr;
}
