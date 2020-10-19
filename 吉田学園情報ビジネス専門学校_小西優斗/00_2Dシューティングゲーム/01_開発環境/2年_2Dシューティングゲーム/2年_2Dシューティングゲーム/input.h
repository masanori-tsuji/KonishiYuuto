//=====================================================
//
// インプット処理 [input.h]
// Author:小西優斗
//
//=====================================================
#ifndef _INPUT_H_
#define _INPUT_H_

//=====================================================
// インクルードファイル
//=====================================================
#include "main.h"
#define DIRECTINPUT_VERSION (0x0800)	// 警告対処用("dinput.h"の直前に定義)
#include "dinput.h"

#pragma comment(lib, "dinput8.lib")

//=====================================================
// マクロ定義
//=====================================================
#define NUM_KEY_MAX (256)			// キー最大数

//=====================================================
// インプットクラス
//=====================================================
class CInput
{
public:
	CInput();					// コンストラクタ
	virtual ~CInput() = 0;		// デストラクタ

	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// 初期化処理
	virtual void Uninit(void);								// 終了処理
	virtual void Update(void) = 0;								// 更新処理

protected:
	static LPDIRECTINPUT8 m_pInput;				// DirectInputオブジェクトへのポインタ
	LPDIRECTINPUTDEVICE8 m_pDevice;				// 入力デバイス(キーボード)へのポインタ

};

#endif
