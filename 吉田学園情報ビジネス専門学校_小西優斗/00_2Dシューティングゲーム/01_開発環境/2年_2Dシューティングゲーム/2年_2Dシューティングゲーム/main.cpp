//=============================================================================
//
// メイン処理 [main.cpp]
// Author : Konishi Yuuto
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2d.h"
#include "manager.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define CLASS_NAME		"AppClass"			// ウインドウのクラス名
#define WINDOW_NAME		"TwilightRaven"	// ウインドウのキャプション名

//=============================================================================
// プロトタイプ宣言
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//=============================================================================
// グローバル変数:
//=============================================================================
int g_nCountFPS;					// FPSカウンタ
DWORD g_dwGameStartTime = 0;		// 開始時刻
DWORD g_dwGameTime = 0;				// 経過時間
DWORD dwCurrentTime;

//=============================================================================
// メイン関数
//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	HWND hWnd;
	MSG msg;
	//DWORD dwCurrentTime;
	DWORD dwFrameCount;
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// 指定したクライアント領域を確保するために必要なウィンドウ座標を計算
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウの作成
	hWnd = CreateWindow(CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		(rect.right - rect.left),
		(rect.bottom - rect.top),
		NULL,
		NULL,
		hInstance,
		NULL);

	// マネージャーメモリ確保
	CManager *pManager = NULL;
	pManager = new CManager;

	// マネージャー初期化処理
	if (FAILED(pManager->Init(hInstance, hWnd, false)))
	{
		return -1;
	}

	// 分解能を設定
	timeBeginPeriod(1);

	// フレームカウント初期化
	dwCurrentTime =
		dwFrameCount = 0;
	dwExecLastTime =
		dwFPSLastTime = timeGetTime();

	// ウインドウの表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳とディスパッチ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			// 現在の時間を取得
			dwCurrentTime = timeGetTime();	

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{	// 0.5秒ごとに実行
#ifdef _DEBUG
				 // FPSを算出
				g_nCountFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);

				// FPSの設定
				pManager->SetFPS(g_nCountFPS);
#endif
				// 現在の時間を保存
				dwFPSLastTime = dwCurrentTime;	
				dwFrameCount = 0;
			}

			// 現在の時間を取得
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{	// 1/60秒経過
				dwExecLastTime = dwCurrentTime;	// 現在の時間を保存

				// レンダラー更新処理
				pManager->Update();

				// レンダラー描画処理
				pManager->Draw();

				dwFrameCount++;
			}
		}
	}

	// 開始直後に時間取得(ゲーム開始時間)
	g_dwGameStartTime = timeGetTime();


	// 敵の全てをリリース
	CScene2D::ReleaseAll();

	// レンダラー終了処理
	pManager->Uninit();

	// g_pRendererが使われているとき
	if (pManager != NULL)
	{
		// レンダラーのメモリ開放
		delete pManager;
		pManager = NULL;
	}

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 分解能を戻す
	timeEndPeriod(1);

	return (int)msg.wParam;
}

//=============================================================================
// ウインドウプロシージャ
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:				// [ESC]キーが押された
			DestroyWindow(hWnd);	// ウィンドウを破棄するよう指示する
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

DWORD GetBossCurrentTime(void)
{
	return dwCurrentTime;
}

DWORD GetBossGameTime(void)
{
	return g_dwGameTime;
}

DWORD GetBossStartTime(void)
{
	return g_dwGameStartTime;
}
