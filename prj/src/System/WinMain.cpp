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
	// DXライブラリの設定
	// ---------------------------------------------------
	{
		SetOutApplicationLogValidFlag(FALSE);			// 「Log.txt」を作らない
		ChangeWindowMode(TRUE);
		SetGraphMode(WINDOW_W, WINDOW_H, 32);			// ウィンドウの設定とカラービット数
		SetBackgroundColor(32, 32, 64);					// 背景の色
		SetMainWindowText("Mutant");					// .exe の名前
		SetZBufferBitDepth(32);							// 32bitデプスバッファ (float)

		// Live2Dのセッティング
		Live2D_SetCubism4CoreDLLPath("data/Live2DCubismCore.dll");
		SetupCamera_Perspective(TO_RADIAN(45.0f));

		// DirectX11を使用するようにする
		SetUseDirect3DVersion(DX_DIRECT3D_11);

		// この２つもないとマウスとかキーボードからの入力を受け付けられない
		DxLib::SetHookWinProc(WndProc);
		DxLib::SetAlwaysRunFlag(true);

		if (DxLib_Init() == -1) { return -1; }			// DXライブラリの初期化処理

		// 描画先をバックスクリーンにする
		SetDrawScreen(DX_SCREEN_BACK);

		// カメラの遠近を設定
		SetCameraNearFar(0.01f, 3000.0f);

		// ---------------------------------
		// ライトの設定
		// ---------------------------------
		// 光の方向
		VECTOR lightDir = VGet(1.0f, -1.0f, -1.0f);
		ChangeLightTypeDir(lightDir);

		// ライトにディフューズカラーの設定
		//SetLightDifColor(GetColorF(0.4f, 0.4f, 0.6f, 1.0f));
		
		// シャドウマップハンドルの生成
		shadowHandle = MakeShadowMap(1024, 1024);

		// シャドウマップが描画される範囲の設定
		//SetShadowMapDrawArea(shadowHandle, VGet(-1000.0f, -1.0f, -1000.0f), VGet(1000.0f, 1000.0f, 1000.0f));
		SetShadowMapDrawArea(shadowHandle, VGet(-100.0f, -100.0f, -100.0f), VGet(100.0f, 30.0f, 100.0f));

		// ---------------------------------
		// Zバッファの設定
		// ---------------------------------
		// Zバッファのしようと書き込みを可能にする
		SetUseZBuffer3D(TRUE);
		SetWriteZBuffer3D(TRUE);

	}

	// ---------------------------------------------------
	// ImGuiの設定
	// ---------------------------------------------------
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows　★ここ消すとマルチウィンドウ解除

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
	// Effekseerの設定
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
		// ImGuiの更新
		// ---------------------------------------------------
		{
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
			//ImGui::ShowDemoWindow();
		}
	
		// ---------------------------------------------------
		// Effekseerの更新
		// ---------------------------------------------------
		// エフェクトの再生処理
		UpdateEffekseer3D();
		// EffekseerとDXライブラリのカメラを接続する
		Effekseer_Sync3DSetting();

		// -------------------
	 	// 更新処理
		// -------------------
		GameUpdate();
	
		// -------------------
		// 描画処理
		// -------------------
		GameDraw();
		GamePostDraw();
		// 再生中のエフェクトを描画する
		DrawEffekseer3D();

		// ---------------------------------------------------
		// ImGuiの描画
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
	// 終了処理
	// -------------------
	GameExit();

	// Effekseerを終了
	Effkseer_End();

	// ---------------------------------------------------
	// ImGuiの終了
	// ---------------------------------------------------
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	// シャドウマップの終了
	DeleteShadowMap(shadowHandle);

	// DXライブラリを終了してプログラムを閉じる
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
