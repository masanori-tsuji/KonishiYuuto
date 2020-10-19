//=============================================================================
//
// コントローラーの処理	
// Author:Konishi Yuuto
//
//=============================================================================
#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_
#define _CRT_SECURE_NO_WARNINGS_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"
#include "input.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define BUTTON_UP     (0)	    //十字キー上
#define BUTTON_DOWN   (18000)	//十字キー下
#define BUTTON_LEFT   (27000)	//十字キー左
#define BUTTON_RIGHT  (9000)	//十字キー右

#define BUTTON_UP_RIGHT     (4500)	//十字キー上右
#define BUTTON_RIGHT_DOWN  (13500)	//十字キー右下
#define BUTTON_DOWN_LEFT   (22500)	//十字キー下左
#define BUTTON_LEFT_UP     (31500)	//十字キー左上

#define BUTTON_NEUTRAL -1       //何も入力していないとき

#define CROSSZONE (4500)        //十字キーの許容範囲

//プレイヤーの番号
#define PLAYER_1 (0)     //プレイヤーのコントローラー番号 (1P)
#define PLAYER_2 (1)     //プレイヤーのコントローラー番号 (2P)
#define PLAYER_3 (2)     //プレイヤーのコントローラー番号 (3P)
#define PLAYER_4 (3)     //プレイヤーのコントローラー番号 (4P)

//=============================================================================
//構造体/列挙型定義
//=============================================================================
typedef enum
{
	BUTTON_X = 0,
	BUTTON_Y,
	BUTTON_A,
	BUTTON_B,
	BUTTON_L1,
	BUTTON_R1,
	BUTTON_L2,
	BUTTON_R2,
	BUTTON_L3,
	BUTTON_R3,
	BUTTON_BACK,
	BUTTON_START,
	BUTTON_Guide,
	MAX_BUTTON
}BUTTON;//ボタンの数

//-----------------------------------------------------------------------------
// プロトタイプ宣言 function-prototypes
//-----------------------------------------------------------------------------
bool InitDInput(HINSTANCE);
HRESULT InitJoypad(HINSTANCE hInstance, HWND hWnd);
void UpdateJoypad(void);
void ReleaseDInput(void);

bool GetJoypadPress(int nController, int nButton);
bool GetJoypadTrigger(int nController, int nButton);
bool GetJoypadRelease(int nController, int nButton);
DIJOYSTATE2 GetController(int nController);

#endif
