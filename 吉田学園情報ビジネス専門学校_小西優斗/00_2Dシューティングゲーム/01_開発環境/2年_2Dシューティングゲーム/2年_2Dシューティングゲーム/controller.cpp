//=============================================================================
// 
// コントローラの処理
// Author : Konishi Yuuto
//
//=============================================================================
#include "controller.h"

//*****************************************************************************
// Joypad用設定値
//*****************************************************************************
#define DEADZONE        2500            // 各軸の25%を無効ゾーンとする
#define RANGE_MAX       10000           // 有効範囲の最大値
#define RANGE_MIN       -10000          // 有効範囲の最小値
#define MAX_CONTROLLER  4               //つなぐコントローラーの最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	LPDIRECTINPUTDEVICE8 pDIJoypad = NULL;   // コントローラーデバイス
	DIJOYSTATE2 State;                          //コントローラーのプレス情報
	DIJOYSTATE2 Trigger;                        //コントローラーのトリガー情報
	DIJOYSTATE2 Release;                        //コントローラーのプレス情報
	bool use;
}CONTROLLER;//コントローラーの情報

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECTINPUT8          pDInput = NULL;     // DirectInputオブジェクト
int g_nCntContoroller;                      // 接続しているコントローラの数
CONTROLLER g_Controller[MAX_CONTROLLER];    //コントローラーの情報

//=============================================================================
// [GetJoystickCallback] ジョイスティック・デバイスを列挙（コールバック関数）
// Author:KUDO AYANO
//=============================================================================
HRESULT CALLBACK GetJoystickCallback(LPDIDEVICEINSTANCE lpddi, LPVOID /*pvRef*/)
{
	HRESULT hr;

	hr = pDInput->CreateDevice(lpddi->guidInstance, &g_Controller[g_nCntContoroller].pDIJoypad, NULL);
	g_nCntContoroller++;
	return DIENUM_CONTINUE; // 次のデバイスを列挙
}

//=============================================================================
// Joypad用オブジェクト作成
// Author:KUDO AYANO
//=============================================================================
HRESULT InitJoypad(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT	hr;
	g_nCntContoroller = 0;

	//Directオブジェクトの作成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&pDInput, NULL)))
	{
		return E_FAIL;
	}

	// コントローラーを探す
	hr = pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)GetJoystickCallback, NULL, DIEDFL_ATTACHEDONLY);

	for (int nCntCntoroller = 0; nCntCntoroller < g_nCntContoroller; nCntCntoroller++)
	{

		if (FAILED(hr))
		{
			g_Controller[nCntCntoroller].State.rgdwPOV[nCntCntoroller] = -1;
		}
		if (!g_Controller[nCntCntoroller].pDIJoypad)
		{
			return false;
		}

		for (int nCntButton = 0; nCntButton < MAX_BUTTON; nCntButton++)
		{//ボタンの初期化
			g_Controller[nCntCntoroller].Trigger.rgbButtons[nCntButton] = '\0';
		}

		//十字キー　の初期化
		for (int nCnt = 0; nCnt < 3; nCnt++)
		{
			g_Controller[nCntCntoroller].State.rgdwPOV[nCnt] = -1;
		}


		// コントローラー用のデータ・フォーマットを設定
		hr = g_Controller[nCntCntoroller].pDIJoypad->SetDataFormat(&c_dfDIJoystick2);
		if (FAILED(hr))
		{
			return false; // データフォーマットの設定に失敗
		}

		// モードを設定（フォアグラウンド＆非排他モード）
		hr = g_Controller[nCntCntoroller].pDIJoypad->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
		if (FAILED(hr))
		{
			return false; // モードの設定に失敗
		}

		// 軸の値の範囲を設定
		// X軸、Y軸のそれぞれについて、オブジェクトが報告可能な値の範囲をセットする。
		// (max-min)は、最大10,000(?)。(max-min)/2が中央値になる。
		// 差を大きくすれば、アナログ値の細かな動きを捕らえられる。(パッドは、関係なし)
		DIPROPRANGE diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = RANGE_MIN;
		diprg.lMax = RANGE_MAX;
		// X軸の範囲を設定
		diprg.diph.dwObj = DIJOFS_X;
		g_Controller[nCntCntoroller].pDIJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y軸の範囲を設定
		diprg.diph.dwObj = DIJOFS_Y;
		g_Controller[nCntCntoroller].pDIJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Z軸の範囲を設定
		diprg.diph.dwObj = DIJOFS_Z;
		g_Controller[nCntCntoroller].pDIJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Z回転の範囲を設定
		diprg.diph.dwObj = DIJOFS_RZ;
		g_Controller[nCntCntoroller].pDIJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);

		// 各軸ごとに、無効のゾーン値を設定する。
		// 無効ゾーンとは、中央からの微少なジョイスティックの動きを無視する範囲のこと。
		// 指定する値は、10000に対する相対値(2000なら20パーセント)。
		DIPROPDWORD	dipdw;
		dipdw.diph.dwSize = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwHow = DIPH_BYOFFSET;
		dipdw.dwData = DEADZONE;
		//X軸の無効ゾーンを設定
		dipdw.diph.dwObj = DIJOFS_X;
		g_Controller[nCntCntoroller].pDIJoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		//Y軸の無効ゾーンを設定
		dipdw.diph.dwObj = DIJOFS_Y;
		g_Controller[nCntCntoroller].pDIJoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		//Z軸の無効ゾーンを設定
		dipdw.diph.dwObj = DIJOFS_Z;
		g_Controller[nCntCntoroller].pDIJoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		//Z回転の無効ゾーンを設定
		dipdw.diph.dwObj = DIJOFS_RZ;
		g_Controller[nCntCntoroller].pDIJoypad->SetProperty(DIPROP_DEADZONE, &dipdw.diph);

		//コントローラー入力制御開始
		g_Controller[nCntCntoroller].pDIJoypad->Acquire();
	}
	return S_OK;
}

//=============================================================================
// コントローラー更新処理
// Author:KUDO AYANO
//=============================================================================
void UpdateJoypad(void)
{
	DIJOYSTATE2 ControllerState;
	//BYTE ControllerState[nCntCntoroller][MAX_BUTTON];
	HRESULT hr;
	int nCntButton;
	for (int nCntCntoroller = 0; nCntCntoroller < MAX_CONTROLLER; nCntCntoroller++)
	{
		//十字キーの入力を、何もないようにしておく
		g_Controller[nCntCntoroller].State.rgdwPOV[0] = BUTTON_NEUTRAL;

		if (g_Controller[nCntCntoroller].pDIJoypad != NULL)
		{

			hr = g_Controller[nCntCntoroller].pDIJoypad->Poll();
			//if (FAILED(hr)) 
			//{
			//    hr = g_Controller[nCntCntoroller].pDIJoypad->Acquire();
			//}

			//デバイスからデータを取得
			if (SUCCEEDED(hr = g_Controller[nCntCntoroller].pDIJoypad->GetDeviceState(sizeof(DIJOYSTATE2), &ControllerState)))
			{


				g_Controller[nCntCntoroller].State.lY = ControllerState.lY;
				g_Controller[nCntCntoroller].State.lX = ControllerState.lX;

				g_Controller[nCntCntoroller].State.lZ = ControllerState.lZ;		//スティックの横（Z軸）
				g_Controller[nCntCntoroller].State.lRz = ControllerState.lRz;		//スティックの縦（Z回転）


																					//十字キーの情報
				for (int nCnt = 0; nCnt < 3; nCnt++)
				{
					//十字キープレス情報を保存
					g_Controller[nCntCntoroller].State.rgdwPOV[nCnt] = ControllerState.rgdwPOV[nCnt];

				}


				//ボタンの情報
				for (nCntButton = 0; nCntButton < MAX_BUTTON; nCntButton++)
				{
					//キートリガー情報を保存
					g_Controller[nCntCntoroller].Trigger.rgbButtons[nCntButton] = (g_Controller[nCntCntoroller].State.rgbButtons[nCntButton] ^ ControllerState.rgbButtons[nCntButton]) & ControllerState.rgbButtons[nCntButton];

					//キーリリース情報を保存
					g_Controller[nCntCntoroller].Release.rgbButtons[nCntButton] = (g_Controller[nCntCntoroller].State.rgbButtons[nCntButton] ^ ControllerState.rgbButtons[nCntButton]) & ControllerState.rgbButtons[nCntButton];

					//キープレス情報を保存
					g_Controller[nCntCntoroller].State.rgbButtons[nCntButton] = ControllerState.rgbButtons[nCntButton];

				}
			}
			else
			{
				//コントローラーのアクセス権を取得
				g_Controller[nCntCntoroller].pDIJoypad->Acquire();
			}
		}
	}
}

//=============================================================================
// [GetJoypadPress] ボタンのプレス状態を取得
// Author:KUDO AYANO
//
// 引数
//  [in] nContllre : コントローラの番号
//  [in] nButton   : ボタンの番号
//
//=============================================================================
bool GetJoypadPress(int nController, int nButton)
{
	return(g_Controller[nController].State.rgbButtons[nButton] & 0x80) ? true : false;
}

//=============================================================================
// ボタンのトリガー状態を取得
// Author:KUDO AYANO
//
// 引数
//  [in] nContllre : コントローラの番号
//  [in] nButton   : ボタンの番号
//
//=============================================================================
bool GetJoypadTrigger(int nController, int nButton)
{
	return (g_Controller[nController].Trigger.rgbButtons[nButton] & 0x80) ? true : false;;
}

//=============================================================================
// ボタンのリリース状態を取得
// Author:KUDO AYANO
//
// 引数
//  [in] nContllre : コントローラの番号
//  [in] nButton   : ボタンの番号
//
//=============================================================================
bool GetJoypadRelease(int nController, int nButton)
{
	return(g_Controller[nController].Release.rgbButtons[nButton] & 0x80) ? true : false;
}

//=============================================================================
// コントローラの状態を取得
// Author:KUDO AYANO
//=============================================================================
DIJOYSTATE2 GetController(int nContllre)
{
	return g_Controller[nContllre].State;
}

//=============================================================================
// コントローラー終了処理
// Author:KUDO AYANO
//=============================================================================
void ReleaseDInput(void)
{
	for (int nCntDevise = 0; nCntDevise < MAX_CONTROLLER; nCntDevise++)
	{

		//入力デバイスの開放
		if (g_Controller[nCntDevise].pDIJoypad != NULL)
		{
			//コントローラのアクセス権を開放
			g_Controller[nCntDevise].pDIJoypad->Unacquire();
			g_Controller[nCntDevise].pDIJoypad->Release();
			g_Controller[nCntDevise].pDIJoypad = NULL;
		}
	}
	//DirectInputオブジェクトの解放
	if (pDInput != NULL)
	{
		pDInput->Release();
		pDInput = NULL;
	}

}
