#include "../external/imgui/imgui.h"
#include "../external/imgui/imgui_impl_win32.h"
#include "../external/imgui/imgui_impl_dx11.h"
#include "WinMain.h"
#include "../Game/3DGame.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);

	return 0;
}



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// ---------------------------------------------------
	// DX���C�u�����̐ݒ�
	// ---------------------------------------------------
	{
		SetOutApplicationLogValidFlag(FALSE);			// �uLog.txt�v�����Ȃ�
		ChangeWindowMode(TRUE);
		SetGraphMode(WINDOW_W, WINDOW_H, 32);			// �E�B���h�E�̐ݒ�ƃJ���[�r�b�g��
		SetBackgroundColor(32, 32, 64);					// �w�i�̐F
		SetMainWindowText("Mutant");					// .exe �̖��O
		SetZBufferBitDepth(32);							// 32bit�f�v�X�o�b�t�@ (float)

		// Live2D�̃Z�b�e�B���O
		Live2D_SetCubism4CoreDLLPath("data/Live2DCubismCore.dll");
		SetupCamera_Perspective(TO_RADIAN(45.0f));

		// DirectX11���g�p����悤�ɂ���
		SetUseDirect3DVersion(DX_DIRECT3D_11);

		// ���̂Q���Ȃ��ƃ}�E�X�Ƃ��L�[�{�[�h����̓��͂��󂯕t�����Ȃ�
		DxLib::SetHookWinProc(WndProc);
		DxLib::SetAlwaysRunFlag(true);

		if (DxLib_Init() == -1) { return -1; }			// DX���C�u�����̏���������

		// �`�����o�b�N�X�N���[���ɂ���
		SetDrawScreen(DX_SCREEN_BACK);

		// �J�����̉��߂�ݒ�
		SetCameraNearFar(0.01f, 3000.0f);

		// ---------------------------------
		// ���C�g�̐ݒ�
		// ---------------------------------
		// ���̕���
		VECTOR lightDir = VGet(1.0f, -1.0f, -1.0f);
		ChangeLightTypeDir(lightDir);

		// ���C�g�Ƀf�B�t���[�Y�J���[�̐ݒ�
		//SetLightDifColor(GetColorF(0.4f, 0.4f, 0.6f, 1.0f));
		
		// �V���h�E�}�b�v�n���h���̐���
		shadowHandle = MakeShadowMap(1024, 1024);

		// �V���h�E�}�b�v���`�悳���͈͂̐ݒ�
		//SetShadowMapDrawArea(shadowHandle, VGet(-1000.0f, -1.0f, -1000.0f), VGet(1000.0f, 1000.0f, 1000.0f));
		SetShadowMapDrawArea(shadowHandle, VGet(-100.0f, -100.0f, -100.0f), VGet(100.0f, 30.0f, 100.0f));

		// ---------------------------------
		// Z�o�b�t�@�̐ݒ�
		// ---------------------------------
		// Z�o�b�t�@�̂��悤�Ə������݂��\�ɂ���
		SetUseZBuffer3D(TRUE);
		SetWriteZBuffer3D(TRUE);

	}

	// ---------------------------------------------------
	// ImGui�̐ݒ�
	// ---------------------------------------------------
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows�@�����������ƃ}���`�E�B���h�E����

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		ImGui_ImplWin32_Init(DxLib::GetMainWindowHandle());
		ImGui_ImplDX11_Init((ID3D11Device*)DxLib::GetUseDirect3D11Device(), (ID3D11DeviceContext*)DxLib::GetUseDirect3D11DeviceContext());
	}

	// ---------------------------------------------------
	// Effekseer�̐ݒ�
	// ---------------------------------------------------
	{
		SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
		Effekseer_SetGraphicsDeviceLostCallbackFunctions();

		if (Effekseer_Init(8000) == -1)
		{
			DxLib_End();
			return -1;
		}
	}

	GameInit();

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_BACK) == 0)
	{
		InputUpdate();
		// ---------------------------------------------------
		// ImGui�̍X�V
		// ---------------------------------------------------
		{
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
			//ImGui::ShowDemoWindow();
		}
	
		// ---------------------------------------------------
		// Effekseer�̍X�V
		// ---------------------------------------------------
		// �G�t�F�N�g�̍Đ�����
		UpdateEffekseer3D();
		// Effekseer��DX���C�u�����̃J������ڑ�����
		Effekseer_Sync3DSetting();

		// -------------------
	 	// �X�V����
		// -------------------
		GameUpdate();
	
		// -------------------
		// �`�揈��
		// -------------------
		GameDraw();
		GamePostDraw();
		// �Đ����̃G�t�F�N�g��`�悷��
		DrawEffekseer3D();

		// ---------------------------------------------------
		// ImGui�̕`��
		// ---------------------------------------------------
		{
			ImGui::Render();
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
			// Update and Render additional Platform Windows
			if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault(); 
			}
			RefreshDxLibDirect3DSetting();
		}

		ScreenFlip();
		ClearDrawScreen();
		MemorizeInputInfo();
	}

	// -------------------
	// �I������
	// -------------------
	GameExit();

	// Effekseer���I��
	Effkseer_End();

	// ---------------------------------------------------
	// ImGui�̏I��
	// ---------------------------------------------------
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	// �V���h�E�}�b�v�̏I��
	DeleteShadowMap(shadowHandle);

	// DX���C�u�������I�����ăv���O���������
	DxLib_End();

	return 0;
}

float TO_RADIAN(float _degree)
{
	return _degree * DX_PI_F / 180.0f;
}

float TO_DEGREE(float radian)
{
	return radian * 180.0f / DX_PI_F;
}

int handleHandle()
{
	return shadowHandle;
}
